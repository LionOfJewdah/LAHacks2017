#include <string>
class CubieCube;
// #include "cubieCube.h"
#include <cstring>

namespace Util {
	bool initedUtil = false;

	constexpr long fact[14] = {1, 1, 2, 6, 24, 120, 720, 5040,
		40320, 362880, 3628800, 39916800, 479001600, 6227020800
	};

	void setNPerm(const char arr[], int idx, int n, bool isEdge);
	int getNPerm(const char arr[], int n, bool isEdge);

	//Moves
	constexpr char Ux1 = 0;
	constexpr char Ux2 = 1;
	constexpr char Ux3 = 2;
	constexpr char Rx1 = 3;
	constexpr char Rx2 = 4;
	constexpr char Rx3 = 5;
	constexpr char Fx1 = 6;
	constexpr char Fx2 = 7;
	constexpr char Fx3 = 8;
	constexpr char Dx1 = 9;
	constexpr char Dx2 = 10;
	constexpr char Dx3 = 11;
	constexpr char Lx1 = 12;
	constexpr char Lx2 = 13;
	constexpr char Lx3 = 14;
	constexpr char Bx1 = 15;
	constexpr char Bx2 = 16;
	constexpr char Bx3 = 17;

	//Facelets
	constexpr char U1 = 0;
	constexpr char U2 = 1;
	constexpr char U3 = 2;
	constexpr char U4 = 3;
	constexpr char U5 = 4;
	constexpr char U6 = 5;
	constexpr char U7 = 6;
	constexpr char U8 = 7;
	constexpr char U9 = 8;
	constexpr char R1 = 9;
	constexpr char R2 = 10;
	constexpr char R3 = 11;
	constexpr char R4 = 12;
	constexpr char R5 = 13;
	constexpr char R6 = 14;
	constexpr char R7 = 15;
	constexpr char R8 = 16;
	constexpr char R9 = 17;
	constexpr char F1 = 18;
	constexpr char F2 = 19;
	constexpr char F3 = 20;
	constexpr char F4 = 21;
	constexpr char F5 = 22;
	constexpr char F6 = 23;
	constexpr char F7 = 24;
	constexpr char F8 = 25;
	constexpr char F9 = 26;
	constexpr char D1 = 27;
	constexpr char D2 = 28;
	constexpr char D3 = 29;
	constexpr char D4 = 30;
	constexpr char D5 = 31;
	constexpr char D6 = 32;
	constexpr char D7 = 33;
	constexpr char D8 = 34;
	constexpr char D9 = 35;
	constexpr char L1 = 36;
	constexpr char L2 = 37;
	constexpr char L3 = 38;
	constexpr char L4 = 39;
	constexpr char L5 = 40;
	constexpr char L6 = 41;
	constexpr char L7 = 42;
	constexpr char L8 = 43;
	constexpr char L9 = 44;
	constexpr char B1 = 45;
	constexpr char B2 = 46;
	constexpr char B3 = 47;
	constexpr char B4 = 48;
	constexpr char B5 = 49;
	constexpr char B6 = 50;
	constexpr char B7 = 51;
	constexpr char B8 = 52;
	constexpr char B9 = 53;

	//Colors
	constexpr char U = 0;
	constexpr char R = 1;
	constexpr char F = 2;
	constexpr char D = 3;
	constexpr char L = 4;
	constexpr char B = 5;

	constexpr char cornerFacelet[8][3] = {
		{ U9, R1, F3 }, { U7, F1, L3 }, { U1, L1, B3 }, { U3, B1, R3 },
		{ D3, F9, R7 }, { D1, L9, F7 }, { D7, B9, L7 }, { D9, R9, B7 }
	};
	constexpr char edgeFacelet[12][2] = {
		{ U6, R2 }, { U8, F2 }, { U4, L2 }, { U2, B2 }, { D6, R8 }, { D2, F8 },
		{ D4, L8 }, { D8, B8 }, { F6, R4 }, { F4, L6 }, { B6, L4 }, { B4, R6 }
	};

