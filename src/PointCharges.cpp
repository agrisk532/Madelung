// PointCharges.cpp: implementation of the PointCharges class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "madelung.h"
#include "MadelungDoc.h"
#include "Lattice.h"
#include "Spg_dll.h"
#include "UnitCell.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// pointer to the CMadelungDoc object
extern CMadelungDoc* g_pDoc;
// forward declaration
double ElStatPotentialOnAtom(int i,CLattice* pLattice,double x[]);

///////////////////////////////////////////////////////////////////////////
//	"C"-linkage function for optimization routine
//
extern "C" double f_opt(double x[])
{
	double f_value = 0.;
	double pot;
	CLattice* pLattice = g_pDoc->m_pLattice;
	UnitCell* pUC = g_pDoc->m_pSpaceGroup->m_pUnitCell;

	int atomsTotal = pLattice->GetSize();

// NOT USING SYMMETRY

	if(!g_pDoc->m_bUseSymmetry)
	{
		int selectedAtoms = g_pDoc->GetNumberOfSelectedAtoms();

		ASSERT(atomsTotal == (g_pDoc->m_atomsInCluster + g_pDoc->m_atomsInExtension));
// We calculate el.stat. potentials on all selected atoms from all unselected atoms
// and subtract the potential calculated by Ewald summation 
		for(int i=0;i<selectedAtoms;i++)
		{
// fill the corresponding dimensions with optimized charges
			int atomNumber = g_pDoc->m_optimized_charges->m_selected_atoms[i];
			double storedPot = 
				pUC->m_unitCellAtomCartPotential[pLattice->m_atomInUnitCell[atomNumber]];
			ASSERT(storedPot != 0.);
			pot = ElStatPotentialOnAtom(atomNumber,pLattice,x) - storedPot;

			pot *= pot;
			f_value += pot;
		}
		return f_value;
	}

// USING SYMMETRY

    else
	{
		int selectedAtoms = g_pDoc->GetNumberOfSelectedSymmIndependentAtoms();

// We calculate el.stat. potentials on all selected atoms from all unselected atoms
// and subtract the potential calculated by Ewald summation 
		for(int i=0;i<selectedAtoms;i++)
		{
// fill the corresponding dimensions with optimized charges
			int atomNumber = g_pDoc->m_optimized_charges->m_selected_atoms[i];
			double storedPot = 
				pUC->m_unitCellAtomCartPotential[pLattice->m_atomInUnitCell[atomNumber]];
			ASSERT(storedPot != 0.);
			pot = ElStatPotentialOnAtom(atomNumber,pLattice,x) - storedPot;
			pot *= pot;
			f_value += pot;
		}
		return f_value;
	}
}

/////////////////////////////////////////////////////////////////////////
//	Calculates the electrostatic potential on CLattice atom i from all
//  other atoms
//	If symmetry is NOT used, x[] contains charges of all atoms in CLattice
//  If symmetry IS used, x[] contains charges only of the symmetry independent atoms in CLattice,
//  both selected and unselected
//
double ElStatPotentialOnAtom(int i,CLattice* pLattice,double x[])
{
	double potential = 0.;
	double charge;
	int size = pLattice->GetSize();

// NOT USING SYMMETRY

	if(!g_pDoc->m_bUseSymmetry)
	{
		for(int j=0;j<size;j++)
		{
// find unselected atoms
//		if(pLattice->m_selected[j])
			if(i == j)
				continue;
// unselected atom. Calculate distance to the selected atom and el.stat. potential on it
			double rx = pLattice->m_X[i]-pLattice->m_X[j];
			double ry = pLattice->m_Y[i]-pLattice->m_Y[j];
			double rz = pLattice->m_Z[i]-pLattice->m_Z[j];
			double r = sqrt(rx*rx+ry*ry+rz*rz);
			charge = x[j];
			potential += charge/r;
// debug macro
			ASSERT(fabs(potential) < 100.);
		}
		return potential;
	}

// USING SYMMETRY

	else
	{
		int size_opt = g_pDoc->m_optimized_charges->GetSize();

		for(int j=0;j<size;j++)
		{
// find unselected atoms
//		if(pLattice->m_selected[j])
			if(i == j)
				continue;
// unselected atom. Calculate distance to the selected atom and el.stat. potential on it
			double rx = pLattice->m_X[i]-pLattice->m_X[j];
			double ry = pLattice->m_Y[i]-pLattice->m_Y[j];
			double rz = pLattice->m_Z[i]-pLattice->m_Z[j];
			double r = sqrt(rx*rx+ry*ry+rz*rz);
// index of the symmetry independent atom (parent atom) for atom 'j' in m_pLattice
			int index = pLattice->m_SymmetryParentAtom[j];
// find the charge of atom number 'index' in x[];
			bool chargeFound = false;
			for(int k=0;k<size_opt;k++)
			{
				if(index == g_pDoc->m_optimized_charges->m_atoms[k])
				{
					charge = x[k];
					potential += charge/r;
					chargeFound = true;
					break;
				}
				else
					continue;
			}
			if(!chargeFound)
			{
				AfxMessageBox("PointCharges.cpp : Charge not found",MB_OK);
				exit(1);
			}
// debug macro
			ASSERT(fabs(potential) < 100.);
		}
		return potential;
	}
}
