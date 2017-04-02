#include "cubieCube.h"
#include "coordinateCube.h"
#include <cassert>
#include "search.h"
#include <algorithm>
#include "util.h"

char CubieCube::FlipR2S[2048];// = new char[2048];
char CubieCube::TwistR2S[2048];// = new char[2187];
char CubieCube::EPermR2S[2048];// = new char[40320];
char CubieCube::FlipS2RF[336*8];

CubieCube CubieCube::urf1 = CubieCube(2531, 1373, 67026819, 1367);
CubieCube CubieCube::urf2 = CubieCube(2089, 1906, 322752913, 2040);

char CubieCube::urfMove[6][18] = {
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
		{6, 7, 8, 0, 1, 2, 3, 4, 5, 15, 16, 17, 9, 10, 11, 12, 13, 14},
		{3, 4, 5, 6, 7, 8, 0, 1, 2, 12, 13, 14, 15, 16, 17, 9, 10, 11},
		{2, 1, 0, 5, 4, 3, 8, 7, 6, 11, 10, 9, 14, 13, 12, 17, 16, 15},
		{8, 7, 6, 2, 1, 0, 5, 4, 3, 17, 16, 15, 11, 10, 9, 14, 13, 12},
		{5, 4, 3, 8, 7, 6, 2, 1, 0, 14, 13, 12, 17, 16, 15, 11, 10, 9}
};

int CubieCube::preMove[9] = {-1, Util::Rx1, Util::Rx3, Util::Fx1, Util::Fx3, Util::Lx1, Util::Lx3, Util::Bx1, Util::Bx3};

char CubieCube::e2c[16] = {0, 0, 0, 0, 1, 3, 1, 3, 1, 3, 1, 3, 0, 0, 0, 0};

CubieCube::CubieCube(int cperm, int twist, int eperm, int flip) {
	for (int i = 0; i < 8; i++) {
		ca[i] = i;
		ea[i] = 2*i;
	}
	for (int i = 8; i < 12; i++) ea[i] = 2*i;
	temps = nullptr;
	this->setCPerm(cperm);
	this->setTwist(twist);
	Util::setNPerm(ea, eperm, 12, true);
	this->setFlip(flip);
}

CubieCube::~CubieCube() {
	if (temps) delete temps;
}

bool CubieCube::operator==(const CubieCube& rhs) const
{
	for (int i = 0; i < 8; i++) {
		if (ca[i] != rhs.ca[i]) {
			return false;
		}
	}
	for (int i = 0; i < 12; i++) {
		if (ea[i] != rhs.ea[i]) {
			return false;
		}
	}

	return true;
}

bool CubieCube::equalsCorn(CubieCube c) const {
	for (int i = 0; i < 8; i++) {
		if (ca[i] != c.ca[i]) {
			return false;
		}
	}
	return true;
}

bool CubieCube::equalsEdge(CubieCube c) const {
	for (int i = 0; i < 12; i++) {
		if (ea[i] != c.ea[i]) {
			return false;
		}
	}
	return true;
}

void CubieCube::invCubieCube() {
	if (!temps) {
		temps = new CubieCube();
	}
	for (char edge = 0; edge < 12; edge++) {
		temps->ea[ea[edge] >> 1] = (char) (edge << 1 | (ea[edge] & 1));
	}
	for (char corn = 0; corn < 8; corn++) {
		int ori = ca[corn] >> 3;
		ori = 4 >> ori & 3; //0->0, 1->2, 2->1
		temps->ca[ca[corn] & 0x7] = (char) (corn | ori << 3);
	}
	*this = *temps;
	delete temps;
}

