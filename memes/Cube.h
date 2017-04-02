/*
 *
 *----------------------------------------------------------------------------
 * Rubik Cube solver.
 * Copyright (C) Marek Karcz 2015. All rights reserved.
 *
 * License agreement between the User and the Author:
 *
 * This software and its source code is free to use and distribute
 * for non-commercial purposes only and as long as following conditions
 * are met:
 *
 * - this copyright notice in its entirety along with
 *   the license agreement and disclaimer notice are included
 *   in the distributed package,
 *
 * - the source code is included in distributed package.
 *
 * User is free to modify the code and distribute the modified code
 * as long as the modifications are properly documented and also
 * included in the distributed package.
 * Following forms of documentation of the changes to the code are
 * accepted:
 *
 * - source code comments,
 *
 * - description of the changes in the form of a readme document
 *   in any commonly used format (text, ms office, open office).
 *
 * Disclaimer:
 *
 * No warranty is offered in any form. Author can not be held
 * responsible for any damage caused by the software.
 * Use at your own risk.
 *----------------------------------------------------------------------------
 *
 */

#ifndef _CUBE_H
#define _CUBE_H

#define DOS 1
//#define UNIX
#define SOCKET_PORT 5025

#include <string>
#include <vector>
#include <map>
#if defined(DOS)
#include <windows.h>
#include <psapi.h>
#endif

#if defined(UNIX)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#include "RCube.h"
#include "MemMgm.h"

using namespace std;

namespace Rubik
{

//#define MYDBG2	1
//#define MYDBG3	1
//#define MYDBG5	1
//#define MYDBG8	1
//#define MYDBG9	1
//#define MYDBG10	1
//#define MYDBG11	1
//#define MYDBG12	1
//--------------------
/*
#define MYDBG 	1
#define MYDBG4	1
#define MYDBG6	1
#define MYDBG7	1
#define MYDBG13	1
*/
//#define MYDBG 	1
//#define MYDBG3	1
//#define MYDBG14	1
//#define MYDBG6	1
//#define MYDBG15	1
//#define MYDBG16	1
//#define MYDBG17	1
//#define MYDBG18	1
//#define MYDBG19	1
//#define MYDBG20	1
//#define MYDBG21	1
//#define MYDBG22


#define NOVISNODESMAP	1

#define KNOWN_PATHS_FN	"KnownPaths.map"
#define PRUNE_G1_FN		"PruneG1.cache"
#define PRUNE_G2_FN		"PruneG2.cache"
#define PRUNE_G3_FN		"PruneG3.cache"
#define PRUNE_G4_FN		"PruneG4.cache"
#define LOOKUP_G1_M_FN	"LookupG1m.cache"
#define LOOKUP_G2_N_FN	"LookupG2n.cache"
#define LOOKUP_G2_M_FN	"LookupG2m.cache"
//#define LOOKUP_G3_N_FN	"LookupG3n.cache"
#define LOOKUP_G3_M_FN	"LookupG3m.cache"
#define CORNPERM_G3_FN	"CornPermG3.cache"
#define LOOKUP_G4_N_FN	"LookupG4n.cache"
#define LOOKUP_G4_M_FN	"LookupG4m.cache"
#define PRUNE_G1_SIZE	2048
#define NUM_OF_EDGES		12
#define NUM_OF_CORNERS	8
#define PRUNE_G2_SIZE_N	2187
#define PRUNE_G2_SIZE_M	495
#define PRUNE_G3_SIZE_N	40320
#define PRUNE_G3_SIZE_M	70
#define PRUNE_G4_SIZE_N	96
#define PRUNE_G4_SIZE_M	6912


/*
 * All possible moves in format:
 * face_plane_direction
 * NOTE: all moves directions are from the perspective while cube is oriented
 *       toward the observer with its front face
 */
enum Moves {
    FRONT_TOP_RIGHT = 0,
    FRONT_TOP_LEFT,
    FRONT_LEFT_UP,
    FRONT_LEFT_DOWN,
    FRONT_UP_RIGHT,
    FRONT_UP_LEFT,
    FRONT_RIGHT_UP,
    FRONT_RIGHT_DOWN,
    FRONT_DOWN_RIGHT,
    FRONT_DOWN_LEFT,
    BOTTOM_TOP_RIGHT, 	// this means twist right while looking from the front side
    BOTTOM_TOP_LEFT  	// this means twist left while looking from the front side
    //MAX_MOVES
};

// Movements in the standardazed notation (see http://solvethecube.com/notation).
enum MovesN {
    MOVE_F = 0,
    MOVE_Fi,
    MOVE_Li,
    MOVE_L,
    MOVE_Ui,
    MOVE_U,
    MOVE_R,
    MOVE_Ri,
    MOVE_D,
    MOVE_Di,
    MOVE_Bi, 	// this means twist right while looking from the front side
    MOVE_B,  	// this means twist left while looking from the front side
    MAX_MOVES	// keep it last
};

// Double (180 deg) moves. The order ust agree with equivalent quater-turn moves
// in MovesN. Note that double-turn reverse moves are defined for symmetry only
// however they are not used in code and not handled in related methods as they
// are equivalent to the forward move double-turns.
enum MovesD {
	MOVE_F2 = MAX_MOVES,
	MOVE_F2i,
	MOVE_L2i,	
	MOVE_L2,
	MOVE_U2i,	
	MOVE_U2,
	MOVE_R2,
	MOVE_R2i,
	MOVE_D2,
	MOVE_D2i,
	MOVE_B2i,	
	MOVE_B2,
	MAX_DMOVES // keep this one at last position
};

enum Faces {
    U0=0, U1, U2,
    U3, U4, U5,
    U6, U7, U8,
    L0, L1, L2, F0, F1, F2, R0, R1, R2, B0, B1, B2,
    L3, L4, L5, F3, F4, F5, R3, R4, R5, B3, B4, B5,
    L6, L7, L8, F6, F7, F8, R6, R7, R8, B6, B7, B8,
    D0, D1, D2,
    D3, D4, D5,
    D6, D7, D8,

