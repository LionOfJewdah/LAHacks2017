
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include "puzlist.h"

CPuzzleList::CPuzzleList ()
{
   m_nIndex = 0;
   m_nLength = 0;
   m_pData = NULL;
   m_pEnd = m_pData;
}

CPuzzleList::~CPuzzleList ()
{
	FreeMem (m_pData);
}

int CPuzzleList::FreeMem (PList p)
{
	if (p == NULL)
		return (-1);
	PList pSave;
	for (PList ptr = p->pNext; ptr != NULL; ptr = ptr->pNext)
	{
		if (ptr->pPrev != NULL)
			delete ptr->pPrev;
		pSave = ptr;
	}
	if (pSave != NULL)
		delete pSave;

	return (0);
}

int CPuzzleList::AddAtEnd (Box anData)
{
   if (m_pEnd == NULL)
   {
	   m_pData = new (ListItem);
	   if (m_pData == NULL)
		   throw (NOMEMORY);
	   for (int i=0; i<3; i++) {
	      for (int j=0; j<3; j++) {
	         m_pData->anData[i][j] = anData[i][j];
		  }
	   }
	   //memcpy (m_pData->anData, anData, sizeof (Box));
	   m_pData->pNext = NULL;
	   m_pData->pPrev = NULL;
	   m_pEnd = m_pData;
	   m_nLength++;
   }
   else
   {
       PList pPrev = m_pEnd;
	   m_pEnd->pNext = new (ListItem);
	   m_pEnd = m_pEnd->pNext;
	   if (m_pEnd == NULL)
		   throw (NOMEMORY);
	   for (int i=0; i<3; i++) {
	      for (int j=0; j<3; j++) {
	         m_pEnd->anData[i][j] = anData[i][j];
		  }
	   }		   
	   //memcpy (m_pEnd->anData, anData, sizeof (Box));
	   m_pEnd->pNext = NULL;
	   m_pEnd->pPrev = pPrev;
	   m_nLength++;
   }
   return (1);
}

int CPuzzleList::RemoveFromAt (int nIndex)
{
	throw (NOTIMPLEMENTED);
	return (-1);
}

int CPuzzleList::RemoveFromEnd ()
{
	throw (NOTIMPLEMENTED);
	return (-1);
}

int CPuzzleList::GetLength ()
{
   return (m_nLength);
}

int CPuzzleList::InsertAt (int nIndex)
{
   throw (NOTIMPLEMENTED);
   return (-1);
}

int CPuzzleList::FindItem (Box anItem)
{
   bool bFound;
   int i = 0;
   
   for (PList p = m_pData; p != NULL; p = p->pNext)
   {
      bFound = true;
      for (int j = 0; j < 3; j++)
      {
         for (int k = 0; k < 3; k++)
         {
            if (p->anData[j][k] != anItem[j][k])
            {
               bFound = false;
               break;
            } 
         }
         if (bFound != true)
            break;
      }
      if (bFound == true)
         return (i);
	  i++;
   }
         
   return (-1);
}

void CPuzzleList::ReverseOrder (void)
{
   if (m_nLength > 0)
   {
	  PList ptr = NULL;

	  for (PList p = m_pData; p != NULL; /*NULL*/)
	  {
	     ptr = p->pNext;
		 p->pNext = p->pPrev;
		 p->pPrev = ptr;
		 p = ptr;
      }
	  ptr = m_pData;
	  m_pData = m_pEnd;
	  m_pEnd = m_pData;
   }
}

#ifndef AIX
void CPuzzleList::AddStateToAssocTbl (Box anData)
{
	AssocTbl.insert (BoxToInt (anData));
}

bool CPuzzleList::IsStateInAssocTbl (Box anData)
{
	return (AssocTbl.find (BoxToInt (anData)) != AssocTbl.end ());
}

void CPuzzleList::ReleaseAssocTbl (void)
{
	AssocTbl.erase (AssocTbl.begin (), AssocTbl.end ());
}

int CPuzzleList::GetAssocTblLength (void)
{
	return (AssocTbl.size ());
}
#endif

int CPuzzleList::BoxToInt (Box anData)
{
	int nIndex = anData[0][0] * 100000000 + anData[0][1] * 10000000 + anData[0][2] * 1000000 +
		          anData[1][0] * 100000 + anData[1][1] * 10000 + anData[1][2] * 1000 +
				  anData[2][0] * 100 + anData[2][1] * 10 + anData[2][2];
	return (nIndex);
}

void CPuzzleList::IntToBox (Box *pBox, int nBoxCode)
{
	char szVal[20], str[2];
	int i, j, k;

	memset (szVal, '0', sizeof (szVal));
	_itoa (nBoxCode, szVal, 10);
    if (nBoxCode < 100000000)
	{
		i = 1;
		j = 0;
		k = 1;
	}
	else
	{
		i = 0;
		j = 0; 
		k = 0;
	}
	while (i < 9)
	{
		memset (str, '\0', sizeof (str));
		strncpy (str, szVal+i, 1);
		(*pBox)[j][k] = atoi (str);
		k++;
		if (k > 2)
		{
			k = 0;
			j++;
			if (j > 2)
				j = 0;
		}
		i++;
	}
}