// prod = a * b, Corner Only.
void CubieCube::CornMult(const CubieCube& a, const CubieCube& b,
CubieCube& prod) {
	for (int corn = 0; corn < 8; corn++) {
		int oriA = a.ca[b.ca[corn] & 7] >> 3;
		int oriB = b.ca[corn] >> 3;
		int ori = oriA;
		ori += (oriA < 3) ? oriB : 6 - oriB;
		ori %= 3;
		if ((oriA >= 3) ^ (oriB >= 3)) {
			ori += 3;
		}
		prod.ca[corn] = (char) ((a.ca[(b.ca[corn] & 7)] & 7) | ori << 3);
	}
}

// prod = a * b, Edge Only.
void CubieCube::EdgeMult(const CubieCube& a, const CubieCube& b,
CubieCube& prod) {
	for (int ed = 0; ed < 12; ed++) {
		prod.ea[ed] = (char) (a.ea[b.ea[ed] >> 1] ^ (b.ea[ed] & 1));
	}
}

// b = S_idx^-1 * a * S_idx, Corner Only.
void CubieCube::CornConjugate(const CubieCube& a, int idx, CubieCube& b) {
	CubieCube sinv = CubeSym[SymInv[idx]];
	CubieCube s = CubeSym[idx];
	for (int corn = 0; corn < 8; corn++) {
		int oriA = sinv.ca[a.ca[s.ca[corn] & 7] & 7] >> 3;
		int oriB = a.ca[(s.ca[corn] & 7)] >> 3;
		int ori = (oriA < 3) ? oriB : (3 - oriB) % 3;
		b.ca[corn] = (char) ((sinv.ca[(a.ca[(s.ca[corn] & 7)] & 7)] & 7) | ori << 3);
	}
}

// b = S_idx^-1 * a * S_idx, Edge Only.
void CubieCube::EdgeConjugate(const CubieCube& a, int idx, CubieCube& b) {
	CubieCube sinv = CubeSym[SymInv[idx]];
	CubieCube s = CubeSym[idx];
	for (int ed = 0; ed < 12; ed++) {
		b.ea[ed] = (char) (sinv.ea[a.ea[s.ea[ed] >> 1] >> 1] ^ ((a.ea[s.ea[ed] >> 1] & 1)) ^ (s.ea[ed] & 1));
	}
}

// this = S_urf^-1 * this * S_urf.
void CubieCube::URFConjugate() {
	if (!temps) {
		temps = new CubieCube();
	}
	CornMult(urf2, *this, *temps);
	CornMult(*temps, urf1, *this);
	EdgeMult(urf2, *this, *temps);
	EdgeMult(*temps, urf1, *this);

	delete temps;
}

// ********************************************* Get and set coordinates *********************************************
// XSym : Symmetry Coordnate of X. MUST be called after initialization of ClassIndexToRepresentantArrays.

// ++++++++++++++++++++ Phase 1 Coordnates ++++++++++++++++++++
// Flip : Orientation of 12 Edges. Raw[0, 2048) Sym[0, 336 * 8)
// Twist : Orientation of 8 Corners. Raw[0, 2187) Sym[0, 324 * 8)
// UDSlice : Positions of the 4 UDSlice edges, the order is ignored. [0, 495)

int CubieCube::getFlip() const {
	int idx = 0;
	for (int i = 0; i < 11; i++) {
		idx = idx << 1 | (ea[i] & 1);
	}
	return idx;
}

void CubieCube::setFlip(int idx) {
	int parity = 0;
	for (int i = 10; i >= 0; i--) {
		int val = (idx & 1);
		ea[i] = (char) ((ea[i] & 0xfe) | val);
		parity ^= val;
		idx >>= 1;
	}
	ea[11] = (char) ((ea[11] & 0xfe) | parity);
}

int CubieCube::getFlipSym() {
	if (!FlipR2S) {
		return FlipR2S[getFlip()];
	}
	if (!temps) {
		temps = new CubieCube();
	}
	for (int k = 0; k < 16; k += 2) {
		EdgeConjugate(*this, SymInv[k], *temps);
		int idx = *std::upper_bound(std::begin(FlipS2R), std::end(FlipS2R),
			(unsigned short) temps->getFlip());
		if (idx >= 0) {
			if (temps) delete temps;
			return idx << 3 | k >> 1;
		}
	}
	assert(false);
	return 0;
}

