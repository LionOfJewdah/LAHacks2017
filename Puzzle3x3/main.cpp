#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "puzlist.h"
#include "puzzle.h"


/* run this program using the console pauser or add your own getch, system("pause") or input loop */

/*
int main(int argc, char** argv) {
	return 0;
}
*/

void Usage (char *szPrgName)
{
	cout << "Usage:" << endl;
	cout << szPrgName << " -tbegin X X X X X X X X X -tend [-depth n] [-moves n]" << endl;
	cout << "   [-obegin Y Y Y Y -oend] [-thisclose n]" << endl;
	cout << "Where:" << endl;
	cout << "-tbegin ... -tend - the initial state to solve puzzle 3x3 given as a list of 9" << endl;
	cout << "                    tiles." << endl;
	cout << "X - the puzzle tile number 1..8 OR 0 for empty spot." << endl;
	cout << "-depth - the depth limit for recursive heuristic algorithm." << endl;
	cout << "-moves - the searching steps limit for recursive heuristic algorithm." << endl;
	cout << "-obegin ... -oend - the order of searching in the first step of recursive" << endl; 
	cout << "                    heuristic algorithm." << endl;
	cout << "Y - the integer number 0..3, where 0,1,2,3 stand for LEFT,UP,RIGHT,DOWN" << endl;
	cout << "    branches." << endl;
	cout << "-thisclose - the heuristic distance for which the current state should be" << endl;
	cout << "             dumped while searching." << endl;
	cout << endl;
	cout << "When only -tbegin ... -tend parameters are used, the linear tree searching" << endl;
	cout << "algorithm is used which generates all the states to the linear table" << endl;
	cout << "representing the tree starting from the root which is the goal state." << endl;
	cout << "Searching is done when initial state is generated and that is" << endl;
	cout << "guaranteed shortest puzzle solution unlike in the heuristic algorithm," << endl;
	cout << "which does not guarantee the solution found to be the shortest one." << endl;
	cout << endl;
	cout << "Puzzle 3x3 by Marek K'2002. Enjoy AI adventure." << endl;
}

/*
 * Scan command line arguments:
 * Common section (brute force & heuristics algorithms):
 * -tbegin - the beginning of the initial state
 * -tend - the end of the initial state
 * Heuristic recursive algorithm only:
 * -depth - the depth of the recursive heuristic algorithm
 * -moves - the number of moves limit for the recursive heuristic algorithm
 * -obegin - beginning of the search order table in the first step of heuristic algorithm
 * -oend - search order table end
 * -thisclose - the heuristic distance for which the program should dump the state to output
 */
int ScanArg (char **argv, 
			 Box *p, 
			 int *pnDepth, 
			 int *pnMoves, 
			 Order *pOrd, 
			 bool *pbUseHeur, 
			 int *pnThisClose,
			 bool *pbUseHeurAlg)
{
	int i = 1;
	while (argv[i] != NULL)
	{
		if (strcmp (argv[i], "-tbegin") == 0)
		{
			int x = 0, y = 0;
			i++;
			while (strcmp (argv[i], "-tend") != 0 && x < 3)
			{
				(*p)[x][y] = atoi (argv[i]);
				if ((*p)[x][y] < 0 || (*p)[x][y] > 8)
				{
					cerr << "Invalid tile number (must be 0..8, 0 - empty)." << endl;
					exit (-1);
				}
				y++;
				if (y == 3)
				{
					y = 0;
					x++;
				}
				i++;
			}
		}
		else if (strcmp (argv[i], "-depth") == 0)
		{
			*pbUseHeurAlg = true;
			i++;
			*pnDepth = atoi (argv[i]);
			if (*pnDepth < 1)
			{
				cerr << "Invalid depth limit value (must be >= 1)." << endl;
				exit (-2);
			}
		}
		else if (strcmp (argv[i], "-moves") == 0)
		{
			*pbUseHeurAlg = true;
			i++;
			*pnMoves = atoi (argv[i]);
			if (*pnMoves < 1)
			{
				cerr << "Invalid moves limit value (must be >= 1)." << endl;
				exit (-3);
			}
		}
		else if (strcmp (argv[i], "-obegin") == 0)
		{
			*pbUseHeurAlg = true;
			int x = 0;
			i++;
			while (strcmp (argv[i], "-oend") != 0)
			{
				(*pOrd)[x] = (ShiftDir) atoi (argv[i]);
				if ((*pOrd)[x] < 0 || (*pOrd)[x] > 3)
				{
					cerr << "Invalid order value (must be 0..3)." << endl;
					exit (-4);
				}
				x++;
				i++;
			}
			*pbUseHeur = false;
		}
		else if (strcmp (argv[i], "-thisclose") == 0)
		{
			*pbUseHeurAlg = true;
			*pnThisClose = atoi (argv[++i]);
			cerr << "Dump data when the heuristic distance to solution is less or equal to ";
			cerr << *pnThisClose << "." << endl;
		}
		else
		{
			cerr << "Unknown parameter:" << argv[i] << endl;
		}
		i++;
	}
	return (i);
}

