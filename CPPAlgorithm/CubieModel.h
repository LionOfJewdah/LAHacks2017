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
 
#ifndef _CUBIE_MODEL_H
#define  _CUBIE_MODEL_H

namespace Rubik {
	
int Bin2Int(int *binary, int from, int length);	
int *Int2Bin(int value, int length);
int EO2Index(int *pedges);
int CO2Index(int *pcorners);
int EP2Index(int *pep);
int LREP2Index(int *pep);
int CP2Index(int *pcp);
int *Index2EO(int idx);
int *Index2CO(int idx);
int *Index2EP(int idx);
int *Index2LREP(int idx);
int *Index2CP(int idx);
void PermEO(int *pedges, int *trfm);
void PermEOF(int *pedges);
void PermEOFi(int *pedges);
void PermEOR(int *pedges);
void PermEORi(int *pedges);
void PermEOB(int *pedges);
void PermEOBi(int *pedges);
void PermEOL(int *pedges);
void PermEOLi(int *pedges);
void PermEOU(int *pedges);
void PermEOUi(int *pedges);
void PermEOD(int *pedges);
void PermEODi(int *pedges);
void PermCO(int *pcorners, int *trfm);
void PermCOF(int *pcorners);
void PermCOFi(int *pcorners);
void PermCOR(int *pcorners);
void PermCORi(int *pcorners);
void PermCOB(int *pcorners);
void PermCOBi(int *pcorners);
void PermCOL(int *pcorners);
void PermCOLi(int *pcorners);
void PermCOU(int *pcorners);
void PermCOUi(int *pcorners);
void PermCOD(int *pcorners);
void PermCODi(int *pcorners);
void EOTrfm(int *peo, int *ptrfm);
void EOU(int *pedges);
void EOD(int *pedges);
void COTrfm(int *pco, int *ptrfm);
void COF(int *pcorners);
void COB(int *pcorners);
void COU(int *pcorners);
void COD(int *pcorners);
void EOCopy(int *src, int *dst);
void COCopy(int *src, int *dst);
void EPCopy(int *src, int *dst);
void CPCopy(int *src, int *dst);
void EOApplyMove(int move, int *eo);
void COApplyMove(int move, int *co);
void CPApplyMove(int move, int *cp);
void EPApplyMove(int move, int *ep);
void ApplyMove(int move, int *ep, int *cp, int *eo, int *co);
bool CubieModelSolved(int *ep, int *cp, int *eo, int *co);
bool SolvedCornersCubieModel(int *cp, int *co);
void CubieModelShowAllStates(int *ep, int *cp, int *eo, int *co);

} // namespace Rubik

#endif // _CUBIE_MODEL_H
