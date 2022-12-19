// ChargeOpt.cpp : Optimizes the point charges
//

#include "stdafx.h"
#include "Madelung.h"
#include "MadelungDoc.h"
#include "StringArray.h"
#include "UnitCell.h"
#include "Spg_dll.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef PI
#define PI 3.141592653589793
#endif

// forward declarations
double fi(double h, double eta, double u, int chargeDistribution);
double fii( double eta,double r,int chargeDistribution);
double erfc(double x);
double fself(double eta,int chargeDistribution);

// Optimizes the point charges of non-selected atoms in Lattice
double OptimizePointCharges(const CMadelungDoc* m_pDoc,	// pointer to document
							const int n,				// number of opt parameters
							double* x,					// Optimized variable array
							StringArray* pSa,			// for keeping text output
							const HWND hTextWindow)		// Text Window HWND
{
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CMadelungDoc)));
// pointer to UnitCell
	UnitCell* pUC = pDoc->m_pSpaceGroup->m_pUnitCell;
// number of atoms in unit cell (ALL atoms in UnitCell. Wyckoff positions expanded)
	int atomsInUnitCell = pUC->m_atomsInUnitCell;

// 
	f_value = solvopt(n,x,