int CubieCube::getTwist() const {
	int idx = 0;
	for (int i = 0; i < 7; i++) {
		idx += (idx << 1) + (ca[i] >> 3);
	}
	return idx;
}

void CubieCube::setTwist(int idx) {
	int twst = 0;
	for (int i = 6; i >= 0; i--) {
		int val = idx % 3;
		ca[i] = (char) ((ca[i] & 0x7 )| (val << 3));
		twst += val;
		idx /= 3;
	}
	ca[7] = (char) ((ca[7] & 0x7 )| (((15 - twst) % 3) << 3));
}

int CubieCube::getTwistSym() {
	// if (TwistR2S != null) {
	// 	return TwistR2S[getTwist()];
	// }
	if (!temps) {
		temps = new CubieCube();
	}
	for (int k = 0; k < 16; k += 2) {
		CornConjugate(*this, SymInv[k], *temps);
		int idx = *std::upper_bound(std::begin(TwistS2R), std::end(TwistS2R),
		 	(unsigned short) temps->getTwist());
		if (idx >= 0) {
			if (temps) delete temps;
			return idx << 3 | k >> 1;
		}
	}
	assert(false);
	return 0;
}

int CubieCube::getUDSlice() const {
	return Util::getComb(ea, 8, true);
}

void CubieCube::setUDSlice(int idx) {
	Util::setComb(ea, idx, 8, true);
}

int CubieCube::getU4Comb() const {
	return Util::getComb(ea, 0, true);
}

int CubieCube::getD4Comb() const {
	return Util::getComb(ea, 4, true);
}

// ++++++++++++++++++++ Phase 2 Coordnates ++++++++++++++++++++
// EPerm : Permutations of 8 UD Edges. Raw[0, 40320) Sym[0, 2187 * 16)
// Cperm : Permutations of 8 Corners. Raw[0, 40320) Sym[0, 2187 * 16)
// MPerm : Permutations of 4 UDSlice Edges. [0, 24)

int CubieCube::getCPerm() const {
	return Util::get8Perm(ca, false);
}

void CubieCube::setCPerm(int idx) {
	Util::set8Perm(ca, idx, false);
}

int CubieCube::getCPermSym() {
	if (!EPermR2S) {
		int idx = EPermR2S[getCPerm()];
		return idx ^ e2c[(idx & 0xf)];
	}
	if (!temps) {
		temps = new CubieCube();
	}
	for (int k = 0; k < 16; k++) {
		CornConjugate(*this, SymInv[k], *temps);
		int idx = *std::upper_bound(std::begin(EPermS2R), std::end(EPermS2R),
			(unsigned short) temps->getCPerm());
		if (idx >= 0) {
			delete temps;
			return idx << 4 | k;
		}
	}
	assert(false);
	return 0;
}

int CubieCube::getEPerm() const {
	return Util::get8Perm(ea, true);
}

void CubieCube::setEPerm(int idx) {
	Util::set8Perm(ea, idx, true);
}

int CubieCube::getEPermSym() {
	if (!EPermR2S) {
		return EPermR2S[getEPerm()];
	}
	if (!temps) {
		temps = new CubieCube();
	}
	for (int k = 0; k < 16; k++) {
		EdgeConjugate(*this, SymInv[k], *temps);
		int idx = *std::upper_bound(std::begin(EPermS2R), std::end(EPermS2R), (char) temps->getEPerm());
		if (idx >= 0) {
		delete temps;
			return idx << 4 | k;
		}
	}
	return 0;
}

int CubieCube::getMPerm() const {
	return Util::getComb(ea, 8, true) >> 9;
}

void CubieCube::setMPerm(int idx) {
	Util::setComb(ea, idx << 9, 8, true);
}