	int preMove[] = { -1, Rx1, Rx3, Fx1, Fx3, Lx1, Lx3, Bx1, Bx3};
	int ud2std[] = {Ux1, Ux2, Ux3, Rx2, Fx2, Dx1, Dx2, Dx3, Lx2, Bx2};
	int std2ud [18];
	bool ckmv2 [11][10];
	int Cnk[13][13];
	int permMult[24][24];

	void initUtil() {
		if (initedUtil) return;
		for (int i = 0; i < 10; i++) {
			std2ud[ud2std[i]] = i;
		}
		for (int i = 0; i < 10; i++) {
			int ix = ud2std[i];
			for (int j = 0; j < 10; j++) {
				int jx = ud2std[j];
				ckmv2[i][j] = (ix / 3 == jx / 3) || ((ix / 3 % 3 == jx / 3 % 3) && (ix >= jx));
			}
			ckmv2[10][i] = false;
		}
		for (int i = 0; i < 13; i++) {
			Cnk[i][0] = Cnk[i][i] = 1;
			for (int j = 1; j < i; j++) {
				Cnk[i][j] = Cnk[i - 1][j - 1] + Cnk[i - 1][j];
			}
		}
		char arr1[4], arr2[4], arr3[4];
		for (int i = 0; i < 24; i++) {
			setNPerm(arr1, i, 4, false);
			for (int j = 0; j < 24; j++) {
				setNPerm(arr2, j, 4, false);
				for (int k = 0; k < 4; k++) {
					arr3[k] = arr1[arr2[k]];
				}
				permMult[i][j] = getNPerm(arr3, 4, false);
			}
		}
		initedUtil = true;
	}


   void toCubieCube(char f[], CubieCube& ccRet) {
		char ori;
		for (int i = 0; i < 8; i++)
			ccRet.ca[i] = 0;// invalidate corners
		for (int i = 0; i < 12; i++)
			ccRet.ea[i] = 0;// and edges
		char col1, col2;
		for (char i = 0; i < 8; i++) {
			// get the colors of the cubie at corner i, starting with U/D
			for (ori = 0; ori < 3; ori++)
				if (f[cornerFacelet[i][ori]] == U || f[cornerFacelet[i][ori]] == D)
					break;
			col1 = f[cornerFacelet[i][(ori + 1) % 3]];
			col2 = f[cornerFacelet[i][(ori + 2) % 3]];

			for (char j = 0; j < 8; j++) {
				if (col1 == cornerFacelet[j][1] / 9 && col2 == cornerFacelet[j][2] / 9) {
					// in cornerposition i we have cornercubie j
					ccRet.ca[i] = (char) (ori % 3 << 3 | j);
					break;
				}
			}
		}
		for (char i = 0; i < 12; i++) {
			for (char j = 0; j < 12; j++) {
				if (f[edgeFacelet[i][0]] == edgeFacelet[j][0] / 9
						&& f[edgeFacelet[i][1]] == edgeFacelet[j][1] / 9) {
					ccRet.ea[i] = (char) (j << 1);
					break;
				}
				if (f[edgeFacelet[i][0]] == edgeFacelet[j][1] / 9
						&& f[edgeFacelet[i][1]] == edgeFacelet[j][0] / 9) {
					ccRet.ea[i] = (char) (j << 1 | 1);
					break;
				}
			}
		}
	}

	std::string toFaceCube(CubieCube& cc) {
		char f[54];
		char ts[] = {'U', 'R', 'F', 'D', 'L', 'B'};
		for (int i = 0; i < 54; i++) {
			f[i] = ts[i / 9];
		}
		for (char c = 0; c < 8; c++) {
			int j = cc.ca[c] & 0x7;// cornercubie with index j is at
			// cornerposition with index c
			int ori = cc.ca[c] >> 3;// Orientation of this cubie
			for (char n = 0; n < 3; n++)
				f[cornerFacelet[c][(n + ori) % 3]] = ts[cornerFacelet[j][n] / 9];
		}
		for (char e = 0; e < 12; e++) {
			int j = cc.ea[e] >> 1;// edgecubie with index j is at edgeposition
			// with index e
			int ori = cc.ea[e] & 1;// Orientation of this cubie
			for (char n = 0; n < 2; n++)
				f[edgeFacelet[e][(n + ori) % 2]] = ts[edgeFacelet[j][n] / 9];
		}
		return f;
	}

