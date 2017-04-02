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
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <malloc.h>
#include "Cube.h"
#include "Exception.h"
#include "CubieModel.h"
#include "MemMgm.h"

using namespace Rubik;
using namespace std;

struct CubeParams {

	bool 	manual_mode;
	RCube 	*start;
	Cube 	*pRubikCube;
	int		method;
	int 	depth;
	int 	scramble_steps;
	bool 	show;
	bool 	bw;
	bool 	caching;
	bool 	runmoves;
	string 	execseq;
	bool 	show_input_notation;
	bool 	show_color_notation;
	bool	show_cubie_notation;
	double 	solved_secs;
	string 	sol_path;
	int 	sol_len;
	bool 	runtest;
	int 	test_steps;
    bool 	servermode;
    int 	socket_port;
    bool	getrandcubes;
    int		numrandcubes;
	
	CubeParams()
	{
		numrandcubes = 100;
		getrandcubes = false;
		socket_port = SOCKET_PORT;
        servermode = false;
		runtest = false;
		test_steps = 100;
		sol_len = 0;
		sol_path = string();
		solved_secs = 0.0;
		show_input_notation = false;
		show_color_notation = false;
		show_cubie_notation = false;
		runmoves = false;
		execseq = string();
		caching = true;
		bw = false;
		manual_mode = false;
		start = NULL;
		pRubikCube = new Cube(unsolved_cube, false);
		method = 0;
		depth = 6;
		scramble_steps = depth-1;
		show = false;
	}
	
	~CubeParams()
	{
		if (NULL != pRubikCube)
			delete pRubikCube;
	}
	
	void SetCube()
	{
		pRubikCube->SetMethod(method);
		pRubikCube->SetColorMode(bw ? false : true);
		pRubikCube->UpdateCache(caching);
   		if (NULL == start)
   			start = pRubikCube->Scramble(scramble_steps, show);
		else
			start = pRubikCube->SetCube(start);
		}
	
	void SetCube(RCube *pCube)
	{
		pRubikCube->SetMethod(method);
		pRubikCube->SetColorMode(bw ? false : true);
		pRubikCube->UpdateCache(caching);
		if (NULL != pCube)
		{
			start = pCube;
			start = pRubikCube->SetCube(start);
		}
		else
			throw Exception("CubeParams::SetCube - NULL pointer!");
	}	
	
	void SetMethod(int alg)
	{
		method = alg;
		pRubikCube->SetMethod(method);
	}
	
	void SetDepth(int search_depth)
	{
		depth = search_depth;
	}
	
	void SetVerbose(bool verbose)
	{
		show = verbose;
	}
	
	bool SolveCube()
	{
		bool ret = pRubikCube->Solve(depth, show);
		
		if (ret) {
			solved_secs = pRubikCube->GetSolvedSecs();
			sol_path.clear();
			sol_path = pRubikCube->GetSolutionPath();
			sol_len = pRubikCube->GetSolLen();
		}
		
		return ret;
	}
	
	void ShowSolution(bool verbose)
	{
		if (verbose)
			pRubikCube->PrintSolution();
		else
			pRubikCube->PrintSolutionPath();
	}
	
	void ShowSolution()
	{
		ShowSolution(show);
	}
	
	void ShowHistory(bool verbose)
	{
		if (verbose)
			pRubikCube->PrintHistory();
		else
			pRubikCube->PrintHistoryPath();
	}
	
	void ShowHistory()
	{
		ShowHistory(show);
	}
	
	void ShowInputNotation()
	{
		if (show_input_notation)
			cout << endl << "Input notation:" << pRubikCube->ConvertCube2InputString(NULL) << endl;
	}
	
	void ShowColorNotation()
	{
		if (show_color_notation)
			cout << endl << "Color notation:" << pRubikCube->ConvertCube2ColorsString(NULL) << endl;		
	}
	
	void ShowCubieNotation()
	{
		if (show_cubie_notation) {
			cout << endl << "Cubie notation:" << endl;
			CubieModelState cubie_state;
			cubie_state = pRubikCube->GetCubieModelState();
			CubieModelShowAllStates(cubie_state.ep, cubie_state.cp, cubie_state.eo, cubie_state.co);
			cout << endl;
		}
	}

