#include "cubieCube.h"

char CubieCube::urfMove[6][18] = {
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
		{6, 7, 8, 0, 1, 2, 3, 4, 5, 15, 16, 17, 9, 10, 11, 12, 13, 14},
		{3, 4, 5, 6, 7, 8, 0, 1, 2, 12, 13, 14, 15, 16, 17, 9, 10, 11},
		{2, 1, 0, 5, 4, 3, 8, 7, 6, 11, 10, 9, 14, 13, 12, 17, 16, 15},
		{8, 7, 6, 2, 1, 0, 5, 4, 3, 17, 16, 15, 11, 10, 9, 14, 13, 12},
		{5, 4, 3, 8, 7, 6, 2, 1, 0, 14, 13, 12, 17, 16, 15, 11, 10, 9}
};

int CubieCube::preMove[9] = {-1, Util::Rx1, Util::Rx3, Util::Fx1, Util::Fx3, Util::Lx1, Util::Lx3, Util::Bx1, Util::Bx3};

CubieCube::CubieCube(int cperm, int twist, int eperm, int flip) {
	this.setCPerm(cperm);
	this.setTwist(twist);
	Util::setNPerm(ea, eperm, 12, true);
	this.setFlip(flip);
}

bool CubieCube::operator==(const CubieCube& rhs)
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

bool CubieCube::equalsCorn(CubieCube c) {
	for (int i = 0; i < 8; i++) {
		if (ca[i] != c.ca[i]) {
			return false;
		}
	}
	return true;
}

bool CubieCube::equalsEdge(CubieCube c) {
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
		temps.ea[ea[edge] >> 1] = (char) (edge << 1 | ea[edge] & 1);
	}
	for (byte corn = 0; corn < 8; corn++) {
		int ori = ca[corn] >> 3;
		ori = 4 >> ori & 3; //0->0, 1->2, 2->1
		temps.ca[ca[corn] & 0x7] = (byte) (corn | ori << 3);
	}
	*this = *temps;
	delete temps;
}

// prod = a * b, Corner Only.
void CubieCube::CornMult(CubieCube a, CubieCube b, CubieCube prod) {
	for (int corn = 0; corn < 8; corn++) {
		int oriA = a.ca[b.ca[corn] & 7] >> 3;
		int oriB = b.ca[corn] >> 3;
		int ori = oriA;
		ori += (oriA < 3) ? oriB : 6 - oriB;
		ori %= 3;
		if ((oriA >= 3) ^ (oriB >= 3)) {
			ori += 3;
		}
		prod.ca[corn] = (byte) (a.ca[b.ca[corn] & 7] & 7 | ori << 3);
	}
}

// prod = a * b, Edge Only.
void CubieCube::EdgeMult(CubieCube a, CubieCube b, CubieCube prod) {
	for (int ed = 0; ed < 12; ed++) {
		prod.ea[ed] = (byte) (a.ea[b.ea[ed] >> 1] ^ (b.ea[ed] & 1));
	}
}