int CubieCube::getCComb() const {
	return 69 - (Util::getComb(ca, 0, false) & 0x1ff);
}

void CubieCube::setCComb(int idx) {
	Util::setComb(ca, 69 - idx, 0, false);
}

/**
 * Check a cubiecube for solvability. Return the error code.
 * 0: Cube is solvable
 * -2: Not all 12 edges exist exactly once
 * -3: Flip error: One edge has to be flipped
 * -4: Not all corners exist exactly once
 * -5: Twist error: One corner has to be twisted
 * -6: Parity error: Two corners or two edges have to be exchanged
 */
int CubieCube::verify() {
	int sum = 0;
	int edgeMask = 0;
	for (int e = 0; e < 12; e++) {
		edgeMask |= 1 << (ea[e] >> 1);
		sum ^= ea[e] & 1;
	}
	if (edgeMask != 0xfff) {
		return -2;// missing edges
	}
	if (sum != 0) {
		return -3;
	}
	int cornMask = 0;
	sum = 0;
	for (int c = 0; c < 8; c++) {
		cornMask |= 1 << (ca[c] & 7);
		sum += ca[c] >> 3;
	}
	if (cornMask != 0xff) {
		return -4;// missing corners
	}
	if (sum % 3 != 0) {
		return -5;// twisted corner
	}
	if ((Util::getNParity(Util::getNPerm(ea, 12, true), 12) ^ Util::getNParity(getCPerm(), 8)) != 0) {
		return -6;// parity error
	}
	return 0;// cube ok
}

long CubieCube::selfSymmetry() {
	CubieCube c(*this);
	CubieCube d;
	long sym = 0L;
	for (int i = 0; i < 48; i++) {
		CornConjugate(c, SymInv[i % 16], d);
		if (this->equalsCorn(d)) {
			EdgeConjugate(c, SymInv[i % 16], d);
			if (this->equalsEdge(d)) {
				sym |= 1L << i;
			}
		}
		if (i % 16 == 15) {
			c.URFConjugate();
		}
	}
	c.invCubieCube();
	for (int i = 0; i < 48; i++) {
		CornConjugate(c, SymInv[i % 16], d);
		if (d.equalsCorn(*this)) {
			EdgeConjugate(c, SymInv[i % 16], d);
			if (d.equalsEdge(*this)) {
				sym |= 1L << 48;
				break;
			}
		}
		if (i % 16 == 15) {
			c.URFConjugate();
		}
	}
	return sym;
}

void CubieCube::setUDSliceFlip(int idx) {
	setFlip(idx & 0x7ff);
	setUDSlice(idx >> 11);
}

int CubieCube::getUDSliceFlip() const {
	return (getUDSlice() & 0x1ff) << 11 | getFlip();
}
//
// int CubieCube::getUDSliceFlipSym() {
// 	int flip = getFlipSym();
// 	int fsym = flip & 0x7;
// 	flip >>= 3;
// 	int udslice = getUDSlice() & 0x1ff;
// 	int udsliceflip = FlipSlice2UDSliceFlip[flip * 495 + CoordinateCube::UDSliceConj[udslice][fsym]];
// 	return udsliceflip & 0xfffffff0 | SymMult[udsliceflip & 0xf][fsym << 1];
// }

// ********************************************* Initialization functions *********************************************

void CubieCube::initMove() {
	moveCube[0] = CubieCube(15120, 0, 119750400, 0);
	moveCube[3] = CubieCube(21021, 1494, 323403417, 0);
	moveCube[6] = CubieCube(8064, 1236, 29441808, 550);
	moveCube[9] = CubieCube(9, 0, 5880, 0);
	moveCube[12] = CubieCube(1230, 412, 2949660, 0);
	moveCube[15] = CubieCube(224, 137, 328552, 137);
	for (int a = 0; a < 18; a += 3) {
		for (int p = 0; p < 2; p++) {
			moveCube[a + p + 1] = CubieCube();
			EdgeMult(moveCube[a + p], moveCube[a], moveCube[a + p + 1]);
			CornMult(moveCube[a + p], moveCube[a], moveCube[a + p + 1]);
		}
	}
}