	void RunMoves()
	{
		if (runmoves)
		{
			cout << "Executing predefined moves sequence..." << endl;
			start = pRubikCube->DoMoves(execseq);
		}		
	}
};

/*
 * Generate a vector of a cube definition out of command line definition.
 */
vector<string> ParseCubeDef(int argc, int start_arg, char **argv)
{
   vector<string> vcubedef;

   for (int i=start_arg; i<start_arg+6*9 && i<argc; i++)
   {
      string face = string(argv[i]);
      vcubedef.push_back(face);
   }
   
   return vcubedef;
}

/*
 * Usage information for the app.
 */
void InteractiveModeHelp()
{
	cout << "Commands:" << endl;
	cout << "   (scr)amble, (met)hod, (dep)th, (sol)ve, (his)tory, (he)lp, (te)st, (mem)ory," << endl; 
	cout << "   (res)et memory, (q)uit" << endl;
	cout << "Moves:" << endl;
	cout << "   F, Fi, Li, L, Ui, U, R, Ri, D, Di, Bi, B, F2, L2, U2, R2, D2, B2" << endl;  
}

/*
 * Run cube solving algorithm.
 */
void RunSolveCube(struct CubeParams *params, bool show)
{
    MemMgm memMgr;
	cout << "Please be very patient..." << endl;
	// Save the original state of the cube.
	//RCube *pTmpCube = new RCube(*(params->start));	
	RCube *pTmpCube = memMgr.GetNewRCube(params->start);
	// Reset the history as we now use algorithm
	// with the current state of the cube as a starting
	// state.
	if (NULL == pTmpCube)
		throw Exception("RunSolveCube - memory allocation problem!");
	pTmpCube->NullParent();		
	params->pRubikCube->ClearHistory();
	bool solved = params->SolveCube();
	if (solved)
	{
		cout << "Cube solved!" << endl;
		params->ShowSolution(show);
		// Restore original state of the cube.
		params->SetCube(pTmpCube);
	}
	else {
		cout << "Cube not solved." << endl;
		if (show) {
			params->ShowHistory(show);
		}		
	}
	// We don't want the app. to end after cube is solved
	// with 'sol' command, becasue we want to preserve the
	// original state of the cube and try the solution
	// found by the algorithm ourselves in interactive mode.
	// Therefore we clear the history again.
	params->pRubikCube->ClearHistory();
	//delete pTmpCube;
}

/*
 * Display usage information.
 */
