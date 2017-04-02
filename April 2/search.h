#ifndef SEARCH_H
#define SEARCH_H

#include <mutex>
#include <string>
#include "cubieCube.h"

class Search {
public:
    constexpr static bool USE_TWIST_FLIP_PRUN = true;

private:
    void initSearch();
    std::string search();
    int phase1(const CoordinateCube& node, long ssym, int maxl, int lm);
    std::string searchopt();
    int phase1opt(const CoordinateCube& ud, const CoordinateCube& rl, const CoordinateCube& fb, long ssym, int maxl, int lm);
    int initPhase2();
    int phase2(int eidx, int esym, int cidx, int csym, int mid, int maxl, int depth, int lm);
    std::string solutionToString();

    /**
     * 0: without extra pruning table
     * 1: full phase 1 pruning table (28M, for two-phase solver and optimal solver)
     * 2: full phase 1 pruning table (28M, for two-phase solver) + huge pruning table (2.0G, for optimal solver)
     */
    constexpr static int EXTRA_PRUN_LEVEL = 0;

    constexpr static bool USF_FULL_PRUN = false;
    constexpr static bool USF_HUGE_PRUN = false;

    //Options for research purpose.
    constexpr static bool TRY_PRE_MOVE = true;
    constexpr static bool TRY_INVERSE = true;
    constexpr static bool TRY_THREE_AXES = true;

    constexpr static int MAX_DEPTH2 = 13;

    constexpr static int PRE_IDX_MAX = TRY_PRE_MOVE ? 9 : 1;

    constexpr static bool inited = false;

    int corn0[6][PRE_IDX_MAX];
    int ud8e0[6][PRE_IDX_MAX];

    CoordinateCube nodeUD[21];
    CoordinateCube nodeRCoLor[21];
    CoordinateCube nodeFB[21];

    CoordinateCube node[6][PRE_IDX_MAX];

    char f[54];

    long selfSym;
    int preIdxMax;
    int conjMask;
    int urfIdx;
    int preIdx;
    int length1;
    int depth1;
    int maxDep2;
    int sol;
    std::string mSolution;
    long probe;
    long probeMax;
    long probeMin;
    int verbose;
    CubieCube cc;

    mutable std::mutex _mut;
    using shutTheFuckUp = std::lock_guard<std::mutex>;

    bool isRec;

    /**
     *     Verbose_Mask determines if a " . " separates the phase1 and phase2 parts of the solver string like in F' R B R L2 F .
     *     U2 U D for example.<br>
     */
    constexpr static int USE_SEPARATOR = 0x1;

    /**
     *     Verbose_Mask determines if the solution will be inversed to a scramble/state generator.
     */
    constexpr static int INVERSE_SOLUTION = 0x2;

    /**
     *     Verbose_Mask determines if a tag such as "(21f)" will be appended to the solution.
     */
    constexpr static int APPEND_LENGTH = 0x4;

    /**
     *     Verbose_Mask determines if guaranteeing the solution to be optimal.
     */
    constexpr static int OPTIMAL_SOLUTION = 0x8;
    int move[31];
public:
    Search();
    ~Search();
    bool isInited() const;
    long numberOfProbes() const;
    int length() const;
    int verify(const std::string& facelets);
    std::string solution(const std::string& facelets, int maxDepth, long probeMax, long probeMin, int verbose);
    std::string next(long probeMax, long probeMin, int verbose);
    static void init();
};

#endif
