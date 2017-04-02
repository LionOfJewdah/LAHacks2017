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

string gsVersion = "Rubik Solver 3.1.7 (C) 2015 by Marek Karcz. All rights reserved.";

// example unsolved cube
int unsolved_cube[] = {
	U0, L1, R6,
	L5, U4, U5,
	L8, B5, F0,
	            
	L0, F3, D0,   
	U1, L4, R7,   
	L6, F7, R0, 
	  
	U8, B3, B0,
	D1, D4, D3,
	D6, B7, D8,
	            
	F6, L3, U6,
	D5, F4, U7,
	F2, R5, R2,
	            
	L2, R1, R6,
	F1, R4, R3,
	U2, L7, R8,
	            
	F8, U3, B2,
	F5, B4, B1,
	B6, D7, B8
};

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
 * Return true if both cubie model states are identical.
 */
bool CubieModelsEqual(CubieModelState cms1, CubieModelState cms2)
{
	bool ret = true;
	
	for (int i=0; ret && i<NUM_OF_EDGES; i++) {
		if (cms1.ep[i] != cms2.ep[i]) {
			ret = false;
		}
		if (ret && cms1.eo[i] != cms2.eo[i]) {
			ret = false;
		}
	}
	for (int i=0; ret && i<NUM_OF_CORNERS; i++) {
		if (cms1.cp[i] != cms2.cp[i]) {
			ret = false;
		}
		if (ret && cms1.co[i] != cms2.co[i]) {
			ret = false;
		}
	}
	
	return ret;
}

/*
 * Run the test case.
 */
