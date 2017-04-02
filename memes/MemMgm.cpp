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
#include "Cube.h"
#include "Exception.h"
#include "MemMgm.h"

using namespace std;

namespace Rubik
{
	
	
/*
 * Method:	MemMgm()
 * Purpose:	Default constructor.
 * Input:
 * Returns:
 */	
MemMgm::MemMgm()
{
	m_vRCubesVect.reserve(10000);
}

/*
 * Method:	MemMgm()
 * Purpose:	Custom constructor. Initialize memory manager and create
 *				a new RCube object from template, add the object to the
 *				manager's list.
 * Input:	Pointer to RCube template object.
 * Returns:
 */
MemMgm::MemMgm(RCube *root)
{
	m_vRCubesVect.reserve(10000);
	GetNewRCube(root);
}

/*
 * Method:	~MemMgm()
 * Purpose:	Destructor. Delete all managed objects. Destroy object.
 * Input:
 * Returns:
 */
MemMgm::~MemMgm()
{
	DeleteAll();
}

/*
 * Method: 		GetNewRCube()
 * Purpose: 	Create new RCube object. Add object to the list.
 * Input: 		void
 * Returns: 	Pointer to new RCube object.
 */			
RCube *MemMgm::GetNewRCube()
{
#if defined(MYDBG15)
	cout << "MemMgm::GetNewRCube()" << endl;
#endif	
	RCube *ret = new RCube();
	if (NULL == ret)
		throw Exception("MemMgm::GetNewRCube() - memory depleted!");
	if (FindRCube(ret))
		throw Exception("MemMgm::GetNewRCube() - duplicate cube!");
	int cube_id = ret->cube_id;
	
	m_vRCubesVect.push_back(ret);
	ret = GetRCubeById(cube_id);
#if defined(MYDBG15)
	cout << "MemMgm::GetNewRCube() - cube_id: " << ret->cube_id << endl;
	cout << "MemMgm::GetNewRCube() - parent : " << (NULL == ret->parent ? 0 : (int)ret->parent) << endl;
#endif	
	
	return ret;
}

/*
 * Method:	GetNewRCube()
 * Purpose:	Create new RCube object. Add object to the list.
 * Input:	Pointer to template/source RCube object.
 * Returns:	Pointer to new Rcube object.
 */
RCube *MemMgm::GetNewRCube(RCube *p)
{
#if defined(MYDBG15)
	cout << "MemMgm::GetNewRCube(RCube *p)" << endl;
#endif
	if (NULL == p)
		throw Exception("MemMgm::GetNewRCube(RCube *p) - null pointer!");
	RCube *pnew = new RCube(*p);
	if (NULL == pnew)
		throw Exception("MemMgm::GetNewRCube(RCube *p) - memory depleted!");
	int cube_id = pnew->cube_id;
	m_vRCubesVect.push_back(pnew);
	RCube *ret = GetRCubeById(cube_id);
#if defined(MYDBG15)
	cout << "MemMgm::GetNewRCube(RCube *p) - cube_id: " << ret->cube_id << endl;
	cout << "MemMgm::GetNewRCube(RCube *p) - parent : " << (NULL == ret->parent ? 0 : (int)ret->parent) << endl;
	bool parent_found = false;
	for (unsigned int i=0; i < m_vRCubesVect.size(); i++)	{
		if (m_vRCubesVect[i]->cube_id == (int)ret->parent) {
			parent_found = true;
		}
	}
	cout << "MemMgm::GetNewRCube(RCube *p) - parent node " << (parent_found ? "found" : "not found") << "." << endl;
#endif				

	return ret;	
}

/*
 * Method:		DeleteRCube()
 * Purpose:		Delete RCube object from memory manager.
 * Input:
 * Returns:
 */
bool	MemMgm::DeleteRCube(RCube *p)
{
#if defined(MYDBG15)
	cout << "MemMgm::DeleteRCube() - cube_id: " << p->cube_id << endl;
#endif	
	bool ret = false;
	int id = p->cube_id;
	
	for (unsigned int i=0; i < m_vRCubesVect.size(); i++)	{
		if (m_vRCubesVect[i]->cube_id == id) {
			delete m_vRCubesVect[i];
			m_vRCubesVect.erase(m_vRCubesVect.begin() + i);
			ret = true;
			break;
		}
	}
	
#if defined(MYDBG15)
	cout << "MemMgm::DeleteRCube() - node " << id << " " << (ret ? "deleted" : "not found") << "." << endl;
#endif			
	
   if (ret) {
		for (unsigned int i=0; i < m_vRCubesVect.size(); i++)	{
			if ((int)m_vRCubesVect[i]->parent == id) {
				m_vRCubesVect[i]->parent = NULL;
#if defined(MYDBG15)
				cout << "MemMgm::DeleteRCube() - parent reference cleaned in node " << m_vRCubesVect[i]->cube_id << "." << endl;
#endif				
			}
		}	   	
	}	
	
	return ret;
}

/*
 * Method:		DeleteRCubeById()
 * Purpose:		Delete RCube object from the manager.
 * Input:		The ID of RCube object.
 * Returns:		bool true if success, false if no such object found.
 */
bool	MemMgm::DeleteRCubeById(int cube_id)
{
#if defined(MYDBG15)
	cout << "MemMgm::DeleteRCubeById() - cube_id: " << cube_id << endl;
#endif	
	bool ret = false;
	
	for (unsigned int i=0; i < m_vRCubesVect.size(); i++)	{
		if (m_vRCubesVect[i]->cube_id == cube_id) {
			delete m_vRCubesVect[i];
			m_vRCubesVect.erase(m_vRCubesVect.begin() + i);
			ret = true;
			break;
		}
	}
	
#if defined(MYDBG15)
	cout << "MemMgm::DeleteRCubeById() - node " << cube_id << " " << (ret ? "deleted" : "not found") << "." << endl;
#endif			
	
   if (ret) {
		for (unsigned int i=0; i < m_vRCubesVect.size(); i++)	{
			if ((int)m_vRCubesVect[i]->parent == cube_id) {
				m_vRCubesVect[i]->parent = NULL;
#if defined(MYDBG15)
				cout << "MemMgm::DeleteRCube() - parent reference cleaned in node " << m_vRCubesVect[i]->cube_id << "." << endl;
#endif				
			}
		}	   	
	}	
	
	return ret;
}

/*
 * Method:		DeleteAll()
 * Purpose:		Delete all objects from manager.
 * Input:
 * Returns:		bool true if any objects were found and deleted, false otherwise.
 */
bool MemMgm::DeleteAll()
{
#if defined(MYDBG15)
	cout << "MemMgm::DeleteAll()" << endl;
#endif	
	bool ret = false;
	
	if (0 < m_vRCubesVect.size())	{
		for (unsigned int i=0; i < m_vRCubesVect.size(); i++)
			delete m_vRCubesVect[i];
		m_vRCubesVect.clear();
		ret = true;
	}
	
#if defined(MYDBG15)
	cout << "MemMgm::DeleteAll() - " << (ret ? "items deleted" : "container is already empty") << "." << endl;
#endif	

	return ret;
}

/*
 * Method:		FindRCube()
 * Purpose:		Find RCube object.
 * Input:		Pointer to RCube object.
 * Returns:		bool true if RCube object with the same id as the input object found,
 *					false otherwise.
 */
bool MemMgm::FindRCube(RCube *p)
{
#if defined(MYDBG15)
	cout << "MemMgm::FindRCube - cube_id: " << p->cube_id << "." << endl;
#endif	
	bool ret = false;
	
	for (unsigned int i=0; i < m_vRCubesVect.size(); i++)	{
		if (m_vRCubesVect[i]->cube_id == p->cube_id) {
			ret = true;
			break;
		}
	}	
	
#if defined(MYDBG15)
	cout << "MemMgm::FindRCube - node " << (ret ? "found" : "not found") << "." << endl;
#endif	
	
	return ret;
}

/*
 * Method:		GetRCubeById()
 * Purpose:		Search for RCube object in manager's list.
 * Input:		integer - the id of the RCube object
 * Returns:		Pointer to the RCube object if a match was found in manager's list,
 *					NULL pointer otherwise.
 */
RCube *MemMgm::GetRCubeById(int cube_id)
{
	RCube *ret = NULL;
	
	for (unsigned int i=0; i < m_vRCubesVect.size(); i++)	{
		if (m_vRCubesVect[i]->cube_id == cube_id) {
			ret = m_vRCubesVect[i];
			break;
		}
	}	
	
	return ret;
}

/*
 * Method:		GetRCubeAt()
 * Purpose:		Search for RCube object in manager's list.
 * Input:		integer - an index of position of the RCube object
 * Returns:		Poiner to the RCube object of index is within range,
 *					NULL otherwise.
 */
RCube *MemMgm::GetRCubeAt(unsigned int idx)
{
	RCube *ret = NULL;
	
	if (idx < m_vRCubesVect.size())
		ret = m_vRCubesVect[idx];
	
	return ret;
}
	
}	// namespace Rubik