void Help()
{
	Cube *pcube = new Cube(2,false);
	cout << "Usage:" << endl << endl;
	cout << "   rubik [-m] [-a method] [-d search_depth] [-s scramble_steps] [-v] [-D]" << endl;
	cout << "         [-i CUBE_STATE_DEF] [-w scramble_moves] [-r run_moves] [-b] [-c]" << endl;
	cout << "         [-e CUBE_COL_DEF] [-N] [-C] [-t num_of_steps] [-S port#] [-g num]" << endl;
	cout << "Where:" << endl << endl;
	cout << "   -m             - interactive mode" << endl;
	cout << "   method         - choice of algorithm:" << endl;
	vector<string> vmn = pcube->GetSolveMethodsNames();
	for (vector<string>::iterator it = vmn.begin(); it != vmn.end(); ++it) {
		cout << "                    " << *it << endl;
	}
	cout << "   search_depth   - recursive depth of the solution search" << endl;
	cout << "   scramble_steps - # of random steps to scramble the cube" << endl;
  	cout << "                    (default=search_depth-1)" << endl;
  	cout << "   scramble_moves - string representing fixed moves to scramble solved cube" << endl;
  	cout << "   run_moves      - string representing fixed moves to execute after cube was" << endl;
  	cout << "                    initialized or scrambled" << endl;
	cout << "   -v             - more verbose, note: slow" << endl;
  	cout << "   CUBE_STATE_DEF - a sequence of scrambled cube faces codes separated with" << endl;
	cout << "                    spaces in the following order:" << endl;
	cout << "                       U, L, F, R, B, D" << endl;
	cout << "                    where:" << endl;
	cout << "                       U,L,F,etc. - indicate the faces of the cube as visible" << endl;
	cout << "                                    from the front (F) side of the cube:" << endl;
	cout << "                                    U-upper, L-left, F-front, R-right, B-bottom" << endl;
  	cout << "                                    (opposite to F), D-down (or 'lower'," << endl;
  	cout << "                                    opposite to U)," << endl;
  	cout << "   CUBE_COL_DEF   - a sequence of scrambled cube face color codes separated" << endl;
  	cout << "                    with spaces in the same order as for CUBE_STATE_DEF." << endl;
  	cout << "                    In this case however user provides one-character color" << endl;
  	cout << "                    codes instead of unique face codes." << endl;
  	cout << "                    Cube color codes and their face designations as in" << endl;
  	cout << "                    the solved cube are:" << endl;
	cout << "                       U - upper face  - (W) white" << endl;
	cout << "                       F - front face  - (R) red" << endl;
	cout << "                       D - lower face  - (Y) yellow" << endl;
	cout << "                       L - left face   - (G) green" << endl;
	cout << "                       R - right face  - (B) blue" << endl;
	cout << "                       B - back face   - (P) purple" << endl;
  	cout << "   -b             - do not use colors on display (B/W)" << endl;
  	cout << "   -c             - disable known paths lookup cache" << endl;
  	cout << "   -N             - display input format of cube def. (like for -i option)" << endl;
  	cout << "   -C             - display color format of cube def. (like for -e option)" << endl;
  	cout << "   -D             - display cubie notation" << endl;
  	cout << "   -t             - run test case provided number of times" << endl;
    cout << "   -S             - run as a server" << endl;
    cout << "   -g             - get 'num' unique random cubes, the # of scramble steps" << endl;
    cout << "                    should be provided via '-s scramble_steps' - it will" << endl;
    cout << "                    result in scramble steps # to be a random number between" << endl;
	cout << "                    [scramble_num, scramble_num*2] - scramble_num is the" << endl;
	cout << "                    guaranteed minimum number of scramble steps to be perf." << endl;
	cout << endl;
	cout << "Examples:" << endl << endl;
	cout << "   rubik -a 0 -d 5 -s 5 -v" << endl;
	cout << "   * scrambles 5 times, then searches solution with depth=5, shows progress," << endl;
	cout << "     uses tree search" << endl;
	cout << "   rubik -a 1 -d 5 -s 5" << endl;
	cout << "   * scrambles 5 times, then searches solution with depth=5, shows no progress," << endl;
	cout << "     uses heuristic tree search" << endl;
	cout << "   rubik -a 2 -d 5" << endl;
	cout << "   * scrambles 5 times, then searches solution with depth=5, shows no progress," << endl;
	cout << "     uses back-tracing tree search" << endl;
	cout << "   rubik -a 2 -d 20 -i U0 L1 R6 L5 U4 U5 L8 B5 F0 L0 F3 D0 U1 L4 R7 L6 F7 R0 F6 L3 U6 D5 F4 U7 F2 R5 R2 L2 R1 R6 F1 R4 R3 U2 L7 R8 F8 U3 B2 F5 B4 B1 B6 D7 B8 U8 B3 B0 D1 D4 D3 D6 B7 D8" << endl;
	cout << "   * parses scrambled cube definition provided in command line, then searches" << endl;
	cout << "     solution with depth=20 using back-tracing tree search, shows no progress" << endl;
	cout << "   rubik -d 6 -a 3 -m -w URRFBRBBRUULBBRUiDiRRFRiLBBUUFF" << endl;
	cout << "   * scrambles the cube using provided steps after option -w, sets the depth" << endl;
	cout << "     to 6, algorithm type to 3 and enters the interactive mode," << endl;
	cout << "     the resulting scrambled cube in this case is in a state called super-flip" << endl;
	cout << "   rubik -a 3 -i B8 R3 D8 B1 U4 L7 D0 F3 R0 L6 U1 F6 R1 L4 F1 B0 D1 U6 L8 L5 U8 U7 F4 B3 F0 R7 D2 F2 D3 B6 R5 R4 D7 F8 L1 B2 R8 F5 D6 B7 B4 U5 U0 B5 U2 L2 D5 R6 F7 D4 U3 R2 L3 L0 -r UFLiUFLiFU2L2FLBiF2LiD2LB2D2LF2RL2F2L2D2L2F2U2F2R2B2L2D2L2D2R2F2" << endl;
	cout << "   * initializes the cube with state defined in arguments of -i switch, then" << endl;
	cout << "     executes moves to solve it defined as arguments of -r switch" << endl;
	cout << "   rubik -a 3 -e P B Y P W G Y R B G W R B G R P Y W G G W W R P R B Y R Y P B B Y R G P B R Y P P W W P W G Y B R Y W B G G -m -c" << endl;
	cout << "   * initializes the cube with colors state defined in arguments of -e switch" << endl;
	cout << "     and enters interactive mode with known solutions cache disabled" << endl;
	cout << endl;
	delete pcube;
}