    FACES_END
};

enum Methods {
    TreeSearch = 0,
    TreeSearchHeur,
    TreeSearchBackTrace,
    MultiStepHuman,
    TreeSearchLookup,
    MAX_METHODS	// keep it last
};

typedef map<string,long> 				MapVisitedNodes;
typedef map<string,long>::iterator		MapVisitedNodesIter;
typedef map<string,string>				MapKnownPaths;
typedef map<string,string>::iterator	MapKnownPathsIter;

// Corner Permutations Structure (for use in vector).
struct CPS {
	int cp[NUM_OF_CORNERS];
};

// Corner Orientations Structure (for use in vector).
struct COS {
	int co[NUM_OF_CORNERS];
};

// Edges Permutation Structure (for use in vector).
struct EPS {
	int ep[NUM_OF_EDGES];
};

// Edges Orientation Structure (for use in vector).
struct EOS {
	int eo[NUM_OF_EDGES];
};

// Cubie model state.
struct CubieModelState {
	int ep[NUM_OF_EDGES];	// edges permutation state
	int eo[NUM_OF_EDGES];	// edges orientation state
	int cp[NUM_OF_CORNERS];	// corners permutation state
	int co[NUM_OF_CORNERS];	// corners orientation state
};

// Aid in pruning table generation.
struct indice {
	int x;
	int y;
};

class Cube
{
		RCube 				*m_pRoot;
		RCube 				*m_pCurrent;
		RCube 				*m_pGoal;
		RCube 				*m_pSolved;
		int    				m_nDepth;
		bool   				m_bSolved;
		long   				m_lNodesChecked;
		long	 			m_lVisitedNodesSkip;
		int    				m_nMethod;
		bool   				m_bShow;
		MapKnownPaths		m_MapKnownPaths;
		bool          		m_bMono;						// monochrome/color
		bool				m_bUpdateCache;					// update known solutions map or not
		bool				m_bCacheLoaded;
		double				m_fSolvedSecs;					// time taken to solve (seconds)
		int					m_nSolLen;						// length of solution
		MemMgm				*m_pMemMgm;
		// prune tables for Thistlethwaite's 45 alg.
		int					m_naPruneG1[PRUNE_G1_SIZE];
		//map<int,int> 		m_MapIndex2mG1; // this map is the lookup index to the 'm' column of prune table
		int					m_naPruneG2[PRUNE_G2_SIZE_N][PRUNE_G2_SIZE_M];
		map<int,int> 		m_MapIndex2nG2; // this map is the lookup index to the 'n' column of prune table
		map<int,int> 		m_MapIndex2mG2; // this map is the lookup index to the 'm' column of prune table
		int					m_naPruneG3[PRUNE_G3_SIZE_N][PRUNE_G3_SIZE_M];
		//map<int,int> 		m_MapIndex2nG3; // this map is the lookup index to the 'n' column of prune table
		map<int,int> 		m_MapIndex2mG3; // this map is the lookup index to the 'm' column of prune table
		vector<CPS>			m_vCornPermG3;	// set of corner permutations in G3
		int					m_naPruneG4[PRUNE_G4_SIZE_N][PRUNE_G4_SIZE_M];
		map<int,int> 		m_MapIndex2nG4; // this map is the lookup index to the 'n' column of prune table
		map<int,int> 		m_MapIndex2mG4; // this map is the lookup index to the 'm' column of prune table		
		bool				m_bPruneG1Gen;		// flag telling if prune table for stage 1 was generated
		bool				m_bPruneG2Gen;		// flag telling if prune table for stage 2 was generated
		bool				m_bPruneG3Gen;		// flag telling if prune table for stage 3 was generated
		bool				m_bPruneG4Gen;		// flag telling if prune table for stage 4 was generated		
		//	cubie model
		int					m_anCornPerm[NUM_OF_CORNERS];
		int					m_anEdgePerm[NUM_OF_EDGES];
		int					m_anCornOrient[NUM_OF_CORNERS];
		int					m_anEdgeOrient[NUM_OF_EDGES];

#if defined(DOS)
		HANDLE 				m_hConsole;
#endif
#if !defined(NOVISNODESMAP)
		MapVisitedNodes 	m_MapVisitedNodes;
#endif

