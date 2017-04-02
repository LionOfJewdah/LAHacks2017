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

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <vector>
#include <unistd.h>
#include <ctime>
#include <string.h>
#include "Cube.h"
#include "Exception.h"
#include "MemMgm.h"
#include "CubieModel.h"

using namespace std;

namespace Rubik
{

///////////////////////////////////////////////////
const int solved_cube_tmpl[] = {
	U0, U1, U2,
	U3, U4, U5,
	U6, U7, U8,

	L0, L1, L2,
	L3, L4, L5,
	L6, L7, L8,

	D0, D1, D2,
	D3, D4, D5,
	D6, D7, D8,

	F0, F1, F2,
	F3, F4, F5,
	F6, F7, F8,

	R0, R1, R2,
	R3, R4, R5,
	R6, R7, R8,

	B0, B1, B2,
	B3, B4, B5,
	B6, B7, B8
};
///////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
const string known_paths_fname = KNOWN_PATHS_FN;
/////////////////////////////////////////////////////////////
const string faces_sym_tbl[] = {
							"U0", "U1", "U2",
							"U3", "U4", "U5",
							"U6", "U7", "U8",
	"L0", "L1", "L2", "F0", "F1", "F2", "R0", "R1", "R2", "B0", "B1", "B2",
	"L3", "L4", "L5", "F3", "F4", "F5", "R3", "R4", "R5", "B3", "B4", "B5",
	"L6", "L7", "L8", "F6", "F7", "F8", "R6", "R7", "R8", "B6", "B7", "B8",
							"D0", "D1", "D2",
							"D3", "D4", "D5",
							"D6", "D7", "D8"
};
/////////////////////////////////////////////////////////////
const string color_sym_tbl[] = {
						"WGP", "WP", "WPB",
						"WG",  "W",  "WB",
						"WRG", "WR", "WBR",
	"GPW", "GW", "GWR",	"RGW", "RW", "RWB", "BRW", "BW", "BWP", "PBW", "PW", "PWG",
	"GP",  "G",  "GR", 	"RG",  "R",  "RB", 	"BR",  "B",  "BP", 	"PB",  "P",  "PG",
	"GYP", "GY", "GRY",	"RYG", "RY", "RBY", "BYR", "BY", "BPY", "PYB", "PY", "PGY",
						"YGR", "YR", "YRB",
						"YG",  "Y",  "YB",
						"YPG", "YP", "YBP"
};
/////////////////////////////////////////////////////////////
const unsigned char compact_faces_sym_tbl[] = {
						'A', 'B', 'C',
						'D', 'E', 'F',
						'G', 'H', 'I',

	'J', 'K', 'L', 		'S', 'T', 'U', 		'b', 'c', 'd', 		'k', 'l', 'm',
	'M', 'N', 'O', 		'V', 'W', 'X', 		'e', 'f', 'g', 		'n', 'o', 'p',
	'P', 'Q', 'R', 		'Y', 'Z', 'a', 		'h', 'i', 'j', 		'q', 'r', 's',

						't', 'u', 'v',
						'w', 'x', 'y',
						'z', '0', '1'
};
/////////////////////////////////////////////////////////////
const string move_names[] = {
	"top    - right",
	"top    - left",
	"left   - up",
	"left   - down",
	"upper  - right",
	"upper  - left",
	"right  - up",
	"right  - down",
	"lower  - right",
	"lower  - left",
	"bottom - right",
	"bottom - left"
};
/////////////////////////////////////////////////////////////
const string o_move_names[] = {
	"top    - left",
	"top    - right",
	"left   - down",
	"left   - up",
	"upper  - left",
	"upper  - right",
	"right  - down",
	"right  - up",
	"lower  - left",
	"lower  - right",
	"bottom - left",
	"bottom - right"
};
/////////////////////////////////////////////////////////////
const string move_names_N[] = {
	"F",
	"Fi",
	"Li",
	"L",
	"Ui",
	"U",
	"R",
	"Ri",
	"D",
	"Di",
	"Bi",
	"B",
	"F2",
	"F2i",
	"L2i",
	"L2",
	"U2i",
	"U2",
	"R2",
	"R2i",
	"D2",
	"D2i",
	"B2i",
	"B2"
};
/////////////////////////////////////////////////////////////
const string o_move_names_N[] = {
	"Fi",
	"F",
	"L",
	"Li",
	"U",
	"Ui",
	"Ri",
	"R",
	"Di",
	"D",
	"B",
	"Bi",
	"F2i",
	"F2",
	"L2",
	"L2i",
	"U2",
	"U2i",
	"R2i",
	"R2",
	"D2i",
	"D2",
	"B2",
	"B2i"	
};
/////////////////////////////////////////////////////////////
const string solve_methods_names[] = {
	"0 - Tree Search (basic)",
	"1 - Tree Search Heuristic",
	"2 - Tree Search Back-Trace",
	"3 - Layer By Layer/Human",
	"4 - Thistlethwaite's 45"
};
/////////////////////////////////////////////////////////////

/*
 * Default constructor.
 */
Cube::Cube()
{
	m_bUpdateCache = true;
	m_nDepth = 7;
	m_pRoot = m_pCurrent = NULL;
	CommonInit();
}

/*
 * Custom constructor.
 */
Cube::Cube(int *pfaces, bool use_cache)
{
	m_bUpdateCache = use_cache;
	m_nDepth = 7;
	m_pRoot = CreateCubeFromTmpl(pfaces);
	m_pCurrent = m_pRoot;
	CommonInit();
}

/*
 * Custom constructor.
 */
Cube::Cube(int depth, bool use_cache)
{
	m_bUpdateCache = use_cache;
	m_nDepth = depth + 1;
	m_pRoot = m_pCurrent = NULL;
	CommonInit();
}

/*
 * Set default console text color attribute.
 */
void Cube::SetDefaultColorAttr()
{
#if defined(DOS)
	if (false == m_bMono)
		SetConsoleTextAttribute(m_hConsole, 10);
#endif
}

/*
 * Set console text color attribute.
 */
void Cube::SetColorAttr(int color_attr)
{
#if defined(DOS)
	if (false == m_bMono)
		SetConsoleTextAttribute(m_hConsole, color_attr);
#endif
}

/*
 * Common class/object initialization code.
 */
void Cube::CommonInit()
{
#if defined(MYDBG15)
	cout << "Cube::CommonInit()" << endl;
#endif	
	if (NULL != m_pRoot) {
		m_pMemMgm = new MemMgm(m_pRoot);
		m_pRoot = m_pMemMgm->GetRCubeAt(0);
		if (NULL == m_pRoot)
			throw Exception("Cube::CommonInit() - null pointer!");
		m_pRoot->NullParent();			
		m_pCurrent = m_pRoot;
	}
	else {
		m_pMemMgm = new MemMgm();
	}
		
	m_fSolvedSecs = 0.0;
	m_nSolLen = 0;
	m_bCacheLoaded = false;
	m_bMono = false;
	m_bSolved = false;
	m_pGoal = NULL;
	m_nMethod = TreeSearch;
	m_bShow = false;
#if defined(DOS)
	m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	srand(time(NULL) + GetTickCount());
#else
	srand(time(NULL));
#endif
	SetDefaultColorAttr();
	UpdateCache(m_bUpdateCache);
	// Initialize cubie model (solved state).
	for(int i=0; i<NUM_OF_CORNERS; i++) {
		m_anCornPerm[i] = i+1;
		m_anCornOrient[i] = 0;
	}
	for(int i=0; i<NUM_OF_EDGES; i++) {
		m_anEdgePerm[i] = i+1;
		m_anEdgeOrient[i] = 1;
	}
	m_bPruneG1Gen = false;
	m_bPruneG2Gen = false;
	m_bPruneG3Gen = false;
	m_bPruneG4Gen = false;
}

/*
 * Set the update known solutions cache mode.
 */
void Cube::UpdateCache(bool upd)
{
	MemMgm memMgr;
	m_bUpdateCache = upd;
	if(m_bUpdateCache) {
		if (m_bShow)
			cout << "Known paths lookup cache is enabled." << endl;		
		if (false == m_bCacheLoaded)
		{
			RCube *ptmp = NULL;
			if (NULL != m_pRoot)
				ptmp = memMgr.GetNewRCube(m_pRoot);
			cout << "Generating cache, this may take several minutes..." << endl;
			clock_t solve_time = clock();		
			if (false == LoadKnownPathsFromCache())
			{
				GenerateKnownPaths(NULL, 7);
				CacheKnownPaths();
				cout << "Cache generated." << endl;
			}
			else
				cout << "Known paths lookup loaded from cache." << endl;	
			double elapsed_sec = (double)(clock() - solve_time) / CLOCKS_PER_SEC;
			cout << endl << "Finished in " << elapsed_sec << " seconds." << endl << endl;	
			m_bCacheLoaded = true;
			cout << "# of items in lookup: " << m_MapKnownPaths.size() << endl;
			if (NULL != ptmp) {
				SetCube(ptmp);
			}
		}
	}
	else
	{
		if (m_bShow)
			cout << "Known paths lookup cache is disabled." << endl;
	}
}

/*
 * Set the mode of cube presentation (color/mono).
 */
void Cube::SetColorMode(bool colr)
{
	m_bMono = colr ? false : true;
}

/*
 * Default destructor.
 */
Cube::~Cube()
{
#if defined(MYDBG7)
	cout << endl << "MYDBG7: Cube::~Cube() - destructor." << endl;
#endif
	if (m_bUpdateCache)
		CacheKnownPaths();
	m_pMemMgm->DeleteAll();
	delete m_pMemMgm;
}

/*
 * Discard current solution and set the new cube.
 * Return pointer to root cube.
 */
RCube *Cube::SetCube(RCube *start)
{
	MemMgm memMgr;
	if (NULL == start)
		throw Exception("Cube::SetCube() - null pointer!");
    RCube *ptmp = memMgr.GetNewRCube(start);
    if (NULL == ptmp) {
		throw Exception("Cube::SetCube() - memory allocation failed!");
    }
	m_pMemMgm->DeleteAll();
	m_pRoot = m_pMemMgm->GetNewRCube(ptmp);
	if (NULL == m_pRoot) {
		delete m_pMemMgm;
		m_pMemMgm = new MemMgm();
		if (NULL == m_pMemMgm)
			throw Exception("Cube::SetCube() - memory depleted! (1)");
		m_pRoot = m_pMemMgm->GetNewRCube(ptmp);		
	}
	if (NULL == m_pRoot)
		throw Exception("Cube::SetCube() - memory depleted! (2)");
			
	m_pRoot->NullParent();
	m_pCurrent = m_pRoot;
	m_pGoal = NULL;
#if defined(MYDBG18)
	cout << endl << "MYDBG18: Cube::SetCube - cube faces:" << endl;
	Print();
#endif
	Convert2CubieModel(m_pCurrent);

	return m_pRoot;
}

/*
 * Return pointer to the root cube.
 */
RCube *Cube::GetRootCube()
{
	return m_pRoot;
}

/*
 * Return pointer to the current cube.
 */
RCube *Cube::GetCurrCube()
{
	return m_pCurrent;
}

/*
 * Reset memory manager. Return pointer to root cube.
 */
RCube *Cube::ResetMemMgm()
{
	MemMgm memMgr;
	RCube *ptmp = NULL;
	if (NULL != m_pRoot) {
		ptmp = memMgr.GetNewRCube(m_pRoot);
	} else {
		ptmp = memMgr.GetNewRCube();
	}
	if (NULL == ptmp) {
		throw Exception("Cube::ResetMemMgm() - memory depleted! (1)");
	}
	if (NULL != m_pMemMgm) {
		delete m_pMemMgm;
	}
	m_pMemMgm = new MemMgm();
	if (NULL == m_pMemMgm) {
		throw Exception("Cube::ResetMemMgm() - memory depleted! (2)");
	}
	m_pRoot = m_pMemMgm->GetNewRCube(ptmp);
	if (NULL == m_pRoot) {
		throw Exception("Cube::ResetMemMgm() - memory depleted! (3)");
	}
	m_pRoot->NullParent();
	m_pCurrent = m_pRoot;
	m_pGoal = NULL;
	
	return m_pRoot;	
}

/*
 * Set the method to solve cube.
 */
void Cube::SetMethod(int method)
{
	if (0 <= method && MAX_METHODS > method)
		m_nMethod = method;
	else
		throw Exception("Cube::SetMethod - invalid method number!");
}

/*
 * Clean up.
 */
bool Cube::DeleteCube(RCube *p)
{
	bool bstat = true;
	if (NULL != p) {
		bstat = m_pMemMgm->DeleteRCube(p);
	}
	if (m_bShow && false == bstat) {
		cout << endl << "Cube::DeleteCube - WARNING: cube was not found in memory manager." << endl;
	}
	return bstat;
}

/*
 * Solve Rubik's cube from provided scrambled state with default depth.
 */
bool Cube::Solve(RCube *start)
{
	bool ret = false;

	if (NULL == start)
		throw Exception("Cube::Solve() - null pointer!");

	m_pMemMgm->DeleteAll();
	m_pRoot = m_pMemMgm->GetNewRCube(start);
	if (NULL == m_pRoot)
		throw Exception("Cube::Solve() - memory depleted!");	
	m_pRoot->NullParent();
	m_pCurrent = m_pRoot;
	ret = SolveCube();

	return ret;
}

/*
 * Solve Rubik's cube from provided scrambled state with custom depth.
 */
bool Cube::Solve(RCube *start, int depth, bool show)
{
	bool ret = false;

	if (NULL == start)
		throw Exception("Cube::Solve() - null pointer!");

	m_pMemMgm->DeleteAll();
	m_pRoot = m_pMemMgm->GetNewRCube(start);
	if (NULL == m_pRoot)
		throw Exception("Cube::Solve() - memory depleted!");	
	m_pRoot->NullParent();
	m_pCurrent = m_pRoot;
	m_nDepth   = depth + 1;

	ret = SolveCube(show);

	return ret;
}

/*
 * Solve Rubik's cube from starting scramble state with default depth.
 */
bool Cube::Solve()
{
	bool ret = false;

	ret = SolveCube();

	return ret;
}

/*
 * Solve Rubik's cube from starting scramble state with custom depth.
 */
bool Cube::Solve(int depth)
{
	bool ret = false;

	m_nDepth = depth + 1;
	ret = SolveCube();

	return ret;
}

/*
 * Solve Rubik's cube from starting scramble state with custom depth and verbosity.
 */
bool Cube::Solve(int depth, bool show)
{
	bool ret = false;

	m_nDepth = depth + 1;
	ret = SolveCube(show);

	return ret;
}

/*
 * Clear cube's moves history.
 */
void Cube::ClearHistory()
{
	MemMgm memMgr;
	RCube *pcurr = NULL;
	if (NULL != m_pCurrent)
		pcurr = memMgr.GetNewRCube(m_pCurrent);
	m_pMemMgm->DeleteAll();
	if (NULL != m_pCurrent)
		m_pCurrent = m_pMemMgm->GetNewRCube(pcurr);
	if (NULL != m_pCurrent)
		m_pCurrent->NullParent();
	m_pRoot = m_pCurrent;
	////
	m_pSolved = NULL;
	m_bSolved = false;
}

/*
 * The cube solve work method.
 */
bool Cube::SolveCube(bool show)
{
	bool ret = false;

	m_bSolved = false;
	m_bShow   = show;
	m_pSolved = NULL;
	m_lNodesChecked = 0L;
	m_lVisitedNodesSkip = 0L;
	clock_t solve_time = clock();
	
	if (false == IsSolvedState()) {
		string known_path = SolutionInKnownPaths(m_pRoot);
	
		if (show)	cout << "Method: " << solve_methods_names[m_nMethod] << endl;
	
		if (0 < known_path.length()) {
			IsSolved(m_pRoot);
			m_bShow = false;
			return true;
		}
	
		switch (m_nMethod) {
			case TreeSearch:
				GenTree(m_pRoot, m_nDepth, show);
				break;
	
			case TreeSearchHeur:
				GenTree2(m_pRoot, m_nDepth, show);
				break;
	
			case TreeSearchBackTrace:
				m_pGoal = m_pRoot;
				m_pRoot = m_pMemMgm->GetNewRCube(CreateSolvedCube());
				GenTree3(m_pRoot, m_nDepth, show);
				if (false == m_bSolved) {
					DeleteCube(m_pRoot);
					m_pRoot = m_pCurrent = m_pGoal;
				}
				break;
	
			case MultiStepHuman:
				LayerByLayer();
				break;
				
			case TreeSearchLookup:
				Thistlethwaite();
				break;
	
			default:
				throw Exception("Cube:SolveCube - unknown method!");
		}		
	}
	else {
#if defined(MYDBG15)
		cout << "MYDBG15: Cube::SolveCube() - nothing to solve (already in solved state)." << endl;
#endif		
		m_bSolved = true;
		m_pSolved = m_pCurrent;		
	}

	double elapsed_sec = (double)(clock() - solve_time) / CLOCKS_PER_SEC;
	m_fSolvedSecs = elapsed_sec;

#ifdef MYDBG13
	cout << "MYDBG: Cube::SolveCube" << endl;
	m_pSolved = m_pCurrent;
	PrintSolution();
#endif


	cout << endl << "Finished in " << elapsed_sec << " seconds." << endl << endl;

	ret = m_bSolved;
	if (show) {
		cout << "\r                                                                                \r";
		cout << "Checked: " << m_lNodesChecked << " nodes." << endl;
		cout << "Skipped: " << m_lVisitedNodesSkip << " nodes." << endl;
	}

#ifdef MYDBG4
	cout << "MYDBG4: Cube::SolveCube - checked " << m_lNodesChecked << " nodes." << endl;
	cout << "MYDBG4: Cube::SolveCube - skipped " << m_lVisitedNodesSkip << " nodes (as already visited)." << endl;
#endif

	m_bShow = false;

#if !defined(NOVISNODESMAP)
	MapVisitedNodesIter it;
	it = m_MapVisitedNodes.begin();
	m_MapVisitedNodes.erase(it, m_MapVisitedNodes.end());
#endif

	if (m_bUpdateCache && ret)
		Add2KnownPaths(m_pSolved);

	return ret;
}

#define T45STAGE(stg,cond,d,func,trc)														\
	for (int i = 1; ret && i <= d && false == cond(); i++) {								\
		if (m_bShow) {																		\
			cout << endl << "Tree search with lookup, max. depth: " << i << endl;			\
		}																					\
		func(m_pCurrent, i, i, m_bShow);													\
	}																						\
	if (cond()) {																			\
		cout << endl << "Stage " << stg << " solved." << endl;								\
	} else {																				\
		cout << "ERROR: Stage " << stg << " failed." << endl;								\
		ret = false;																		\
	}																						\
	if (trc) {																				\
		cout << endl << "MYDBG18: Cube::Thistlethwaite - Cubie Model States:" << endl;		\
		CubieModelShowAllStates(m_anEdgePerm, m_anCornPerm, m_anEdgeOrient, m_anCornOrient);\
	}

/*
 * Solve the cube using IDA tree search with lookup/prune tables algorithm.
 * Algorithm is also known as Thistlethwaite's 45 (T45), designed by Finnish 
 * mathematician and named after him. He originally designed 52-step algorithm
 * which was later optimized to 45 maximum steps.
 * It divides the problem into 4 distinct phases, each solved separately
 * using more and more limited sets of moves until the entire cube is solved.
 * This algorithm should solve cube in no more than 45 steps, where double 
 * turn moves like U2, D2 etc. are also counted as single step).
 * Therefore if we use 90-degree turn as a definition of a singe move,
 * the number of steps will be of course greater.
 */
bool Cube::Thistlethwaite()
{
	bool ret = true;
	RCube *ptmp = NULL;
	MemMgm memMgr;
	bool trace = false;
	
#if defined(MYDBG18)
	trace = true;
#endif	
	
	if (NULL != m_pRoot)
		ptmp = memMgr.GetNewRCube(m_pRoot);
		
	if (NULL == ptmp)
		throw Exception("Cube::Thistlethwaite() - memory allocation failed!");

	CreateLookupGx();	// Create (or load from cache) prune tables.
	SetCube(ptmp);		// Restore initial cube.

	if (trace) {
		cout << endl << "MYDBG18: Cube::Thistlethwaite START - Cubie Model States:" << endl;
		CubieModelShowAllStates(m_anEdgePerm, m_anCornPerm, m_anEdgeOrient, m_anCornOrient);
	}
	if (m_bShow) cout << "Solving stage 1..." << endl;
	int ntry = 5;
	string help_moves = "F2R2";	
	while (0 < ntry) {
		ret = true;
		if (5 > ntry) cout << "Trying again, help moves: " << help_moves << endl;
			
		T45STAGE(1,IsG1Group,7,SolveThistlethwaiteG0G1,trace);

		if (ret && m_bShow) cout << "Solving stage 2..." << endl;

		T45STAGE(2,IsG2Group,11,SolveThistlethwaiteG1G2,trace);

		if (IsG2Group()) {
			break;
		} else {
			SetCube(ptmp);
			DoMoves(help_moves);
			help_moves = help_moves + "F2R2";
		}
		ntry--;
	}

	if (ret && m_bShow) cout << "Solving stage 3..." << endl;

	T45STAGE(3,IsG3Group,14,SolveThistlethwaiteG2G3,trace);
	string curr_path = GetSolutionPath(m_pCurrent);

	if (ret) {
		if (m_bShow) cout << "Solving stage 4..." << endl;
	
		T45STAGE(4,IsSolvedCubieModel,15,SolveThistlethwaiteG3G4,trace);
	
		if (false == ret) {			// if stage 4 fails
			ret = true;
			SetCube(ptmp);			// restore original cube
			DoMoves(curr_path);		// restore state after stage 3
			if (m_bShow) cout << "Solving stage 4a - corners..." << endl;
			// attempt to solve corners (stage 4a) and then stage 4
			T45STAGE("4a",IsCubieSolvedCorn,4,SolveThistlethwaiteG3Corners,trace);
			if (ret && m_bShow) cout << "Solving stage 4..." << endl;
			T45STAGE(4,IsSolvedCubieModel,15,SolveThistlethwaiteG3G4,trace);		
		}
	}

	if (false == IsSolved()) {
		SetCube(ptmp);
		ret = false;		
	}
	
	return ret;
}

/*
 * Algorithm T45.
 * Check if solution is in G1 group (all edge orientations fixed).
 */
bool Cube::IsG1Group()
{
	bool ret = true;
	int *eo = m_anEdgeOrient;
	
	if (m_bSolved) {
		return true;
	}
	
	if (m_bCacheLoaded && IsSolved()) {
		return true;
	}
	
	for (int i=0; ret && i<NUM_OF_EDGES; i++) {
		if (0 == eo[i]) {
			ret = false;
		}
	}
	
	return ret;
}

// T45, cubie model, macros to store/restore status.
#define SAVE_CUBIE_STATUS(ep,cp,eo,co)		\
	{										\
		EOCopy(m_anEdgeOrient, eo);			\
		COCopy(m_anCornOrient, co);			\
		EPCopy(m_anEdgePerm, ep);			\
		CPCopy(m_anCornPerm, cp);			\
	}
	
#define RESTORE_CUBIE_STATUS(ep,cp,eo,co)	\
	{										\
		EOCopy(eo, m_anEdgeOrient);			\
		COCopy(co, m_anCornOrient);			\
		EPCopy(ep, m_anEdgePerm);			\
		CPCopy(cp, m_anCornPerm);			\
	}

// T45, macro for recursive search.
#define T45LOOP(Func,IsGroup,showroot)														\
	int eotmp[NUM_OF_EDGES], cotmp[NUM_OF_CORNERS], eptmp[NUM_OF_EDGES], cptmp[NUM_OF_CORNERS];				\
	SAVE_CUBIE_STATUS(eptmp,cptmp,eotmp,cotmp);								\
	for (unsigned int i=0; i<sizeof(moves)/sizeof(moves[0]); i++) {			\
		RCube *p = NULL;													\
		int move = moves[i];												\
																			\
		if (showroot && depth == maxd)													\
			ShowRootInfo(move);												\
																			\
		if (show) {															\
			cout << "\r" << "Depth: " << (maxd-depth) << "    Node: " << m_lNodesChecked << "    Branch: ";	\
			cout << move_names_N[move] << "     ";							\
		}																	\
																			\
		p = DoMove(start, move);											\
		Func(p, depth-1, maxd, show);										\
																			\
		if (m_bSolved || IsGroup()) {										\
			break;															\
		}																	\
		else {																\
			if (NULL != p) {												\
				m_pCurrent = p->parent;										\
				DeleteCube(p);												\
			}																\
		}																	\
		RESTORE_CUBIE_STATUS(eptmp,cptmp,eotmp,cotmp);						\
	}	

/*
 * Algorithm T45.
 * Solve stage 1, recursive IDA tree search.
 */
void Cube::SolveThistlethwaiteG0G1(RCube *start, int depth, int maxd, bool show)
{
	usleep(1); // let the OS/kernel "breathe"
	if (NULL == start)
		throw Exception("Cube::SolveThistlethwaiteG0G1 - null start node!");

	m_lNodesChecked++;

	if (0 == depth || IsG1Group()) return;
	// retrieve index of prune table from current cube state and perform search
	int m = EO2Index(m_anEdgeOrient);
	if (m_naPruneG1[m] <= depth) {
		int moves[] = {MOVE_L, MOVE_R, MOVE_F, MOVE_B, MOVE_U, MOVE_D, MOVE_L2, MOVE_R2, MOVE_F2, MOVE_B2, MOVE_U2, MOVE_D2, MOVE_Li, MOVE_Ri, MOVE_Fi, MOVE_Bi, MOVE_Ui, MOVE_Di};
		T45LOOP(SolveThistlethwaiteG0G1,IsG1Group,false);
	}
	else
		m_lVisitedNodesSkip++;

	return;	
}

/*
 * Algorithm T45.
 * Check if cube state is in G2 group.
 */
bool Cube::IsG2Group()
{
	bool ret = true;
	int *ep = m_anEdgePerm;
	int *co = m_anCornOrient;
	
	if (m_bSolved) {
		return true;
	}
	
	if (m_bCacheLoaded && IsSolved()) {
		return true;
	}	
	
	for (int i=8; ret && i<NUM_OF_EDGES; i++)
	{
		if (ep[i] != i+1) {
			ret = false;
		}
	}
	for (int i=0; ret && i<NUM_OF_CORNERS; i++) {
		if (0 != co[i]) {
			ret = false;
		}
	}
	
	return ret;
}

/*
 * Algorithm T45.
 * Solve stage 2, recursive IDA tree search.
 */
void Cube::SolveThistlethwaiteG1G2(RCube *start, int depth, int maxd, bool show)
{
	usleep(1); // let the OS/kernel "breathe"
	if (NULL == start)
		throw Exception("Cube::SolveThistlethwaiteG1G2 - null start node!");

	m_lNodesChecked++;

	if (0 == depth || IsG2Group()) return;
	// retrieve indexes of prune table from current cube state and perform search
	int n = m_MapIndex2nG2[CO2Index(m_anCornOrient)];
	int ep[NUM_OF_EDGES];
	for (int i=0; i<NUM_OF_EDGES; i++) {
		ep[i] = ((8 < m_anEdgePerm[i]) ? 1 : 0);
	}
	int m = m_MapIndex2mG2[EO2Index(ep)];
	if (m_naPruneG2[n][m] <= depth) {
		int moves[] = {MOVE_L, MOVE_R, MOVE_F, MOVE_B, MOVE_U2, MOVE_L2, MOVE_R2, MOVE_F2, MOVE_B2, MOVE_D2, MOVE_Li, MOVE_Ri, MOVE_Fi, MOVE_Bi};
		T45LOOP(SolveThistlethwaiteG1G2,IsG2Group,false);
	}
	else
		m_lVisitedNodesSkip++;

	return;	
}

/*
 * Algorithm T45.
 * Check if cube is in G3 group.
 */
bool Cube::IsG3Group()
{
	bool ret=false;
	int *cp = m_anCornPerm;
	int *ep = m_anEdgePerm;
	
	if (m_bSolved) {
		return true;
	}
	
	if (m_bCacheLoaded && IsSolved()) {
		return true;
	}		
	
	for (vector<CPS>::iterator it = m_vCornPermG3.begin(); false == ret && it != m_vCornPermG3.end(); ++it) {
		ret = true;
		for (int i=0; ret && i<NUM_OF_CORNERS; i++) {
			if (cp[i] != (*it).cp[i]) {
				ret = false;
			}
		}
	}
	for (int i=0; ret && i<4; i++) {
		if (4 < ep[i]) {
			ret = false;
		}
	}
	
	return ret;
}

/*
 * Algorithm T45.
 * Solve stage 3, recursive IDA tree search.
 */
void Cube::SolveThistlethwaiteG2G3(RCube *start, int depth, int maxd, bool show)
{
	usleep(1); // let the OS/kernel "breathe"
	if (NULL == start)
		throw Exception("Cube::SolveThistlethwaiteG2G3 - null start node!");

	m_lNodesChecked++;

	if (0 == depth || IsG3Group()) return;
	// retrieve indexes of prune table from current cube state and perform search
	int n = CP2Index(m_anCornPerm);
	int ep[NUM_OF_EDGES];
	for (int i=0; i<NUM_OF_EDGES; i++) {
		if (5 > m_anEdgePerm[i]) {
			ep[i] = 0;
		}
		ep[i] = ((4 < m_anEdgePerm[i] && 9 > m_anEdgePerm[i]) ? 1 : ep[i]);
		ep[i] = ((8 < m_anEdgePerm[i]) ? 2 : ep[i]);		
	}
	int m = m_MapIndex2mG3[LREP2Index(ep)];
	if (m_naPruneG3[n][m] <= depth) {
		int moves[] = {MOVE_L, MOVE_Li, MOVE_L2, MOVE_R, MOVE_Ri, MOVE_R2, MOVE_F2, MOVE_B2, MOVE_U2, MOVE_D2};
		T45LOOP(SolveThistlethwaiteG2G3,IsG3Group,false);
	}
	else
		m_lVisitedNodesSkip++;

	return;	
}

/*
 * Check if corners are solved in cubie model cube.
 */
bool Cube::IsCubieSolvedCorn()
{
	bool ret = false;
	if (m_bSolved) {
		return true;
	}
	if (m_bCacheLoaded) {
		ret = IsSolved();
	}
	if (false == ret) {
		ret = SolvedCornersCubieModel(m_anCornPerm, m_anCornOrient);		
	}
	
	return ret;
}

/*
 * Algorithm T45.
 * After G3 stage is reached, solve the corners (no more than 4 moves) 
 * before proceeding to stage 4 IDA search.
 */
void Cube::SolveThistlethwaiteG3Corners(RCube *start, int depth, int maxd, bool show)
{
	usleep(1); // let the OS/kernel "breathe"
	if (NULL == start)
		throw Exception("Cube::SolveThistlethwaiteG3Corners - null start node!");

	m_lNodesChecked++;

	if (0 == depth || IsCubieSolvedCorn()) return;

	int moves[] = {MOVE_L2, MOVE_R2, MOVE_F2, MOVE_B2, MOVE_U2, MOVE_D2};
	T45LOOP(SolveThistlethwaiteG3Corners,IsCubieSolvedCorn,false);

	return;	
}

/*
 * Function returns always false (needed for a generic macro use).
 */
bool Cube::AlwaysFalse()
{
	return false;
}

/*
 * Algorithm T45.
 * Solve stage 4, recursive IDA tree search.
 */
void Cube::SolveThistlethwaiteG3G4(RCube *start, int depth, int maxd, bool show)
{
	usleep(1); // let the OS/kernel "breathe"
	if (NULL == start)
		throw Exception("Cube::SolveThistlethwaiteG3G4 - null start node!");

	m_lNodesChecked++;

	if (0 == depth || IsSolvedCubieModel()) return;
	// retrieve indexes of prune table from current cube state and perform search
	int n = m_MapIndex2nG4[CP2Index(m_anCornPerm)];
	int m = m_MapIndex2mG4[EP2Index(m_anEdgePerm)];
	if (m_naPruneG4[n][m] <= depth) {
		int moves[] = {MOVE_L2, MOVE_R2, MOVE_F2, MOVE_B2, MOVE_U2, MOVE_D2};
		T45LOOP(SolveThistlethwaiteG3G4,AlwaysFalse,false);
	}
	else
		m_lVisitedNodesSkip++;

	return;		
}

/*
 * Algorithm T45.
 * Generate (or load from cache) lookup and prune tables.
 */
void Cube::CreateLookupGx()
{
	if (false == m_bPruneG1Gen) {
		cout << "Initializing prune table P1..." << endl;
		if (false == LoadLookupG1()) {
			GenLookupG1();
			SaveLookupG1();
		}
		m_bPruneG1Gen = true;
	}
	if (false == m_bPruneG2Gen) {
		cout << "Initializing prune table P2..." << endl;
		if (false == LoadLookupG2()) {
			GenLookupG2();
			SaveLookupG2();
		}
		m_bPruneG2Gen = true;
	}
	if (false == m_bPruneG3Gen) {
		cout << "Initializing prune table P3..." << endl;
		if (false == LoadLookupG3()) {
			GenLookupG3();
			SaveLookupG3();
		}
		m_bPruneG3Gen = true;
	}
	if (false == m_bPruneG4Gen) {
		cout << "Initializing prune table P4..." << endl;
		if (false == LoadLookupG4()) {
			GenLookupG4();			
			SaveLookupG4();
		}
		m_bPruneG4Gen = true;
	}
}

/*
 * Load 2-dimensional prune table from file.
 */
#define LOAD_PRUNE_TABLE(t,fn,ns,ms,b) 							\
{																\
	b = false;													\
	string buf;													\
	ifstream cache;												\
																\
	cache.open(fn.c_str());										\
	if (cache.is_open()) {										\
		int i=0, j=0, n=0;										\
		while(getline(cache, buf) && false == cache.eof()) {	\
			t[i][j] = atoi(buf.c_str());						\
			if (++j == ms) {									\
				j = 0;											\
				if (++i == ns) {								\
					break;										\
				}												\
			}													\
			n++;												\
		}														\
		cout << "Loaded " << n << " entries to prune table." << endl;	\
		cache.close();											\
		b = true;												\
	}															\
}

/*
 * Load <int,int> lookup map from file.
 * Check number of loaded elements, if 0, load failed.
 */
map<int,int> Cube::LoadLookupMap(string fn)
{
	map<int,int> ret;
	
	ifstream cache;
	cache.open(fn.c_str());
	if(cache.is_open()) {
		int n=0, m=0, i=0;
		while (cache >> n && cache >> m) {
			ret[n] = m;
			n=m=0; i++;
		}
		cout << "Loaded " << i << " entries to lookup table." << endl;
		cache.close();
	}	
	
	return ret;
}

/*
 * Algorithm T45.
 * Load G1 lookup (prune table and lookup) from disk file.
 */
bool Cube::LoadLookupG1()
{
	bool ret = false;
	string fn = PRUNE_G1_FN;
	string buf;
	ifstream cache;
	
	cache.open(fn.c_str());
	if (cache.is_open()) {
		int i=0;
		while(false == cache.eof() && getline(cache, buf) && i<PRUNE_G1_SIZE ) {
			m_naPruneG1[i++] = atoi(buf.c_str());
		}
		cout << "Loaded " << i << " entries to G1 prune table." << endl;
		cache.close();
		ret = true;
	}
	
	if (m_bShow) {
		cout << "Cube::LoadLookupG1 - " << ((ret) ? "OK" : "ERR") << endl;
	}
	
	return ret;
}

/*
 * Algorithm T45.
 * Save G1 lookup (prune table and lookup) in disk file.
 */
bool Cube::SaveLookupG1()
{
	bool ret = false;
	string fn = PRUNE_G1_FN;
	ofstream cache(fn.c_str());

	if(cache.is_open()) {
		for (int i=0; i<PRUNE_G1_SIZE; i++) {
			cache << m_naPruneG1[i] << endl;
		}
		cache.close();
		ret = true;
	} else {
		cout << endl << "ERROR: Cube::SaveLookupG1() - unable to save cache of prune table!" << endl;	
	}
	
	return ret;	
}

/*
 * Algorithm T45.
 * Load G2 lookup (prune table and lookup) from disk file.
 */
bool Cube::LoadLookupG2()
{
	bool ret = false;
	string fn = PRUNE_G2_FN;
	string buf;
	ifstream cache;
	
	LOAD_PRUNE_TABLE(m_naPruneG2,fn,PRUNE_G2_SIZE_N,PRUNE_G2_SIZE_M,ret);
	
	if (ret) {
		fn = LOOKUP_G2_N_FN;
		m_MapIndex2nG2 = LoadLookupMap(fn);
		if (m_MapIndex2nG2.empty()) {
			ret = false;
		}
	}
	
	if (ret) {
		fn = LOOKUP_G2_M_FN;
		m_MapIndex2mG2 = LoadLookupMap(fn);
		if (m_MapIndex2mG2.empty()) {
			ret = false;
		}
	}

	if (m_bShow) {
		cout << "Cube::LoadLookupG2 - " << ((ret) ? "OK" : "ERR") << endl;
	}
	
	return ret;
}

/*
 * Algorithm T45.
 * Generic method to save map in disk file.
 */
template<typename T> 
bool Cube::SaveLookupMap(string fn, T map2save)
{
	ofstream cache;
	bool ret = false;
	
	cache.open(fn.c_str());
	if(cache.is_open()) {
		for (typename T::iterator it = map2save.begin(); it != map2save.end(); ++it) {
			cache << it->first << endl; 
			cache << it->second << endl;
		}
		cache.close();
		ret = true;
	} else {
		cout << endl << "ERROR: Cube::SaveLookupMap() - unable to save cache of lookup! File: [" << fn << "]." << endl;	
	}
	
	return ret;
}

/*
 * Algorithm T45.
 * Save G2 lookup (prune table and lookup) in disk file.
 */
bool Cube::SaveLookupG2()
{
	bool ret = false;
	string fn = PRUNE_G2_FN;
	ofstream cache(fn.c_str());

	if(cache.is_open()) {
		for (int i=0; i<PRUNE_G2_SIZE_N; i++) {
			for (int j=0; j<PRUNE_G2_SIZE_M; j++) {
				cache << m_naPruneG2[i][j] << endl;
			}
		}
		cache.close();
		ret = true;
	} else {
		cout << endl << "ERROR: Cube::SaveLookupG2() - unable to save cache of prune table!" << endl;	
	}
	
	if (ret) {
		fn = LOOKUP_G2_N_FN;
		ret = SaveLookupMap(fn, m_MapIndex2nG2);
	}
	
	if (ret) {
		fn = LOOKUP_G2_M_FN;
		ret = SaveLookupMap(fn, m_MapIndex2mG2);
	}	
	
	return ret;	
}

/*
 * Algorithm T45.
 * Load G3 lookup (prune table and lookup) from disk file.
 */
bool Cube::LoadLookupG3()
{
	bool ret = false;
	string fn = PRUNE_G3_FN;
	string buf;
	ifstream cache;
	
	LOAD_PRUNE_TABLE(m_naPruneG3,fn,PRUNE_G3_SIZE_N,PRUNE_G3_SIZE_M,ret);
	
	if (ret) {
		fn = LOOKUP_G3_M_FN;
		m_MapIndex2mG3 = LoadLookupMap(fn);
		if (m_MapIndex2mG3.empty()) {
			ret = false;
		}
	}	
	
	if (ret) {
		ifstream cache;
		ret = false;
		fn = CORNPERM_G3_FN;
		cache.open(fn.c_str());
		if(cache.is_open()) {
			string buf;
			int i=0;
			while (cache >> buf) {
				CPS cps;
				for (unsigned int i=0; i<buf.length(); i++) {
					cps.cp[i] = buf[i] - '0';
				}
				m_vCornPermG3.push_back(cps);
				i++;
			}
			cout << "Loaded " << i << " entries to G3 corner perm. lookup table." << endl;
			cache.close();
			ret = true;
		}
	}	
	
	if (m_bShow) {
		cout << "Cube::LoadLookupG3 - " << ((ret) ? "OK" : "ERR") << endl;
	}
	
	return ret;
}

/*
 * Algorithm T45.
 * Save G3 lookup (prune table and lookup) in disk file.
 */
bool Cube::SaveLookupG3()
{
	bool ret = false;
	string fn = PRUNE_G3_FN;
	ofstream cache(fn.c_str());

	if(cache.is_open()) {
		for (int i=0; i<PRUNE_G3_SIZE_N; i++) {
			for (int j=0; j<PRUNE_G3_SIZE_M; j++) {
				cache << m_naPruneG3[i][j] << endl;
			}
		}
		cache.close();
		ret = true;
	} else {
		cout << endl << "ERROR: Cube::SaveLookupG3() - unable to save cache of prune table!" << endl;	
	}
	
	if (ret) {
		fn = LOOKUP_G3_M_FN;
		ret = SaveLookupMap(fn, m_MapIndex2mG3);
	}
	
	if (ret) {
		ret = false;
		fn = CORNPERM_G3_FN;
		cache.open(fn.c_str());
		if(cache.is_open()) {
			for (vector<CPS>::iterator it = m_vCornPermG3.begin(); it != m_vCornPermG3.end(); ++it) {
				CPS cps = *it;
				for (int i=0; i<NUM_OF_CORNERS; i++) {
					cache << cps.cp[i];
				}
				cache << endl;
			}
			cache.close();
			ret = true;
		} else {
			cout << endl << "ERROR: Cube::SaveLookupG3() - unable to save cache of lookup!" << endl;	
		}
	}	
	
	return ret;	
}

/*
 * Algorithm T45.
 * Load G4 lookup (prune table and lookup) from disk file.
 */
bool Cube::LoadLookupG4()
{
	bool ret = false;
	string fn = PRUNE_G4_FN;
	string buf;
	ifstream cache;
	
	LOAD_PRUNE_TABLE(m_naPruneG4,fn,PRUNE_G4_SIZE_N,PRUNE_G4_SIZE_M,ret);
	
	if (ret) {
		fn = LOOKUP_G4_N_FN;
		m_MapIndex2nG4 = LoadLookupMap(fn);
		if (m_MapIndex2nG4.empty()) {
			ret = false;
		}
	}
	
	if (ret) {
		fn = LOOKUP_G4_M_FN;
		m_MapIndex2mG4 = LoadLookupMap(fn);
		if (m_MapIndex2mG4.empty()) {
			ret = false;
		}
	}	

	if (m_bShow) {
		cout << "Cube::LoadLookupG4 - " << ((ret) ? "OK" : "ERR") << endl;
	}
	
	return ret;
}

/*
 * Algorithm T45.
 * Save G4 lookup (prune table and lookup) in disk file.
 */
bool Cube::SaveLookupG4()
{
	bool ret = false;
	string fn = PRUNE_G4_FN;
	ofstream cache(fn.c_str());

	if(cache.is_open()) {
		for (int i=0; i<PRUNE_G4_SIZE_N; i++) {
			for (int j=0; j<PRUNE_G4_SIZE_M; j++) {
				cache << m_naPruneG4[i][j] << endl;
			}
		}
		cache.close();
		ret = true;
	} else {
		cout << endl << "ERROR: Cube::SaveLookupG4() - unable to save cache of prune table!" << endl;	
	}
	
	if (ret) {
		fn = LOOKUP_G4_N_FN;
		ret = SaveLookupMap(fn, m_MapIndex2nG4);
	}
	
	if (ret) {
		fn = LOOKUP_G4_M_FN;
		ret = SaveLookupMap(fn, m_MapIndex2mG4);
	}	
	
	return ret;	
}

/*
 * Load lookup map from cache.
 */
bool Cube::LoadCache(string fn, MapKnownPaths &m)
{
	bool ret = false;
	
	ifstream cached_map;
	string key, buf;
	int mode=0;

	cached_map.open(fn.c_str());
	if(cached_map.is_open()) {
		while(getline(cached_map, buf)) {
			switch(mode) {
				case 0:
					key = buf;
					mode = 1;
					break;

				case 1:
					m[key] = buf;
					mode = 0;
					break;

				default:
					break;
			}
		}
		cached_map.close();
		ret = true;
	}	
	
	return ret;	
}

/*
 * Algorithm T45.
 * Generate prune tables for G0->G1 lookup map.
 * (edges orientation).
 */
void Cube::GenLookupG1()
{
	int eo[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	int empty_slots = PRUNE_G1_SIZE;
	for(unsigned int i=0; i < PRUNE_G1_SIZE; i++) {
		m_naPruneG1[i] = -1;
	}
	int m = EO2Index(eo);
	int d = 0;
	m_naPruneG1[m] = d;
#if defined(MYDBG18)
	cout << "MYDBG18: Cube::GenLookupG1 - m=" << m << endl;
	cout << "MYDBG18: Cube::GenLookupG1 - generating prune table..." << endl;
#endif
	empty_slots--;
	while (0 < empty_slots) {
		d++;
		if (m_bShow)
			cout << "Cube::GenLookupG1 - empty_slots=" << empty_slots << ", d=" << d << ", memory: " << PrivateMemoryUsed() << endl;
		vector<int> v_indicesy;
		v_indicesy.clear();
		for (unsigned int i=0; i < PRUNE_G1_SIZE; i++) {
			if (d-1 == m_naPruneG1[i]) {
				v_indicesy.push_back(i);
			}
		}
		for (vector<int>::iterator it = v_indicesy.begin(); it != v_indicesy.end(); ++it) {
			int x = *it;
			EOCopy(Index2EO(x), eo);
			int moves[] = {MOVE_L, MOVE_R, MOVE_F, MOVE_B, MOVE_U, MOVE_D, MOVE_L2, MOVE_R2, MOVE_F2, MOVE_B2, MOVE_U2, MOVE_D2, MOVE_Li, MOVE_Ri, MOVE_Fi, MOVE_Bi, MOVE_Ui, MOVE_Di};
			for (unsigned int j = 0; j < sizeof(moves)/sizeof(moves[0]); j++) {
				EOApplyMove(moves[j], eo);
				m = EO2Index(eo);
				if (0 > m || PRUNE_G1_SIZE <= m) {	// sanity check
					cout << "Cube::GenLookupG1 - m=" << m << endl;
					cout << "{";
					for (int i=0; i<NUM_OF_EDGES-1; i++) cout << eo[i] << ",";
					cout << eo[NUM_OF_EDGES-1] << "}" << endl;
					throw Exception("Cube::GenLookupG1 - prune table index out of bounds. Algorithm failure!");
				}
				if (-1 == m_naPruneG1[m]) {
					m_naPruneG1[m] = d;
					empty_slots--;
				}
				EOCopy(Index2EO(x), eo);
			}
		}
		if (m_bShow)
			cout << "Cube::GenLookupG1 - distance: " << d-1 << ", items: " << v_indicesy.size() << endl;
	}
}

/*
 * Algorithm T45.
 * Generate prune tables for G1->G2 lookup map.
 */
void Cube::GenLookupG2()
{
	int ep[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}; 	// edges permutation state table (last 4 in place)
	int co[] = {0, 0, 0, 0, 0, 0, 0, 0};				// corners orientation state table (solved)
	int empty_slots = PRUNE_G2_SIZE_N * PRUNE_G2_SIZE_M;
	for(unsigned int i=0; i < PRUNE_G2_SIZE_N; i++) {
		for (unsigned int j=0; j < PRUNE_G2_SIZE_M; j++) {
			m_naPruneG2[i][j] = -1;
		}
	}
	int nx=0, ny=0;
	int n = CO2Index(co);		// corners orientation state to index
	int m = EO2Index(ep);		// edge permutation (pieces 9-12 location) state to index (11-bits: 0-2047)	
	COS co2st;
	EPS ep2st;	
	COCopy(co, co2st.co);
	EPCopy(ep, ep2st.ep);
	map<int,COS> MapCornIdx2St;
	map<int,EPS> MapEdgeIdx2St;
	MapCornIdx2St[nx] = co2st;
	MapEdgeIdx2St[ny] = ep2st;
	m_MapIndex2nG2[n] = nx;		// create a lookup index (0-PRUNE_G2_SIZE_N) to the real index		
	m_MapIndex2mG2[m] = ny;		// create a lookup index (0-PRUNE_G2_SIZE_M) to the real index	
	n = nx++;					// lookup index used to prune table 'n' column
	m = ny++;					// lookup index used to prune table 'm' column
	int d = 0;
	m_naPruneG2[n][m] = d;
#if defined(MYDBG18)
	cout << "MYDBG18: Cube::GenLookupG2 - {n, m}={" << n << ", " << m << "}" << endl;
	cout << "MYDBG18: Cube::GenLookupG2 - generating prune table..." << endl;
#endif
	empty_slots--;
	while (0 < empty_slots) {
		d++;
		if (m_bShow)
			cout << "Cube::GenLookupG2 - empty_slots=" << empty_slots << ", d=" << d << ", memory: " << PrivateMemoryUsed() << endl;
		vector<indice> v_indicesxy;
		v_indicesxy.clear();
		for (unsigned int i=0; i<PRUNE_G2_SIZE_N; i++) {
			for (unsigned int j=0; j<PRUNE_G2_SIZE_M; j++) {
				if (d-1 == m_naPruneG2[i][j]) {
					indice ind;
					ind.x = i; ind.y = j;
					v_indicesxy.push_back(ind);
				}
			}
		}
		for (vector<indice>::iterator it = v_indicesxy.begin(); it != v_indicesxy.end(); ++it) {
			int x=(*it).x, y=(*it).y;
			COCopy(MapCornIdx2St[x].co, co);
			EPCopy(MapEdgeIdx2St[y].ep, ep);
			//int moves[] = {MOVE_L, MOVE_R, MOVE_F, MOVE_B, MOVE_U2, MOVE_L2, MOVE_R2, MOVE_F2, MOVE_B2, MOVE_D2, MOVE_Li, MOVE_Ri, MOVE_Fi, MOVE_Bi};
			int moves[] = {MOVE_L, MOVE_Li, MOVE_L2, MOVE_R, MOVE_Ri, MOVE_R2, MOVE_F, MOVE_Fi, MOVE_F2, MOVE_B, MOVE_Bi, MOVE_B2, MOVE_U2, MOVE_D2};
			for (unsigned int j = 0; j < sizeof(moves)/sizeof(moves[0]); j++) {
				COApplyMove(moves[j], co);
				EPApplyMove(moves[j], ep);
				n = CO2Index(co);	// calculate prune table index 'n' from corners orientations state
				m = EO2Index(ep);	// calculate prune table index 'm' from edges permutations state
				// find a lookup index to the 'n' column of prune table
				pair<map<int,int>::iterator,bool> keyfound;
				keyfound = m_MapIndex2nG2.insert(pair<int,int>(n,nx));
				if (keyfound.second) {
					COCopy(co, co2st.co);
					MapCornIdx2St[nx] = co2st;
				}
				n = ((keyfound.second) ? nx++ : keyfound.first->second);				
				// find a lookup index to the 'm' column of prune table
				keyfound = m_MapIndex2mG2.insert(pair<int,int>(m,ny));
				if (keyfound.second) {
					EPCopy(ep, ep2st.ep);
					MapEdgeIdx2St[ny] = ep2st;
				}				
				m = ((keyfound.second) ? ny++ : keyfound.first->second);
#if defined(MYDBG21)
				cout << "MYDBG21: Cube::GenLookupG2 - {n, m}={" << n << ", " << m << "}" << endl;
#endif
				// these are error conditions to detect bugs in algorithm
				if (0>n || PRUNE_G2_SIZE_N<=n || 0>m || PRUNE_G2_SIZE_M<=m) {
					cout << "Cube::GenLookupG2 - {n, m}={" << n << ", " << m << "}" << endl;
					throw Exception("Cube::GenLookupG2 - wrong prune table index. Algorithm failure!");
				}
				// end of error conditions
				if (-1 == m_naPruneG2[n][m]) {	// if slot not taken, fill it with distance 'd'
					m_naPruneG2[n][m] = d;
					empty_slots--;
				}
				COCopy(MapCornIdx2St[x].co, co);
				EPCopy(MapEdgeIdx2St[y].ep, ep);				
			}
		}
		if (m_bShow)
			cout << "MYDBG18: Cube::GenLookupG2 - distance: " << d-1 << ", items: " << v_indicesxy.size() << endl;
	}
}

/*
 * Algorithm T45.
 * Generate the set of 96 corner permutations starting with solved cube (provided externally)
 * and applying moves from group G3. These will all be initial states (with d=0) for prune
 * table.
 */
void Cube::GenInitStatesG3(int *ep, int *cp, int &empty_slots, int &index)
{
	int g3init_slots = 96;
	int n = CP2Index(cp);
	int m = LREP2Index(ep);			// generate the real index (11-bits: 0-2047)
	m_MapIndex2mG3[m] = index;		// create a lookup index (0-PRUNE_G3_SIZE_M) to the real index
	m = index++;					// the lookup index is used on prune table 'm' column
	int d=0;
	m_naPruneG3[n][m] = d;
	CPS cp2st;
	CPCopy(cp, cp2st.cp);
	m_vCornPermG3.push_back(cp2st);	// add new unique G3 corner permutation state
#if defined(MYDBG18)
	cout << "MYDBG18: Cube::GenInitStatesG3 - {n, m}={" << n << ", " << m << "}" << endl;
	cout << "MYDBG18: Cube::GenInitStatesG3 - generating initial states..." << endl;
#endif
	empty_slots--;
	g3init_slots--;
	while (0 < g3init_slots) {
		d++;
		vector<indice> v_indicesxy;
		v_indicesxy.clear();
		for (unsigned int i=0; i<PRUNE_G3_SIZE_N; i++) {
			for (unsigned int j=0; j<PRUNE_G3_SIZE_M; j++) {
				if (d-1 == m_naPruneG3[i][j]) {
					indice ind;
					ind.x = i; ind.y = j;
					v_indicesxy.push_back(ind);
				}
			}
		}	
		for (vector<indice>::iterator it = v_indicesxy.begin(); it != v_indicesxy.end(); ++it) {
			int x=(*it).x, y=(*it).y, mm=-1;
			//m = -1; // 'y' is a lookup index to colum 'm' of the prune table
			// do a linear map search to find a key associated with the lookup index - that key is a real index
			for (map<int,int>::iterator it = m_MapIndex2mG3.begin(); it != m_MapIndex2mG3.end(); ++it) {
				if (it->second == y) {
					mm = it->first;
					break;
				}
			}
			if (0 > mm)	// this should never happen
				throw Exception("Cube::GenInitStatesG3 - real index not found in map. Algorithm failure!");
			CPCopy(Index2CP(x), cp);	// restore corners permutations status from index x
			EPCopy(Index2LREP(mm), ep);	// restore edges permutation status from index m via lookup index y
			int g3moves[] = {MOVE_L2, MOVE_R2, MOVE_F2, MOVE_B2, MOVE_U2, MOVE_D2};
			for (unsigned int j = 0; j < sizeof(g3moves)/sizeof(g3moves[0]); j++) {
				CPApplyMove(g3moves[j], cp);
				EPApplyMove(g3moves[j], ep);
				n = CP2Index(cp);	// calculate prune table index 'n' from corners permutations state
				m = LREP2Index(ep);	// calculate prune table index 'm' from edges permutations state
				// find lookup indexes to the 'm' column of prune table
				pair<map<int,int>::iterator,bool> keyfound;
				keyfound = m_MapIndex2mG3.insert(pair<int,int>(m,index));
				m = ((keyfound.second) ? index++ : keyfound.first->second);
#if defined(MYDBG21)
				cout << "MYDBG21: Cube::GenInitStatesG3 - {n, m}={" << n << ", " << m << "}" << endl;
#endif
				// these are error conditions to detect bugs in algorithm
				if (0>n || PRUNE_G3_SIZE_N<=n || 0>m || PRUNE_G3_SIZE_M<=m) {
					cout << "ep=";
					for (int j=0; j<NUM_OF_EDGES; j++) {
						cout << ep[j];
					}
					cout << endl;
					cout << "Cube::GenInitStatesG3 - {n, m}={" << n << ", " << m << "}" << endl;
					throw Exception("Cube::GenInitStatesG3 - wrong prune table index. Algorithm failure!");
				}
				// end of error conditions
				if (-1 == m_naPruneG3[n][m]) {	// if slot not taken, fill it with distance 'd'
					m_naPruneG3[n][m] = d;
					CPCopy(cp, cp2st.cp);				// add new unique G3 corner permutation state
					m_vCornPermG3.push_back(cp2st);
					empty_slots--;
					g3init_slots--;
				}
				CPCopy(Index2CP(x), cp);	// restore corners permutations status from index x
				EPCopy(Index2LREP(mm), ep);
			}			
		}
		if (m_bShow)
			cout << "Cube::GenInitStatesG3 - distance: " << d-1 << ", items: " << v_indicesxy.size() << endl;
	}	
	for (unsigned int i=0; i<PRUNE_G3_SIZE_N; i++) {
		for (unsigned int j=0; j<PRUNE_G3_SIZE_M; j++) {
			if (-1 != m_naPruneG3[i][j])
				m_naPruneG3[i][j] = 0;
		}
	}	
}

/*
 * Algorithm T45.
 * Generate prune tables for G2->G3 lookup map.
 */
void Cube::GenLookupG3()
{
	int ep[] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2}; 	// edges permutation state table
	int cp[] = {1, 2, 3, 4, 5, 6, 7, 8};				// corners permutation state table
	int empty_slots = PRUNE_G3_SIZE_N * PRUNE_G3_SIZE_M;
	for(unsigned int i=0; i < PRUNE_G3_SIZE_N; i++) {
		for (unsigned int j=0; j < PRUNE_G3_SIZE_M; j++) {
			m_naPruneG3[i][j] = -1;
		}
	}
	int index=0;
	GenInitStatesG3(ep, cp, empty_slots, index);
	int n=0, m=0, d=0;
#if defined(MYDBG18)
	cout << "MYDBG18: Cube::GenLookupG3 - {n, m}={" << n << ", " << m << "}" << endl;
	cout << "MYDBG18: Cube::GenLookupG3 - generating prune table..." << endl;
#endif	
	while (0 < empty_slots) {
		d++;
		if (m_bShow)
			cout << "Cube::GenLookupG3 - empty_slots=" << empty_slots << ", d=" << d << ", memory: " << PrivateMemoryUsed() << endl;
		vector<indice> v_indicesxy;
		v_indicesxy.clear();
		for (unsigned int i=0; i<PRUNE_G3_SIZE_N; i++) {
			for (unsigned int j=0; j<PRUNE_G3_SIZE_M; j++) {
				if (d-1 == m_naPruneG3[i][j]) {
					indice ind;
					ind.x = i; ind.y = j;
					v_indicesxy.push_back(ind);
				}
			}
		}
		for (vector<indice>::iterator it = v_indicesxy.begin(); it != v_indicesxy.end(); ++it) {
			int x=(*it).x, y=(*it).y, mm=-1; // 'y' is a lookup index to the columns 'm' of the prune table
			// so, do a linear map search to find a key associated with the lookup index - that key is a real index
			for (map<int,int>::iterator it = m_MapIndex2mG3.begin(); it != m_MapIndex2mG3.end(); ++it) {
				if (it->second == y) {
					mm = it->first;
					break;
				}
			}
			if (0 > mm)	// this should never happen
				throw Exception("Cube::GenLookupG3 - real index not found in map. Algorithm failure!");
			CPCopy(Index2CP(x), cp);	// restore corners permutation status from index x
			EPCopy(Index2LREP(mm), ep);	// restore edges permutation status from index m via lookup index y
			int moves[] = {MOVE_L, MOVE_Li, MOVE_L2, MOVE_R, MOVE_Ri, MOVE_R2, MOVE_F2, MOVE_B2, MOVE_U2, MOVE_D2};
			for (unsigned int j = 0; j < sizeof(moves)/sizeof(moves[0]); j++) {
				CPApplyMove(moves[j], cp);
				EPApplyMove(moves[j], ep);
				n = CP2Index(cp);	// calculate prune table index 'n' from corners permutation state
				m = LREP2Index(ep);	// calculate prune table index 'm' from edges permutations state
				// find a lookup index to the 'm' column of prune table
				pair<map<int,int>::iterator,bool> keyfound;
				keyfound = m_MapIndex2mG3.insert(pair<int,int>(m,index));
				m = ((keyfound.second) ? index++ : keyfound.first->second);
#if defined(MYDBG21)
				cout << "MYDBG21: Cube::GenLookupG3 - {n, m}={" << n << ", " << m << "}" << endl;
#endif
				// these are error conditions to detect bugs in algorithm
				if (0>n || PRUNE_G3_SIZE_N<=n || 0>m || PRUNE_G3_SIZE_M<=m) {
					cout << "Cube::GenLookupG3 - {n, m}={" << n << ", " << m << "}" << endl;
					throw Exception("Cube::GenLookupG3 - wrong prune table index. Algorithm failure!");
				}
				// end of error conditions
				if (-1 == m_naPruneG3[n][m]) {	// if slot not taken, fill it with distance 'd'
					m_naPruneG3[n][m] = d;
					empty_slots--;
				}
				CPCopy(Index2CP(x), cp);
				EPCopy(Index2LREP(mm), ep);
			}
		}
		if (m_bShow)
			cout << "Cube::GenLookupG3 - distance: " << d-1 << ", items: " << v_indicesxy.size() << endl;
	}
}

/*
 * Algorithm T45.
 * Generate prune tables for G3->G4 lookup map.
 */
void Cube::GenLookupG4()
{
	int ep[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; 	// edges permutation state table
	int cp[] = {1, 2, 3, 4, 5, 6, 7, 8};					// corners permutation state table
	int empty_slots = PRUNE_G4_SIZE_N * PRUNE_G4_SIZE_M;
	for(unsigned int i=0; i < PRUNE_G4_SIZE_N; i++) {
		for (unsigned int j=0; j < PRUNE_G4_SIZE_M; j++) {
			m_naPruneG4[i][j] = -1;
		}
	}
	int n=0, m=0, d=0, nx=0, mx=0;
	n = CP2Index(cp);
	m = EP2Index(ep);
	m_MapIndex2nG4[n] = nx;
	m_MapIndex2mG4[m] = mx;
	n = nx++;
	m = mx++;
#if defined(MYDBG18)
	cout << "MYDBG18: Cube::GenLookupG4 - {n, m}={" << n << ", " << m << "}" << endl;
	cout << "MYDBG18: Cube::GenLookupG4 - generating prune table..." << endl;
#endif	
	m_naPruneG4[n][m] = d;
	empty_slots--;
	while (0 < empty_slots) {
		d++;
		if (m_bShow)
			cout << "Cube::GenLookupG4 - empty_slots=" << empty_slots << ", d=" << d << ", memory: " << PrivateMemoryUsed() << endl;
		vector<indice> v_indicesxy;
		v_indicesxy.clear();
		for (unsigned int i=0; i<PRUNE_G4_SIZE_N; i++) {
			for (unsigned int j=0; j<PRUNE_G4_SIZE_M; j++) {
				if (d-1 == m_naPruneG4[i][j]) {
					indice ind;
					ind.x = i; ind.y = j;
					v_indicesxy.push_back(ind);
				}
			}
		}
		for (vector<indice>::iterator it = v_indicesxy.begin(); it != v_indicesxy.end(); ++it) {
			int x=(*it).x, y=(*it).y, nn=-1, mm=-1;
			// x, y are not real indexes but a lookup indexes in the columns 'n' and 'm' of the prune table
			// so, do a linear map search to find the key values associated with the lookup indexes - these keys are the real indexes
			for (map<int,int>::iterator it = m_MapIndex2nG4.begin(); it != m_MapIndex2nG4.end(); ++it) {
				if (it->second == x) {
					nn = it->first;
					break;
				}
			}			
			for (map<int,int>::iterator it = m_MapIndex2mG4.begin(); it != m_MapIndex2mG4.end(); ++it) {
				if (it->second == y) {
					mm = it->first;
					break;
				}
			}
			if (0 > nn)	// this should never happen
				throw Exception("Cube::GenLookupG4 - real index N not found in map. Algorithm failure!");			
			if (0 > mm)	// this should never happen
				throw Exception("Cube::GenLookupG4 - real index M not found in map. Algorithm failure!");
			CPCopy(Index2CP(nn), cp);	// restore corners permutation status from index x
			EPCopy(Index2EP(mm), ep);	// restore edges permutation status from index y
			int moves[] = {MOVE_L2, MOVE_R2, MOVE_F2, MOVE_B2, MOVE_U2, MOVE_D2};			
			for (unsigned int j = 0; j < sizeof(moves)/sizeof(moves[0]); j++) {
				CPApplyMove(moves[j], cp);
				EPApplyMove(moves[j], ep);
				n = CP2Index(cp);	// calculate prune table lookup index 'n' from corners permutation state
				m = EP2Index(ep);	// calculate prune table lookup index 'm' from edges permutations state
				// convert to actual indexes
				pair<map<int,int>::iterator,bool> keyfound;
				keyfound = m_MapIndex2nG4.insert(pair<int,int>(n,nx));
				n = ((keyfound.second) ? nx++ : keyfound.first->second);
				keyfound = m_MapIndex2mG4.insert(pair<int,int>(m,mx));
				m = ((keyfound.second) ? mx++ : keyfound.first->second);
#if defined(MYDBG21)
				cout << "MYDBG21: Cube::GenLookupG4 - {n, m}={" << n << ", " << m << "}" << endl;
#endif
				// these are error conditions to detect bugs in algorithm
				if (0>n || PRUNE_G4_SIZE_N<=n || 0>m || PRUNE_G4_SIZE_M<=m) {
					cout << "Cube::GenLookupG4 - {n, m}={" << n << ", " << m << "}" << endl;
					throw Exception("Cube::GenLookupG4 - wrong prune table index. Algorithm failure!");
				}
				// end of error conditions
				if (-1 == m_naPruneG4[n][m]) {	// if slot not taken, fill it with distance 'd'
					m_naPruneG4[n][m] = d;
					empty_slots--;
				}
				CPCopy(Index2CP(nn), cp);	// restore corners permutation status from index x
				EPCopy(Index2EP(mm), ep);	// restore edges permutation status from index y				
			}
		}
		if (m_bShow)
			cout << "Cube::GenLookupG4 - distance: " << d-1 << ", items: " << v_indicesxy.size() << endl;
	}
}

/*
 * Convert edges orientation states to integer.
 * Facelet cube model.
 */
int Cube::EdgesOrientStates2Index(RCube *p)
{
	int ret = 0;
	int *pedges = NULL;
	
	pedges = ConvertCubeEdges2OrientTbl(p);

	ret = EO2Index(pedges);

#if defined(MYDBG19)
	cout << "MYDBG19: Cube::EdgesOrientStates2Index - index=" << ret << endl;
#endif	
	
	return ret;
}

/*
 * Solve the cube using layer-by-layer human method.
 */
bool Cube::LayerByLayer()
{
	bool ret = false;

	if (SolveCrossDet(m_pRoot))
		ret = Solve1stLayerCorners(m_pCurrent);

	if (m_bSolved)	return true;

	if (ret)
		ret = SolveMiddleLayer(m_pCurrent);

	if (m_bSolved)	return true;
	
	if (ret)
		ret = L3EdgeOrientation(m_pCurrent);

	if (m_bSolved)	return true;

	if (ret)
		ret = PositionBottomLayerCorners(m_pCurrent);

	if (m_bSolved)	return true;

	if (ret)
		ret = OrientBottomLayerCorners(m_pCurrent);

	if (m_bSolved)	return true;

	if (ret)
		ret = L3EdgePermutation(m_pCurrent);

	ret = m_bSolved || IsSolved(m_pCurrent);

	return ret;
}

// Layer-by-layer algorithm
// helper macro
#define CROSS_SOLVED_1(p)	\
	(p->up[0][1] == U1 || p->up[0][1] == U3 || p->up[0][1] == U5 || p->up[0][1] == U7)	\
	&&	\
	(p->up[1][0] == U1 || p->up[1][0] == U3 || p->up[1][0] == U5 || p->up[1][0] == U7)	\
	&&	\
	(p->up[1][2] == U1 || p->up[1][2] == U3 || p->up[1][2] == U5 || p->up[1][2] == U7)	\
	&&	\
	(p->up[2][1] == U1 || p->up[2][1] == U3 || p->up[2][1] == U5 || p->up[2][1] == U7)

#define CROSS_MAX_ATTEMPTS 30

/*
 * Solve the upper-layer (1st layer) cross.
 * Deterministic algorithm (covering all cases).
 */
bool Cube::SolveCrossDet(RCube *start)
{
	if (NULL == start)
		throw("Cube::SolveCrossDet - NULL pointer!");

	if (m_bShow)
		cout << "Solving L1 cross..." << endl;

	bool ret = false;
	int n = CROSS_MAX_ATTEMPTS;

	while (false == (ret = CROSS_SOLVED_1(start)) && 0 < n--) {
		start = SolveCrossStep1(start);
	}
	if (ret) {
		n = CROSS_MAX_ATTEMPTS;
		while (false == (ret = IsCrossSolved(start)) && 0 < n--) {
			start = SolveCrossStep2(start);
		}
	}

	if (m_bShow)
		cout << "L1 cross " << ((ret) ? "solved" : "unsolved") << "." << endl;

	m_pCurrent = start;

	return ret;
}

// Layer-by-layer algorithm
// big and ugly helper macro
#define SEARCH_CROSS_EDGES_1(p,fl1,fl2,fl3,fl4,move,en)											\
	if (fl1 == U1 || fl2 == U1 || fl3 == U1														\
	        ||																					\
	        fl1 == U3 || fl2 == U3 || fl3 == U3													\
	        ||																					\
	        fl1 == U5 || fl2 == U5 || fl3 == U5													\
	        ||																					\
	        fl1 == U7 || fl2 == U7 || fl3 == U7													\
	   )																						\
	{																							\
		if (fl4 == U1 || fl4 == U3 || fl4 == U5 || fl4 == U7)									\
			p = DoMove(p, MOVE_U);																\
		if (false == m_bSolved && false == IsSolved(p) 											\
				&& fl4 != U1 && fl4 != U3 && fl4 != U5 && fl4 != U7)							\
		{																						\
			int n=4;																			\
			while (false == m_bSolved && false == IsSolved(p) 									\
						&& fl4 != U1 && fl4 != U3 && fl4 != U5 && fl4 != U7 					\
			        	&& 0 < n--)																\
			{																					\
				p = DoMove(p, move);															\
			}																					\
			p = (m_bSolved ? m_pCurrent : p);													\
			if (fl4 != U1 && fl4 != U3 && fl4 != U5 && fl4 != U7)								\
			{																					\
				cout << "************************** ERROR: " << en << endl;						\
				m_pSolved = ret;																\
				PrintSolution();																\
				throw Exception("Cube::SolveCrossStep1 - unexpected algorithm failure");		\
			}																					\
		}																						\
		ret = (m_bSolved ? m_pCurrent : p);														\
	}

// Layer-by-layer algorithm
// helper macros
#define IS_TOP_CROSS_PIECE(p) (p == U1 || p == U3 || p == U5 || p == U7)

#define BAIL_IF_SOLVED(p)										\
				if (m_bSolved || IsSolved(p) || (CROSS_SOLVED_1(p)))	\
					return (m_bSolved ? m_pCurrent : p);

/*
 * Layer-by-layer algorithm.
 * Move Upper edge pieces to the empty spots on center Upper face.
 * Don't worry about their adjacent face position yet.
 * Input - unsolved cube.
 * Output - cube with upper face cross step 1 solved (no side colors adjusted yet).
 */
RCube *Cube::SolveCrossStep1(RCube *start)
{
	RCube *ret = start;
	RCube *p = ret;

	BAIL_IF_SOLVED(p);
	SEARCH_CROSS_EDGES_1(p, p->front[1][0], p->down[1][0], p->bottom[1][2], p->up[1][0], MOVE_L, 1);
	BAIL_IF_SOLVED(p);
	SEARCH_CROSS_EDGES_1(p, p->front[1][2], p->down[1][2], p->bottom[1][0], p->up[1][2], MOVE_R, 2);
	BAIL_IF_SOLVED(p);
	SEARCH_CROSS_EDGES_1(p, p->right[1][2], p->left[1][0], p->down[2][1], p->up[0][1], MOVE_B, 3);
	BAIL_IF_SOLVED(p);
	SEARCH_CROSS_EDGES_1(p, p->right[1][0], p->left[1][2], p->down[0][1], p->up[2][1], MOVE_F, 4);
	BAIL_IF_SOLVED(p);

	if (IS_TOP_CROSS_PIECE(p->down[0][1])) {
		if (false == IS_TOP_CROSS_PIECE(p->up[2][1])) {
			p = DoMoves(p, "F2");
		} else {
			p = DoMove(p, MOVE_D);
		}
		ret = p;
	} else if (IS_TOP_CROSS_PIECE(p->down[2][1])) {
		if (false == IS_TOP_CROSS_PIECE(p->up[0][1])) {
			p = DoMoves(p, "B2");
		} else {
			p = DoMove(p, MOVE_Di);
		}
		ret = p;
	}
	BAIL_IF_SOLVED(p);

	if (IS_TOP_CROSS_PIECE(p->left[0][1])
	        ||
	        IS_TOP_CROSS_PIECE(p->right[0][1])
	        ||
	        IS_TOP_CROSS_PIECE(p->bottom[0][1])
	   ) {
		int n=4;
		while(false == IS_TOP_CROSS_PIECE(p->front[0][1]) && 0 < n--) {
			p = DoMove(p, MOVE_U);
			BAIL_IF_SOLVED(p);
		}
		ret = p;
	}

	if (IS_TOP_CROSS_PIECE(p->front[0][1])) {
		if (false == IS_TOP_CROSS_PIECE(p->up[1][2])) {
			p = DoMoves(p, "FR");
		} else if (false == IS_TOP_CROSS_PIECE(p->up[0][2])) {
			p = DoMoves(p, "FiLi");
		} else if (false == IS_TOP_CROSS_PIECE(p->up[0][1])) {
			p = DoMoves(p, "FUR");
		} else {
			p = DoMoves(p, "F2DRLDiRiLi");
		}
		ret = p;
		BAIL_IF_SOLVED(p);
	}

	if (IS_TOP_CROSS_PIECE(p->left[2][1])
	        ||
	        IS_TOP_CROSS_PIECE(p->bottom[2][1])
	        ||
	        IS_TOP_CROSS_PIECE(p->right[2][1])
	   ) {
		int n=4;
		while(false == IS_TOP_CROSS_PIECE(p->front[2][1]) && 0 < n--) {
			p = DoMove(p, MOVE_D);
			BAIL_IF_SOLVED(p);
		}
		ret = p;
	}

	if (IS_TOP_CROSS_PIECE(p->front[2][1])) {
		if (false == IS_TOP_CROSS_PIECE(p->up[1][2])) {
			p = DoMoves(p, "FiRF");
		} else if (false == IS_TOP_CROSS_PIECE(p->up[1][0])) {
			p = DoMoves(p, "FLiFi");
		} else
			p = DoMove(p, MOVE_U);
		ret = p;
	}

	return ret;
}

// Layer-by-layer algorithm
// helper macro
#define BAIL_IF_SOLVED2(p)											\
				if (m_bSolved || IsSolved(p) || IsCrossSolved(p))	\
					return (m_bSolved ? m_pCurrent : p);

/*
 * Layer-by-layer algorithm.
 * Swap the upper face edge cubes so the side colors are aligned.
 * Input - solved upper face cross step 1 (no side colors aligned).
 * Output - solved upper face cross (side colors aligned).
 */
RCube *Cube::SolveCrossStep2(RCube *start)
{
	RCube *ret = start, *p = ret;
	int n = 4;

	BAIL_IF_SOLVED2(p);
	// right and left swap case
	if (p->up[1][0] == U5 && p->up[1][2] == U3) {
		n = 10;
		while (p->up[1][0] != U3 && p->up[1][2] != U5 && 0 < n--) {
			p = DoMoves(p,"F2B2U2F2B2");
			BAIL_IF_SOLVED2(p);
		}
		ret = p;
	}

	// front and right swap case
	if(p->up[2][1] == U5 && p->up[1][2] == U7) {
		n = 10;
		while (p->up[2][1] != U7 && p->up[1][2] != U5 && 0 < n--) {
			p = DoMoves(p, "F2DR2DiF2");
			BAIL_IF_SOLVED2(p);
		}
		ret = p;
	}

	// left and back swap case
	if(p->up[1][0] == U1 && p->up[0][1] == U3) {
		p = DoMoves(p, "U2");
		BAIL_IF_SOLVED2(p);
		n = 10;
		while (p->up[2][1] != U1 && p->up[1][2] != U3 && 0 < n--) {
			p = DoMoves(p, "F2DR2DiF2");
			BAIL_IF_SOLVED2(p);
		}
		p = DoMoves(p, "U2");
		ret = p;
		BAIL_IF_SOLVED2(p);
	}

	// front and left swap case
	if(p->up[2][1] == U3 && p->up[1][0] == U7) {
		n = 10;
		while (p->up[2][1] != U7 && p->up[1][0] != U3 && 0 < n--) {
			p = DoMoves(p, "F2DiL2DF2");
			BAIL_IF_SOLVED2(p);
		}
		ret = p;
	}

	// back and right swap case
	if(p->up[0][1] == U5 && p->up[1][2] == U1) {
		p = DoMoves(p, "U2");
		BAIL_IF_SOLVED2(p);
		n = 10;
		while (p->up[2][1] != U1 && p->up[1][0] != U5 && 0 < n--) {
			p = DoMoves(p, "F2DiL2DF2");
			BAIL_IF_SOLVED2(p);
		}
		p = DoMoves(p, "U2");
		ret = p;
		BAIL_IF_SOLVED2(p);
	}

	// front and back swap case (like left/right swap, just move
	// upper face clockwise, perform sequence and move upper face
	// counter-clockwise to return to proper orientation).
	if(p->up[0][1] == U7 && p->up[2][1] == U1) {
		p = DoMove(p, MOVE_U);
		BAIL_IF_SOLVED2(p);
		int n=10;
		while (p->up[1][2] != U1 && p->up[1][0] != U7 && 0 < n--) {
			p = DoMoves(p, "F2B2U2F2B2");
			BAIL_IF_SOLVED2(p);
		}
		p = DoMove(p, MOVE_Ui);
		ret = p;
		BAIL_IF_SOLVED2(p);
	}

	// align back face color
	if (p->up[0][1] != U1) {
		n = 4;
		while (p->up[0][1] != U1 && 0 < n--) {
			p = DoMove(p, MOVE_U);
			BAIL_IF_SOLVED2(p);
		}
		ret = p;
	} else if (p->up[2][1] != U7) { // align front face color
		n = 4;
		while (p->up[2][1] != U7 && 0 < n--) {
			p = DoMove(p, MOVE_U);
			BAIL_IF_SOLVED2(p);
		}
		ret = p;
	} else if (p->up[1][0] != U3) { // align left face color
		n = 4;
		while (p->up[1][0] != U3 && 0 < n--) {
			p = DoMove(p, MOVE_U);
			BAIL_IF_SOLVED2(p);
		}
		ret = p;
	} else if (p->up[1][2] != U5) { // align right face color
		n = 4;
		while (p->up[1][2] != U5 && 0 < n--) {
			p = DoMove(p, MOVE_U);
			BAIL_IF_SOLVED2(p);
		}
		ret = p;
	}

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Return true if node p is a solved cross (upper face).
 */
bool Cube::IsCrossSolved(RCube *p)
{
	bool ret = false;

	ret = (
	          p->up[0][1] == U1
	          &&
	          p->up[1][0] == U3
	          &&
	          p->up[1][1] == U4
	          &&
	          p->up[1][2] == U5
	          &&
	          p->up[2][1] == U7
	      );

	return ret;
}

// Layer-by-layer algorithm - helper macros.
#define IS_TOPLR_CRPIECE_LUB(p)	(p == U0 || p == L0 || p == B2)
#define IS_TOPLR_CRPIECE_RUB(p)	(p == U2 || p == R2 || p == B0)
#define IS_TOPLR_CRPIECE_LUF(p)	(p == U6 || p == F0 || p == L2)
#define IS_TOPLR_CRPIECE_RUF(p)	(p == U8 || p == F2 || p == R0)

#define IS_TOP_LAYER_CORNER_PIECE(p)		\
	(IS_TOPLR_CRPIECE_LUB(p)	||			\
	 IS_TOPLR_CRPIECE_RUB(p) ||				\
	 IS_TOPLR_CRPIECE_LUF(p) || 			\
	 IS_TOPLR_CRPIECE_RUF(p))

/*
 * Layer-by-layer algorithm.
 * Return true if node is a solved cross (top layer)
 * with corners inserted but not yet positioned
 * or oriented.
 */
bool Cube::IsTopLayerSolvedStep1(RCube *p)
{
	bool ret = false;

	ret = (
	          IsCrossSolved(p)
	          && (IS_TOP_LAYER_CORNER_PIECE(p->up[0][0])
	              &&
	              IS_TOP_LAYER_CORNER_PIECE(p->up[0][2])
	              &&
	              IS_TOP_LAYER_CORNER_PIECE(p->up[2][0])
	              &&
	              IS_TOP_LAYER_CORNER_PIECE(p->up[2][2]))
	      );

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Return true if node is a solved cross (top layer)
 * with corners inserted and positioned
 * but not yet oriented.
 */
bool Cube::IsTopLayerSolvedStep2(RCube *p)
{
	bool ret = false;

	ret = (
	          IsCrossSolved(p)
	          && (IS_TOPLR_CRPIECE_LUB(p->up[0][0])
	              &&
	              IS_TOPLR_CRPIECE_RUB(p->up[0][2])
	              &&
	              IS_TOPLR_CRPIECE_LUF(p->up[2][0])
	              &&
	              IS_TOPLR_CRPIECE_RUF(p->up[2][2]))
	      );

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Return true if node is a solved top layer
 * (cross solved and corners inserted and properly
 * positioned and oriented).
 */
bool Cube::IsTopLayerSolved(RCube *p)
{
	bool ret = false;

	ret = (
	          IsCrossSolved(p)
	          && (p->up[0][0] == U0
	              &&
	              p->up[0][2] == U2
	              &&
	              p->up[2][0] == U6
	              &&
	              p->up[2][2] == U8)
	      );

	return ret;
}

// Layer-by-layer algorithm
// helper macro
#define IS_CUBE_SOLVED(p)				\
			(m_bSolved || IsSolved(p))

/*
 * Layer-by-layer algorithm.
 * Insert 1st layer corners (regardless of position and orientation).
 * Input: *p - a solved 1st layer cross with edge pieces aligned to middle center pieces.
 * Output: *p - 1st layer cross solved with corner pieces inserted (position and orientation not important)
 * Return: ret - true:solved/false:unsolved
 */
bool Cube::Insert1stLayerCorners(RCube *p)
{
	bool ret = false;
	int m = 4;

	while (false == (ret = (IS_CUBE_SOLVED(p) || IsTopLayerSolvedStep1(p))) && 0 < m--) {
		int n = 4;
		while(false == IS_CUBE_SOLVED(p) && IS_TOP_LAYER_CORNER_PIECE(p->up[2][2]) && 0 < n--)
			p = DoMove(p, MOVE_U);
		n = 4;
		while(false == IS_CUBE_SOLVED(p) && false == IS_TOP_LAYER_CORNER_PIECE(p->down[0][2]) && 0 < n--)
			p = DoMove(p, MOVE_D);
		if (false == IS_CUBE_SOLVED(p))
			p = DoMoves(p, "RiDiRD");
		if (false == IS_CUBE_SOLVED(p))
			p = DoMove(p, MOVE_U);
		if (IS_CUBE_SOLVED(p)) return true;
	}

	m = 4;
	while(false == IS_CUBE_SOLVED(p) && p->up[2][1] != U7 && 0 < m--)
		p = DoMove(p, MOVE_U);

	ret = IsTopLayerSolvedStep1(p) || IS_CUBE_SOLVED(p);
	
	if (IS_CUBE_SOLVED(p))
		p = m_pCurrent;	

	m_pCurrent = p;

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Position 1st layer corners (regardless of the orientation).
 * Input: *p - solved 1st layer cross with inserted corners (position and orientation not important)
 * Output: *p - solved 1st layer cross with corner pieces positioned, but not oriented
 * Return: ret - true:solved/false:unsolved
 */
bool Cube::Position1stLayerCorners(RCube *p)
{
	if (IsTopLayerSolvedStep2(p) || IS_CUBE_SOLVED(p))
		return true;

	bool ret = false;
	int corners[] = {p->up[2][2], p->up[0][2], p->up[0][0], p->up[2][0]};
	bool pieces_positioned[] = {false, false, false, false};

	pieces_positioned[0] = IS_TOPLR_CRPIECE_RUF(corners[0]);
	pieces_positioned[1] = IS_TOPLR_CRPIECE_RUB(corners[1]);
	pieces_positioned[2] = IS_TOPLR_CRPIECE_LUB(corners[2]);
	pieces_positioned[3] = IS_TOPLR_CRPIECE_LUF(corners[3]);

	for (int i=0; i < 4 && false == IS_CUBE_SOLVED(p); i++) {
		if (pieces_positioned[i])
			p = DoMove(p, MOVE_U);
		else
			p = DoMoves(p, "RiDiRDU");
	}
	ret = IsTopLayerSolvedStep2(p) || IS_CUBE_SOLVED(p);
	if (IS_CUBE_SOLVED(p))
		p = m_pCurrent;
			
	m_pCurrent = p;

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Orient 1st layer corners.
 * Input: *p - solved 1st layer cross with corner pieces positioned, but not oriented
 * Output: *p - solved 1st layer cross with corner pieces positioned and oriented
 * Return: ret - true:solved/false:unsolved
 */
bool Cube::Orient1stLayerCorners(RCube *p)
{
	if (IsTopLayerSolved(p) || IS_CUBE_SOLVED(p)) return true;

	bool ret = false;
	int corners[]	= {p->up[2][2], p->up[0][2], p->up[0][0], p->up[2][0]};
	int goal[]	  	= {U8,          U2,          U0,          U6};
	bool pieces_oriented[] = {false, false, false, false};

	pieces_oriented[0] = (corners[0] == U8);
	pieces_oriented[1] = (corners[1] == U2);
	pieces_oriented[2] = (corners[2] == U0);
	pieces_oriented[3] = (corners[3] == U6);

	for (int i=0; i < 4 && false == IS_CUBE_SOLVED(p); i++) {
		if (pieces_oriented[i])
			p = DoMove(p, MOVE_U);
		else {
			int n=4;
			while(false == IS_CUBE_SOLVED(p) && p->up[2][2] != goal[i] && 0 < n--)
				p = DoMoves(p, "RiDiRD");
			if (false == IS_CUBE_SOLVED(p))
				p = DoMove(p, MOVE_U);
		}
	}
	
	ret = IsTopLayerSolved(p) || IS_CUBE_SOLVED(p);
	
	if (IS_CUBE_SOLVED(p))
		p = m_pCurrent;
		
	m_pCurrent = p;

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Fully solve 1-st layer (cross, corners, positions, orientations).
 * Input: solved top layer cross.
 */
bool Cube::Solve1stLayerCorners(RCube *p)
{
	bool ret = false;
	int n = 8;
	
	if (m_bShow)
		cout << "Solving L1 corners..." << endl;

	while(false == (ret = Insert1stLayerCorners(p)) && 0 < n--) {}

	n = 4;
	while(false == (ret = Position1stLayerCorners(m_pCurrent)) && 0 < n--) {}

	n = 4;
	while(false == (ret = Orient1stLayerCorners(m_pCurrent)) && 0 < n--) {}
	
	if (m_bShow)
		cout << "L1 corners " << ((ret) ? "solved" : "unsolved") << "." << endl;

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Return true if top and layers are solved.
 */
bool Cube::IsMiddleLayerSolved(RCube *p)
{
	bool ret = false;

	ret = (IsTopLayerSolved(p)
	       &&
	       p->left[1][0] == L3
	       &&
	       p->left[1][2] == L5
	       &&
	       p->front[1][0] == F3
	       &&
	       p->front[1][2] == F5
	       &&
	       p->right[1][0] == R3
	       &&
	       p->right[1][2] == R5
	       &&
	       p->bottom[1][0] == B3
	       &&
	       p->bottom[1][2] == B5
	      );

	return ret;
}

#define MAX_SOLVE_MIDLR_ITER 10

/*
 * Layer-by-layer algorithm.
 * Solve middle layer.
 * Input: *p - solved 1st layer cross with corner pieces positioned and oriented
 * Output: *p - solved 1st layer cross with corner pieces positioned and oriented and middle layer pieces positioned and oriented
 * Return: ret - true:solved/false:unsolved
 */
bool Cube::SolveMiddleLayer(RCube *p)
{
	if (m_bShow)
		cout << "Solving L2..." << endl;
		
	if (IsMiddleLayerSolved(p) || IS_CUBE_SOLVED(p)) return true;

	bool ret = false;
	int k = 4;

	while (0 < k--) {
		int n = MAX_SOLVE_MIDLR_ITER;
		while(false == (ret = (IsMiddleLayerSolved(p) || IS_CUBE_SOLVED(p))) && 0 < n--) {
			// left
			if (p->front[2][1] == F3) 	p = DoMoves(p, "DLDiLiDiFiDF");
			if (false == IS_CUBE_SOLVED(p) && p->right[2][1] == R3) 	p = DoMoves(p, "DFDiFiDiRiDR");
			if (false == IS_CUBE_SOLVED(p) && p->bottom[2][1] == B3)	p = DoMoves(p, "DRDiRiDiBiDB");
			if (false == IS_CUBE_SOLVED(p) && p->left[2][1] == L3)		p = DoMoves(p, "DBDiBiDiLiDL");
			// right
			if (false == IS_CUBE_SOLVED(p) && p->front[2][1] == F5)		p = DoMoves(p, "DiRiDRDFDiFi");
			if (false == IS_CUBE_SOLVED(p) && p->right[2][1] == R5)		p = DoMoves(p, "DiBiDBDRDiRi");
			if (false == IS_CUBE_SOLVED(p) && p->bottom[2][1] == B5)	p = DoMoves(p, "DiLiDLDBDiBi");
			if (false == IS_CUBE_SOLVED(p) && p->left[2][1] == L5)		p = DoMoves(p, "DiFiDFDLDiLi");
			if (false == IS_CUBE_SOLVED(p))
				p = DoMove(p, MOVE_D);
		}

		if (ret)
			break;
		else {
			// find dis-oriented middle layer pieces and move them to bottom layer
			// they will be entered properly into middle layer in the next attempt
			int midedges[] = {p->front[1][0], p->front[1][2],
			                  p->bottom[1][0], p->bottom[1][2]
			                 };
			if (midedges[0] != F3)	p = DoMoves(p, "DLDiLiDiFiDF");
			if (false == IS_CUBE_SOLVED(p) && midedges[1] != F5)	p = DoMoves(p, "DiRiDRDFDiFi");
			if (false == IS_CUBE_SOLVED(p) && midedges[2] != B3)	p = DoMoves(p, "DRDiRiDiBiDB");
			if (false == IS_CUBE_SOLVED(p) && midedges[3] != B5)	p = DoMoves(p, "DiLiDLDBDiBi");
		}

	}
	
	if (IS_CUBE_SOLVED(p))
		p = m_pCurrent;

	m_pCurrent = p;
	
	if (m_bShow)
		cout << "L2 " << ((ret) ? "solved" : "unsolved") << "." << endl;

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Return true if the following stages of the cube are solved:
 * - 1st layer (L1 or U)
 * - 2nd layer (L2 or middle layer)
 * - L3-Edge-Orientation: 3rd layer (L3 or D) cross oriented, not necessarily positioned.
 * - L3-Corner-Permutation: 3rd layer (L3 or D) corners positioned, not necessarily oriented.
 */
bool Cube::BottomLayerCornersPositioned(RCube *p)
{
	bool ret = false;

	ret = (IsBottomCrossSolvedNotPositioned(p)
	       &&
	       (p->down[0][0] == D0 || p->down[0][0] == F6 || p->down[0][0] == L8)
	       &&
	       (p->down[0][2] == D2 || p->down[0][2] == F8 || p->down[0][0] == R6)
	       &&
	       (p->down[2][0] == D6 || p->down[2][0] == L6 || p->down[2][0] == B8)
	       &&
	       (p->down[2][2] == D8 || p->down[2][2] == R8 || p->down[2][2] == B6)
	      );

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * L3-Corner-Permutation
 * Bottom layer - position corners.
 * Input: *p - solved 1st layer cross with corner pieces positioned and oriented and middle layer pieces positioned and oriented
 * Output: *p - solved 1st layer cross with corner pieces positioned and oriented and middle layer pieces positioned and oriented
 *              with bottom layer corners insterted and positioned, but not necessarilly oriented.
 * Return: ret - true:solved/false:unsolved
 */
bool Cube::PositionBottomLayerCorners(RCube *p)
{
	bool ret = false;
	int n=8;
	
	if (m_bShow)
		cout << "Solving L3 corners position..." << endl;

	while (false == (ret = (BottomLayerCornersPositioned(p) || IS_CUBE_SOLVED(p))) && 0 < n--) {
		int m=4;
		while (false == IS_CUBE_SOLVED(p)
		       &&
		       0 < m--
		       &&
				 false == L3DiagonalCornersPositioned(p)
		       &&
		       false == L3AdjacentCornersPositioned(p)
		      ) {
			p = DoMove(p, MOVE_D);
		}
		if (false == IS_CUBE_SOLVED(p) && L3AdjacentCornersPositioned(p)) {
			p = L3PositionRemainingAdjacentCorners(p);
		} else if (false == IS_CUBE_SOLVED(p) && L3DiagonalCornersPositioned(p)) {
			p = DoMoves(p, "RDiLiDRiDiLD2");
		}
	}
	
	if (IS_CUBE_SOLVED(p))
		p = m_pCurrent;

	m_pCurrent = p;
	
	if (m_bShow)
		cout << "L3 corners position " << ((ret) ? "solved" : "unsolved") << "." << endl;

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Return true if any of the diagonal corners of the bottom (L3, D) layer
 * are positioned properly.
 */
bool Cube::L3DiagonalCornersPositioned(RCube *p)
{
	bool ret = false;

	ret =  (
	           (
	               (p->down[0][0] == D0 || p->down[0][0] == F6 || p->down[0][0] == L8)
	               &&
	               (p->down[2][2] == D8 || p->down[2][2] == R8 || p->down[2][2] == B6)
	           )
	           	||
	           (
	               (p->down[0][2] == D2 || p->down[0][2] == F8 || p->down[0][0] == R6)
	               &&
	               (p->down[2][0] == D6 || p->down[2][0] == L6 || p->down[2][0] == B8)
	           )
	       );

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Return true if any of the adjacent corners of the bottom (L3, D) layer
 * are positioned properly.
 */
bool Cube::L3AdjacentCornersPositioned(RCube *p)
{
	bool ret = false;

	ret =  (
	           (
	               (p->down[0][0] == D0 || p->down[0][0] == F6 || p->down[0][0] == L8)
	               &&
	               (p->down[0][2] == D2 || p->down[0][2] == F8 || p->down[0][0] == R6)
	           )
	           	||
	           (
	               (p->down[0][2] == D2 || p->down[0][2] == F8 || p->down[0][0] == R6)
	               &&
	               (p->down[2][2] == D8 || p->down[2][2] == R8 || p->down[2][2] == B6)
	           )
	           	||
	           (
	               (p->down[2][2] == D8 || p->down[2][2] == R8 || p->down[2][2] == B6)
	               &&
	               (p->down[2][0] == D6 || p->down[2][0] == L6 || p->down[2][0] == B8)
	           )
	           	||
	           (
	               (p->down[2][0] == D6 || p->down[2][0] == L6 || p->down[2][0] == B8)
	               &&
	               (p->down[0][0] == D0 || p->down[0][0] == F6 || p->down[0][0] == L8)
	           )
	       );

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Return a case number of the adjacent corners of the bottom (L3, D) layer
 * that are positioned properly.
 */
int Cube::L3AdjacentCornersPositioned2(RCube *p)
{
	int ret = -1;

	if ((p->down[0][0] == D0 || p->down[0][0] == F6 || p->down[0][0] == L8)
	        &&
	        (p->down[0][2] == D2 || p->down[0][2] == F8 || p->down[0][0] == R6)
	   )
		ret = 0;
	else if ((p->down[0][2] == D2 || p->down[0][2] == F8 || p->down[0][0] == R6)
	         &&
	         (p->down[2][2] == D8 || p->down[2][2] == R8 || p->down[2][2] == B6)
	        )
		ret = 1;
	else if ((p->down[2][2] == D8 || p->down[2][2] == R8 || p->down[2][2] == B6)
	         &&
	         (p->down[2][0] == D6 || p->down[2][0] == L6 || p->down[2][0] == B8)
	        )
		ret = 2;
	else if ((p->down[2][0] == D6 || p->down[2][0] == L6 || p->down[2][0] == B8)
	         &&
	         (p->down[0][0] == D0 || p->down[0][0] == F6 || p->down[0][0] == L8)
	        )
		ret = 3;

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Rotate the bottom (L3, D) layer until the not positioned adjacent corners
 * of that layer are located at the L (left) side, then perform permutation
 * to switch the cubies and make these corners positioned properly.
 * Then, rotate the bottom (L3, D) layer again to properly position all the corners.
 */
RCube *Cube::L3PositionRemainingAdjacentCorners(RCube *p)
{
	RCube *ret = p;
	string moves[] = {"D", "", "Di", "D2"};
	string imoves[] = {"Di", "", "D", "D2"};
	int n = L3AdjacentCornersPositioned2(ret);

	if (0 <= n) {
		ret = DoMoves(ret, moves[n]);
		if (IsSolved(ret)) return m_pCurrent;
		ret = DoMoves(ret, "RDiLiDRiDiLD2");
		if (IsSolved(ret)) return m_pCurrent;
		ret = DoMoves(ret, imoves[n]);
		if (IsSolved(ret)) return m_pCurrent;
	}

	return 	ret;
}

/*
 * Layer-by-layer algorithm.
 * Bottom layer - orient corners.
 * Input: *p - solved 1st layer cross with corner pieces positioned and oriented and middle layer pieces positioned and oriented
 *             with bottom layer corners insterted and positioned
 * Output: *p - solved 1st layer cross with corner pieces positioned and oriented and middle layer pieces positioned and oriented
 *              with bottom layer corners insterted and positioned and oriented
 * Return: ret - true:solved/false:unsolved
 */
bool Cube::OrientBottomLayerCorners(RCube *p)
{
   bool ret = false;
   int n = 8;
   string corners_orient;
   string a10 = "LiDiLDiLiD2LD2";
   string a11 = "LDLiDLD2LiD2";
   
	if (m_bShow)
		cout << "Solving L3 corners orientation..." << endl;
   
   while (false == (ret = L3CornersOriented(p)) && 0 < n--)
   {
  		corners_orient = L3CheckCornersOrient(p);
  		// Reorient bottom (L3, D) layer to find one of recognized corners orientation patterns.
  		int m = 4;
  		while ((corners_orient.compare("0111") != 0)
  					&&
  				 (corners_orient.compare("2202") != 0)
  					&&
  				 (corners_orient.compare("2010") != 0)
  					&&
  				 (corners_orient.compare("2100") != 0)
  					&&
  				 (corners_orient.compare("2001") != 0)
  					&&
  				 (corners_orient.compare("1212") != 0)
  					&&
  				 (corners_orient.compare("2112") != 0)
  					&&
  				 0 < m--
				)
		{
			corners_orient = corners_orient.substr(3, 1) + corners_orient.substr(0, 3);
			p = DoMove(p, MOVE_D);
			if (IsSolved(p)) return true;
		}
		// Perform the moves to reorient corners based on found corners orientations.
  		if (corners_orient.compare("0111") == 0)
  		{
  			p = DoMoves(p, a10);
  			if (IsSolved(p)) return true;
		}
  		else if (corners_orient.compare("2202") == 0)
  		{
 				p = DoMoves(p, a11);
 				if (IsSolved(p)) return true;
		}
  		else if (corners_orient.compare("2010") == 0)
  		{
  			p = DoMoves(p, a11);
  			if (IsSolved(p)) return true;
  			p = DoMoves(p, a10);
  			if (IsSolved(p)) return true;
		}
  		else if (corners_orient.compare("2100") == 0)
  		{
				p = DoMoves(p, "D2");
				if (IsSolved(p)) return true;
				p = DoMoves(p, a11);
				if (IsSolved(p)) return true;
				p = DoMoves(p, "D2");
				if (IsSolved(p)) return true;
				p = DoMoves(p, a10);
				if (IsSolved(p)) return true;
		}
  		else if (corners_orient.compare("2001") == 0)
  		{
				p = DoMove(p, MOVE_D);
				if (IsSolved(p)) return true;
				p = DoMoves(p, a11);
				if (IsSolved(p)) return true;
				p = DoMove(p, MOVE_Di);
				if (IsSolved(p)) return true;
				p = DoMoves(p, a10);
				if (IsSolved(p)) return true;
		}
  		else if (corners_orient.compare("1212") == 0)
  		{
				p = DoMove(p, MOVE_D);
				if (IsSolved(p)) return true;
				p = DoMoves(p, a10);
				if (IsSolved(p)) return true;
				p = DoMove(p, MOVE_Di);
				if (IsSolved(p)) return true;
				p = DoMoves(p, a10);
				if (IsSolved(p)) return true;	
		}
  		else if (corners_orient.compare("2112") == 0)
  		{
				p = DoMoves(p, "D2");
				if (IsSolved(p)) return true;
				p = DoMoves(p, a11);
				if (IsSolved(p)) return true;
				p = DoMoves(p, "D2");
				if (IsSolved(p)) return true;
				p = DoMoves(p, a11);  			
				if (IsSolved(p)) return true;	
		}
		// Return bottom (L3, D) layer to proper position.
		while (4 > m++)
		{
			p = DoMove(p, MOVE_Di);
			if (IsSolved(p)) return true;
		}
	 }
	 
	 m_pCurrent = p;
	 
	 if (m_bShow)
	 	cout << "L3 corners orientation " << ((ret) ? "solved" : "unsolved") << "." << endl;
	 	
   return ret;   
}

/*
 * Layer-by-layer algorithm.
 * Return true if corners on the bottom (L3, D) layer are properly oriented.
 */
bool Cube::L3CornersOriented(RCube *p)
{
	bool ret = false;

	ret = (L3CheckCornersOrient(p).compare("0000") == 0);

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Return the status of bottom (L3, D) layer corners orientation in the form of string
 * "NNNN"
 * Where:
 *    N = 0..2
 *    0 - piece properly oriented
 *    1 - piece rotated clockwise
 *    2 - piece rotated counter-clockwise
 */
string Cube::L3CheckCornersOrient(RCube *p)
{
	string ret;
	char ctbl[] = "----\0";

	if (p->down[2][2] == B6 && p->right[2][2] == D8 && p->bottom[2][0] == R8)
		ctbl[0] = '1';
	else if (p->down[2][2] == R8 && p->right[2][2] == B6 && p->bottom[2][0] == D8)
		ctbl[0] = '2';
	else if (p->down[2][2] == D8 && p->right[2][2] == R8 && p->bottom[2][0] == B6)
		ctbl[0] = '0';

	if (p->down[2][0] == L6 && p->left[2][0] == B8 && p->bottom[2][2] == D6)
		ctbl[1] = '1';
	else 	if (p->down[2][0] == B8 && p->left[2][0] == D6 && p->bottom[2][2] == L6)
		ctbl[1] = '2';
	else 	if (p->down[2][0] == D6 && p->left[2][0] == L6 && p->bottom[2][2] == B8)
		ctbl[1] = '0';

	if (p->down[0][2] == R6 && p->front[2][2] == D2 && p->right[2][0] == F8)
		ctbl[2] = '1';
	else if (p->down[0][2] == F8 && p->front[2][2] == R6 && p->right[2][0] == D2)
		ctbl[2] = '2';
	else if (p->down[0][2] == D2 && p->front[2][2] == F8 && p->right[2][0] == R6)
		ctbl[2] = '0';

	if (p->down[0][0] == F6 && p->left[2][2] == D0 && p->front[2][0] == L8)
		ctbl[3] = '1';
	else if (p->down[0][0] == L8 && p->left[2][2] == F6 && p->front[2][0] == D0)
		ctbl[3] = '2';
	else if (p->down[0][0] == D0 && p->left[2][2] == L8 && p->front[2][0] == F6)
		ctbl[3] = '0';

	ret = string(ctbl);

	return ret;
}

// Layer-by-layer algorithm - helper macro.
#define IS_PIECE_DOWNLAYER_COLOR(p) (p == D1 || p == D3 || p == D5 || p == D7)

/*
 * Layer-by-layer algorithm.
 *	Return true if the top and middle layers are solved
 *  together with bottom cross cubies oriented, but the cross pieces may not be
 *  yet positioned properly on the bottom (L3 or D) layer.
 */
bool Cube::IsBottomCrossSolvedNotPositioned(RCube *p)
{
	bool ret = false;

	ret = (IsMiddleLayerSolved(p)
	       &&
	       IS_PIECE_DOWNLAYER_COLOR(p->down[0][1])
	       &&
	       IS_PIECE_DOWNLAYER_COLOR(p->down[1][0])
	       &&
	       IS_PIECE_DOWNLAYER_COLOR(p->down[1][2])
	       &&
	       IS_PIECE_DOWNLAYER_COLOR(p->down[2][1])
	      );

	return ret;
}

// Layer-by-layer algorithm - helper macros.
#define IS_DOWN_EDGE_PIECE(p)	(p == D1 || p == D3 || p == D5 || p == D7)

#define DETECT_BLSC_L(p)		\
	(		\
	        (IS_DOWN_EDGE_PIECE(p[0][1]) && IS_DOWN_EDGE_PIECE(p[1][0]))		\
	        ||	\
	        (IS_DOWN_EDGE_PIECE(p[0][1]) && IS_DOWN_EDGE_PIECE(p[1][2]))		\
	        ||	\
	        (IS_DOWN_EDGE_PIECE(p[2][1]) && IS_DOWN_EDGE_PIECE(p[1][2]))		\
	        ||	\
	        (IS_DOWN_EDGE_PIECE(p[1][0]) && IS_DOWN_EDGE_PIECE(p[2][1]))		\
	)

#define DETECT_BLSC_BAR(p)	\
	(		\
	        (IS_DOWN_EDGE_PIECE(p[0][1]) && IS_DOWN_EDGE_PIECE(p[2][1]))		\
	        ||	\
	        (IS_DOWN_EDGE_PIECE(p[1][0]) && IS_DOWN_EDGE_PIECE(p[1][2]))		\
	)

/*
 * Layer-by-layer algorithm.
 * L3-Edge-Orientation
 * Solve edges of the bottom (D or L3) layer so they are all aligned
 * to bottom layer color (properly oriented), but not necessarily positioned
 * properly/aligned to the adjacent side colors.
 */
bool Cube::L3EdgeOrientation(RCube *p)
{
	if (m_bShow)
		cout << "Solving L3 edges orientation..." << endl;
		
	if (IsBottomCrossSolvedNotPositioned(p) || IS_CUBE_SOLVED(p)) return true;

	bool ret = false;
	enum BottomLayerStartingCases {BLSC_DOT, BLSC_BAR, BLSC_L};
	BottomLayerStartingCases start_scenario = BLSC_DOT;

	int n = 8;
	while(false == ret && 0 < n--) {
		if (DETECT_BLSC_L(p->down))	start_scenario = BLSC_L;
		else if (DETECT_BLSC_BAR(p->down)) start_scenario = BLSC_BAR;
		else start_scenario = BLSC_DOT;

		int m = 2;
		switch(start_scenario) {
			case BLSC_DOT:
				p = DoMoves(p, "FLDLiDiFiD2FLDLiDiFi");
				break;

			case BLSC_BAR:
				while((false == IS_DOWN_EDGE_PIECE(p->down[1][0])
				        ||
				        false == IS_DOWN_EDGE_PIECE(p->down[1][2])
				      )
				        &&
				        false == IsSolved(p)
				        &&
				        0 < m--
				     ) {
					p = DoMove(p, MOVE_D);
				}
				if (false == IS_CUBE_SOLVED(p))
					p = DoMoves(p, "FLDLiDiFi");
				break;

			case BLSC_L:
				m = 4;
				while((false == IS_DOWN_EDGE_PIECE(p->down[1][2])
				        ||
				        false == IS_DOWN_EDGE_PIECE(p->down[2][1])
				      )
				        &&
				        false == IsSolved(p)
				        &&
				        0 < m--
				     ) {
					p = DoMove(p, MOVE_D);
				}
				if (false == IS_CUBE_SOLVED(p))
					p = DoMoves(p, "FDLDiLiFi");
				break;

			default:
				break;
		}
		ret = IsBottomCrossSolvedNotPositioned(p) || IS_CUBE_SOLVED(p);
	}
	
	if (m_bSolved)
		p = m_pCurrent;

	m_pCurrent = p;
	
	if (m_bShow)
		cout << "L3 edges orientation " << ((ret) ? "solved" : "unsolved") << "." << endl;

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Return true if bottom (L3, D) layer is solved (positioned and oriented).
 */
bool Cube::L3Solved(RCube *p)
{
	bool ret = false;

	ret = (L3CheckCornersOrient(p).compare("0000") == 0)
	      &&
	      p->down[0][1] == D1
	      &&
	      p->down[1][0] == D3
	      &&
	      p->down[1][2] == D5
	      &&
	      p->down[2][1] == D7;

	return ret;
}

/*
 * Layer-by-layer algorithm.
 * Bottom (L3, D) layer - position edges (L3-Edge-Permutation).
 * Input: *p - solved 1st layer cross with corner pieces positioned and oriented and middle layer pieces positioned and oriented
 *             with bottom layer corners insterted and positioned and oriented
 * Output: *p - solved 1st layer cross with corner pieces positioned and oriented and middle layer pieces positioned and oriented
 *              with bottom layer corners insterted and positioned and oriented and edges positioned
 * Return: ret - true:solved/false:unsolved
 */
bool Cube::L3EdgePermutation(RCube *p)
{
	bool ret = false;
	string a12 = "L2DFBiL2FiBDL2";
	string a13 = "L2DiFBiL2FiBDiL2";
	string a13_z90 = "B2DiLRiB2LiRDiB2";
	string a12_zn90 = "F2DRLiF2RiLDF2";
	bool edges_pos[] = {false, false, false, false};
	int  edges_cub[4];
	int n = 8;
	
	if (m_bShow)
		cout << "Solving L3 edges permutation..." << endl;

	while (false == (ret = (IS_CUBE_SOLVED(p) || L3Solved(p))) && 0 < n--) {
		edges_pos[0] = (p->down[0][1] == D1) ? true : false;
		edges_pos[1] = (p->down[1][2] == D5) ? true : false;
		edges_pos[2] = (p->down[2][1] == D7) ? true : false;
		edges_pos[3] = (p->down[1][0] == D3) ? true : false;
		//------------------------------------
		edges_cub[0] = p->down[0][1];
		edges_cub[1] = p->down[1][2];
		edges_cub[2] = p->down[2][1];
		edges_cub[3] = p->down[1][0];
		//------------------------------------
		int m = 0;
		if (edges_pos[0]) {
			p = DoMove(p, MOVE_D);
			m = 1;
		} else if (edges_pos[2]) {
			p = DoMove(p, MOVE_Di);
			m = -1;
		} else if (edges_pos[3]) {
			p = DoMoves(p, "D2");
			m = 2;
		} else if (edges_pos[1]) {
			m = 3;
		}
		if (false == IS_CUBE_SOLVED(p)) {
			switch (m) {
				case 0:
					p = DoMoves(p, a12);					
					break;
				case 1:
					if (edges_cub[3] == D5)
						p = DoMoves(p, a12);
					else if (edges_cub[3] == D7)
						p = DoMoves(p, a13);
					if (false == IS_CUBE_SOLVED(p))
						p = DoMove(p, MOVE_Di);
					break;

				case -1:
					if (edges_cub[0] == D5)
						p = DoMoves(p, a12);
					else if (edges_cub[0] == D3)
						p = DoMoves(p, a13);
					if (false == IS_CUBE_SOLVED(p))
						p = DoMove(p, MOVE_D);
					break;

				case 2:
					if (edges_cub[0] == D7 || edges_cub[0] == D5)
						p = DoMoves(p, a13);
					else if (edges_cub[0] == D3 || edges_cub[0] == D1)
						p = DoMoves(p, a12);
					if (false == IS_CUBE_SOLVED(p))
						p = DoMoves(p, "D2");
					break;

				case 3:
					if (edges_cub[0] == D7)
						p = DoMoves(p, a12);
					else if (edges_cub[0] == D3)
						p = DoMoves(p, a13);
					break;

				default:
					break;
			}
		} 
	}
	
	if (IS_CUBE_SOLVED(p))
		p = m_pCurrent;

	m_pCurrent = p;
	
	if (m_bShow)
		cout << "L3 edges permutation " << ((ret) ? "solved" : "unsolved") << "." << endl;

	return ret;
}

/*
 * Cube engine.
 * Show the history of moves.
 */
void Cube::PrintMoves(RCube *p)
{
	int nMoves = 0;
	string moves = string();

	while (p != NULL) {
		//nMoves++;
		Print(*p);
		if (NULL != p->parent) {
			nMoves++;
			if (m_nMethod != TreeSearchBackTrace) {
				cout << "^" << endl;
				cout << "| " << move_names_N[p->parent_move] << endl;
				moves = move_names_N[p->parent_move] + moves;
			} else {
				cout << "| " << o_move_names_N[p->parent_move] << endl;
				cout << "v" << endl;
				moves = moves + o_move_names_N[p->parent_move];
			}
		}
		p = p->parent;
		cout << "-------------------------------------------------------------------------------" << endl;
	}
	if (1 < nMoves) {
		cout << "Solution has " << nMoves << " moves: " << moves << endl << endl;
		PrintSolPathEyeFriendly(moves);
	}
}

/*
 * Cube engine.
 * Show all the moves to solve the cube.
 */
void Cube::PrintSolution()
{
	PrintMoves(m_pSolved);
}

/*
 * Cube engine.
 * Show all the moves so far.
 */
void Cube::PrintHistory()
{
	PrintMoves(m_pCurrent);
}

/*
 * Cube engine.
 * Print path to the cube.
 */
void Cube::PrintPath(RCube *p)
{
	if (NULL == p)
		throw Exception("Cube::PrintPath - null node!");

	string sol_path = GetSolutionPath(p);
	int num_moves = 0;
	RCube *ptr = p;

	while (NULL != ptr->parent) {
		num_moves++;
		ptr = ptr->parent;
	}

	if (sol_path.length()) {
		cout << "Solution has " << num_moves << " moves: " << sol_path << endl << endl;
		PrintSolPathEyeFriendly(sol_path);
	}
	
	m_nSolLen = num_moves;
}

/*
 * Cube engine - presentation (console).
 * Output to the console the solution path in human-eye friendly format.
 */
void Cube::PrintSolPathEyeFriendly(string sol_path)
{
	int n=0, m=1;
	string move;
	string spc1 = " ";
	string spc2	= "  ";
	
	cout << m << ")\t";
	for (unsigned int i=0; i < sol_path.length(); i++) {
		char c = sol_path[i];
		move.push_back(c);
		if ('i' == c || '2' == c) {
			cout << move << spc1;
			move.clear();
			n++;
		} else {
			if (1 < move.length()) {
				move.erase(move.end()-1);
				cout << move << spc2;
				move.clear();
				move.push_back(c);
				n++;
			}
		}
		if (20 == n) {
			n = 0; m++;
			cout << endl << m << ")\t";			
		}
	}
	if (0 < move.length())
		cout << move << endl;
	cout << endl;
}

/*
 * Cube engine.
 * Show the path moves so far.
 */
void Cube::PrintHistoryPath()
{
	PrintPath(m_pCurrent);
}

/*
 * Cube engine.
 * Show the path only to solve the cube.
 */
void Cube::PrintSolutionPath()
{
	PrintPath(m_pSolved);
}

/*
 * Cube engine.
 * Random cube scramble, use provided # of moves.
 */
RCube *Cube::Scramble(int moves, bool show)
{
	ClearHistory();
	if (NULL != m_pCurrent) {
		m_pMemMgm->DeleteRCube(m_pCurrent);
		m_pCurrent = NULL;
	}
	RCube *start = m_pMemMgm->GetNewRCube(CreateSolvedCube());
	RCube *p = NULL, *ret = start;

	if (show) {
		cout << endl << "Scrambling the cube, depth=" << moves << "." << endl;
		cout << endl << "Unscrambled (solved) cube:" << endl;
		Print(*start);
		cout << "-----------------------------------------------------------------" << endl;
	}
	map<string,string> states_map;
	string cube_str = ConvertCube2String(start);
	states_map[cube_str] = "solved";
	for (int i=0; i<moves; i++) {
		int move = RandomInt(0, MAX_MOVES-1);

		if (show) cout << "Move #" << (i+1) << ") " << move_names_N[move] << ":" << endl;
		p = ret;
		int eotmp[NUM_OF_EDGES], cotmp[NUM_OF_CORNERS], eptmp[NUM_OF_EDGES], cptmp[NUM_OF_CORNERS];
		SAVE_CUBIE_STATUS(eptmp,cptmp,eotmp,cotmp);		
		ret = DoMove(p, move);
		cube_str = ConvertCube2String(ret);
		while (0 < states_map.count(cube_str)) {
			if (show) cout << "Move '" << move_names_N[move] << "' rejected (leading to duplicate state)." << endl;
			DeleteCube(ret);
			RESTORE_CUBIE_STATUS(eptmp,cptmp,eotmp,cotmp);
			move = RandomInt(0, MAX_MOVES-1);
			if (show) cout << "Repeat move #" << (i+1) << ") " << move_names_N[move] << ":" << endl;
			ret = DoMove(p, move);
			cube_str = ConvertCube2String(ret);
		}
		states_map[cube_str] = move_names_N[move];
		if (show) {
			Print(*ret);
			cout << "-----------------------------------------------------------------" << endl;
		}
		DeleteCube(p);
	}
	if (show) cout << "Finished scrambling the cube." << endl;
	ret->NullParent();
	m_pSolved = NULL;
	m_bSolved = false;
	m_pRoot = m_pCurrent = ret;

	return ret;
}

/*
 * Cube engine.
 * Random cube scramble, use provided # of moves.
 * Do not reallocate new RCube object, operate on the existing object.
 */
RCube *Cube::ScrambleStatic(int moves, bool show)
{
	if (NULL == m_pRoot)
		m_pRoot = new RCube();
	if (NULL == m_pRoot)
		throw Exception("Cube::ScrambleStatic() - memory allocation failed!");
	RCube *ret = SetSolvedCube(m_pRoot);

	if (show) {
		cout << endl << "Scrambling the cube, depth=" << moves << "." << endl;
		cout << endl << "Unscrambled (solved) cube:" << endl;
		Print(*ret);
		cout << "-----------------------------------------------------------------" << endl;
	}
	map<string,string> states_map;
	string cube_str = ConvertCube2String(ret);
	states_map[cube_str] = "solved";
	for (int i=0; i<moves; i++) {
		int move = RandomInt(0, MAX_MOVES-1);

		if (show) cout << "Move #" << (i+1) << ") " << move_names_N[move] << ":" << endl;
		ret = DoMoveStatic(ret, move);
		cube_str = ConvertCube2String(ret);
		while (0 < states_map.count(cube_str)) {
			for (int n=0; n<3; n++)
				ret = DoMoveStatic(ret, move);
			if (show) cout << "Move '" << move_names_N[move] << "' rejected (leading to duplicate state)." << endl;
			move = RandomInt(0, MAX_MOVES-1);
			if (show) cout << "Repeat move #" << (i+1) << ") " << move_names_N[move] << ":" << endl;
			ret = DoMoveStatic(ret, move);
			cube_str = ConvertCube2String(ret);
		}
		states_map[cube_str] = move_names_N[move];
		if (show) {
			Print(*ret);
			cout << "-----------------------------------------------------------------" << endl;
		}
	}
	if (show) cout << "Finished scrambling the cube." << endl;
	ret->NullParent();
	m_pSolved = NULL;
	m_bSolved = false;
	m_pRoot = m_pCurrent = ret;

	return ret;
}

/*
 * Generate random integer between start and end (inclusive).
 */
int Cube::RandomInt(int start, int end)
{
	int ret = 0;

	if (end < start)
		return 0;

	ret = rand()%(end-start+1) + start;

	return ret;
}

/*
 * Cube engine.
 * Create cube object from int array template.
 */
RCube *Cube::CreateCubeFromTmpl(int *ptmpl)
{
	RCube *ret = new RCube();

	int i=0;

	while (i<6*9) {
		switch (solved_cube_tmpl[i]) {
			case U0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						ret->up[j][k] = ptmpl[i++];
				break;

			case L0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						ret->left[j][k] = ptmpl[i++];
				break;

			case D0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						ret->down[j][k] = ptmpl[i++];
				break;

			case F0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						ret->front[j][k] = ptmpl[i++];
				break;

			case R0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						ret->right[j][k] = ptmpl[i++];
				break;

			case B0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						ret->bottom[j][k] = ptmpl[i++];
				break;

			default:
				throw Exception("Cube::CreateCubeFromTmpl - unknown face code");
				break;
		}
	}

	return ret;
}

/*
 * Cube engine.
 * Create cube from string sequence definition.
 * Input: a vector of scrambled cube faces codes
 *        in the following order: U, L, F, R, B, D
 *        Where:
 *        U,L,F,etc. - indicate the faces of the cube as visible
 *                     from the front (F) side of the cube.
 *        U-upper, L-left, F-front, R-right, B-bottom (opposite to F),
 *        D-down (or 'lower', opposite to U).
 */
RCube *Cube::CreateCubeFromStrVectDef(vector<string> cube_def, bool clrmem)
{
	if (clrmem)
		m_pMemMgm->DeleteAll();
	RCube *ret = m_pMemMgm->GetNewRCube(); //new RCube();
	if (NULL == ret)
		throw Exception("Cube::CreateCubeFromStrVectDef() - memory depleted!");
		
	int x=0, y=0;

	for (int i=0; i<6*9; i++) {
		for (int j=0; j<6*9; j++) {
			string face = cube_def[i];
			if (0 == face.compare(faces_sym_tbl[j])) {
				if (i<9)
					ret->up[x][y] = j;
				else if (i>=9 && i<2*9)
					ret->left[x][y] = j;
				else if (i>=2*9 && i<3*9)
					ret->front[x][y] = j;
				else if (i>=3*9 && i<4*9)
					ret->right[x][y] = j;
				else if (i>=4*9 && i<5*9)
					ret->bottom[x][y] = j;
				else if (i>=5*9 && i<6*9)
					ret->down[x][y] = j;
				break;
			}
		}
		y++;
		if (y>2) {
			y=0;
			x++;
			if (x>2) x=0;
		}
	}
	m_pRoot = ret;
	m_pCurrent = m_pRoot;
	m_pRoot->NullParent();

	return ret;
}

/*
 * Cube engine.
 * Create cube from the color codes sequence stored in a string vector.
 * This method is similar to CreateCubeFromStrVectDef, however instead of unique face codes
 * it takes color codes instead and build the definition consisting of unique face codes from that.
 * Please note that it is possible to supply impossible combination of colors and in this case solution
 * wil lnot be found. The color representation of the scrambled cube must be valid (one that can be
 * generated by performing valid cube moves starting from solved cube).
 * Input: a vector of scrambled cube color codes
 *        in the following order of faces: U, L, F, R, B, D
 *        Where:
 *        U,L,F,etc. - indicate the faces of the cube as visible
 *                     from the front (F) side of the cube.
 *        U-upper, L-left, F-front, R-right, B-bottom (opposite to F),
 *        D-down (or 'lower', opposite to U). 
 * The color codes and their face designations in a solved cube:
 *    U - upper face  -  (W) white
 *    F - front face  -  (R) red
 *    D - lower face  -  (Y) yellow
 *    L - left face   -  (G) green
 *    R - right face  -  (B) blue
 *    B - back face   -  (P) purple
 */
RCube *Cube::CreateCubeFromColorStrVectDef(vector<string> cube_def)
{
	RCube *ret = NULL;
	string colors[6*9];
	int conv_ord[][3] = {
		{0,		9,		20},
		{1,		19,		-1},
		{2,		18,		17},
		{3,		10,		-1},
		{4,		-1,		-1},
		{5,		16,		-1},
		{6,		12,		11},
		{7,		13,		-1},
		{8,		15,		14},
		{9,		20,		0},
		{10,	3,		-1},
		{11,	6,		12},		
		{21,	32,		-1},
		{22,	-1,		-1},
		{23,	24,		-1},		
		{33,	51,		44},
		{34,	48,		-1},
		{35,	36,		45},		
		{12,	11,		6},
		{13,	7,		-1},
		{14,	8,		15},		
		{24,	23,		-1},
		{25,	-1,		-1},
		{26,	27,		-1},				
		{36,	45,		35},
		{37,	46,		-1},
		{38,	39,		47},		
		{15,	14,		8},
		{16,	5,		-1},
		{17,	2, 		18},		
		{27,	26,		-1},
		{28,	-1,		-1},
		{29,	30,		-1},				
		{39,	47,		38},
		{40,	50,		-1},
		{41,	42,		53},						
		{18,	17,		2},
		{19,	1,		-1},
		{20,	0, 		9},		
		{30,	29,		-1},
		{31,	-1,		-1},
		{32,	21,		-1},		
		{42,	53,		41},
		{43,	52,		-1},
		{44,	33,		51},
		{45,	35,		36},
		{46,	37,		-1},
		{47,	38,		39},
		{48,	34,		-1},
		{49,	-1,		-1},
		{50,	40,		-1},
		{51,	44,		33},
		{52,	43,		-1},
		{53,	41,		42}
	};
	vector<string> conv_cube_def;
	int i=0;
	for (vector<string>::iterator it = cube_def.begin(); it < cube_def.end(); ++it, i++) {
		colors[conv_ord[i][0]] = string(*it);
	}
	for (i=0; i < 6*9; i++) {
		string cubie = string();
		for (int j=0; j < 3 && conv_ord[i][j] != -1; j++) {
			cubie += colors[conv_ord[i][j]];
		}
		for (int j=0; j < 6*9; j++) {
			if (0 == cubie.compare(color_sym_tbl[j])) {
				conv_cube_def.push_back(FaceSymbol(j));
				break;
			}
		}
	}
	
	if (conv_cube_def.size() != 6*9)
		throw Exception("Cube::CreateCubeFromColorStrVectDef - invalid cube colors definition");
		
	ret = CreateCubeFromStrVectDef(conv_cube_def);

	return ret;
}

/*
 * Cube engine.
 * Create brand new solved cube.
 */
RCube *Cube::CreateSolvedCube()
{
	MemMgm memMgr;
	RCube *ret = memMgr.GetNewRCube();
	
	ret = m_pMemMgm->GetNewRCube(SetSolvedCube(ret));
	if (NULL == ret)
		throw Exception("Cube::CreateSolvedCube() - memory depleted!");
		
	// Initialize cubie model (solved state).
	for(int i=0; i<NUM_OF_CORNERS; i++) {
		m_anCornPerm[i] = i+1;
		m_anCornOrient[i] = 0;
	}
	for(int i=0; i<NUM_OF_EDGES; i++) {
		m_anEdgePerm[i] = i+1;
		m_anEdgeOrient[i] = 1;
	}	

	return ret;
}

/*
 * Cube engine.
 * Set cube in solved state.
 * Create new one if m_pRoot is NULL;
 */
RCube *Cube::SetSolvedCube(RCube *p)
{
	RCube *ret = p;
	
	if (NULL == ret)
		throw Exception("Cube::SetSolvedCube - NULL pointer!");

	int i=0;

	while (i<6*9) {
		switch (solved_cube_tmpl[i]) {
			case U0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						ret->up[j][k] = solved_cube_tmpl[i++];
				break;

			case L0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						ret->left[j][k] = solved_cube_tmpl[i++];
				break;

			case D0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						ret->down[j][k] = solved_cube_tmpl[i++];
				break;

			case F0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						ret->front[j][k] = solved_cube_tmpl[i++];
				break;

			case R0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						ret->right[j][k] = solved_cube_tmpl[i++];
				break;

			case B0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						ret->bottom[j][k] = solved_cube_tmpl[i++];
				break;

			default:
				throw Exception("Cube::SetSolvedCube - unknown face code");
				break;
		}
	}
	
	// Initialize cubie model (solved state).
	for(int i=0; i<NUM_OF_CORNERS; i++) {
		m_anCornPerm[i] = i+1;
		m_anCornOrient[i] = 0;
	}
	for(int i=0; i<NUM_OF_EDGES; i++) {
		m_anEdgePerm[i] = i+1;
		m_anEdgeOrient[i] = 1;
	}	

	return ret;
}

/*
 * Cube engine.
 * Return true if the two nodes have equal states.
 */
bool Cube::NodesEqual(RCube *pn1, RCube *pn2)
{
	bool ret = true;

	for (int i=0; i<3 && ret; i++)
		for (int j=0; j<3; j++) {
			if (pn1->up[i][j] != pn2->up[i][j]) {
				ret = false;
				break;
			}
		}

	for (int i=0; i<3 && ret; i++)
		for (int j=0; j<3; j++) {
			if (pn1->down[i][j] != pn2->down[i][j]) {
				ret = false;
				break;
			}
		}

	for (int i=0; i<3 && ret; i++)
		for (int j=0; j<3; j++) {
			if (pn1->front[i][j] != pn2->front[i][j]) {
				ret = false;
				break;
			}
		}

	for (int i=0; i<3 && ret; i++)
		for (int j=0; j<3; j++) {
			if (pn1->bottom[i][j] != pn2->bottom[i][j]) {
				ret = false;
				break;
			}
		}

	for (int i=0; i<3 && ret; i++)
		for (int j=0; j<3; j++) {
			if (pn1->left[i][j] != pn2->left[i][j]) {
				ret = false;
				break;
			}
		}

	for (int i=0; i<3 && ret; i++)
		for (int j=0; j<3; j++) {
			if (pn1->right[i][j] != pn2->right[i][j]) {
				ret = false;
				break;
			}
		}

	return ret;
}

/*
 * Print memory use to the console.
 */
void Cube::ShowMemUse()
{
	cout << PrivateMemoryUsed() << " / " << MemoryUsed() << " / " << TotalSystemMemory() << endl;
}

/*
 * Simple tree search.
 * Return true if node was already visited in the current branch.
 */
bool Cube::NodeVisited(RCube *p, int depth)
{
	bool ret = false;
	RCube *pn = (NULL != p) ? p->parent : NULL;

	while (NULL != pn && false == ret) {
		ret = NodesEqual(p, pn);
		pn = pn->parent;
	}

#if !defined(NOVISNODESMAP)
	if (false == ret && depth > 0) {
		string cube_str = this->ConvertCube2String(p);
		MapVisitedNodesIter itVisNode = m_MapVisitedNodes.find(cube_str);
		int visnode_depth = (m_MapVisitedNodes.end() != itVisNode) ? itVisNode->second : -1;
		ret = (depth <= visnode_depth);
		if (0 < visnode_depth)
			m_MapVisitedNodes.erase(itVisNode);
		m_MapVisitedNodes[cube_str] = depth;
	}
#endif

	if (ret) m_lVisitedNodesSkip++;

	return ret;
}

/*
 * Simple tree search.
 * Generate tree of moves from provided root (start)
 * until the cube is solved.
 */
void Cube::GenTree(RCube *start, int depth, bool show)
{
	usleep(1); // let the OS/kernel "breathe"
	if (NULL == start)
		throw Exception("Cube::GenTree - null start node!");

	m_lNodesChecked++;

	if (0 == depth || IsSolved(start) || NodeVisited(start, depth)) return;

	for (int i=0; i<MAX_MOVES; i++) {
		RCube *p = NULL;

		if (depth == m_nDepth)
			ShowRootInfo(i);

		if (show) {
			cout << "\r" << "Depth: " << (m_nDepth-depth) << "    Node: " << m_lNodesChecked << "    Move: ";
			cout << move_names_N[i] << "     ";
		}

		p = DoMove(start, i);
		GenTree(p, depth-1, show);

		if (m_bSolved)
			break;
		else {
			if (NULL != p) {
				m_pCurrent = p->parent;
				DeleteCube(p);
			}
		}
	}

	return;
}

/*
 * Simple tree search.
 * Generate tree of moves from provided root (start)
 * until the cube is solved.
 * Use heuristic evaluation to determine the next search node.
 */
void Cube::GenTree2(RCube *start, int depth, bool show)
{
	usleep(1); // let the OS/kernel "breathe"
	if (NULL == start)
		throw Exception("Cube::GenTree2 - null start node!");

	m_lNodesChecked++;

	if (0 == depth || IsSolved(start) || NodeVisited(start, depth)) return;

	RCube *p[BOTTOM_TOP_LEFT+1];
	int eval_nodes_array[BOTTOM_TOP_LEFT+1], *peval = eval_nodes_array;
	int srch_order[BOTTOM_TOP_LEFT+1], *porder = srch_order;

	for (int i=0; i<MAX_MOVES; i++) {
		srch_order[i] = i;
		p[i]	 = DoMove(start, i);
		eval_nodes_array[i] = EvalNode(p[i]);
	}

	SortSrchOrder(peval, porder);

	for (int i=0; i<MAX_MOVES; i++) {
		int dir = srch_order[i];

		if (depth == m_nDepth)
			ShowRootInfo(dir);

		if (show) {
			cout << "\r" << "Depth: " << (m_nDepth-depth) << "    Node: " << m_lNodesChecked << "    Move: ";
			cout << move_names_N[dir] << "     ";
		}

		GenTree2(p[dir], depth-1, show);

		if (m_bSolved)
			break;
		else {
			if (NULL != p[dir]) {
				if (m_pCurrent == p[dir])
					m_pCurrent = m_pCurrent->parent;
				DeleteCube(p[dir]);
			}
		}
	}

	return;
}

/*
 * Simple tree search.
 * Generate tree of moves starting from the solved cube as root
 * until the state of cube reaches the start state.
 */
void Cube::GenTree3(RCube *start, int depth, bool show)
{
	usleep(1); // let the OS/kernel "breathe"
	if (NULL == start)
		throw Exception("Cube::GenTree3 - null start node!");

	m_lNodesChecked++;

	if (NodesEqual(start, m_pGoal)) {
		m_bSolved = true;
		m_pSolved = start;
		return;
	}

	if (0 == depth || NodeVisited(start, depth)) return;

	if (1 == depth && m_bUpdateCache)
		Add2KnownPaths(start);

	for (int i=0; i<MAX_MOVES; i++) {
		RCube *p;

		if (depth == m_nDepth)
			ShowRootInfo(i);

		if (show) {
			cout << "\r" << "Depth: " << (m_nDepth-depth) << "    Node: " << m_lNodesChecked << "    Move: ";
			cout << move_names_N[i] << "     ";
		}

		p = DoMove(start, i);

		GenTree3(p, depth-1, show);

		if (m_bSolved)
			break;
		else {
			if (NULL != p) {
				m_pCurrent = p->parent;
				DeleteCube(p);
			}
		}
	}

	return;
}

/*
 * Determine the order of search based on heuristic evaluation.
 */
void Cube::SortSrchOrder(int *peval, int *porder)
{
	for (int i=0; i<MAX_MOVES-1; i++) {
		for (int j=i+1; j<MAX_MOVES; j++) {
			if (peval[i] > peval[j]) {
				int tmp;
				tmp = porder[i];
				porder[i] = porder[j];
				porder[j] = tmp;
				/////////////////////////
				tmp = peval[i];
				peval[i] = peval[j];
				peval[j] = tmp;
			}
		}
	}
}

/*
 * Get a distance of cube's face f1 to the cube's face f2.
 * The possible distances returned are:
 * 0 - both faces are equal (this is the same face)
 * 1 - both cubes faces are on the same face
 * 2 - cubes are on adjacent faces
 * 3 - cubes are on the opposite faces
 */
int Cube::GetDistance(int f1, int f2)
{
	int ret = 0;

	if (f1 == f2) return 0;

	int na_f_faces[] = {F0, F1, F2, F3, F4, F5, F6, F7, F8};
	vector<int> f_faces (na_f_faces, na_f_faces+sizeof(na_f_faces)/sizeof(int));
	int na_u_faces[] = {U0, U1, U2, U3, U4, U5, U6, U7, U8};
	vector<int> u_faces (na_u_faces, na_u_faces+sizeof(na_u_faces)/sizeof(int));
	int na_l_faces[] = {L0, L1, L2, L3, L4, L5, L6, L7, L8};
	vector<int> l_faces (na_l_faces, na_l_faces+sizeof(na_l_faces)/sizeof(int));
	int na_r_faces[] = {R0, R1, R2, R3, R4, R5, R6, R7, R8};
	vector<int> r_faces (na_r_faces, na_r_faces+sizeof(na_r_faces)/sizeof(int));
	int na_b_faces[] = {B0, B1, B2, B3, B4, B5, B6, B7, B8};
	vector<int> b_faces (na_b_faces, na_b_faces+sizeof(na_b_faces)/sizeof(int));
	int na_d_faces[] = {D0, D1, D2, D3, D4, D5, D6, D7, D8};
	vector<int> d_faces (na_d_faces, na_d_faces+sizeof(na_d_faces)/sizeof(int));

	vector<int>::iterator it_f1, it_f2;

	if ((it_f1 = find(f_faces.begin(), f_faces.end(), f1)) != f_faces.end()) {
		it_f2 = find(b_faces.begin(), b_faces.end(), f2);
		if (it_f2 != b_faces.end()) ret = 2;
		if (0 == ret) {
			it_f2 = find(f_faces.begin(), f_faces.end(), f2);
			if (it_f2 == f_faces.end()) ret = 1;
		}
	} else if ((it_f1 = find(u_faces.begin(), u_faces.end(), f1)) != u_faces.end()) {
		it_f2 = find(d_faces.begin(), d_faces.end(), f2);
		if (it_f2 != d_faces.end()) ret = 2;
		if (0 == ret) {
			it_f2 = find(u_faces.begin(), u_faces.end(), f2);
			if (it_f2 == u_faces.end()) ret = 1;
		}
	} else if ((it_f1 = find(l_faces.begin(), l_faces.end(), f1)) != l_faces.end()) {
		it_f2 = find(r_faces.begin(), r_faces.end(), f2);
		if (it_f2 != r_faces.end()) ret = 2;
		if (0 == ret) {
			it_f2 = find(l_faces.begin(), l_faces.end(), f2);
			if (it_f2 == l_faces.end()) ret = 1;
		}
	} else if ((it_f1 = find(r_faces.begin(), r_faces.end(), f1)) != r_faces.end()) {
		it_f2 = find(l_faces.begin(), l_faces.end(), f2);
		if (it_f2 != l_faces.end()) ret = 2;
		if (0 == ret) {
			it_f2 = find(r_faces.begin(), r_faces.end(), f2);
			if (it_f2 == r_faces.end()) ret = 1;
		}
	} else if ((it_f1 = find(b_faces.begin(), b_faces.end(), f1)) != b_faces.end()) {
		it_f2 = find(f_faces.begin(), f_faces.end(), f2);
		if (it_f2 != f_faces.end()) ret = 2;
		if (0 == ret) {
			it_f2 = find(b_faces.begin(), b_faces.end(), f2);
			if (it_f2 == b_faces.end()) ret = 1;
		}
	} else if ((it_f1 = find(d_faces.begin(), d_faces.end(), f1)) != d_faces.end()) {
		it_f2 = find(u_faces.begin(), u_faces.end(), f2);
		if (it_f2 != u_faces.end()) ret = 2;
		if (0 == ret) {
			it_f2 = find(d_faces.begin(), d_faces.end(), f2);
			if (it_f2 == d_faces.end()) ret = 1;
		}
	}
	ret++;

	return ret;
}

/*
 * Evaluate how far the node is from solution.
 * The more scrambled cube returns greater value.
 * The solved cube returns 0.
 */
int Cube::EvalNode(RCube *p)
{
	int i=0;
	int ret=0;

	if (NULL == p)
		throw Exception("Cube::EvalNode - null pointer!");

	while (i<6*9) {
		switch (solved_cube_tmpl[i]) {
			case U0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						if (p->up[j][k] != solved_cube_tmpl[i++])
							ret = ret + 1 + GetDistance(p->up[j][k], solved_cube_tmpl[i-1]);
				break;

			case L0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						if(p->left[j][k] != solved_cube_tmpl[i++])
							ret = ret + 1 + GetDistance(p->left[j][k], solved_cube_tmpl[i-1]);
				break;

			case D0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						if(p->down[j][k] != solved_cube_tmpl[i++])
							ret = ret + 1 + GetDistance(p->down[j][k], solved_cube_tmpl[i-1]);
				break;

			case F0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						if(p->front[j][k] != solved_cube_tmpl[i++])
							ret = ret + 1 + GetDistance(p->front[j][k], solved_cube_tmpl[i-1]);
				break;

			case R0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						if(p->right[j][k] != solved_cube_tmpl[i++])
							ret = ret + 1 + GetDistance(p->right[j][k], solved_cube_tmpl[i-1]);
				break;

			case B0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						if(p->bottom[j][k] != solved_cube_tmpl[i++])
							ret = ret + 1 + GetDistance(p->bottom[j][k], solved_cube_tmpl[i-1]);
				break;

			default:
				throw Exception("Cube::EvalNode - unknown face code");
				break;
		}
	}

	return ret;
}

/*
 * Cube engine.
 * Return true if current cube state is a solved one.
 */
bool Cube::IsSolved()
{
	bool ret = false;

	if (NULL != m_pCurrent) {
		ret = IsSolved(m_pCurrent);
	}

	return ret;
}

/*
 * Cube engine.
 * Clean cube and solution from memory.
 * Remember to make a copy of your cube if you need to keep it before calling this method.
 */
int Cube::ClearPath(RCube *p)			
{
	int n = 0;

	if (m_pMemMgm->FindRCube(p)) {
		m_pMemMgm->DeleteAll();
		n++;
	}
	
	return n;
}

/*
 * Cube engine.
 * Return true if given cube is a solved one.
 * Optimize solution/reallocate memory in the process.
 */
bool Cube::IsSolved(RCube *p)
{
	bool ret = true;
	
	if (NULL == p)
		throw Exception("Cube::IsSolved() - null pointer!");

	ret = IsSolvedState(p);
	m_bSolved = ret;
	if (ret) {
		string sol_path = GetSolutionPath(p);
		unsigned int sol_len = 0;
		do {
			sol_len = sol_path.length();
			sol_path = OptimizeSolutionPath(sol_path);
		} while (sol_len > sol_path.length());
		RCube *pTmpCube = new RCube(*m_pRoot);
		if (NULL == pTmpCube)
			throw Exception("Cube::IsSolved() - memory depleted!");
		ClearPath(p);
		m_pRoot = m_pMemMgm->GetNewRCube(pTmpCube);
		m_pRoot->NullParent();
		delete pTmpCube;
		Convert2CubieModel(m_pRoot);
		m_pCurrent = DoMoves(m_pRoot, sol_path);
		m_bSolved = ret = true;
		m_pSolved = m_pCurrent;
	} else if (m_bCacheLoaded) {
		string known_path = SolutionInKnownPaths(p);
		if (0 < known_path.length()) {
			string full_path = GetSolutionPath(p);
			full_path += known_path;
			unsigned int sol_len = 0;
			do {
				sol_len = full_path.length();
				full_path = OptimizeSolutionPath(full_path);
			} while (sol_len > full_path.length());
			RCube *pTmpCube = new RCube(*m_pRoot);
			if (NULL == pTmpCube)
				throw Exception("Cube::IsSolved() - memory depleted!");			
			ClearPath(p);
			m_pRoot = m_pMemMgm->GetNewRCube(pTmpCube);
			m_pRoot->NullParent();
			delete pTmpCube;
			Convert2CubieModel(m_pRoot);
			m_pCurrent = DoMoves(m_pRoot, full_path);
			m_bSolved = ret = true;
			m_pSolved = m_pCurrent;
		}
	}

	return ret;
}

/*
 * Cube engine.
 * Return true if current cube state is a solved one.
 */
bool Cube::IsSolvedState()
{
	return IsSolvedState(m_pCurrent);	
}

/*
 * Cube engine.
 * Return true if cube state is a solved one.
 */
bool Cube::IsSolvedState(RCube *p)
{
	bool ret = true;
	int i=0;
	
	if (NULL == p) return false;

	while (i<6*9 && ret) {
		switch (solved_cube_tmpl[i]) {
			case U0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						if (p->up[j][k] != solved_cube_tmpl[i++]) {
							ret = false;
							break;
						}
				break;

			case L0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						if(p->left[j][k] != solved_cube_tmpl[i++]) {
							ret = false;
							break;
						}
				break;

			case D0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						if(p->down[j][k] != solved_cube_tmpl[i++]) {
							ret = false;
							break;
						}
				break;

			case F0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						if(p->front[j][k] != solved_cube_tmpl[i++]) {
							ret = false;
							break;
						}
				break;

			case R0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						if(p->right[j][k] != solved_cube_tmpl[i++]) {
							ret = false;
							break;
						}
				break;

			case B0:
				for (int j=0; j<3; j++)
					for (int k=0; k<3 && i<6*9; k++)
						if(p->bottom[j][k] != solved_cube_tmpl[i++]) {
							ret = false;
							break;
						}
				break;

			default:
				throw Exception("Cube::IsSolvedState - unknown face code");
				break;
		}
	}
	
	return ret;
}

/*
 * Cube engine.
 * Algorithm T45.
 * Return true if cubie model arrays in solved state and set pointers.
 */
bool Cube::IsSolvedCubieModel()
{
	bool ret = false;
	
	ret = CubieModelSolved(m_anEdgePerm, m_anCornPerm, m_anEdgeOrient, m_anCornOrient);
	if (ret) {
		m_bSolved = true;
		m_pSolved = m_pCurrent;		
	}
	
	return ret;
}

/*
 * Optimize the solution path.
 * Remove redundant/self-cancelling sequences.
 * Replace longer sequences with shorter equivalents.
 */
string Cube::OptimizeSolutionPath(string sol_path)
{
   string ret, cache;
   char prev = ' ', prev2 = ' ';
   int count = 0;
   
#if defined(MYDBG18)
   cout << "MYDBG18: Cube::OptimizeSolutionPath() - START" << endl;
   cout << "MYDBG18: Before: " << sol_path << endl;
#endif   

   sol_path += ' ';   // add space at the end for proper algorithm function at the end of the string
   for (string::iterator it = sol_path.begin(); it != sol_path.end(); ++it) {
      char c = *it;
      if ('i' == c) continue;
      if ('2' == c) c = prev;
      char ci = *(it+1);
      if ('i' == ci)
         c = tolower(c);
      if (prev == c && 3 > count)
         count++;
      else {
         if (3 == count) {
            cache.clear();
            count = 0;
         } else if (2 == count) {
            if (isupper(prev))
               ret = ret + (char)tolower(prev);
            else
               ret = ret + (char)toupper(prev);
            cache.clear();
            count = 0;
         } else {
            if (0 < cache.length())
               ret += cache;
            cache.clear();
            count = 0;
         }
      }
      cache += c;
      prev = c;
   }
   if (0 < cache.length())
      ret += cache;
   cache = string();
   prev = ' ';
   prev2 = ' ';
   for (string::iterator it = ret.begin(); it != ret.end(); ++it) {
      char c = *it;
      if (prev != ' '
              &&
              (toupper(prev) == toupper(c)
               &&
               ((isupper(prev) && islower(c)) || (islower(prev) && isupper(c))))
         ) // detect adjacent moves that cancel each other
      {
         string::iterator cache_tail = cache.end() - 1;
         cache.erase(cache_tail);
         prev = ' ';
         continue;
      } else if (prev2 != ' '
                 && toupper(prev2) == toupper(c)
                 && ((isupper(prev2) && islower(c)) 
                      || (islower(prev2) && isupper(c)))
                ) // detect moves on opposite layers that cancel each other
      {
         string layer_pairs = "UDLRFB";
         bool bopt = false;
         for (unsigned int i=0; !bopt && i<layer_pairs.size(); i+=2) {
            if (((layer_pairs[i] == toupper(c)
                  && layer_pairs[i+1] == toupper(prev)
                 ) ||
                 (layer_pairs[i] == toupper(prev)
                  && layer_pairs[i+1] == toupper(c)
                 )) 
               )
            {
               bopt = true;
            }
         }
         if (bopt) {
            string::iterator cache_tail = cache.end() - 2;
            cache.erase(cache_tail);
            prev = ((cache.size()>0) ? cache[cache.size()-1] : ' ');
            prev2 = ((cache.size()>1) ? cache[cache.size()-2] : ' ');
            continue;
         }
      }
      cache += c;
      prev2 = prev;
      prev = c;
   }
   ret = cache;
   cache.clear();
   // convert 2 individual quarter turns to double turn
   for (string::iterator it = ret.begin(); it != ret.end(); ++it) {
      char c = *it;
	  cache = cache + c;
	  if (1 < cache.length()) {
	     if (cache[cache.length()-2] == c) {
	     	cache[cache.length()-2] = toupper(c);
	     	cache[cache.length()-1] = '2';
		 }
	  }
   }
   ret = cache;
   cache.clear();
   // convert lower case to upper case + 'i'
   for (string::iterator it = ret.begin(); it != ret.end(); ++it) {
      char c = *it;
      if (islower(c)) {
      	cache = cache + (char)toupper(c) + 'i';
	  } else {
	  	cache = cache + c;
	  }
   }
   ret = cache;
   // remove the extra space from the end - we don't need it anymore
   if (' ' == ret[ret.length()-1]) {
	  string::iterator ret_tail = ret.end() - 1;
   	  ret.erase(ret_tail);   	
   }
   
#if defined(MYDBG18)
   cout << "MYDBG18: After : " << ret << endl;
   cout << "MYDBG18: Cube::OptimizeSolutionPath() - END" << endl;   
#endif      

   return ret;
}

/*
 * Cube engine.
 * Get new RCube object from memory manager.
 * Throw exception if unable to.
 */
RCube *Cube::GetNewRCube4Move(RCube *p, int move)
{
	RCube *ret = m_pMemMgm->GetNewRCube(p);
	
	if (NULL == ret) {
		throw Exception ("Cube::GetNewRCube4Move() - memory depleted!");
	}
	
	ret->parent_move = move;
	
	return ret;
}

/*
 * Cube engine.
 * Generate the next cube state.
 */
RCube *Cube::DoMove(RCube *start, int move)
{
	// facelet model
	RCube *cube = GetNewRCube4Move(start, move);

	switch (move) {
		case MOVE_F2:	MoveFrontTopRight(cube);
		case MOVE_F:	MoveFrontTopRight(cube);
							break;

		case MOVE_Fi:	MoveFrontTopLeft(cube);
							break;

		case MOVE_Li:	MoveFrontLeftUp(cube);
							break;

		case MOVE_L2:	MoveFrontLeftDown(cube);
		case MOVE_L:	MoveFrontLeftDown(cube);
							break;

		case MOVE_Ui:	MoveFrontUpRight(cube);
							break;

		case MOVE_U2:	MoveFrontUpLeft(cube);
		case MOVE_U:	MoveFrontUpLeft(cube);
							break;

		case MOVE_R2:	MoveFrontRightUp(cube);
		case MOVE_R:	MoveFrontRightUp(cube);
							break;

		case MOVE_Ri:	MoveFrontRightDown(cube);
							break;

		case MOVE_D2:	MoveFrontDownRight(cube);
		case MOVE_D:	MoveFrontDownRight(cube);
							break;

		case MOVE_Di:	MoveFrontDownLeft(cube);
							break;

		case MOVE_Bi:	MoveBottomTopRight(cube);
							break;

		case MOVE_B2:	MoveBottomTopLeft(cube);
		case MOVE_B:	MoveBottomTopLeft(cube);
							break;

		default:
			throw Exception("Cube::DoMove - unknown move!");
			break;
	}
	// cubie model
	ApplyMove(move, m_anEdgePerm, m_anCornPerm, m_anEdgeOrient, m_anCornOrient);
	
	m_pCurrent = cube;

	return cube;
}

/*
 * Cube engine.
 * Change the cube state.
 */
RCube *Cube::DoMoveStatic(RCube *start, int move)
{
	// facelet model
	RCube *cube = start;

	switch (move) {
		case MOVE_F2:	MoveFrontTopRight(cube);
		case MOVE_F:	MoveFrontTopRight(cube);
							break;
		
		case MOVE_Fi:	MoveFrontTopLeft(cube);
							break;

		case MOVE_Li:	MoveFrontLeftUp(cube);
							break;

		case MOVE_L2:	MoveFrontLeftDown(cube);
		case MOVE_L:	MoveFrontLeftDown(cube);
							break;

		case MOVE_Ui:	MoveFrontUpRight(cube);
							break;

		case MOVE_U2:	MoveFrontUpLeft(cube);
		case MOVE_U:	MoveFrontUpLeft(cube);
							break;

		case MOVE_R2:	MoveFrontRightUp(cube);
		case MOVE_R:	MoveFrontRightUp(cube);
							break;

		case MOVE_Ri:	MoveFrontRightDown(cube);
							break;

		case MOVE_D2:	MoveFrontDownRight(cube);
		case MOVE_D:	MoveFrontDownRight(cube);
							break;

		case MOVE_Di:	MoveFrontDownLeft(cube);
							break;

		case MOVE_Bi:	MoveBottomTopRight(cube);
							break;

		case MOVE_B2:	MoveBottomTopLeft(cube);
		case MOVE_B:	MoveBottomTopLeft(cube);
							break;

		default:
			throw Exception("Cube::DoMove - unknown move!");
			break;
	}
	// cubie model
	ApplyMove(move, m_anEdgePerm, m_anCornPerm, m_anEdgeOrient, m_anCornOrient);

	m_pCurrent = cube;

	return cube;
}

/*
 * Cube engine.
 * Generate the next cube state from standardized notation for moves:
 * Terminology: qt - quarter-turn (90 deg), dt - double-turn (180 deg),
 *              cw - clock-wise, ccw - counter clock-wise (irrelevant for dt)
 * "F" 	- front face cw qt
 * "Fi"	- front face ccw qt
 * "F2" - front face dt
 * "Li"	- left face ccw qt
 * "L"	- left face cw qt
 * "L2" - left face dt
 * "U"	- top (upper) face cw qt
 * "Ui"	- top (upper) face ccw qt
 * "U2" - top (upper) face dt
 * "R"	- right face cw qt
 * "Ri"	- right face ccw qt
 * "R2" - right face dt
 * "D"	- bottom (lower) face cw qt
 * "Di"	- bottom (lower) face ccw qt
 * "D2" - bottom (lower) face dt
 * "Bi"	- back face ccw qt
 * "B"	- back face cw qt
 * "B2" - back face dt
 * The 'moves' string can look like: "RiUFiUi" or "RiDiRD" or "URUiRiUiFiUF" or "R2F2LiRU2" etc.
 * Each state along the way remains on the list, and can be tracked back
 * via 'parent' link.
 */
RCube *Cube::DoMoves(RCube *pcube, string moves)
{
	RCube *pret = pcube;
	if (NULL == pret)
		throw Exception("Cube::DoMoves - NULL pointer!");

	// initialize vector with standardized cube moves codes
	vector<string> vnmoves (move_names_N, move_names_N+sizeof(move_names_N)/sizeof(string));
	int move_num = 0;	// the integer move code

	for (unsigned int i=0; i < moves.length(); i++) {	// parse the moves string 2-characters at a time
		char c1 = moves[i];	// the face code
		char c2 = '\0';			// a potential reverse or counter-clockwise rotation code: 'i'
		char sm[3];

		if (i < moves.length()-1)
			c2 = moves[i+1];

		// look for the code c1c2 in the standardized moves codes vector
		int n=0;	// move code iteration
		bool found = false, dbl = false;
		sm[0] = c1;
		sm[1] = ('i' == c2) ? c2 : '\0';
		sm[2] = '\0';
		string smove(sm);
		for (vector<string>::iterator its = vnmoves.begin() ; its != vnmoves.end(); ++its) {
			string s1 = *its;
			if (0 == smove.compare(s1)) {
				if ('i' == c2) i++;
				if ('2' == c2) {
					i++; dbl=true;
				}
				found = true;
				break;
			}

			n++;				 // next move code
		}

		if (found) {
			move_num = n;	// n is our integer move code
			if (dbl) {
				pret = DoMove(pret, move_num+MAX_MOVES);
			 } else {
			 	pret = DoMove(pret, move_num);
			 }
		} else
			throw Exception("Cube::DoMoves - invalid move code " + smove + "!");
	}

	return pret;
}

/*
 * Cube engine.
 * Generate the next cube state from standardized notation for moves.
 */
RCube *Cube::DoMoves(string moves)
{
	return DoMoves(m_pCurrent, moves);
}

/*
 * Cube engine.
 * Twist front top cube's plane to the right.
 */
void Cube::MoveFrontUpRight(RCube *p)
{
	int buf[3][3];

	ShiftFacesLR(0, p); // shift the side faces
	// Rotate upper faces accordingly.
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			buf[i][j] = p->up[i][j];

	for (int i=0; i<3; i++)
		p->up[0][i] = buf[i][2];
	for (int i=0; i<3; i++)
		p->up[i][2] = buf[2][2-i];
	for (int i=0; i<3; i++)
		p->up[2][i] = buf[i][0];
	for (int i=0; i<3; i++)
		p->up[i][0] = buf[0][2-i];
}

/*
 * Cube engine.
 * Twist front top cube's plane to the left.
 */
void Cube::MoveFrontUpLeft(RCube *p)
{
	int buf[3][3];

	ShiftFacesRL(0, p); // shift the side faces.
	// Rotate upper faces accordingly.
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			buf[i][j] = p->up[i][j];

	for (int i=0; i<3; i++)
		p->up[0][i] = buf[2-i][0];
	for (int i=0; i<3; i++)
		p->up[i][0] = buf[2][i];
	for (int i=0; i<3; i++)
		p->up[2][i] = buf[2-i][2];
	for (int i=0; i<3; i++)
		p->up[i][2] = buf[0][i];
}

/*
 * Cube engine.
 * Twist front left cube's plane up.
 */
void Cube::MoveFrontLeftUp(RCube *p)
{
	int buf[3][3];

	ShiftFacesDU(0, p); // shift the side faces
	// Rotate left faces accordingly.
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			buf[i][j] = p->left[i][j];

	for (int i=0; i<3; i++)
		p->left[0][i] = buf[i][2];
	for (int i=0; i<3; i++)
		p->left[i][2] = buf[2][2-i];
	for (int i=0; i<3; i++)
		p->left[2][i] = buf[i][0];
	for (int i=0; i<3; i++)
		p->left[i][0] = buf[0][2-i];
}

/*
 * Cube engine.
 * Twist front left cube's plane down.
 */
void Cube::MoveFrontLeftDown(RCube *p)
{
	int buf[3][3];

	ShiftFacesUD(0, p); // shift the side faces
	// Rotate left faes accordingly.
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			buf[i][j] = p->left[i][j];

	for (int i=0; i<3; i++)
		p->left[0][i] = buf[2-i][0];
	for (int i=0; i<3; i++)
		p->left[i][0] = buf[2][i];
	for (int i=0; i<3; i++)
		p->left[2][i] = buf[2-i][2];
	for (int i=0; i<3; i++)
		p->left[i][2] = buf[0][i];
}

/*
 * Cube engine.
 * Twist front left cube's plane up.
 */
void Cube::MoveFrontRightUp(RCube *p)
{
	int buf[3][3];

	ShiftFacesDU(2, p); // shift the side faces
	// Rotate right faces accordingly.
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			buf[i][j] = p->right[i][j];

	for (int i=0; i<3; i++)
		p->right[0][i] = buf[2-i][0];
	for (int i=0; i<3; i++)
		p->right[i][0] = buf[2][i];
	for (int i=0; i<3; i++)
		p->right[2][i] = buf[2-i][2];
	for (int i=0; i<3; i++)
		p->right[i][2] = buf[0][i];
}

/*
 * Cube engine.
 * Twist front left cube's plane down.
 */
void Cube::MoveFrontRightDown(RCube *p)
{
	int buf[3][3];

	ShiftFacesUD(2, p); // shift the side faces
	// Rotate right faces accordingly.
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			buf[i][j] = p->right[i][j];

	for (int i=0; i<3; i++)
		p->right[0][i] = buf[i][2];
	for (int i=0; i<3; i++)
		p->right[i][2] = buf[2][2-i];
	for (int i=0; i<3; i++)
		p->right[2][i] = buf[i][0];
	for (int i=0; i<3; i++)
		p->right[i][0] = buf[0][2-i];
}

/*
 * Cube engine.
 * Twist front down cube's plane to the right.
 */
void Cube::MoveFrontDownRight(RCube *p)
{
	int buf[3][3];

	ShiftFacesLR(2, p); // shift the side faces
	// Rotate down faces accordingly.
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			buf[i][j] = p->down[i][j];

	for (int i=0; i<3; i++)
		p->down[0][i] = buf[2-i][0];
	for (int i=0; i<3; i++)
		p->down[i][0] = buf[2][i];
	for (int i=0; i<3; i++)
		p->down[2][i] = buf[2-i][2];
	for (int i=0; i<3; i++)
		p->down[i][2] = buf[0][i];
}

/*
 * Cube engine.
 * Twist front down cube's plane to the left.
 */
void Cube::MoveFrontDownLeft(RCube *p)
{
	int buf[3][3];

	ShiftFacesRL(2, p); // shift the side faces
	// Rotate down faces accordingly.
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			buf[i][j] = p->down[i][j];

	for (int i=0; i<3; i++)
		p->down[0][i] = buf[i][2];
	for (int i=0; i<3; i++)
		p->down[i][2] = buf[2][2-i];
	for (int i=0; i<3; i++)
		p->down[2][i] = buf[i][0];
	for (int i=0; i<3; i++)
		p->down[i][0] = buf[0][2-i];
}

/*
 * Cube engine.
 * Twist top plane right.
 * NOTE:
 *   Inefficient. Multiple unnecesary copying happens here (corner cubes).
 */
void Cube::MoveFrontTopRight(RCube *p)
{
	int buf[3][3];
	int buf2[3];

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			buf[i][j] = p->front[i][j];

	for (int i=0; i<3; i++)
		p->front[0][i] = buf[2-i][0];
	for (int i=0; i<3; i++)
		p->front[i][0] = buf[2][i];
	for (int i=0; i<3; i++)
		p->front[2][i] = buf[2-i][2];
	for (int i=0; i<3; i++)
		p->front[i][2] = buf[0][i];

	for (int i=0; i<3; i++)
		buf2[i] = p->up[2][i];
	for (int i=0; i<3; i++)
		p->up[2][i] = p->left[2-i][2];
	for (int i=0; i<3; i++)
		p->left[i][2] = p->down[0][i];
	for (int i=0; i<3; i++)
		p->down[0][i] = p->right[2-i][0];
	for (int i=0; i<3; i++)
		p->right[i][0] = buf2[i];
}

/*
 * Cube engine.
 * Twist top plane left.
 */
void Cube::MoveFrontTopLeft(RCube *p)
{
	int buf[3][3];
	int buf2[3];

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			buf[i][j] = p->front[i][j];

	for (int i=0; i<3; i++)
		p->front[0][i] = buf[i][2];
	for (int i=0; i<3; i++)
		p->front[i][2] = buf[2][2-i];
	for (int i=0; i<3; i++)
		p->front[2][i] = buf[i][0];
	for (int i=0; i<3; i++)
		p->front[i][0] = buf[0][2-i];

	for (int i=0; i<3; i++)
		buf2[i] = p->up[2][i];
	for (int i=0; i<3; i++)
		p->up[2][i] = p->right[i][0];
	for (int i=0; i<3; i++)
		p->right[i][0] = p->down[0][2-i];
	for (int i=0; i<3; i++)
		p->down[0][i] = p->left[i][2];
	for (int i=0; i<3; i++)
		p->left[i][2] = buf2[2-i];
}

/*
 * Cube engine.
 * Twist bottom plane right.
 */
void Cube::MoveBottomTopRight(RCube *p)
{
	int buf[3][3];
	int buf2[3];

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			buf[i][j] = p->bottom[i][j];

	for (int i=0; i<3; i++)
		p->bottom[0][i] = buf[i][2];
	for (int i=0; i<3; i++)
		p->bottom[i][2] = buf[2][2-i];
	for (int i=0; i<3; i++)
		p->bottom[2][i] = buf[i][0];
	for (int i=0; i<3; i++)
		p->bottom[i][0] = buf[0][2-i];

	for (int i=0; i<3; i++)
		buf2[i] = p->up[0][i];
	for (int i=0; i<3; i++)
		p->up[0][i] = p->left[2-i][0];
	for (int i=0; i<3; i++)
		p->left[i][0] = p->down[2][i];
	for (int i=0; i<3; i++)
		p->down[2][i] = p->right[2-i][2];
	for (int i=0; i<3; i++)
		p->right[i][2] = buf2[i];
}

/*
 * Cube engine.
 * Twist bottom plane left.
 */
void Cube::MoveBottomTopLeft(RCube *p)
{
	int buf[3][3];
	int buf2[3];

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			buf[i][j] = p->bottom[i][j];

	for (int i=0; i<3; i++)
		p->bottom[0][i] = buf[2-i][0];
	for (int i=0; i<3; i++)
		p->bottom[i][0] = buf[2][i];
	for (int i=0; i<3; i++)
		p->bottom[2][i] = buf[2-i][2];
	for (int i=0; i<3; i++)
		p->bottom[i][2] = buf[0][i];

	for (int i=0; i<3; i++)
		buf2[i] = p->up[0][i];
	for (int i=0; i<3; i++)
		p->up[0][i] = p->right[i][2];
	for (int i=0; i<3; i++)
		p->right[i][2] = p->down[2][2-i];
	for (int i=0; i<3; i++)
		p->down[2][i] = p->left[i][0];
	for (int i=0; i<3; i++)
		p->left[i][0] = buf2[2-i];
}

/*
 * Cube engine.
 * Shift faces in proper order in Left to Right direction.
 */
void Cube::ShiftFacesLR(int idx, RCube *p)
{
	int buf[3];

	for (int i=0; i<3; i++) {
		buf[i] = p->bottom[idx][i];
		p->bottom[idx][i] = p->right[idx][i];
		p->right[idx][i] = p->front[idx][i];
		p->front[idx][i] = p->left[idx][i];
		p->left[idx][i] = buf[i];
	}
}

/*
 * Cube engine.
 * Shift faces in proper order in Right to Left direction.
 */
void Cube::ShiftFacesRL(int idx, RCube *p)
{
	int buf[3];

	for (int i=0; i<3; i++) {
		buf[i] = p->bottom[idx][i];
		p->bottom[idx][i] = p->left[idx][i];
		p->left[idx][i] = p->front[idx][i];
		p->front[idx][i] = p->right[idx][i];
		p->right[idx][i] = buf[i];
	}
}

/*
 * Cube engine.
 * Shift faces in proper order in Down to Up direction.
 */
void Cube::ShiftFacesDU(int idx, RCube *p)
{
	int buf[3];

	for (int i=0; i<3; i++) {
		buf[i] = p->bottom[2-i][2-idx];
		p->bottom[2-i][2-idx] = p->up[i][idx];
		p->up[i][idx] = p->front[i][idx];
		p->front[i][idx] = p->down[i][idx];
		p->down[i][idx] = buf[i];
	}
}

/*
 * Cube engine.
 * Shift faces in proper order in Up to Down direction.
 */
void Cube::ShiftFacesUD(int idx, RCube *p)
{
	int buf[3];

	for (int i=0; i<3; i++) {
		buf[i] = p->bottom[2-i][2-idx];
		p->bottom[2-i][2-idx] = p->down[i][idx];
		p->down[i][idx] = p->front[i][idx];
		p->front[i][idx] = p->up[i][idx];
		p->up[i][idx] = buf[i];
	}
}

/*
 * Cube engine - presentation (console).
 * Print the presentation of the internal current cube state m_pCurrent.
 */
void Cube::Print()
{
	if (NULL != m_pCurrent) {
		Print(*m_pCurrent);
	}
}

/*
 * Cube engine - presentation (console).
 * Print the presentation of the cube state.
 */
void Cube::Print(const RCube &cube)
{
	SetDefaultColorAttr();
	cout << endl;
	for (int i=0; i<3; i++) {
		cout << "          ";
		for (int j=0; j<3; j++) {
			SetColorAttr(GetColorAttr(cube.up[i][j]));
			cout << FaceSymbol(cube.up[i][j]);
			SetDefaultColorAttr();
			cout << " ";
		}
		cout << endl;
	}
	cout << endl;
	for (int j=0; j<3; j++) {
		for (int i=0; i<3; i++) {
			SetColorAttr(GetColorAttr(cube.left[j][i]));
			cout << FaceSymbol(cube.left[j][i]);
			SetDefaultColorAttr();
			cout << " ";
		}
		cout << " ";
		for (int i=0; i<3; i++) {
			SetColorAttr(GetColorAttr(cube.front[j][i]));
			cout << FaceSymbol(cube.front[j][i]);
			SetDefaultColorAttr();
			cout << " ";
		}
		cout << " ";
		for (int i=0; i<3; i++) {
			SetColorAttr(GetColorAttr(cube.right[j][i]));
			cout << FaceSymbol(cube.right[j][i]);
			SetDefaultColorAttr();
			cout << " ";
		}
		cout << " ";
		for (int i=0; i<3; i++) {
			SetColorAttr(GetColorAttr(cube.bottom[j][i]));
			cout << FaceSymbol(cube.bottom[j][i]);
			SetDefaultColorAttr();
			cout << " ";
		}
		cout << endl;
	}
	cout << endl;
	for (int i=0; i<3; i++) {
		cout << "          ";
		for (int j=0; j<3; j++) {
			SetColorAttr(GetColorAttr(cube.down[i][j]));
			cout << FaceSymbol(cube.down[i][j]);
			SetDefaultColorAttr();
			cout << " ";
		}
		cout << endl;
	}
#if defined(MYDBG22)
	CubieModelShowAllStates(m_anEdgePerm, m_anCornPerm, m_anEdgeOrient, m_anCornOrient);
#endif	
}

//#define BACKGROUND_ORANGE BACKGROUND_RED|BACKGROUND_GREEN
#define BACKGROUND_ORANGE 87 // not really orange color
#define BACKGROUND_YELLOW BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_INTENSITY

/*
 * Cube engine - presentation (console).
 * Return color attribute for provided face code.
 */
int Cube::GetColorAttr(int face_code)
{
	int ret = 10;
#if defined(DOS)
	switch(face_code) {
		case U0:
		case U1:
		case U2:
		case U3:
		case U4:
		case U5:
		case U6:
		case U7:
		case U8:
			ret = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED; // 20
			break;

		case L0:
		case L1:
		case L2:
		case L3:
		case L4:
		case L5:
		case L6:
		case L7:
		case L8:
			ret = BACKGROUND_GREEN; // 30;
			break;

		case F0:
		case F1:
		case F2:
		case F3:
		case F4:
		case F5:
		case F6:
		case F7:
		case F8:
			ret = BACKGROUND_RED|BACKGROUND_INTENSITY; // 46;
			break;

		case R0:
		case R1:
		case R2:
		case R3:
		case R4:
		case R5:
		case R6:
		case R7:
		case R8:
			ret = BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED; // 55;
			break;

		case B0:
		case B1:
		case B2:
		case B3:
		case B4:
		case B5:
		case B6:
		case B7:
		case B8:
			ret = BACKGROUND_ORANGE; // 65;
			break;

		case D0:
		case D1:
		case D2:
		case D3:
		case D4:
		case D5:
		case D6:
		case D7:
		case D8:
			ret = BACKGROUND_YELLOW; // 70;
			break;

		default:
			break;
	}
#endif

	return ret;
}

/*
 * Cube engine.
 * Return symbolic representation of the cubie's single face.
 */
string Cube::FaceSymbol(int face_code)
{
	ostringstream conv;

	conv << face_code;
	if (face_code < 0 || face_code >= FACES_END)
		throw Exception("Cube::FaceSymbol: face_code out of range [" + conv.str() + "]");

	string ret = string(faces_sym_tbl[face_code]);

	return ret;
}

/*
 * Cube engine.
 * Return symbolic representation of the face's color.
 */
string Cube::ColorCode(int face_code)
{
	ostringstream conv;

	conv << face_code;
	if (face_code < 0 || face_code >= FACES_END)
		throw Exception("Cube::ColorCode: face_code out of range [" + conv.str() + "]");

	string color = string(color_sym_tbl[face_code]);
	string ret = string(1, color[0]);

	return ret;
}

/*
 * Cube engine.
 * Returns names of solve methods recognized by the engine.
 */
vector<string> Cube::GetSolveMethodsNames()
{
	vector<string> ret;

	for (int i = 0; i < MAX_METHODS; i++) {
		ret.push_back(solve_methods_names[i]);
	}

	return ret;
}

/*
 * Cube engine.
 * Return the integer code of cube solve method currently set.
 */
int Cube::GetMethodNum()
{
	return m_nMethod;
}

/*
 * Cube engine.
 * Return the name of the cube solve method currently set.
 */
string Cube::GetMethodName()
{
	return solve_methods_names[m_nMethod];
}

// another ugly macro
#define CONV_FACE_CODES 		\
	for (int i=0; i<3; i++) 	\
		for (int j=0; j<3; j++)

/*
 * Cube engine.
 * Convert cube to the string representation in following format:
 * "u0u1u2u3u4u5u6u7u8l0l1l2l3l4l5l6l7l8f0f1f2f3f4f5f6f7f8r0r1r2r3r4r5r6r7r8b0b1b2b3b4b5b6b7b8d0d1d2d3d4d5d6d7d8"
 * Where:
 * ux, lx, fx... - the current 2-character face code equivalents that are present at respective positions.
 */
string Cube::ConvertCube2String(const RCube *p)
{
	string ret = string();
	if (NULL == p)
		p = m_pCurrent;	

	CONV_FACE_CODES { ret = ret + FaceSymbol(p->up[i][j]);		}
	CONV_FACE_CODES { ret = ret + FaceSymbol(p->left[i][j]);	}
	CONV_FACE_CODES { ret = ret + FaceSymbol(p->front[i][j]);	}
	CONV_FACE_CODES { ret = ret + FaceSymbol(p->right[i][j]);	}
	CONV_FACE_CODES { ret = ret + FaceSymbol(p->bottom[i][j]);	}
	CONV_FACE_CODES { ret = ret + FaceSymbol(p->down[i][j]);	}

	if (ret.length() != 108)
		throw Exception("Cube::ConvertCube2String - wrong input format!");

	return ret;
}

/*
 * Cube engine.
 * Convert cube to the string representation in following format:
 * "u0 u1 u2 u3 u4 u5 u6 u7 u8 l0 l1 l2 l3 l4 l5 l6 l7 l8 f0 f1 f2 f3 f4 f5 f6 f7 f8 r0 r1 r2 r3 r4 r5 r6 r7 r8 b0 b1 b2 b3 b4 b5 b6 b7 b8 d0 d1 d2 d3 d4 d5 d6 d7 d8"
 * Where:
 * ux, lx, fx... - the current 2-character face code equivalents that are present at respective positions.
 */
string Cube::ConvertCube2InputString(const RCube *p)
{
	string ret = string();
	if (NULL == p)
		p = m_pCurrent;

	CONV_FACE_CODES { ret = ret + ' ' + FaceSymbol(p->up[i][j]);		}
	CONV_FACE_CODES { ret = ret + ' ' + FaceSymbol(p->left[i][j]);		}
	CONV_FACE_CODES { ret = ret + ' ' + FaceSymbol(p->front[i][j]);		}
	CONV_FACE_CODES { ret = ret + ' ' + FaceSymbol(p->right[i][j]);		}
	CONV_FACE_CODES { ret = ret + ' ' + FaceSymbol(p->bottom[i][j]);	}
	CONV_FACE_CODES { ret = ret + ' ' + FaceSymbol(p->down[i][j]);		}
	
	string::iterator ret_front = ret.begin();
	ret.erase(ret_front);	

	if (ret.length() != 108 + 108/2 - 1)
		throw Exception("Cube::ConvertCube2InputString - wrong input format!");

	return ret;
}

/*
 * Cube engine.
 * Convert cube to the string representation in following format:
 * "u0 u1 u2 u3 u4 u5 u6 u7 u8 l0 l1 l2 l3 l4 l5 l6 l7 l8 f0 f1 f2 f3 f4 f5 f6 f7 f8 r0 r1 r2 r3 r4 r5 r6 r7 r8 b0 b1 b2 b3 b4 b5 b6 b7 b8 d0 d1 d2 d3 d4 d5 d6 d7 d8"
 * Where:
 * ux, lx, fx... - the current 1-character face color code equivalents that are present at respective positions.
 */
string Cube::ConvertCube2ColorsString(const RCube *p)
{
	string ret = string();
	if (NULL == p)
		p = m_pCurrent;

	CONV_FACE_CODES { ret = ret + ' ' + ColorCode(p->up[i][j]);			}
	CONV_FACE_CODES { ret = ret + ' ' + ColorCode(p->left[i][j]);		}
	CONV_FACE_CODES { ret = ret + ' ' + ColorCode(p->front[i][j]);		}
	CONV_FACE_CODES { ret = ret + ' ' + ColorCode(p->right[i][j]);		}
	CONV_FACE_CODES { ret = ret + ' ' + ColorCode(p->bottom[i][j]);		}
	CONV_FACE_CODES { ret = ret + ' ' + ColorCode(p->down[i][j]);		}
	
	string::iterator ret_front = ret.begin();
	ret.erase(ret_front);	

	if (ret.length() != 107)
		throw Exception("Cube::ConvertCube2ColorsString - wrong input format!");

	return ret;
}

/*
 * Convert cube to color definition, no spaces.
 */
string Cube::GetColorCubeDefNoSpaces(const RCube *p)
{
	string color_cube = ConvertCube2ColorsString(p);
	string colcube_ns;
	for (string::iterator it = color_cube.begin(); it != color_cube.end(); ++it) {
		char c = *it;
		if (c != ' ') {
			colcube_ns = colcube_ns + c;
		}
	}
	
	return colcube_ns;
}

/*
 * Cube engine.
 * Convert cube to the compact string representation in following format:
 * "u0u1u2u3u4u5u6u7u8l0l1l2l3l4l5l6l7l8f0f1f2f3f4f5f6f7f8r0r1r2r3r4r5r6r7r8b0b1b2b3b4b5b6b7b8d0d1d2d3d4d5d6d7d8"
 * Where:
 * ux, lx, fx... - the current 1-character face code equivalents that are present at respective positions.
 */
string Cube::ConvertCube2CompactString(const RCube *p)
{
	string ret;
	if (NULL == p)
		p = m_pCurrent;	
		
	CONV_FACE_CODES { ret = ret + string(1,compact_faces_sym_tbl[p->up[i][j]]);		}
	CONV_FACE_CODES { ret = ret + string(1,compact_faces_sym_tbl[p->left[i][j]]);	}
	CONV_FACE_CODES { ret = ret + string(1,compact_faces_sym_tbl[p->front[i][j]]);	}
	CONV_FACE_CODES { ret = ret + string(1,compact_faces_sym_tbl[p->right[i][j]]);	}
	CONV_FACE_CODES { ret = ret + string(1,compact_faces_sym_tbl[p->bottom[i][j]]);	}
	CONV_FACE_CODES { ret = ret + string(1,compact_faces_sym_tbl[p->down[i][j]]);	}

	if (ret.length() != 54)
		throw Exception("Cube::ConvertCube2CompactString - wrong input format!");

	return ret;
}

/*
 * Cube engine.
 * Convert string representation of the cube in the no-spaces 2-char face codes
 * (Cube::ConvertCube2String) to the RCube object.
 */
RCube *Cube::ConvertString2Cube(const string s)
{
	RCube *pret = NULL;
	vector<string> vcubedef;

	if (108 == s.length()) {
		for (unsigned int i=0; i<s.length(); i+=2) {
			string sfc = s.substr(i, 2);
			vcubedef.push_back(sfc);
		}
		pret = CreateCubeFromStrVectDef(vcubedef, false);
	} else
		throw Exception("Cube::ConvertString2Cube - wrong input format!");

	return pret;
}

/*
 * Cube engine.
 * Convert compact string representation of the cube in the no-spaces 1-char face codes
 * (Cube::ConvertCube2CompactString) to the RCube object.
 */
RCube *Cube::ConvertCompactString2Cube(const string s)
{
	RCube *pret = NULL;
	vector<string> vcubedef;

	if (54 == s.length()) {
		for (unsigned int i=0; i<s.length(); i++) {
			string sfc = ConvCompactStrCode2FaceCode(string(1,s[i]));
			vcubedef.push_back(sfc);
		}
		pret = CreateCubeFromStrVectDef(vcubedef, false);
	} else
		throw Exception("Cube::ConvertCompactString2Cube - wrong input format!");
		
	return pret;
}

/*
 * Cube engine.
 * Convert compact string code to face code.
 */
string Cube::ConvCompactStrCode2FaceCode(string cpct_code)
{
	string ret;
	
	if (1 < cpct_code.size())
		throw Exception("Cube::ConvCompactStrCode2FaceCode - invalid input code!");
	
	for (int i=0; i < 6*9; i++) {
		if (compact_faces_sym_tbl[i] == cpct_code.at(0)) {
			ret = faces_sym_tbl[i];
			break;
		}
	}
	
	return ret;
}

/*
 * Return the amount of memory used by the system.
 */
unsigned long long Cube::MemoryUsed()
{
	unsigned long long ret = 0L;

#if defined(DOS)
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
	DWORDLONG virtualMemUsed = totalVirtualMem - memInfo.ullAvailPageFile;
	ret = virtualMemUsed;
#endif

	return ret;
}

/*
 * Return the total amount of memory in the system.
 */
unsigned long long Cube::TotalSystemMemory()
{
	unsigned long long ret = 0L;

#if defined(DOS)
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
	ret = totalVirtualMem;
#endif

	return ret;
}

/*
 * Return the amount of memory used by the system.
 */
size_t Cube::PrivateMemoryUsed()
{
	size_t ret = 0;

#if defined(DOS)
	//PROCESS_MEMORY_COUNTERS_EX pmc;
	PROCESS_MEMORY_COUNTERS pmc;
	PPROCESS_MEMORY_COUNTERS ppmc = &pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), ppmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PagefileUsage;
	ret = virtualMemUsedByMe;
#endif

	return ret;
}

/*
 * Cube engine. Tree search.
 * Add node and it's path to the known paths map.
 */
void Cube::Add2KnownPaths(const RCube *p)
{
	string cube_str = ConvertCube2CompactString(p);
	string path = GetSolutionPath(p);
	string kp = SolutionInKnownPaths(p, false);
	if (kp.length()) {
		if (kp.length() > path.length()) {
			m_MapKnownPaths[cube_str] = path;
		}
	} else
		m_MapKnownPaths[cube_str] = path;
}

/*
 * Cube engine.
 * Return path leading from the node p to the solved cube.
 */
string Cube::GetSolutionPath(const RCube *p)
{
	if (NULL == p)
		throw Exception("Cube::GetSolutionPath(RCube *p) - null pointer!");	
	string moves;
	RCube *ptr = (RCube *)p;
	
	if (false == m_pMemMgm->FindRCube(ptr))
		throw Exception ("Cube::GetSolutionPath() - fatal failure - pointer not found in mem. mgm. object!");

	m_nSolLen = 0;
	while (ptr != NULL) {
		if (NULL != ptr->parent) {
			if (m_nMethod != TreeSearchBackTrace)
				moves = move_names_N[ptr->parent_move] + moves;
			else
				moves = moves + o_move_names_N[ptr->parent_move];
			m_nSolLen++;
		}
		ptr = ptr->parent;
	}
	
	return moves;
}

/*
 * Cube engine.
 * Return path to the solved cube.
 */
string Cube::GetSolutionPath()
{
	RCube *ptr = m_pSolved;
	if (NULL == ptr)
		throw Exception("Cube::GetSolutionPath() - null pointer!");
	if (false == m_pMemMgm->FindRCube(ptr))
		throw Exception ("Cube::GetSolutionPath() - fatal failure - pointer not found in mem. mgm. object!");		
	string moves;
	m_nSolLen = 0;
	while (ptr != NULL) {
		if (NULL != ptr->parent) {
			if (0 > ptr->parent_move || ptr->parent_move >= MAX_DMOVES)
				throw Exception("Cube::GetSolutionPath() - move code out of range!");
			if (m_nMethod != TreeSearchBackTrace)
				moves = move_names_N[ptr->parent_move] + moves;
			else
				moves = moves + o_move_names_N[ptr->parent_move];
			m_nSolLen++;
		}
		ptr = ptr->parent;
	}
	
	return moves;
}

/*
 * Cube engine. Tree search.
 * Generate known paths map.
 */
void Cube::GenerateKnownPaths(const RCube *start, int depth)
{
	usleep(1); // let the OS/kernel "breathe"

	if (0 == depth) return;

	int tmp_depth = -1;
	int tmp_method = -1;
	RCube *root = (RCube *)start;
	if (NULL == root) {
		root = CreateSolvedCube();
		root->NullParent();
		// preserve current settings
		tmp_depth = m_nDepth;
		tmp_method = m_nMethod;
		// setup for generating cached tree
		m_nDepth = depth;
		m_nMethod = TreeSearchBackTrace;
	} else {
		if (1 == depth)
			Add2KnownPaths(root);
	}

	for (int i=0; i<MAX_MOVES; i++) {
		RCube *p;

		if (depth == m_nDepth) {
			cout << "Start:";
			ShowRootInfo(i);
		}

		p = DoMove(root, i);

		if (NULL != p) {
			GenerateKnownPaths(p, depth-1);
			if (depth == m_nDepth) {
				cout << "Done:";
				ShowRootInfo(i);
			}
			DeleteCube(p);
		}
	}

	// restore original settings
	if (-1 != tmp_depth)
		m_nDepth = tmp_depth;
	if (-1 != tmp_method)
		m_nMethod = tmp_method;

	return;
}

/*
 * Cube engine. Tree search.
 * Check if solution exists in known paths map.
 */
string Cube::SolutionInKnownPaths(const RCube *start, bool show)
{
	string ret;
	string cube_str = ConvertCube2CompactString(start);

	if (0 < m_MapKnownPaths.count(cube_str)) {
		ret = m_MapKnownPaths[cube_str];
		if (show)
			cout << endl << "Known solution found: '" << ret << "'" << endl;
	}

	return ret;
}

/*
 * Show some info at the root of the tree search.
 */
void Cube::ShowRootInfo(int branch)
{
	cout << endl << "ROOT->branch: " << move_names_N[branch] << ", method: " << solve_methods_names[m_nMethod] << endl;
	cout << "Memory use: ";
	ShowMemUse();
}

/*
 * Cube engine.
 * Load pre-generated solutions from cache.
 */
bool Cube::LoadKnownPathsFromCache()
{
	return LoadCache(KNOWN_PATHS_FN, m_MapKnownPaths);
}

/*
 * Cube engine.
 * Save pre-generated solutions in cache.
 */
void Cube::CacheKnownPaths()
{
	char nname[40] = KNOWN_PATHS_FN;
	int nfopstat = 0;

	strcat(nname, "_bak");
	if(ifstream(nname))
		nfopstat = remove(nname);
	if(0 != nfopstat)
		cout << endl << "ERROR: Cube::CacheKnownPaths() - unable to remove old backup of cache!" << endl;
	if(ifstream(KNOWN_PATHS_FN)) {
		nfopstat = rename(KNOWN_PATHS_FN, nname);
		if(0 != nfopstat)
			cout << endl << "ERROR: Cube::CacheKnownPaths() - unable to create backup copy of cache!" << endl;
	}

	SaveLookupMap(KNOWN_PATHS_FN, m_MapKnownPaths);
}

/*
 * Return number of seconds that last solution took.
 */
double Cube::GetSolvedSecs()
{
	return m_fSolvedSecs;
}

/*
 * Cube engine.
 * Return solution length (call after GetSolutionPath).
 */
int Cube::GetSolLen()
{
	return m_nSolLen;
}

/*
 * Cube engine. T45.
 * Convert edges orientation state (facelet model) to integer table (cubie model).
 */
int *Cube::ConvertCubeEdges2OrientTbl(RCube *p)
{
	static int eo[NUM_OF_EDGES];
	int lsc[] = {U3, U5, D3, D5, F1, F3, F5, F7, B1, B3, B5, B7};
	
	for (int i=0; i<NUM_OF_EDGES; i++) {
		eo[i] = 0;	// start with wrong orientations
	}
	//  set edges that are in any of the correct orientations
	for (unsigned int i=0; i<sizeof(lsc)/sizeof(lsc[0]); i++) {
		if (p->up[1][0] == lsc[i]) {		// E1
			eo[0] = 1;
		}
		if (p->down[1][0] == lsc[i]) {		// E2
			eo[1] = 1;
		}
		if (p->up[1][2] == lsc[i]) {		// E3
			eo[2] = 1;
		}
		if (p->down[1][2] == lsc[i]) {		// E4
			eo[3] = 1;
		}
		if (p->bottom[1][2] == lsc[i]) {	// E5
			eo[4] = 1;
		}
		if (p->front[1][0] == lsc[i]) {		// E6
			eo[5] = 1;
		}
		if (p->front[1][2] == lsc[i]) {		// E7
			eo[6] = 1;
		}
		if (p->bottom[1][0] == lsc[i]) {	// E8
			eo[7] = 1;
		}
		if (p->front[0][1] == lsc[i]) {		// E9
			eo[8] = 1;
		}
		if (p->front[2][1] == lsc[i]) {		// E10
			eo[9] = 1;
		}
		if (p->bottom[2][1] == lsc[i]) {	// E11
			eo[10] = 1;
		}		
		if (p->bottom[0][1] == lsc[i]) {	// E12
			eo[11] = 1;
		}
	}
	
	// sanity check - cube law #2
	int sum = 0;
	for (int i=0; i<NUM_OF_EDGES; i++) {
		sum += eo[i];
	}
	if (0 != (sum%2))
		throw Exception("Cube::ConvertCubeEdges2OrientTbl - algorithm error: cube law #2 failed!");
	
	
	return eo;
}

/*
 * Cube engine. T45.
 * Convert edges permutation states (facelet model) to integer table (cubie model).
 */
int *Cube::ConvertCubeEdges2PermTbl(RCube *p)
{
	static int ep[NUM_OF_EDGES];
	int ef[] = {L1, U3, L7, D3, R1, U5, R7, D5, L3, B5, L5, F3, R3, F5, R5, B3, F1, U7, F7, D1, B7, D7, B1, U1};
	int ec[] = {1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9,  10, 10, 11, 11, 12, 12};
	for (int i=0; i<NUM_OF_EDGES; i++) {
		ep[i] = 0;
	}
	for (unsigned int i=0; i<sizeof(ef)/sizeof(ef[0]); i++) {
		if (p->left[0][1] == ef[i]) {	// E1
			ep[0] = ec[i];
		}
		if (p->left[2][1] == ef[i]) {	// E2
			ep[1] = ec[i];
		}
		if (p->right[0][1] == ef[i]) {	// E3
			ep[2] = ec[i];
		}
		if (p->right[2][1] == ef[i]) {	// E4
			ep[3] = ec[i];
		}		
		if (p->left[1][0] == ef[i]) {	// E5
			ep[4] = ec[i];
		}
		if (p->left[1][2] == ef[i]) {	// E6
			ep[5] = ec[i];
		}
		if (p->right[1][0] == ef[i]) {	// E7
			ep[6] = ec[i];
		}
		if (p->right[1][2] == ef[i]) {	// E8
			ep[7] = ec[i];
		}
		if (p->front[0][1] == ef[i]) {	// E9
			ep[8] = ec[i];
		}
		if (p->front[2][1] == ef[i]) {	// E10
			ep[9] = ec[i];
		}
		if (p->bottom[2][1] == ef[i]) {	// E11
			ep[10] = ec[i];
		}
		if (p->bottom[0][1] == ef[i]) {	// E12
			ep[11] = ec[i];
		}		
	}
	
	return ep;
}

/*
 * Cube engine. T45.
 * Convert corners orientation state (facelet model) to integer table (cubie model).
 */
int *Cube::ConvertCubeCorners2OrientTbl(RCube *p)
{
	static int co[NUM_OF_CORNERS];
	// correctly oriented no-twist left and right face cubies
	int ntol[] = {L0, L2, L6, L8, R8, R2, R0, R6};
	int ntor[] = {R0, R2, R6, R8, L6, L0, L8, L2};
	// left and right face cubies twisted clock-wise
	int rtol[] = {U2, D2, F2, B6, D6, F6, U6, B2};
	int rtor[] = {D6, U6, B2, F6, F2, B6, U2, D2};

	for (int i=0; i<NUM_OF_CORNERS; i++) {
		co[i] = -1; // start with undefined orientations, can be used later for sanity check
						// since the allowed orientations are 0, 1 and 2
	}
	//  set the state of left and right face corners that are in the correct not-twisted orientations (0)
	// or in the clock-wise twisted position (1)
	for (unsigned int i=0; i<sizeof(ntol)/sizeof(ntol[0]); i++) {
		if (p->left[0][0] == ntol[i]) {	// C1
			co[0] = 0;
		} else if (p->left[0][0] == rtol[i]) {
			co[0] = 1;
		}
		if (p->left[0][2] == ntol[i]) {	// C5
			co[4] = 0;
		} else if (p->left[0][2] == rtol[i]) {
			co[4] = 1;
		}
		if (p->left[2][2] == ntol[i]) {	// C2
			co[1] = 0;
		} else if (p->left[2][2] == rtol[i]) {
			co[1] = 1;
		}
		if (p->left[2][0] == ntol[i]) {	// C6
			co[5] = 0;
		} else if (p->left[2][0] == rtol[i]) {
			co[5] = 1;
		}
		if (p->right[0][0] == ntor[i]) {	// C4
			co[3] = 0;
		} else if (p->right[0][0] == rtor[i]) {
			co[3] = 1;
		}
		if (p->right[0][2] == ntor[i]) {	// C8
			co[7] = 0;
		} else if (p->right[0][2] == rtor[i]) {
			co[7] = 1;
		}
		if (p->right[2][2] == ntor[i]) {	// C3
			co[2] = 0;
		} else if (p->right[2][2] == rtor[i]) {
			co[2] = 1;
		}
		if (p->right[2][0] == ntor[i]) {	// C7
			co[6] = 0;
		} else if (p->right[2][0] == rtor[i]) {
			co[6] = 1;
		}
	}
	for (int i=0; i<NUM_OF_CORNERS; i++) {
		if (-1 == co[i]) co[i] = 2;
	}

	// sanity check - cube law #3
	int v=3;
	int s=0;
	for (int i=0; i<NUM_OF_CORNERS; i++) {
		s += co[i];
	}
	if (0 != (s%v)) {
		cout << "Cube::ConvertCubeCorners2OrientTbl - s=" << s << ", v=" << v << ", s%v=" << (s%v) << endl;
		throw Exception("Cube::ConvertCubeCorners2OrientTbl - algorithm error: cube law #3 failed!");	
	}

	return co;
}

/*
 * Cube engine. T45.
 * Convert corners permutation state (facelet model) to integer table (cubie model).
 */
int *Cube::ConvertCubeCorners2PermTbl(RCube *p)
{
	static int cp[NUM_OF_CORNERS];
	int cf[] = {L0, U0, B2, L8, D0, F6, R8, B6, D8, F2, R0, U8, L2, U6, F0, L6, D6, B8, F8, R6, D2, R2, B0, U2};
	int cc[] = { 1,  1,  1,  2,  2,  2,  3,  3,  3,  4,  4,  4,  5,  5,  5,  6,  6,  6,  7,  7,  7,  8,  8,  8};
	for (int i=0; i<NUM_OF_CORNERS; i++) {
		cp[i] = 0;
	}
	for (unsigned int i=0; i<sizeof(cf)/sizeof(cf[0]); i++) {
		if (p->left[0][0] == cf[i]) {	// C1
			cp[0] = cc[i];
		}
		if (p->left[2][2] == cf[i]) {	// C2
			cp[1] = cc[i];
		}
		if (p->right[2][2] == cf[i]) {	// C3
			cp[2] = cc[i];
		}
		if (p->right[0][0] == cf[i]) {	// C4
			cp[3] = cc[i];
		}
		if (p->left[0][2] == cf[i]) {	// C5
			cp[4] = cc[i];
		}
		if (p->left[2][0] == cf[i]) {	// C6
			cp[5] = cc[i];
		}
		if (p->right[2][0] == cf[i]) {	// C7
			cp[6] = cc[i];
		}
		if (p->right[0][2] == cf[i]) {	// C8
			cp[7] = cc[i];
		}
	}
	
	return cp;	
}

/*
 * Cube engine. T45.
 * Convert facelet cube model to cubie model.
 * Result in m_anEdgePerm, m_anEdgeOrient, m_anCornPerm, m_anCornOrient.
 */
void Cube::Convert2CubieModel(RCube *p)
{
	EOCopy(ConvertCubeEdges2OrientTbl(p), m_anEdgeOrient);
	EPCopy(ConvertCubeEdges2PermTbl(p), m_anEdgePerm);
	COCopy(ConvertCubeCorners2OrientTbl(p), m_anCornOrient);
	CPCopy(ConvertCubeCorners2PermTbl(p), m_anCornPerm);
#if defined(MYDBG18)	
	cout << "MYDBG18: Cube::Convert2CubieModel - converted cubie model:" << endl;
	CubieModelShowAllStates(m_anEdgePerm, m_anCornPerm, m_anEdgeOrient, m_anCornOrient);
#endif	
}

/*
 * Cube engine.
 * Return structure containing a copy of current cubie model state.
 */
CubieModelState Cube::GetCubieModelState()
{
	CubieModelState ret;
	
	EOCopy(m_anEdgeOrient, ret.eo);
	EPCopy(m_anEdgePerm, ret.ep);
	COCopy(m_anCornOrient, ret.co);
	CPCopy(m_anCornPerm, ret.cp);
	
	return ret;
}

} // namespace Rubik
