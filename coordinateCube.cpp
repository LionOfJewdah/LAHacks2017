#include "coordinateCube.h"

using namespace std;

void CoordinateCube::init() {
		CubieCube.initPermSym2Raw();

		initCPermMove();
		initEPermMove();
		initMPermMoveConj();
		initCombMoveConj();

		initMEPermPrun();
		initMCPermPrun();
		initPermCombPrun();

		CubieCube.initFlipSym2Raw();
		CubieCube.initTwistSym2Raw();
		initFlipMove();
		initTwistMove();
		initUDSliceMoveConj();

		initTwistFlipPrun();
		initSliceTwistPrun();
		initSliceFlipPrun();
}

void CoordinateCube::calcPruning(boolean isPhase1) {
		prun = max(
								max(
										getPruning(UDSliceTwistPrun, twist * N_SLICE + UDSliceConj[slice & 0x1ff][tsym]),getPruning(UDSliceFlipPrun,
																	flip * N_SLICE + UDSliceConj[slice & 0x1ff][fsym])),
									 getPruning(TwistFlipPrun,
													 twist << 11 | CubieCube.FlipS2RF[flip << 3 | CubieCube.Sym8MultInv[fsym << 3 | tsym]]); // twist multiplied by 2048
													 // flip is multiplied by 8 because CubieCube stores the symmetry class differently
}

		/*
		
		Uses bit operations to save time due to large number of function calls
		
		table[index >> 3] ^= (0xf ^ value) << ((index & 7) << 2);
		int indDiv8 = index / 8;
		int xorMask = 0b1111 ^ value;
		int indMod8 = index % 8;
		xorMask = xorMask * 2^(4*indMod8);
		table[indDiv8] = table[indDiv8] ^ xorMask;
		
		*/
void CoordinateCube::setPruning(int[] table, int index, int value) {
		 table[index >> 3] ^= (0xf ^ value) << ((index & 7) << 2);
}

int CoordinateCube::getPruning(int[] table, int index) {
		return table[index >> 3] >> ((index & 7) << 2) & 0xf;
}
		
		
void CoordinateCube::initRawSymPrun(
	 int[] PrunTable, const int INV_DEPTH,
	 const std::vector<std::string>& RawMove, const std::vector<std::string>& RawConj,
	 const std::vector<std::string>& SymMove, const std::string& SymState,
	 const int PrunFlag) 
{
		const int SYM_SHIFT = PrunFlag & 0xf;
		const bool SymSwitch = ((PrunFlag >> 4) & 1) == 1;
		const bool MoveMapSym = ((PrunFlag >> 5) & 1) == 1;
		const bool MoveMapRaw = ((PrunFlag >> 6) & 1) == 1;

		const int SYM_MASK = (1 << SYM_SHIFT) - 1;
		const int N_RAW = RawMove.length;
		const int N_SYM = SymMove.length;
		const int N_SIZE = N_RAW * N_SYM;
		const int N_MOVES = MoveMapRaw ? 10 : RawMove[0].length;

		for (int i = 0; i < (N_RAW * N_SYM + 7) / 8; i++) {
				PrunTable[i] = -1;
		}
		setPruning(PrunTable, 0, 0);

		int depth = 0;
		int done = 1;

		while (done < N_SIZE) {
				bool inv = depth > INV_DEPTH;
				int select = inv ? 0xf : depth;
				int check = inv ? depth : 0xf;
				depth++;
				int val = 0;
				for (int i = 0; i < N_SIZE; i++, val >>= 4) {
						if ((i & 7) == 0) {
								val = PrunTable[i >> 3];
								if (!inv && val == -1) {
										i += 7;
										continue;
								}
						}
						if ((val & 0xf) != select) {
								continue;
						}
						int raw = i % N_RAW;
						int sym = i / N_RAW;
						for (int m = 0; m < N_MOVES; m++) {
								int symx = SymMove[sym][MoveMapSym ? Util.ud2std[m] : m];
								int rawx = RawConj[RawMove[raw][MoveMapRaw ? Util.ud2std[m] : m] & 0x1ff][symx & SYM_MASK];
								symx >>= SYM_SHIFT;
								int idx = symx * N_RAW + rawx;
								if (getPruning(PrunTable, idx) != check) {
										continue;
								}
								done++;
								if (inv) {
										setPruning(PrunTable, i, depth);
										break;
								}
								setPruning(PrunTable, idx, depth);
								for (int j = 1, symState = SymState[symx]; (symState >>= 1) != 0; j++) {
										if ((symState & 1) != 1) {
												continue;
										}
										int idxx = symx * N_RAW + RawConj[rawx][j ^ (SymSwitch ? CubieCube.e2c[j] : 0)];
										if (getPruning(PrunTable, idxx) == 0xf) {
												setPruning(PrunTable, idxx, depth);
												done++;
										}
								}
						}
				}
		}
}

void CoordinateCube::initUDSliceMoveConj() {
		CubieCube c, d;
		for (int i = 0; i < N_SLICE; i++) {
				c.setUDSlice(i);
				for (int j = 0; j < N_MOVES; j += 3) {
						CubieCube.EdgeMult(c, CubieCube.moveCube[j], d);
						UDSliceMove[i][j] = (char) d.getUDSlice();
				}
				for (int j = 0; j < 16; j += 2) {
						CubieCube.EdgeConjugate(c, CubieCube.SymInv[j], d);
						UDSliceConj[i][j >> 1] = (char) (d.getUDSlice() & 0x1ff);
				}
		}
		for (int i = 0; i < N_SLICE; i++) {
				for (int j = 0; j < N_MOVES; j += 3) {
						int udslice = UDSliceMove[i][j];
						for (int k = 1; k < 3; k++) {
								int cx = UDSliceMove[udslice & 0x1ff][j];
								udslice = Util.permMult[udslice >> 9][cx >> 9] << 9 | cx & 0x1ff;
								UDSliceMove[i][j + k] = (char) udslice;
						}
				}
		}
}

void CoordinateCube::initFlipMove() {
		CubieCube c, d;
		for (int i = 0; i < N_FLIP_SYM; i++) {
				c.setFlip(CubieCube.FlipS2R[i]);
				for (int j = 0; j < N_MOVES; j++) {
						CubieCube.EdgeMult(c, CubieCube.moveCube[j], d);
						FlipMove[i][j] = (char) d.getFlipSym();
				}
		}
}

void CoordinateCube::initTwistMove() {
		CubieCube c, d;
		for (int i = 0; i < N_TWIST_SYM; i++) {
				c.setTwist(CubieCube.TwistS2R[i]);
				for (int j = 0; j < N_MOVES; j++) {
						CubieCube.CornMult(c, CubieCube.moveCube[j], d);
						TwistMove[i][j] = (char) d.getTwistSym();
				}
		}
}

void CoordinateCube::initCPermMove() {
		CubieCube c, d;
		for (int i = 0; i < N_PERM_SYM; i++) {
				c.setCPerm(CubieCube.EPermS2R[i]);
				for (int j = 0; j < N_MOVES; j++) {
						CubieCube.CornMult(c, CubieCube.moveCube[j], d);
						CPermMove[i][j] = (char) d.getCPermSym();
				}
		}
}

void CoordinateCube::initEPermMove() {
		CubieCube c, d;
		for (int i = 0; i < N_PERM_SYM; i++) {
				c.setEPerm(CubieCube.EPermS2R[i]);
				for (int j = 0; j < N_MOVES2; j++) {
						CubieCube.EdgeMult(c, CubieCube.moveCube[Util.ud2std[j]], d);
						EPermMove[i][j] = (char) d.getEPermSym();
				}
		}
}

void CoordinateCube::initMPermMoveConj() {
		CubieCube c, d;
		for (int i = 0; i < N_MPERM; i++) {
				c.setMPerm(i);
				for (int j = 0; j < N_MOVES2; j++) {
						CubieCube.EdgeMult(c, CubieCube.moveCube[Util.ud2std[j]], d);
						MPermMove[i][j] = (char) d.getMPerm();
				}
				for (int j = 0; j < 16; j++) {
						CubieCube.EdgeConjugate(c, CubieCube.SymInv[j], d);
						MPermConj[i][j] = (char) d.getMPerm();
				}
		}
}

void CoordinateCube::initCombMoveConj() {
		CubieCube c, d;
		for (int i = 0; i < N_COMB; i++) {
				c.setCComb(i);
				for (int j = 0; j < N_MOVES; j++) {
						CubieCube.CornMult(c, CubieCube.moveCube[j], d);
						CCombMove[i][j] = (char) d.getCComb();
				}
				for (int j = 0; j < 16; j++) {
						CubieCube.CornConjugate(c, CubieCube.SymInv[j], d);
						CCombConj[i][j] = (char) d.getCComb();
				}
		}
}

void CoordinateCube::initTwistFlipPrun() {
		int depth = 0;
		int done = 1;
		boolean inv;
		int select;
		int check;
		final int N_SIZE = N_FLIP * N_TWIST_SYM;
		for (int i = 0; i < N_SIZE / 8; i++) {
				TwistFlipPrun[i] = -1;
		}
		setPruning(TwistFlipPrun, 0, 0);

		while (done < N_SIZE) {
				inv = depth > 6;
				select = inv ? 0xf : depth;
				check = inv ? depth : 0xf;
				depth++;
				int val = 0;
				for (int i = 0; i < N_SIZE; i++, val >>= 4) {
						if ((i & 7) == 0) {
								val = TwistFlipPrun[i >> 3];
								if (!inv && val == -1) {
										i += 7;
										continue;
								}
						}
						if ((val & 0xf) != select) {
								continue;
						}
						int twist = i >> 11;
						int flip = CubieCube.FlipR2S[i & 0x7ff];
						int fsym = flip & 7;
						flip >>= 3;
						for (int m = 0; m < N_MOVES; m++) {
								int twistx = TwistMove[twist][m];
								int tsymx = twistx & 7;
								twistx >>= 3;
								int flipx = FlipMove[flip][CubieCube.Sym8Move[m << 3 | fsym]];
								int fsymx = CubieCube.Sym8MultInv[CubieCube.Sym8Mult[flipx & 7 | fsym << 3] << 3 | tsymx];
								flipx >>= 3;
								int idx = twistx << 11 | CubieCube.FlipS2RF[flipx << 3 | fsymx];
								if (getPruning(TwistFlipPrun, idx) != check) {
										continue;
								}
								done++;
								if (inv) {
										setPruning(TwistFlipPrun, i, depth);
										break;
								}
								setPruning(TwistFlipPrun, idx, depth);
								char sym = CubieCube.SymStateTwist[twistx];
								if (sym == 1) {
										continue;
								}
								for (int k = 0; k < 8; k++) {
										if ((sym & 1 << k) == 0) {
												continue;
										}
										int idxx = twistx << 11 | CubieCube.FlipS2RF[flipx << 3 | CubieCube.Sym8MultInv[fsymx << 3 | k]];
										if (getPruning(TwistFlipPrun, idxx) == 0xf) {
												setPruning(TwistFlipPrun, idxx, depth);
												done++;
										}
								}
						}
				}
		}
}

CoordinateCube& CoordinateCube::operator=(const CubieCube& cc) {
		twist = cc.getTwistSym();
		flip = cc.getFlipSym();
		slice = cc.getUDSlice();
		tsym = twist & 7;
		twist = twist >> 3;
		fsym = flip & 7;
		flip = flip >> 3;
		return *this;
}

void CoordinateCube::initSliceTwistPrun() {
		initRawSymPrun(
				UDSliceTwistPrun, 6,
				UDSliceMove, UDSliceConj,
				TwistMove, CubieCube.SymStateTwist, 0x3
		);
}

void CoordinateCube::initSliceFlipPrun() {
		initRawSymPrun(
				UDSliceFlipPrun, 6,
				UDSliceMove, UDSliceConj,
				FlipMove, CubieCube.SymStateFlip, 0x3
		);
}

void CoordinateCube::initMEPermPrun() {
		initRawSymPrun(
				MEPermPrun, 7,
				MPermMove, MPermConj,
				EPermMove, CubieCube.SymStatePerm, 0x4
		);
}

void CoordinateCube::initMCPermPrun() {
		initRawSymPrun(
				MCPermPrun, 10,
				MPermMove, MPermConj,
				CPermMove, CubieCube.SymStatePerm, 0x34
		);
}

void CoordinateCube::initPermCombPrun() {
		initRawSymPrun(
				EPermCCombPrun, 8,
				CCombMove, CCombConj,
				EPermMove, CubieCube.SymStatePerm, 0x44
		);
}

int CoordinateCube::doMovePrun(CoordCube cc, int m, boolean isPhase1) {
		slice = UDSliceMove[cc.slice & 0x1ff][m] & 0x1ff;

		flip = FlipMove[cc.flip][CubieCube.Sym8Move[m << 3 | cc.fsym]];
		fsym = CubieCube.Sym8Mult[flip & 7 | cc.fsym << 3];
		flip >>= 3;

		twist = TwistMove[cc.twist][CubieCube.Sym8Move[m << 3 | cc.tsym]];
		tsym = CubieCube.Sym8Mult[twist & 7 | cc.tsym << 3];
		twist >>= 3;

		prun = std::max(
							 std::max(
									 getPruning(UDSliceTwistPrun,
															twist * N_SLICE + UDSliceConj[slice][tsym]),
									 getPruning(UDSliceFlipPrun,
															flip * N_SLICE + UDSliceConj[slice][fsym])),
													getPruning(TwistFlipPrun, twist << 11 | 
															CubieCube.FlipS2RF[flip << 3 | CubieCube.Sym8MultInv[fsym << 3 | tsym]]));
		return prun;
}