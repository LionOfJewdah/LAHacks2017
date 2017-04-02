#ifndef COORDINATECUBE_H
#define COORDINATECUBE_H
#include "cubieCube.h"
#include <vector>
#include <string>

constexpr int N_MOVES = 18;
constexpr int N_MOVES2 = 10;
constexpr int N_SLICE = 495;
constexpr int N_TWIST = 2187;
constexpr int N_TWIST_SYM = 324;
constexpr int N_FLIP = 2048;
constexpr int N_FLIP_SYM = 336;
constexpr int N_PERM = 40320;
constexpr int N_PERM_SYM = 2768;
constexpr int N_MPERM = 24;
constexpr int N_COMB = 70;

class CoordinateCube {
public:
	CoordinateCube() = default;
	CoordinateCube(CoordinateCube&&) = default;
	CoordinateCube(const CoordinateCube&) = default;
	CoordinateCube& operator=(CoordinateCube&&) = default;
	CoordinateCube& operator=(const CoordinateCube&) = default;
	CoordinateCube& operator=(const CubieCube& cc);
	~CoordinateCube() = default;

private:
	//phase1
	static unsigned short UDSliceMove[N_SLICE][N_MOVES];
	static unsigned short TwistMove[N_TWIST_SYM][N_MOVES];
	static unsigned short FlipMove[N_FLIP_SYM][N_MOVES];
	static unsigned short UDSliceConj[N_SLICE][8];
	static int UDSliceTwistPrun[N_SLICE * N_TWIST_SYM / 8 + 1];
	static int UDSliceFlipPrun[N_SLICE * N_FLIP_SYM / 8];
	static int TwistFlipPrun[N_FLIP * N_TWIST_SYM / 8];

	//phase2
	static unsigned short CPermMove[N_PERM_SYM][N_MOVES];
	static unsigned short EPermMove[N_PERM_SYM][N_MOVES2];
	static unsigned short MPermMove[N_MPERM][N_MOVES2];
	static unsigned short MPermConj[N_MPERM][16];
	static unsigned short CCombMove[N_COMB][N_MOVES];
	static unsigned short CCombConj[N_COMB][16];
	static int MCPermPrun[N_MPERM * N_PERM_SYM / 8];
	static int MEPermPrun[N_MPERM * N_PERM_SYM / 8];
	static int EPermCCombPrun[N_COMB * N_PERM_SYM / 8];

public:
	static void init();
	void calcPruning(bool isPhase1);
	int doMovePrun(CoordinateCube cc, int m, bool isPhase1);

	static void setPruning(int table[], int index, int value);
	static int getPruning(int table[], int index);

private:
	static void initCPermMove();
	static void initEPermMove();
	static void initMPermMoveConj();
	static void initCombMoveConj();

	static void initMEPermPrun();
	static void initMCPermPrun();
	static void initPermCombPrun();
	static void initFlipMove();
	static void initTwistMove();
	static void initUDSliceMoveConj();

	static void initTwistFlipPrun();
	static void initSliceTwistPrun();
	static void initSliceFlipPrun();

	static void initRawSymPrun(
	   int PrunTable[], const int INV_DEPTH,
	   const std::vector<std::string>& RawMove, const std::vector<std::string>& RawConj,
	   const std::vector<std::string>& SymMove, const std::string& SymState,
	   const int PrunFlag
	);

	int twist;
	int tsym;
	int flip;
	int fsym;
	int slice;
	int prun;
	friend class CubieCube;
};

#endif
