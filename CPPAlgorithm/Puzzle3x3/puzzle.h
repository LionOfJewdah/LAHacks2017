#ifndef _PUZZLE_H_
#define _PUZZLE_H_

#include <iostream>
#include <stdlib.h>
#include "puzlist.h"

#define MAX_PERM 362882
#define TREE_SIZE 362882


//typedef int Order[4];

enum eDir
{
	LEFT = 0, UP, RIGHT, DOWN
};

typedef eDir ShiftDir;
typedef ShiftDir Order[4];

struct PuzzleTree
{
	Box anBox;
	PuzzleTree *pUp;
	PuzzleTree *pDown;
	PuzzleTree *pRight;
	PuzzleTree *pLeft;
	long lID;
};

typedef struct PuzzleTree PTree;
typedef PTree *PTreePtr;

class CPuzzle
{

   long m_lID;


public:
   PTree m_Tree;
   CPuzzleList m_GoodWay;
   CPuzzleList m_Visited;
   Order m_Order;
   PTreePtr *m_pTree;
   int *m_pDescIndexTbl;

    CPuzzle ();
	CPuzzle (Box a);
	CPuzzle (Box a, Order b);
	~CPuzzle ();

	void DeleteBranch (PTree *p);

	Stat Solve (int nDeepLimit, int nMaxMoves, int nDumpThisClose, PTree *pBox, bool bUseHeurFirstStep = true);
	Stat Solve2 (Box b);
	bool SolveNonRecursive (int nDeepLimit);
	void HeuristicEval (PTree *pBox, Order p);
	int Misplaced (Box b);
	void SortHeurTbl (Order p, int *pH);
	PTree * Shift (int x, int y, ShiftDir z, PTree *pBox);
	bool CompareBox (Box x, Box y);
	int NoSign (int a);
	void ShowGoodWay (bool bReverseList = true);
	void ShowState (Box state);
	bool IsGoal (Box a);
	Box *GetGoal (void);
	void ShowVisited (void);
	void GenerateBranches (PTree *p);
	void GenerateTree (Box b);
	void GenerateTree (void);
	void CreateGoodWayList (int nIndex);

	bool operator== (Box x)
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (m_Tree.anBox[i][j] != x[i][j])
					return (false);

		return (true);
	}
	
  friend ostream & operator<< (ostream &os, PTree *p)
	{
		if (p != NULL)
		{
			os << "Root <" << p->lID << ">:" << endl;
			os << p->anBox << endl;
			os << "------------------------------" << endl;
			os << "Left branch <" << p->lID << ">:" << endl;
			os << p->pLeft << endl;
			os << "------------------------------" << endl;
			os << "Right branch <" << p->lID << ">:" << endl;
			os << p->pRight << endl;
			os << "------------------------------" << endl;
			os << "Up branch <" << p->lID << ">:" << endl;
			os << p->pUp << endl;
			os << "------------------------------" << endl;
			os << "Down branch <" << p->lID << ">:" << endl;
			os << p->pDown << endl;
			os << "------------------------------" << endl;
			os << "End Root <" << p->lID << "> **************" << endl;
		}
		else
		   os << "*NIL" << endl;
		return (os);
	}

};

#endif //_PUZZLE_H_
