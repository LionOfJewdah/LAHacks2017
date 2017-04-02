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
 
#include "Cube.h" 
#include "Exception.h"
#include "CubieModel.h" 

using namespace std;

namespace Rubik {

/*
 * Convert binary number represented as 0-s and 1-s in binary tbl to integer.
 */	
int Bin2Int(int *binary, int from, int length) 
{
	int ret = 0;
	
	for (int i=from; i<from+length; i++) {
		ret *= 2;
		ret += binary[i];
	}
	
	return ret;
}	

/*
 * Convert integer to binary array of 0 and 1 values of size length.
 * The maximum length is 12 bits.
 */
int *Int2Bin(int value, int length)
{
	static int bt[12];
	for (int i=0; i<12; i++) {
		bt[i] = 0;
	}
	
	int num=value, sum=0, v=2;
	
	for (int i=length-1; 0<=i; i--) {		
		bt[i] = ((num % v) ? 1 : 0);
		sum += bt[i];
		num /= v;
	}
	
	return bt;
}

/*
 * Convert edges orientation vector (11 bits) to index.
 * Edges orientations can have values:
 * 0 - incorrectly oriented piece
 * 1 - correctly oriented piece
 */
int EO2Index(int *pedges)
{
	int sum=0, v=2;
	for (int i=0; i<NUM_OF_EDGES; i++) {
		sum += pedges[i];
	}
	if (0 != (sum%v))
		throw Exception("EO2Index - algorithm error: cube law #2 failed!");
			
	int ret=0;
	
	for (int i=0; i<NUM_OF_EDGES-1; i++) {
		ret += pedges[i] * (1 << i);
	}
    
   return ret;
}

/*
 * Convert 11-bit integer to a binary vector.
 * Calculate the 12-th bit based on cube's 2-nd law.
 */
int *Index2EO(int idx)
{
	static int eo[NUM_OF_EDGES];
	int v=2;
	
	for (unsigned int i=0; i<NUM_OF_EDGES;i++)
		eo[i] = 0;
		
	int sum=0; //, index=idx;
	for (int i=0; i<NUM_OF_EDGES-1; i++) {
		eo[i] = ((idx & (1 << i)) > 0 ? 1 : 0);
		sum += eo[i];
	}
	eo[NUM_OF_EDGES-1] = sum%v;

	sum = 0;
	for (int i=0; i<NUM_OF_EDGES; i++) {
		sum += eo[i];
	}
	if (0 != (sum%v))
		throw Exception("Index2EO - algorithm error: cube law #2 failed!");
	
	return eo;
}

/*
 * Convert corners orientation vector (7 bits) to index.
 * Corners orientation can have values:
 * 0 - properly oriented piece
 * 1 - piece is rotated clockwise
 * 2 - piece is rotated counter clockwise
 */
int CO2Index(int *pcorners)
{
	int v=3;
	int s = 0;
	for (int i=0; i<NUM_OF_CORNERS; i++) {
		s += pcorners[i];
	}
	if (0 != (s%v))
		throw Exception("CO2Index - algorithm error: cube law #3 failed!");	

	int ret=0;	
	for (int i=0; i<NUM_OF_CORNERS-1; i++) {
	  ret *= v;
	  ret += pcorners[i];
	}
    
   return ret;
}

/*
 * Convert edges permutation vector to index.
 * Edges permutation can have values: 1-8
 */
int EP2Index(int *pep)
{
	int ret=0;
	
	for (unsigned int i=0; i<NUM_OF_EDGES-1; i++) {
		ret *= (NUM_OF_EDGES-i);
		for (unsigned int j=i+1; j<NUM_OF_EDGES; j++) {
			if (pep[i] > pep[j])
				ret++;
		}
	}
	
	return ret;
}

/*
 * Convert edges permutation vector to index.
 * Edges permutation can have values:
 * 0 - edges 1-4
 * 1 - edges 5-8
 * 2 - edges 9-12
 */
int LREP2Index(int *pep)
{
	int ret=0;	
	int v=3;
	
	for (int i=0; i<NUM_OF_EDGES; i++) {
	  ret *= v;
	  ret += pep[i];
	}	
	
	return ret;
}

/*
 * Convert corners permutation vector to index.
 * Corners permutation can have values:
 * 0 - for piece in unsolved position
 * 1 - for piece in solved position
 */
int CP2Index(int *pcp)
{
	int ret=0;
	
	for (unsigned int i=0; i<NUM_OF_CORNERS-1; i++) {
		ret *= (NUM_OF_CORNERS-i);
		for (unsigned int j=i+1; j<NUM_OF_CORNERS; j++) {
			if (pcp[i] > pcp[j])
				ret++;
		}
	}
	
	return ret;
}

/*
 * Convert integer to corners orientation vector.
 */
int *Index2CO(int idx)
{
	int s=0, v=3, index=idx;
	static int co[NUM_OF_CORNERS];
	
	for (unsigned int i=0; i<NUM_OF_CORNERS;i++)
		co[i] = 0;
		
	for (int i=NUM_OF_CORNERS-2; 0<=i; i--) {
		co[i] = index%v;
		s -= co[i];
		if (0>s) s+=v;
		index = (index-co[i])/v;
	}
	co[NUM_OF_CORNERS-1] = s;
	
	s = 0;
	for (int i=0; i<NUM_OF_CORNERS; i++) {
		s += co[i];
	}
	if (0 != (s%v))
		throw Exception("Index2CO - algorithm error: cube law #2 failed!");	
	
	return co;
}

/*
 * Convert integer to edges permutation vector.
 */
int *Index2EP(int idx)
{
	static int ep[NUM_OF_EDGES];
	int index=idx;
	
	for (unsigned int i=0; i<NUM_OF_EDGES;i++)
		ep[i] = 0;
		
	ep[NUM_OF_EDGES-1] = 1;
	
	for (int i=NUM_OF_EDGES-2; 0<=i; i--) {
		ep[i] = 1 + (index%(NUM_OF_EDGES-i));
		index = (index - (index%(NUM_OF_EDGES-i)))/(NUM_OF_EDGES-i);
		for (int j=i+1; j<NUM_OF_EDGES; j++) {
			if (ep[j] >= ep[i])
				ep[j]++;
		}
	}
	
	return ep;
}

/*
 * Convert integer to edges permutation vector.
 */
int *Index2LREP(int idx)
{
	static int ep[NUM_OF_EDGES];
	int s=0, v=3, index=idx;
	
	for (unsigned int i=0; i<NUM_OF_EDGES;i++)
		ep[i] = 0;
		
	for (int i=NUM_OF_EDGES-1; 0<=i; i--) {
		ep[i] = index%v;
		s -= ep[i];
		if (0>s) s+=v;
		index = (index-ep[i])/v;
	}
	
	return ep;
}

/*
 * Convert integer to corners permutation vector.
 */
int *Index2CP(int idx)
{
	static int cp[NUM_OF_CORNERS];
	int index=idx;
	
	for (unsigned int i=0; i<NUM_OF_CORNERS;i++)
		cp[i] = 0;
		
	cp[NUM_OF_CORNERS-1] = 1;
	
	for (int i=NUM_OF_CORNERS-2; 0<=i; i--) {
		cp[i] = 1 + (index%(NUM_OF_CORNERS-i));
		index = (index - (index%(NUM_OF_CORNERS-i)))/(NUM_OF_CORNERS-i);
		for (int j=i+1; j<NUM_OF_CORNERS; j++) {
			if (cp[j] >= cp[i])
				cp[j]++;
		}
	}
	
	return cp;
}

/*
 * Cubie model permutation functions for edge cubies.
 */
void PermEO(int *pedges, int *trfm)
{
	int newedges[NUM_OF_EDGES];
	
	for (unsigned int i=0; i<NUM_OF_EDGES; i++) {
		newedges[i] = pedges[trfm[i]-1];
	}
	for (unsigned int i=0; i<NUM_OF_EDGES; i++) {
		pedges[i] = newedges[i];
	}		
}

void PermEOF(int *pedges)
{
	int trfm[] = {1, 2, 3, 4, 5, 10, 9, 8, 6, 7, 11, 12};
	PermEO(pedges, trfm);
}

void PermEOFi(int *pedges)
{
	int trfm[] = {1, 2, 3, 4, 5, 9, 10, 8, 7, 6, 11, 12};
	PermEO(pedges, trfm);	
}

void PermEOR(int *pedges)
{
	int trfm[] = {1, 2, 7, 8, 5, 6, 4, 3, 9, 10, 11, 12};
	PermEO(pedges, trfm);	
}

void PermEORi(int *pedges)
{
	int trfm[] = {1, 2, 8, 7, 5, 6, 3, 4, 9, 10, 11, 12};
	PermEO(pedges, trfm);	
}

void PermEOB(int *pedges)
{
	int trfm[] = {1, 2, 3, 4, 12, 6, 7, 11, 9, 10, 5, 8};
	PermEO(pedges, trfm);	
}

void PermEOBi(int *pedges)
{
	int trfm[] = {1, 2, 3, 4, 11, 6, 7, 12, 9, 10, 8, 5};
	PermEO(pedges, trfm);	
}

void PermEOL(int *pedges)
{
	int trfm[] = {5, 6, 3, 4, 2, 1, 7, 8, 9, 10, 11, 12};
	PermEO(pedges, trfm);	
}

void PermEOLi(int *pedges)
{
	int trfm[] = {6, 5, 3, 4, 1, 2, 7, 8, 9, 10, 11, 12};
	PermEO(pedges, trfm);	
}

void PermEOU(int *pedges)
{
	int trfm[] = {9, 2, 12, 4, 5, 6, 7, 8, 3, 10, 11, 1};
	PermEO(pedges, trfm);	
}

void PermEOUi(int *pedges)
{
	int trfm[] = {12, 2, 9, 4, 5, 6, 7, 8, 1, 10, 11, 3};
	PermEO(pedges, trfm);	
}

void PermEOD(int *pedges)
{
	int trfm[] = {1, 11, 3, 10, 5, 6, 7, 8, 9, 2, 4, 12};
	PermEO(pedges, trfm);	
}

void PermEODi(int *pedges)
{
	int trfm[] = {1, 10, 3, 11, 5, 6, 7, 8, 9, 4, 2, 12};
	PermEO(pedges, trfm);	
}

/*
 * Cubie model permutation functions for corner cubies.
 */
void PermCO(int *pcorners, int *trfm)
{
	int newcorners[NUM_OF_CORNERS];
	
	for (unsigned int i=0; i<NUM_OF_CORNERS; i++) {
		newcorners[i] = pcorners[trfm[i]-1];
	}
	for (unsigned int i=0; i<NUM_OF_CORNERS; i++) {
		pcorners[i] = newcorners[i];
	}		
}

void PermCOF(int *pcorners)
{
	int trfm[] = {1, 7, 3, 5, 2, 6, 4, 8};
	PermCO(pcorners, trfm);
}

void PermCOFi(int *pcorners)
{
	int trfm[] = {1, 5, 3, 7, 4, 6, 2, 8};
	PermCO(pcorners, trfm);	
}

void PermCOR(int *pcorners)
{
	int trfm[] = {1, 2, 8, 7, 5, 6, 3, 4};
	PermCO(pcorners, trfm);	
}

void PermCORi(int *pcorners)
{
	int trfm[] = {1, 2, 7, 8, 5, 6, 4, 3};
	PermCO(pcorners, trfm);	
}

void PermCOB(int *pcorners)
{
	int trfm[] = {8, 2, 6, 4, 5, 1, 7, 3};
	PermCO(pcorners, trfm);	
}

void PermCOBi(int *pcorners)
{
	int trfm[] = {6, 2, 8, 4, 5, 3, 7, 1};
	PermCO(pcorners, trfm);	
}

void PermCOL(int *pcorners)
{
	int trfm[] = {6, 5, 3, 4, 1, 2, 7, 8};
	PermCO(pcorners, trfm);	
}

void PermCOLi(int *pcorners)
{
	int trfm[] = {5, 6, 3, 4, 2, 1, 7, 8};
	PermCO(pcorners, trfm);	
}

void PermCOU(int *pcorners)
{
	int trfm[] = {5, 2, 3, 8, 4, 6, 7, 1};
	PermCO(pcorners, trfm);	
}

void PermCOUi(int *pcorners)
{
	int trfm[] = {8, 2, 3, 5, 1, 6, 7, 4};
	PermCO(pcorners, trfm);	
}

void PermCOD(int *pcorners)
{
	int trfm[] = {1, 6, 7, 4, 5, 3, 2, 8};
	PermCO(pcorners, trfm);	
}

void PermCODi(int *pcorners)
{
	int trfm[] = {1, 7, 6, 4, 5, 2, 3, 8};
	PermCO(pcorners, trfm);	
}

/*
 * Edges orientations transofrmation.
 */
void EOTrfm(int *peo, int *ptrfm)
{
	for (int i=0; i<NUM_OF_EDGES; i++) {
		peo[i] = (peo[i] + ptrfm[i]) % 2;
	}	
}

/*
 * Apply U-move transformation to edges orientations vector.
 */
void EOU(int *pedges)
{
	int tfrm[] = {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1};
	EOTrfm(pedges, tfrm);
}

/*
 * Apply D-move transformation to edges orientations vector.
 */
void EOD(int *pedges)
{
	int tfrm[] = {0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0};
	EOTrfm(pedges, tfrm);
}

/*
 * Corners orientations transformation.
 */
void COTrfm(int *pco, int *ptrfm)
{
	for (int i=0; i<NUM_OF_CORNERS; i++) {
		pco[i] = pco[i] + ptrfm[i];
		if (0 > pco[i]) pco[i] += 3;
		pco[i] = pco[i] % 3;
	}	
}

/*
 * Apply F-move transformation to corners orientations vector.
 */
void COF(int *pcorners)
{
	int tfrm[] = {0, 1, 0, 1, -1, 0, -1, 0};
	COTrfm(pcorners, tfrm);
}

/*
 * Apply B-move transformation to corners orientations vector.
 */
void COB(int *pcorners)
{
	int tfrm[] = {1, 0, 1, 0, 0, -1, 0, -1};
	COTrfm(pcorners, tfrm);
}

/*
 * Apply U-move transformation to corners orientations vector.
 */
void COU(int *pcorners)
{
	int tfrm[] = {-1, 0, 0, -1, 1, 0, 0, 1};
	COTrfm(pcorners, tfrm);
}

/*
 * Apply D-move transformation to corners orientations vector.
 */
void COD(int *pcorners)
{
	int tfrm[] = {0, -1, -1, 0, 0, 1, 1, 0};
	COTrfm(pcorners, tfrm);
}

/*
 * Copy edges orientations vector from src to dst.
 */
void EOCopy(int *src, int *dst)
{
	for (int i=0; i<NUM_OF_EDGES; i++) {
		dst[i] = src[i];
	}
}

/*
 * Copy corners orientations vector from src to dst.
 */
void COCopy(int *src, int *dst)
{
	for (int i=0; i<NUM_OF_CORNERS; i++) {
		dst[i] = src[i];
	}
}

/*
 * Copy corners permutations vector from src to dst.
 */
void CPCopy(int *src, int *dst)
{
	COCopy(src, dst);
}

/*
 * Copy edges permutations vector from src to dst.
 */
void EPCopy(int *src, int *dst)
{
	EOCopy(src, dst);
}

/*
 * Apply move to the EO table (perform permutation
 * and orientation adjustments).
 */
void EOApplyMove(int move, int *eo)
{
	switch (move) {
		case MOVE_F2: 	PermEOF(eo);
		case MOVE_F: 	PermEOF(eo); 	break;

		case MOVE_Fi:	PermEOFi(eo);	break;

		case MOVE_R2:	PermEOR(eo);
		case MOVE_R:	PermEOR(eo);	break;

		case MOVE_Ri:	PermEORi(eo);	break;

		case MOVE_B2:	PermEOB(eo);
		case MOVE_B:	PermEOB(eo);	break;

		case MOVE_Bi:	PermEOBi(eo);	break;

		case MOVE_L2:	PermEOL(eo);
		case MOVE_L:	PermEOL(eo);	break;

		case MOVE_Li:	PermEOLi(eo);	break;

		case MOVE_U2:	PermEOU(eo);	EOU(eo);
		case MOVE_U:	PermEOU(eo);	EOU(eo);	break;

		case MOVE_Ui:	PermEOUi(eo);	EOU(eo);	break;

		case MOVE_D2:	PermEOD(eo);	EOD(eo);
		case MOVE_D:	PermEOD(eo);	EOD(eo);	break;

		case MOVE_Di:	PermEODi(eo);	EOD(eo);	break;
		
		default:
			cout << "EOApplyMove - wrong move code: " << move << endl;
			throw Exception("EOApplyMove - wrong move code/algorithm failure!");
			break;
	}
}

/*
 * Apply move to the CO table (perform permutation
 * and orientation adjustments).
 */
void COApplyMove(int move, int *co)
{
	switch (move) {
		case MOVE_F2:	PermCOF(co);	COF(co);
		case MOVE_F: 	PermCOF(co);	COF(co);
							break;

		case MOVE_Fi:	PermCOFi(co);	COF(co);
							break;

		case MOVE_R2:	PermCOR(co);
		case MOVE_R:	PermCOR(co);
							break;

		case MOVE_Ri:	PermCORi(co);
							break;

		case MOVE_B2:	PermCOB(co);	COB(co);
		case MOVE_B:	PermCOB(co);	COB(co);
							break;

		case MOVE_Bi:	PermCOBi(co);	COB(co);
							break;

		case MOVE_L2:	PermCOL(co);
		case MOVE_L:	PermCOL(co);
							break;

		case MOVE_Li:	PermCOLi(co);
							break;

		case MOVE_U2:	PermCOU(co);	COU(co);
		case MOVE_U:	PermCOU(co);	COU(co);
							break;

		case MOVE_Ui:	PermCOUi(co);	COU(co);
							break;

		case MOVE_D2:	PermCOD(co);	COD(co);
		case MOVE_D:	PermCOD(co);	COD(co);
							break;

		case MOVE_Di:	PermCODi(co);	COD(co);
							break;

		default:
			cout << "COApplyMove - wrong move code: " << move << endl;
			throw Exception("COApplyMove - wrong move code/algorithm failure!");
			break;
	}	
}

/*
 * Apply move to the CP table (perform permutation).
 */
void CPApplyMove(int move, int *cp)
{
	switch (move) {
		case MOVE_F2:	PermCOF(cp);
		case MOVE_F: 	PermCOF(cp); 	break;
		
		case MOVE_Fi:	PermCOFi(cp);	break;

		case MOVE_R2:	PermCOR(cp);
		case MOVE_R:	PermCOR(cp);	break;
		
		case MOVE_Ri:	PermCORi(cp);	break;

		case MOVE_B2:	PermCOB(cp);
		case MOVE_B:	PermCOB(cp);	break;

		case MOVE_Bi:	PermCOBi(cp);	break;

		case MOVE_L2:	PermCOL(cp);
		case MOVE_L:	PermCOL(cp);	break;

		case MOVE_Li:	PermCOLi(cp);	break;

		case MOVE_U2:	PermCOU(cp);
		case MOVE_U:	PermCOU(cp);	break;

		case MOVE_Ui:	PermCOUi(cp);	break;

		case MOVE_D2:	PermCOD(cp);
		case MOVE_D:	PermCOD(cp);	break;

		case MOVE_Di:	PermCODi(cp);	break;

		default:
			cout << "CPApplyMove - wrong move code: " << move << endl;
			throw Exception("CPApplyMove - wrong move code/algorithm failure!");
			break;
	}	
}

/*
 * Apply move to the EP table (perform permutation).
 */
void EPApplyMove(int move, int *ep)
{
	switch (move) {
		case MOVE_F2: 	PermEOF(ep);
		case MOVE_F: 	PermEOF(ep); 	break;
		
		case MOVE_Fi:	PermEOFi(ep);	break;
		
		case MOVE_R2:	PermEOR(ep);
		case MOVE_R:	PermEOR(ep);	break;
		
		case MOVE_Ri:	PermEORi(ep);	break;

		case MOVE_B2:	PermEOB(ep);
		case MOVE_B:	PermEOB(ep);	break;
		
		case MOVE_Bi:	PermEOBi(ep);	break;

		case MOVE_L2:	PermEOL(ep);
		case MOVE_L:	PermEOL(ep);	break;
		
		case MOVE_Li:	PermEOLi(ep);	break;

		case MOVE_U2:	PermEOU(ep);
		case MOVE_U:	PermEOU(ep);	break;
		
		case MOVE_Ui:	PermEOUi(ep);	break;
		
		case MOVE_D2:	PermEOD(ep);
		case MOVE_D:	PermEOD(ep);	break;
		
		case MOVE_Di:	PermEODi(ep);	break;

		default:
			cout << "EPApplyMove - wrong move code: " << move << endl;
			throw Exception("EPApplyMove - wrong move code/algorithm failure!");
			break;
	}
}

/*
 *	Apply move to the whole cube.
 */
void ApplyMove(int move, int *ep, int *cp, int *eo, int *co)
{
	switch (move) {
		case MOVE_F2: 	PermEOF(ep); 	PermCOF(cp);	PermEOF(eo);	PermCOF(co);	COF(co);
		case MOVE_F: 	PermEOF(ep); 	PermCOF(cp);	PermEOF(eo);	PermCOF(co);	COF(co);
							break;
		
		case MOVE_Fi:	PermEOFi(ep);	PermCOFi(cp);	PermEOFi(eo);	PermCOFi(co);	COF(co);
							break;
		
		case MOVE_R2:	PermEOR(ep);	PermCOR(cp);	PermEOR(eo);	PermCOR(co);
		case MOVE_R:	PermEOR(ep);	PermCOR(cp);	PermEOR(eo);	PermCOR(co);
							break;

		case MOVE_Ri:	PermEORi(ep);	PermCORi(cp);	PermEORi(eo);	PermCORi(co);
							break;
		

		case MOVE_B2:	PermEOB(ep);	PermCOB(cp);	PermEOB(eo);	PermCOB(co);	COB(co);
		case MOVE_B:	PermEOB(ep);	PermCOB(cp);	PermEOB(eo);	PermCOB(co);	COB(co);
							break;
		
		case MOVE_Bi:	PermEOBi(ep);	PermCOBi(cp);	PermEOBi(eo);	PermCOBi(co);	COB(co);
							break;
		
		case MOVE_L2:	PermEOL(ep);	PermCOL(cp);	PermEOL(eo);	PermCOL(co);
		case MOVE_L:	PermEOL(ep);	PermCOL(cp);	PermEOL(eo);	PermCOL(co);
							break;
		
		case MOVE_Li:	PermEOLi(ep);	PermCOLi(cp);	PermEOLi(eo);	PermCOLi(co);
							break;
		
		case MOVE_U2:	PermEOU(ep);	PermCOU(cp);	PermEOU(eo);	PermCOU(co);	EOU(eo);	COU(co);
		case MOVE_U:	PermEOU(ep);	PermCOU(cp);	PermEOU(eo);	PermCOU(co);	EOU(eo);	COU(co);
							break;
		
		case MOVE_Ui:	PermEOUi(ep);	PermCOUi(cp);	PermEOUi(eo);	PermCOUi(co);	EOU(eo);	COU(co);
							break;

		case MOVE_D2:	PermEOD(ep);	PermCOD(cp);	PermEOD(eo);	PermCOD(co);	EOD(eo);	COD(co);
		case MOVE_D:	PermEOD(ep);	PermCOD(cp);	PermEOD(eo);	PermCOD(co);	EOD(eo);	COD(co);
							break;
		
		case MOVE_Di:	PermEODi(ep);	PermCODi(cp);	PermEODi(eo);	PermCODi(co);	EOD(eo);	COD(co);
							break;

		default:
			cout << "ApplyMove - wrong move code: " << move << endl;
			throw Exception("ApplyMove - wrong move code/algorithm failure!");
			break;
	}	
}

/*
 * Return true if cube is in solved state.
 */
bool CubieModelSolved(int *ep, int *cp, int *eo, int *co)
{
	bool ret = true;
	
	for (int i=0; ret && i<NUM_OF_EDGES; i++) {
		if (ep[i] != i+1 || eo[i] != 1) {
			ret = false;
		}
	}
	for (int i=0; ret && i<NUM_OF_CORNERS; i++) {
		if (cp[i] != i+1 || co[i] != 0) {
			ret = false;
		}
	}
	
	return ret;
}

/*
 * Return true if corners are in solved state.
 */
bool SolvedCornersCubieModel(int *cp, int *co)
{
	bool ret = true;
	
	for (int i=0; ret && i<NUM_OF_CORNERS; i++) {
		if (cp[i] != i+1 || co[i] != 0) {
			ret = false;
		}
	}
	
	return ret;
}

/*
 * Rudimentary display of all cubie model states to the console.
 */
void CubieModelShowAllStates(int *ep, int *cp, int *eo, int *co)
{
	cout << "EdgePerm   {";
	for (int i=0; i<NUM_OF_EDGES-1; i++) {
		cout << ep[i] << ",";
	}
	cout << ep[NUM_OF_EDGES-1] << "}" << endl;
	cout << "EdgeOrient {";
	for (int i=0; i<NUM_OF_EDGES-1; i++) {
		cout << eo[i] << ",";
	}
	cout << eo[NUM_OF_EDGES-1] << "}" << endl;	
	cout << "CornPerm   {";
	for (int i=0; i<NUM_OF_CORNERS-1; i++) {
		cout << cp[i] << ",";
	}
	cout << cp[NUM_OF_CORNERS-1] << "}" << endl;		
	cout << "CornOrient {";
	for (int i=0; i<NUM_OF_CORNERS-1; i++) {
		cout << co[i] << ",";
	}
	cout << co[NUM_OF_CORNERS-1] << "}" << endl;
}

} // namespace Rubik