	int getNParity(int idx, int n) {
		int p = 0;
		for (int i = n - 2; i >= 0; i--) {
			p ^= idx % (n - i);
			idx /= (n - i);
		}
		return p & 1;
	}

	char setVal(int val0, int val, bool isEdge) {
		return (char) (isEdge ? (val << 1 | val0 & 1) : (val | val0 & 0xf8));
	}

	int getVal(int val0, bool isEdge) {
		return isEdge ? val0 >> 1 : val0 & 7;
	}

	void set8Perm(char arr[], int idx, bool isEdge) {
		int val = 0x76543210;
		for (int i = 0; i < 7; i++) {
			int p = fact[7 - i];
			int v = idx / p;
			idx -= v * p;
			v <<= 2;
			arr[i] = setVal(arr[i], (val >> v & 0x7), isEdge);
			int m = (1 << v) - 1;
			val = val & m | val >> 4 & ~m;
		}
		arr[7] = setVal(arr[7], val, isEdge);
	}

	int get8Perm(char arr[], bool isEdge) {
		int idx = 0;
		int val = 0x76543210;
		for (int i = 0; i < 7; i++) {
			int v = getVal(arr[i], isEdge) << 2;
			idx = (8 - i) * idx + (val >> v & 0x7);
			val -= 0x11111110 << v;
		}
		return idx;
	}

	void setNPerm(char arr[], int idx, int n, bool isEdge) {
		arr[n - 1] = setVal(arr[n - 1], 0, isEdge);
		for (int i = n - 2; i >= 0; i--) {
			int arri = idx % (n - i);
			arr[i] = setVal(arr[i], arri, isEdge);
			idx /= (n - i);
			for (int j = i + 1; j < n; j++) {
				int arrj = getVal(arr[j], isEdge);
				if (arrj >= arri) {
					arr[j] = setVal(arr[j], ++arrj, isEdge);
				}
			}
		}
	}

	int getNPerm(char arr[], int n, bool isEdge) {
		int idx = 0;
		for (int i = 0; i < n; i++) {
			idx *= (n - i);
			int arri = getVal(arr[i], isEdge);
			for (int j = i + 1; j < n; j++) {
				int arrj = getVal(arr[j], isEdge);
				if (arrj < arri) {
					idx++;
				}
			}
		}
		return idx;
	}

	int getComb(char arr[], int mask, bool isEdge) {
		int end = std::strlen(arr);
		int idxC = 0, idxP = 0, r = 4, val = 0x0123;
		for (int i = end; i >= 0; i--) {
			int perm = getVal(arr[i], isEdge);
			if ((perm & 0xc) == mask) {
				int v = (perm & 3) << 2;
				idxP = r * idxP + (val >> v & 0xf);
				val -= 0x0111 >> (12 - v);
				idxC += Cnk[i][r--];
			}
		}
		return idxP << 9 | Cnk[end + 1][4] - 1 - idxC;
	}

	int setComb(char arr[], int idx, int mask, bool isEdge) {
		int end = std::strlen(arr);
		int r = 4, fill = end, val = 0x0123;
		int idxC = Cnk[end+1][4] - 1 - (idx & 0x1ff);
		int idxP = idx >> 9;
		for (int i = end; i >= 0; i--) {
			if (idxC >= Cnk[i][r]) {
				idxC -= Cnk[i][r--];
				int p = fact[r];
				int v = idxP / p << 2;
				idxP %= p;
				arr[i] = setVal(arr[i], val >> v & 3 | mask, isEdge);
				int m = (1 << v) - 1;
				val = val & m | val >> 4 & ~m;
			} else {
				if ((fill & 0xc) == mask) {
					fill -= 4;
				}
				arr[i] = setVal(arr[i], fill--, isEdge);
			}
		}
	}

}