void CubieCube::initSym() {
	CubieCube c;
	CubieCube d;
	CubieCube t;

	CubieCube f2 = CubieCube(28783, 0, 259268407, 0);
	CubieCube u4 = CubieCube(15138, 0, 119765538, 7);
	CubieCube lr2 = CubieCube(5167, 0, 83473207, 0);
	for (int i = 0; i < 8; i++) {
		lr2.ca[i] |= 3 << 3;
	}

	for (int i = 0; i < 16; i++) {
		CubeSym[i] = CubieCube(c);
		CornMult(c, u4, d);
		EdgeMult(c, u4, d);
		t = d;  d = c;  c = t;
		if (i % 4 == 3) {
			CornMult(c, lr2, d);
			EdgeMult(c, lr2, d);
			t = d;  d = c;  c = t;
		}
		if (i % 8 == 7) {
			CornMult(c, f2, d);
			EdgeMult(c, f2, d);
			t = d;  d = c;  c = t;
		}
	}
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			CornMult(CubeSym[i], CubeSym[j], c);
			for (int k = 0; k < 16; k++) {
				if (CubeSym[k].equalsCorn(c)) {
					SymMult[i][j] = k;
					if (k == 0) {
						SymInv[i] = j;
					}
					break;
				}
			}
		}
	}
	for (int j = 0; j < 18; j++) {
		for (int s = 0; s < 16; s++) {
			CornConjugate(moveCube[j], SymInv[s], c);
			for (int m = 0; m < 18; m++) {
				if (c.equalsCorn(moveCube[m])) {
					SymMove[s][j] = m;
					break;
				}
			}
		}
	}
	for (int s = 0; s < 16; s++) {
		for (int j = 0; j < 10; j++) {
			SymMoveUD[s][j] = Util::std2ud[SymMove[s][Util::ud2std[j]]];
		}
		for (int j = 0; j < 16; j++) {
			SymMultInv[j][s] = SymMult[j][SymInv[s]];
		}
	}
	for (int s = 0; s < 8; s++) {
		for (int j = 0; j < 8; j++) {
			Sym8Mult[s << 3 | j] = SymMult[j << 1][s << 1] >> 1;
			Sym8MultInv[j << 3 | s] = SymMult[j << 1][SymInv[s << 1]]>>1;
		}
		for (int j = 0; j < 18; j++) {
			Sym8Move[j << 3 | s] = SymMove[s << 1][j];
		}
	}
	for (int i = 0; i < 18; i++) {
		moveCubeSym[i] = moveCube[i].selfSymmetry();
	}
	for (int i = 0; i < 18; i++) {
		int j = i;
		for (int s = 0; s < 48; s++) {
			if (SymMove[s % 16][j] < i) {
				firstMoveSym[s] |= 1 << i;
			}
			if (s % 16 == 15) {
				j = urfMove[2][j];
			}
		}
	}
}

void CubieCube::initFlipSym2Raw() {
	CubieCube c;
	CubieCube d;
	int count = 0;
	FlipR2S[2048];
	for (int i = 0; i < 2048; i++) {
		if (FlipR2S[i] != 0) {
			continue;
		}
		c.setFlip(i);
		for (int s = 0; s < 16; s += 2) {
			EdgeConjugate(c, s, d);
			int idx = d.getFlip();
			if (idx == i) {
				SymStateFlip[count] |= 1 << (s >> 1);
			}
			FlipR2S[idx] = (char) (count << 3 | s >> 1);
			if (Search::USE_TWIST_FLIP_PRUN) {
				FlipS2RF[count << 3 | s >> 1] = (char) idx;
			}
		}
		FlipS2R[count++] = (char) i;
	}
	assert(count == 336);
}