/*
 * Parse command line arguments.
 */
void ParseArgs(int argc, char *argv[], struct CubeParams *pPar)
{
	for (int i=1; i<argc; i++)
	{
		if (0 == strcmp("-m", argv[i]))
			pPar->manual_mode = true;
		else if (0 == strcmp("-i", argv[i])) {
			vector<string> vcubedef = ParseCubeDef(argc, i+1, argv);
			pPar->start = pPar->pRubikCube->CreateCubeFromStrVectDef(vcubedef);
			i += 6*9;
		} else if (0 == strcmp("-a", argv[i])) {
			pPar->method = atoi(argv[++i]);
		} else if (0 == strcmp("-d", argv[i])) {
			pPar->depth = atoi(argv[++i]);
		} else if (0 == strcmp("-s", argv[i])) {
			pPar->scramble_steps = atoi(argv[++i]);
		} else if (0 == strcmp("-v", argv[i])) {
			pPar->show = true;
		} else if (0 == strcmp("-w", argv[i])) {
			pPar->start = pPar->pRubikCube->CreateSolvedCube();
			pPar->start = pPar->pRubikCube->DoMoves(pPar->start, argv[++i]);
			pPar->pRubikCube->ClearHistory();
		} else if (0 == strcmp("-b", argv[i])) {
			pPar->bw = true;
		} else if (0 == strcmp("-c", argv[i])) {
			pPar->caching = false;
		} else if (0 == strcmp("-r", argv[i])) {
			pPar->runmoves = true;
			pPar->execseq = string(argv[++i]);
		} else if (0 == strcmp("-e", argv[i])) {
			vector<string> vcubedef = ParseCubeDef(argc, i+1, argv);
			pPar->start = pPar->pRubikCube->CreateCubeFromColorStrVectDef(vcubedef);
			i += 6*9;
		} else if (0 == strcmp("-N", argv[i])) {
			pPar->show_input_notation = true;
		} else if (0 == strcmp("-C", argv[i])) {
			pPar->show_color_notation = true;
		} else if (0 == strcmp("-D", argv[i])) {
			pPar->show_cubie_notation = true;			
		} else if (0 == strcmp("-t", argv[i])) {
			pPar->runtest = true;
			pPar->test_steps = atoi(argv[++i]);
		} else if (0 == strcmp("-S", argv[i])) {
			pPar->servermode = true;
			pPar->socket_port = atoi(argv[++i]);
        } else if (0 == strcmp("-g", argv[i])) {
        	pPar->getrandcubes = true;
        	pPar->numrandcubes = atoi(argv[++i]);
		} else {
			Help();
			exit(2);
		}
	}
}

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	
	try {
		bool solved = false;
		
		if (argc == 1)
		{
			Help();
			exit(0); 
		}
		
		struct CubeParams Params;		
		
		ParseArgs(argc, argv, &Params);
		
		Params.SetCube();
	if (Params.getrandcubes) {
		GetRandCubes(&Params);
	} else if (Params.servermode) {
        Server(&Params);
    } else if (Params.manual_mode)	{
   		Interactive(&Params);
	} else {
			if (Params.runtest) {
				TestCase(&Params, Params.test_steps, Params.scramble_steps);
			} else {
				cout << "Solving cube:" << endl;
				Params.pRubikCube->Print();
				Params.ShowInputNotation();
				Params.ShowColorNotation();
				Params.ShowCubieNotation();
				Params.RunMoves();
				solved = Params.SolveCube();
				if (solved)
				{
					cout << "Cube solved!" << endl;
					Params.ShowSolution();
				}
				else
					cout << "Cube not solved." << endl;				
			}
	   }
	} catch (Exception& e) {
		cout << endl;
		e.Print();
	}
	
	return 0;
}