		bool 				SolveCube (bool show=false);
		void 				GenTree (RCube *start, int depth, bool show=false);
		void 				GenTree2 (RCube *start, int depth, bool show=false);
		void 				GenTree3 (RCube *start, int depth, bool show=false);
		int 				EvalNode(RCube *p);
		int 				GetDistance(int f1, int f2);
		void 				SortSrchOrder(int *pEval, int *pOrder);
		RCube 				*DoMove (RCube *start, int move);
		RCube 				*DoMoveStatic (RCube *start, int move);
		RCube 				*CreateCubeFromTmpl(int *ptmpl);
		string 				FaceSymbol(int face_code);
		string 				ColorCode(int face_code);
		void 				MoveFrontUpRight(RCube *p);
		void 				MoveFrontUpLeft(RCube *p);
		void 				MoveFrontLeftUp(RCube *p);
		void 				MoveFrontLeftDown(RCube *p);
		void 				MoveFrontRightUp(RCube *p);
		void 				MoveFrontRightDown(RCube *p);
		void 				MoveFrontDownRight(RCube *p);
		void 				MoveFrontDownLeft(RCube *p);
		void 				MoveFrontTopRight(RCube *p);
		void 				MoveFrontTopLeft(RCube *p);
		void 				MoveBottomTopRight(RCube *p);
		void 				MoveBottomTopLeft(RCube *p);
		void 				ShiftFacesLR(int idx, RCube *p);
		void 				ShiftFacesRL(int idx, RCube *p);
		void 				ShiftFacesUD(int idx, RCube *p);
		void 				ShiftFacesDU(int idx, RCube *p);
		bool 				NodeVisited(RCube *p, int depth=-1);
		bool 				NodesEqual(RCube *pn1, RCube *pn2);
		bool 				LayerByLayer();
		bool				Thistlethwaite();
		bool 				SolveCrossDet(RCube *start);
		RCube 				*SolveCrossStep1(RCube *start);
		RCube 				*SolveCrossStep2(RCube *start);
		bool 				IsCrossSolved(RCube *p);
		bool 				IsTopLayerSolvedStep1(RCube *p);
		bool 				IsTopLayerSolvedStep2(RCube *p);
		bool 				IsTopLayerSolved(RCube *p);
		bool 				IsMiddleLayerSolved(RCube *p);
		bool 				Insert1stLayerCorners(RCube *p);
		bool 				Position1stLayerCorners(RCube *p);
		bool 				Orient1stLayerCorners(RCube *p);
		bool 				Solve1stLayerCorners(RCube *p);
		bool 				SolveMiddleLayer(RCube *p);
		bool 				PositionBottomLayerCorners(RCube *p);
		bool 				BottomLayerCornersPositioned(RCube *p);
		bool 				L3DiagonalCornersPositioned(RCube *p);
		bool 				L3AdjacentCornersPositioned(RCube *p);
		int	 				L3AdjacentCornersPositioned2(RCube *p);
		RCube 				*L3PositionRemainingAdjacentCorners(RCube *p);
		bool 				OrientBottomLayerCorners(RCube *p);
		bool 				L3CornersOriented(RCube *p);
		string 				L3CheckCornersOrient(RCube *p);
		bool 				L3EdgePermutation(RCube *p);
		bool 				L3EdgeOrientation(RCube *p);
		bool 				L3Solved(RCube *p);
		bool 				IsBottomCrossSolvedNotPositioned(RCube *p);
		void 				PrintMoves(RCube *p);
		int	 				GetColorAttr(int face_code);
		void 				SetDefaultColorAttr();
		void 				SetColorAttr(int color_attr);
		void 				CommonInit();
		string				ConvertCube2String(const RCube *p);
		string				ConvertCube2CompactString(const RCube *p);
		RCube  				*ConvertString2Cube(const string s);
		RCube  				*ConvertCompactString2Cube(const string s);
		void 				Add2KnownPaths(const RCube *p);
		string 				GetSolutionPath(const RCube *p);
		void				GenerateKnownPaths(const RCube *start, int depth);
		string				SolutionInKnownPaths(const RCube *start, bool show=true);
		void				ShowRootInfo(int branch);
		bool				LoadKnownPathsFromCache();
		void				CacheKnownPaths();
		string  			OptimizeSolutionPath(string sol_path);
		void				PrintPath(RCube *p);
		RCube				*SetSolvedCube(RCube *p);
		int					ClearPath(RCube *p);
		bool 				IsSolvedState(RCube *p);
		void				CreateLookupGx();
		void				GenLookupG1();
		void				GenLookupG2();
		void 				GenInitStatesG3(int *ep, int *cp, int &empty_slots, int &index);
		void				GenLookupG3();
		void				GenLookupG4();
		bool				LoadCache(string fn, MapKnownPaths &m);
		void 				SolveThistlethwaiteG0G1(RCube *start, int depth, int maxd, bool show=false);
		void 				SolveThistlethwaiteG1G2(RCube *start, int depth, int maxd, bool show=false);
		void 				SolveThistlethwaiteG2G3(RCube *start, int depth, int maxd, bool show=false);
		void 				SolveThistlethwaiteG3Corners(RCube *start, int depth, int maxd, bool show=false);
		void 				SolveThistlethwaiteG3G4(RCube *start, int depth, int maxd, bool show=false);
		int					EdgesOrientStates2Index(RCube *p);
		bool				SaveLookupG1();
		bool				LoadLookupG1();
		bool				SaveLookupG2();
		bool				LoadLookupG2();
		bool				SaveLookupG3();
		bool				LoadLookupG3();		
		bool				SaveLookupG4();
		bool				LoadLookupG4();		
		int					*ConvertCubeEdges2OrientTbl(RCube *p);
		int					*ConvertCubeEdges2PermTbl(RCube *p);
		int					*ConvertCubeCorners2OrientTbl(RCube *p);
		int					*ConvertCubeCorners2PermTbl(RCube *p);
		string				ConvCompactStrCode2FaceCode(string cpct_code);
		void				PrintSolPathEyeFriendly(string sol_path);
		bool				IsG1Group();
		bool				IsG2Group();
		bool				IsG3Group();
		bool				IsCubieSolvedCorn();
		bool				IsSolvedCubieModel();
		bool				AlwaysFalse();
		RCube 				*GetNewRCube4Move(RCube *p, int move);
		void				Convert2CubieModel(RCube *p);
		map<int,int> 		LoadLookupMap(string fn);
		template<typename T>
		bool 				SaveLookupMap(string fn, T map2save);
		unsigned long long 	MemoryUsed();
		unsigned long long 	TotalSystemMemory();		