void TestCase(struct CubeParams *params, int test_steps, int cube_scr_steps)
{
    MemMgm memMgr;
	CubieModelState cms1, cms2;
	// Save the original state of the cube.
	//RCube *pTmpCube = new RCube(*(params->start));
    RCube *pTmpCube = memMgr.GetNewRCube(params->start);
	if (NULL == pTmpCube)
		throw Exception("TestCase: NULL pointer! Memory allocation problem.");
	pTmpCube->NullParent();
	params->pRubikCube->ClearHistory();
	cout << "Run test case." << endl;
	double solved_secs = 0.0, longest_time = 0.0;
	int n = test_steps;
	unsigned int longest = 0, shortest = 1000, avg_steps = 0, num_failed=0;
	enum err_type {
		ErrTypeMemAlloc = 0,
		ErrTypeCubeConv,
		ErrTypeWrongSol,
		ErrTypeUnsolved,
		ErrTypeTooLong
	};
	int err_tbl[5] = {0, 0, 0, 0, 0};	// holds the number of errors for 4 types of problems
	bool success = true;
	string steps;
	RCube *pstrt = NULL;
	vector<int> v_err_steps;	// vector to remember failed step numbers
	vector<CubieModelState> v_err_cubes;	// vector to remember failed cubes
	cout << "Test begins..." << endl;
	while (0 < n--) {
		cout << "Test #" << test_steps-n << "/" << test_steps << " START, memory: " << params->pRubikCube->PrivateMemoryUsed() << "." << endl;
		success = true;
		params->pRubikCube->ClearHistory();
		RCube *p = params->pRubikCube->ScrambleStatic((cube_scr_steps == 0 ? params->pRubikCube->RandomInt(20,99) : cube_scr_steps), params->show);
		cms1 = params->pRubikCube->GetCubieModelState();
        pstrt = memMgr.GetNewRCube(p);
		if (steps.size()) steps.clear();
		if (params->SolveCube()) {
			solved_secs += params->solved_secs;
			if (params->solved_secs > longest_time) {
				longest_time = params->solved_secs;
			}
			steps.clear();
			steps = params->sol_path;
			unsigned int sol_len = params->sol_len;
			if (NULL != pstrt)
				pstrt = params->pRubikCube->SetCube(pstrt);
			if (NULL != pstrt) {
				cms2 = params->pRubikCube->GetCubieModelState();
				if (false == CubieModelsEqual(cms1, cms2)) {
					success = false;
					cout << "Test #" << test_steps-n << " ERROR: Facelet to cubie model conversion problem." << endl;
					err_tbl[ErrTypeCubeConv]++;
					num_failed++;
					cout << "State #1 (generated during scramble):" << endl;
					CubieModelShowAllStates(cms1.ep, cms1.cp, cms1.eo, cms1.co);
					cout << "State #2 (converted):" << endl;
					CubieModelShowAllStates(cms2.ep, cms2.cp, cms2.eo, cms2.co);					
				}
				if (success) {
					if (45 < sol_len) err_tbl[ErrTypeTooLong]++;
					params->pRubikCube->DoMoves(steps);
					if (params->pRubikCube->IsSolvedState()) {
						cout << "===============================================================================" << endl;
						cout << "Test #" << test_steps-n << ", solution length: " << sol_len << ", memory: " << params->pRubikCube->PrivateMemoryUsed() << " - OK" << endl;
						cout << "Test #" << test_steps-n << ", Cube: " << params->pRubikCube->GetColorCubeDefNoSpaces(pstrt) << endl;
						cout << "Test #" << test_steps-n << ", Solution: " << steps << endl;
						cout << "===============================================================================" << endl;
					}						
					else {
						success = false;
						cout << "Test #" << test_steps-n << ", ERROR: Invalid solution." << endl;
						cout << "Test #" << test_steps-n << ", Solution: " << steps << endl;
						err_tbl[ErrTypeWrongSol]++;
						num_failed++;
					}
					if (success) {
						if (sol_len > longest)
							longest = sol_len;
						if (sol_len < shortest)
							shortest = sol_len;
						avg_steps += sol_len;
						params->pRubikCube->DeleteCube(pstrt);
					}
				}
			} else {
				success = false;
				cout << "Test #" << test_steps-n << " ERROR: Memory allocation problem." << endl;
				err_tbl[ErrTypeMemAlloc]++;
				num_failed++;				
			}
		}
		else {
			success = false;
			num_failed++;
			err_tbl[ErrTypeUnsolved]++;
			cout << "Test #" << test_steps-n << " ERROR: Cube unsolved." << endl;
		}
		if (false == success) {
			v_err_steps.push_back(test_steps-n);
			v_err_cubes.push_back(cms1);
			cout << "Cube that failed:" << endl;
			if (NULL != pstrt) {
				params->pRubikCube->Print(*pstrt);
				if (params->show_input_notation) {
					cout << params->pRubikCube->ConvertCube2InputString(pstrt) << endl;
				}
				if (params->show_color_notation) {
					cout << params->pRubikCube->ConvertCube2ColorsString(pstrt) << endl;
					cout << params->pRubikCube->GetColorCubeDefNoSpaces(pstrt) << endl;
				}
				params->pRubikCube->DeleteCube(pstrt);
			} else {
				cout << "ERROR: Unable to show cube with problem - memory depleted!" << endl;
			}
		}
	}
	cout << "===============================================================================" << endl;
	if (0 == num_failed) {
		cout << "TCS: Test succeeded." << endl;
	}
	else {
		cout << "ERROR: Test failed!" << endl;
		cout << "ERROR: Errors encountered with their frequency:" << endl;
		cout << "ERROR:   Memory allocation : " << err_tbl[ErrTypeMemAlloc] << endl;
		cout << "ERROR:   Facelet conversion: " << err_tbl[ErrTypeCubeConv] << endl;
		cout << "ERROR:   Invalid solution  : " << err_tbl[ErrTypeWrongSol] << endl;
		cout << "ERROR:   Unable to solve   : " << err_tbl[ErrTypeUnsolved] << endl;
		cout << "ERROR:   Total # of errors : " << num_failed << " out of " << test_steps << " tests performed." << endl;
		
		cout << endl << "Failed steps numbers:" << endl;
		for (vector<int>::iterator it = v_err_steps.begin(); it != v_err_steps.end(); ++it) {
			cout << *it << ", ";
		}
		cout << "END." << endl;
		cout << endl << "Failed cubes:" << endl;
		for (vector<CubieModelState>::iterator it = v_err_cubes.begin(); it != v_err_cubes.end(); ++it) {
			CubieModelShowAllStates((*it).ep, (*it).cp, (*it).eo, (*it).co);
			cout << "----------------------------------------" << endl;
		}		
	}
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<< Summary of the test case >>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
	cout << "TCS: Total # of steps  : " << test_steps << endl;
	cout << "TCS: Longest time      : " << longest_time << endl;
	cout << "TCS: Average time      : " << (double)(solved_secs/test_steps) << " seconds." << endl;
	cout << "TCS: Average length    : " << (double)avg_steps/test_steps << " steps." << endl;
	cout << "TCS: Longest solution  : " << longest << " steps." << endl;
	cout << "TCS: Shortest solution : " << shortest << " steps." << endl;
	cout << "TCS: # of 45+ solutions: " << err_tbl[ErrTypeTooLong] << endl;
	cout << "NOTE: Above number greater than 0 is only a problem when testing T45 algorithm." << endl;
	cout << "===============================================================================" << endl;
	// Restore original state of the cube.
	params->SetCube(pTmpCube);
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
 * Allow manual play with the cube engine.
 */
void Interactive (struct CubeParams *params)
{
	int scramble_moves = params->scramble_steps;
	int solve_method = params->method;
	int search_depth = params->depth;
	string cmd;

	cout << endl;
	InteractiveModeHelp();
	cout << endl;
	
	if (params->runmoves)
	{
		cout << "Executing predefined moves sequence..." << endl;
		params->start = params->pRubikCube->DoMoves(params->execseq);
	}
	
	while(true)
	{
		cout << endl << "Current state:" << endl << endl;
		params->pRubikCube->Print();
		params->ShowInputNotation();
		params->ShowColorNotation();
		params->ShowCubieNotation();
		if (params->pRubikCube->IsSolvedState())
		{
			//params->pRubikCube->IsSolved();
			// switch to Tree Search method to ensure
			// proper history printout. Cube class uses different
			// approach to linking cube states when back-trace
			// searching and the PrintHistory() method would not
			// output correctly the history in interactive mode
			// and back-tracing search algorithm.
			params->pRubikCube->SetMethod(TreeSearch);			
			cout << "Cube solved!" << endl;
			params->ShowHistory();
			params->pRubikCube->SetMethod(params->method);
			//break;
		}				
		cout << "> ";
		cin >> cmd;
		if (0 == cmd.compare(0, 1, "q", 1))
			break;
		else if (0 == cmd.compare(0, 2, "he", 2))
		{
	    cout << endl;
	    InteractiveModeHelp();
	  	cout << endl;
		}			
		else if (0 == cmd.compare(0, 3, "scr", 3))
		{
			char cshow = 'n';
			cout << "   # of moves? : ";
			cin >> scramble_moves;
			cout << "   Show steps? (y/n) : ";
			cin >> cshow;
			params->start = params->pRubikCube->Scramble(scramble_moves, ('y' == cshow));
		}
		else if (0 == cmd.compare(0, 3, "met", 3))
		{
			cout << "Current method: " << params->pRubikCube->GetMethodName() << endl;
			cout << "Available methods:" << endl;
			vector<string> methods = params->pRubikCube->GetSolveMethodsNames();
			for (int i=0; i<MAX_METHODS; i++)
			{
				cout << "   " << methods[i] << endl;
			}
			cout << "Selection: ";
			cin >> solve_method;
			while (0 > solve_method || MAX_METHODS <= solve_method)
			{
				cout << "ERROR: Invalid method selection." << endl;
				cout << "Selection: ";
				cin >> solve_method;				
			}
			params->SetMethod(solve_method);
			cout << "Method set to: " << params->pRubikCube->GetMethodName() << endl;
		}
		else if (0 == cmd.compare(0, 3, "dep", 3))
		{
			cout << "Current search depth: " << search_depth << endl;
			cout << "New search depth: ";
			cin >> search_depth;
			while (2 > search_depth)
			{
				cout << "ERROR: search depth must be greater than 1." << endl;
				cout << "New search depth: ";
				cin >> search_depth;				
			}
			params->SetDepth(search_depth);
		}
		else if (0 == cmd.compare(0, 3, "sol", 3))
		{
			int ask;
			bool mem_show = params->show;
			bool show = false;
			cout << "   Be verbose (1) or be quiet (2): ";
			cin >> ask;
			while (ask != 1 && ask != 2)
			{
				cout << "ERROR: Invalid selection." << endl;
				cout << "   Be verbose (1) or be quiet (2): ";
				cin >> ask;				
			}
			if (ask == 1)
				show = true;
			params->SetVerbose(show);
			RunSolveCube(params, show);
			params->SetVerbose(mem_show);
		}
		else if (0 == cmd.compare(0, 3, "his", 3))
		{
			// switch temporarily to Tree Search method to ensure
			// proper history printout. Cube class uses different
			// approach to linking cube states when back-trace
			// searching and the PrintHistory() method would not
			// output correctly the history in interactive mode
			// and back-tracing search algorithm.
			params->pRubikCube->SetMethod(TreeSearch);
			cout << endl << "History of moves:" << endl << endl;
			params->pRubikCube->PrintHistory();
			params->pRubikCube->SetMethod(solve_method);
		}
		else if (0 == cmd.compare(0, 2, "te", 2))
		{
			int test_steps, cube_scr_steps;
			cout << "   # of steps? : ";
			cin >> test_steps;
			while (0 >= test_steps) {
				cout << "ERR: must be greater than 0." << endl;
				cout << "   # of steps? : ";
				cin >> test_steps;				
			}
			cout << "   # of cube scramble steps (0 - random)? : ";
			cin >> cube_scr_steps;
			while (0 > cube_scr_steps) {
				cout << "ERR: must be equal or greater than 0." << endl;
				cout << "   # of cube scramble steps (0 - random)? : ";
				cin >> cube_scr_steps;				
			}
			TestCase(params, test_steps, cube_scr_steps);
		}
		else if (0 == cmd.compare(0, 3, "mem", 3))
		{
			params->pRubikCube->ShowMemUse();
		}
		else if (0 == cmd.compare(0, 3, "res", 3))
		{
			params->pRubikCube->ResetMemMgm();
		}		
		else // perform ordered moves
		{
			params->start = params->pRubikCube->DoMoves(cmd);
		}
	}	
}


/*
 * Serves the cube engine over socket (client->server).
 */
void Server (struct CubeParams *params)
{
#if defined(UNIX)
     int sockfd, newsockfd, portno=params->socket_port;
     socklen_t clilen;
     char buffer[1024];
     struct sockaddr_in serv_addr, cli_addr;
     int n;


	 sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 if (sockfd < 0) 
		throw Exception("Server: ERROR opening socket!");
	 bzero((char *) &serv_addr, sizeof(serv_addr));
	 serv_addr.sin_family = AF_INET;
	 serv_addr.sin_addr.s_addr = INADDR_ANY;
	 serv_addr.sin_port = htons(portno);
	 if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
	 	throw Exception("Server: ERROR on binding!");
     cout << "Server: online, listening on port " << portno << "." << endl;
     while (1) {
		 listen(sockfd,5);
		 clilen = sizeof(cli_addr);
		 newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		 if (newsockfd < 0) 
		 	throw Exception("Server: ERROR on accept!");
		 bzero(buffer,1024);
		 n = read(newsockfd,buffer,1024);
		 if (n < 0) throw Exception("Server: ERROR reading from socket");
		 //printf("Here is the message: %s\n",buffer);
         if (0 == strcmp(buffer, "end")) {
            cout << "Server: Received END command. Good bye!" << endl;
		 	n = write(newsockfd,"BYE!",4);
		    close(newsockfd);
            break;
         } else if (0 == strcmp(buffer, "reset")) {
         	cout << "Server: Received MEMORY RESET command." << endl;
         	params->pRubikCube->ResetMemMgm();
         	n = write(newsockfd,"OK",2);
         } else if (0 == strcmp(buffer, "alg_lbl")) {
         	cout << "Server: Received SET ALGORITHM TO LAYER-BY-LAYER command." << endl;
         	params->SetMethod(MultiStepHuman);
         	n = write(newsockfd,"M:LBL-OK",8);
         } else if (0 == strcmp(buffer, "alg_t45")) {
         	cout << "Server: Received SET ALGORITHM TO T45 command." << endl;
         	params->SetMethod(TreeSearchLookup);
         	n = write(newsockfd,"M:T45-OK",8);
		 } else {
	         cout << "Server: Received cube definition: " << buffer << endl;
			 cout << "Server: Processing..." << endl;
	   		 vector<string> vcubedef;
	   		 for (unsigned int i=0; i<strlen(buffer) && isupper(buffer[i]); i++) {
	      		string face = string(1,buffer[i]);
	      		vcubedef.push_back(face);
	            //cout << "Added face: [" << face << "]" << endl;
	   		 }
			 RCube *pCube = params->pRubikCube->CreateCubeFromColorStrVectDef(vcubedef);
	         if (NULL != pCube) {
	 		    cout << "Server: Cube definition accepted." << endl;
			 } else {
				throw Exception("Server: ERROR processing cube definition!");
			 }
             MemMgm memMgr;
	         //RCube *p = new RCube(*pCube);
			 RCube *p = memMgr.GetNewRCube(pCube);
			 if (NULL == p)			  {
			 	throw Exception("Server: ERROR - memory allocation failed!");
			 }
		     params->pRubikCube->ClearHistory();
			 params->SetCube(p);
			 //delete p;
	         cout << "Server: Solving cube..." << endl;
		     if (params->SolveCube()) {
	            string sol_path = params->pRubikCube->GetSolutionPath();
	            cout << "Server: Cube solved! Path: " << sol_path << endl;
	            cout << "Server: Sending solution path to client..." << endl;
			    n = write(newsockfd,sol_path.c_str(),sol_path.length());
			 } else {
	            cout << "Server: ERROR! Cube unsolved." << endl;
			 	n = write(newsockfd,"ERROR!",6);
	         }
		 	params->pRubikCube->DeleteCube(pCube);	         
			 if (n < 0) throw Exception("Server: ERROR writing to socket!");
		 }
		 close(newsockfd);
     }
	 close(sockfd);
#else
   cout << "Sorry! Server mode is not available in this version." << endl;
#endif
}

/*
 * Generate unique set of random cubes.
 */
void GetRandCubes (struct CubeParams *params) 
{
	map<int,string> MapCubes;
	int rand_steps = params->pRubikCube->RandomInt(params->scramble_steps, params->scramble_steps*2);
	for (int i=0; i<params->numrandcubes; /*empty*/) {
		RCube *p = params->pRubikCube->ScrambleStatic(rand_steps, false);
		string colcube_ns = params->pRubikCube->GetColorCubeDefNoSpaces(p);
		pair<map<int,string>::iterator,bool> keyfound;
		keyfound = MapCubes.insert(pair<int,string>(i,colcube_ns));
		if (keyfound.second) {
			i++;
			cout << colcube_ns << endl;
		}
		usleep(100);
	}
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
	
	cout << gsVersion << endl;
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