eSolveStat SolvePuzzle (int nDeep, int nMoves, int nDump, PTree *pBox, bool bUseHeurFirst)
{
	Order anOrder = {LEFT, UP, RIGHT, DOWN};
	eSolveStat stat;

	CPuzzle Puzzle (pBox->anBox, anOrder);

	stat = Puzzle.Solve (nDeep, nMoves, nDump, pBox, bUseHeurFirst);
	if (stat == SOLVED)
	{
		Puzzle.ShowGoodWay ();
	}

	return (stat);
}

int main (int argc, char *argv[])
{
	Box anStart =
	{
		{ 5, 4, 8},
		{ 7, 0, 3},
		{ 1, 2, 6}
	};
	Order anOrder = {LEFT, UP, RIGHT, DOWN};
	int nMaxDeep = 100;
	int nMaxMoves = 10000;
	bool bUseHeurFirstStep = true;
	int nThisClose = 0;
	bool bUseHeuristicAlg = false;

	cout << endl;
	cout << "Slide puzzle 3x3. Public domain. Author: Marek K.'2002." << endl;
	cout << endl;

	if (argc > 1)
		ScanArg (argv, &anStart, &nMaxDeep, &nMaxMoves, 
				 &anOrder, &bUseHeurFirstStep, &nThisClose,
				 &bUseHeuristicAlg
				);
	else
	{
		Usage (argv[0]);
		exit (0);
	}

	CPuzzle Puzzle1 (anStart, anOrder);
	CPuzzle Puzzle ;

	cout << "Starting state:" << endl;
	//cout << anStart << endl;
	Puzzle.ShowState (anStart);

	cout << "Goal state:" << endl;
	//cout << *(Puzzle.GetGoal ()) << endl;
	Puzzle.ShowState (*(Puzzle.GetGoal ()));

	if (bUseHeuristicAlg == true)
	{
		cout << "Algorithm type: recursive heuristic." << endl;

		cout << "Algorithm depth limit = " << nMaxDeep << ", Visited branches limit = ";
		cout << nMaxMoves << "." << endl;

		if (bUseHeurFirstStep != true)
			cout << "Suppress heuristics in first step." << endl;
	}
	else
		cout << "Algorithm type: linear tree search." << endl;

	try
	{
		Stat stat;
		cerr << "Please wait..." << endl;
		if (bUseHeuristicAlg == true)
		{
			int nDeepControl = 10;
		
			while (nDeepControl < nMaxDeep &&
					(stat = SolvePuzzle (nDeepControl++, 
			                        nMaxMoves, 
									nThisClose, 
									&(Puzzle1.m_Tree), 
									bUseHeurFirstStep)) != SOLVED
				  ) ;
		}  
		else
		{
			if (!(_access ("tree.bin", 0) != -1))
				Puzzle.GenerateTree ();
			else
				Puzzle.GenerateTree (anStart);
			stat = Puzzle.Solve2 (anStart);
		}
		
		if (stat != SOLVED)
			cout << "The solution for this puzzle initial configuration could not be found." << endl;
		
		/**
		if (Puzzle.Solve (nMaxDeep, nMaxMoves, nThisClose, &(Puzzle.m_Tree), bUseHeurFirstStep) == SOLVED)
		{
			Puzzle.ShowGoodWay ();	
		}
		else
		{
		   cout << "Puzzle has not been solved." << endl;
		}
		
		cout << "Tree generated:" << endl;
		cout << &(Puzzle.m_Tree) << endl;
		**/
	}
	catch (eSolveStat e)
	{
		cerr << endl;
		cerr << "Can't solve the puzzle, reason:" << endl;
		switch (e)
		{
		case MAXMOVES:
			cerr << "The number of moves limit reached." << endl;
			break;
		case WRONGTILE:
			cerr << "Wrong tile number detected in the puzzle box." << endl;
			break;
		case NOMEMORY:
			cerr << "Out of memory error." << endl;
			break;
		case NOTIMPLEMENTED:
			cerr << "Attempt to use method that is not implemented." << endl;
			break;
		case FILEERROR:
			cerr << "File operation error." << endl;
			break;
		default:
			cerr << "Unknown." << endl;
			break;
		}
		exit (-1);
	}
	catch (...)
	{
		cerr << endl;
		cerr << "Unhandled exception error." << endl;
		exit (-2);
	}

	return (0);
}