	public:
		Cube ();
		Cube (int *pfaces, bool use_cache=true);
		Cube (int depth, bool use_cache=true);
		~Cube ();

		RCube			*CreateSolvedCube();
		RCube			*SetCube(RCube *start);
		RCube			*GetRootCube();
		RCube			*GetCurrCube();
		RCube			*ResetMemMgm();
		void 			SetMethod(int method);
		int 	 		GetMethodNum();
		string 			GetMethodName();
		RCube 			*CreateCubeFromStrVectDef(vector<string> cube_def, bool clrmem=true);
		RCube 			*CreateCubeFromColorStrVectDef(vector<string> cube_def);
		bool 			Solve (RCube *start);
		bool 			Solve (RCube *start, int depth, bool show=false);
		bool 			Solve ();
		bool 			Solve (int depth);
		bool 			Solve (int depth, bool show);
		RCube 			*Scramble(int moves, bool show=false);
		RCube 			*ScrambleStatic(int moves, bool show=false);
		void 			Print(const RCube &cube);
		void 			Print();
		void 			PrintSolution();
		void 			PrintHistory();
		void 			PrintHistoryPath();
		void           	PrintSolutionPath();
		RCube 			*DoMoves(RCube *pcube, string moves);
		RCube 			*DoMoves(string moves);
		vector<string> 	GetSolveMethodsNames();
		bool 			IsSolved(RCube *p);
		bool 			IsSolved();
		bool 			IsSolvedState();
		bool 			DeleteCube (RCube *p);
		void			ClearHistory();
		void			SetColorMode(bool colr);
		void			UpdateCache(bool upd);
		string			ConvertCube2InputString(const RCube *p);
		string			ConvertCube2ColorsString(const RCube *p);
		string			GetColorCubeDefNoSpaces(const RCube *p);
		double			GetSolvedSecs();
		int 			RandomInt(int start, int end);
		string 			GetSolutionPath();
		size_t 			PrivateMemoryUsed();
		void 			ShowMemUse();
		int				GetSolLen();
		CubieModelState	GetCubieModelState();
};

} // namespace Rubik

#endif //_CUBE_H