// void CubieCube::initTwistSym2Raw() {
// ubieCube c;
// ubieCube d;
// nt count = 0;
// wistR2S[2187];
// or (int i = 0; i < 2187; i++) {
// if (TwistR2S[i] != 0) {
// 	continue;
// }
// c.setTwist(i);
// for (int s = 0; s < 16; s += 2) {
// 	CornConjugate(c, s, d);
// 	int idx = d.getTwist();
// 	if (idx == i) {
// 		SymStateTwist[count] |= 1 << (s >> 1);
// 	}
// 	TwistR2S[idx] = (char) (count << 3 | s >> 1);
// 	if (Search::EXTRA_PRUN_LEVEL > 0) {
// 		TwistS2RF[count << 3 | s >> 1] = (char) idx;
// 	}
// }
// TwistS2R[count++] = (char) i;
//
// ssert(count == 324);
//

char CubieCube::Perm2Comb[2768];

void CubieCube::initPermSym2Raw() {
	CubieCube c;
	CubieCube d;
	int count = 0;
	EPermR2S[40320];

	for (int i = 0; i < 40320; i++) {
		if (EPermR2S[i] != 0) {
			continue;
		}
		c.setEPerm(i);
		for (int s = 0; s < 16; s++) {
			EdgeConjugate(c, s, d);
			int idx = d.getEPerm();
			if (idx == i) {
				SymStatePerm[count] |= 1 << s;
			}
			int a = d.getU4Comb();
			int b = d.getD4Comb() >> 9;
			int m = 494 - (a & 0x1ff) + (a >> 9) * 70 + b * 1680;
			MtoEPerm[m] = EPermR2S[idx] = (char) (count << 4 | s);
			if (s == 0) {
				Perm2Comb[count] = (char) (494 - (a & 0x1ff));
			}
		}
		EPermS2R[count++] = (char) i;
	}
	assert(count == 2768);
}

// void CubieCube::initUDSliceFlipSym2Raw() {
// 	CubieCube c;
// 	CubieCube d;
// 	int occ[2048 * 495 >> 5];
// 	int count = 0;
// 	for (int i = 0; i < 2048 * 495; i++) {
// 		if ((occ[i >> 5] & 1 << (i & 0x1f)) != 0) {
// 			continue;
// 		}
// 		c.setUDSliceFlip(i);
// 		for (int s = 0; s < 16; s++) {
// 			EdgeConjugate(c, s, d);
// 			int idx = d.getUDSliceFlip();
// 			if (idx == i) {
// 				SymStateUDSliceFlip[count] |= 1 << s;
// 			}
// 			occ[idx >> 5] |= 1 << (idx & 0x1f);
// 			// int fidx = Arrays.binarySearch(FlipS2R, (char) (idx & 0x7ff));
// 			int fidx = *std::upper_bound(std::begin(FlipS2R), std::end(FlipS2R),
// 			 	(unsigned short) (idx & 0x7ff));
// 			if (fidx >= 0) {
// 				FlipSlice2UDSliceFlip[fidx * CoordinateCube::N_SLICE + (idx >> 11)] = count << 4 | s;
// 			}
// 		}
// 		UDSliceFlipS2R[count++] = i;
// 	}
// 	assert(count == 64430);
// }

std::string CubieCube::toString() const {
	std::string thePussy;
	thePussy.reserve(69); // save room for 69 ;)
	for (int i = 0; i < 8; i++) {
		thePussy.append("|" + std::to_string(ca[i] & 7) + " " + std::to_string(ca[i] >> 3));
	}
	thePussy.append("\n");
	for (int i = 0; i < 12; i++) {
		thePussy.append("|" + std::to_string(ea[i] >> 1) + " " + std::to_string(ea[i] & 1));
	}
	return thePussy;
}
