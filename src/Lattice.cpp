// Lattice.cpp: implementation of the CLattice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "madelung.h"
#include "Lattice.h"
#include "MadelungDoc.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLattice::CLattice()
{

}

CLattice::~CLattice()
{
}

///////////////////////////////////////////////////////////////////////
//	used to clear the previous lattice parameters at file->new

void CLattice::Clear()
{
	m_X.clear();				// cartesian X coordinate of an atom
	m_Y.clear();				// cartesian Y coordinate of an atom
	m_Z.clear();				// cartesian Z coordinate of an atom
	m_charge.clear();			// charge of an atom
	m_type.clear();				// contains the type of an atom
	m_WPosition.clear();		// Wyckoff position of atom
	m_selected.clear();			// true if atom selected
	m_rendered.clear();			// true if atom rendered
// Next items indicates to which unit cell an atom belongs.
// It is used in the UnitCell::AddToLattice() method to prevent drawing an atom two
// times in the same point of the (x,y,z) space.
	m_unitCellA.clear();			// unit cell location in the lattice (along A axis)
	m_unitCellB.clear();			// unit cell location in the lattice (along A axis)
	m_unitCellC.clear();			// unit cell location in the lattice (along A axis)
	m_SymmetricallyIndependent.clear();	// is atom symmetrically independent or not
	m_SymmetryParentAtom.clear();
}

///////////////////////////////////////////////////////////////////////
//	i,j are the atom indices; a,b,c - distances**2 to check for bond
//
bool CLattice::AtomsAreBonded(int i,int j, double a, double b, double c)
{
/*
	if(m_type[i] == m_type[j] && m_WPosition[i] == m_WPosition[j])
	{
		if(abs(m_unitCellA[i]-m_unitCellA[j])==1 && m_unitCellB[i]-m_unitCellB[j] == 0 &&
			m_unitCellC[i]-m_unitCellC[j] == 0) return true;

		if(abs(m_unitCellB[i]-m_unitCellB[j])==1 && m_unitCellA[i]-m_unitCellA[j] == 0 &&
			m_unitCellC[i]-m_unitCellC[j] == 0) return true;

		if(abs(m_unitCellC[i]-m_unitCellC[j])==1 && m_unitCellB[i]-m_unitCellB[j] == 0 &&
			m_unitCellA[i]-m_unitCellA[j] == 0) return true;
	}
	return false;
*/
	double dist = GetDistance(i,j);
	if(fabs(dist-a)<0.01 || fabs(dist-b)<0.01 || fabs(dist-c)<0.01)
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
// checks whether a unitcell already exists at (na,nb,nc)
bool CLattice::UnitCellExists(int na,int nb,int nc)
{
// get size of the CLattice object
	int size = this->GetSize();
// now check the object for existence of a unitcell at (na,nb,nc)
	vector<int> va;	// holds indexes of elements, whose m_unitCellA == na
	vector<int> vb;	// holds indexes of elements, whose m_unitCellA == na and m_unitCellB == nb
	vector<int> vc; // holds indexes of elements, identical to (na,nb,nc)
// check the 'na' index
	for(int i=0;i<size;i++)
		if(na == this->m_unitCellA[i])
			va.push_back(i);
	size = va.size();
	if(size == 0) return false;
// check the 'na' & 'nb' indices
	for(i=0;i<size;i++)
		if(nb == this->m_unitCellB[va[i]])
			vb.push_back(va[i]);
	size = vb.size();
	if(size == 0) return false;
// check the 'na' & 'nb' & 'nc' indices
	for(i=0;i<size;i++)
		if(nc == this->m_unitCellC[vb[i]])
			vc.push_back(vb[i]);
	size = vc.size();
	if(size == 0) return false;
	else
		return true;
}

///////////////////////////////////////////////////////////////////////////
//	returns the squared distance between two atoms in CLattice
//
double CLattice::GetDistance(int i,int j)
{
	double dx = m_X[i]-m_X[j];
	double dy = m_Y[i]-m_Y[j];
	double dz = m_Z[i]-m_Z[j];
	double r = dx*dx + dy*dy +dz*dz;
	return r;
}

//////////////////////////////////////////////////////////////////////////
//	returns the number of selected atoms
//
int CLattice::SelectedAtoms()
{
	int size = this->GetSize();
	int sa = 0;
	for(int i=0;i<size;i++)
		if(this->m_selected[i]) sa++;
	return sa;
}

//////////////////////////////////////////////////////////////////////////
//	returns the number of symmetry independent atoms
//
int CLattice::SymmetryIndependentAtoms()
{
	int size = this->GetSize();
	int sa = 0;
	for(int i=0;i<size;i++)
		if(this->m_SymmetricallyIndependent[i]) sa++;
	return sa;
}
//////////////////////////////////////////////////////////////////////////
//	returns the number of s-shell functions on atoms
//
int CLattice::sFunctions(int atomNumber)
{
	int size = this->GetSize();
	if(atomNumber < 0 || atomNumber >= size)
	{
		AfxMessageBox("Invalid atom number in CLattice",MB_OK);
		return 0;
	}
// atom type
	int type = this->m_type[atomNumber];

	switch(type)
	{
	case 0:	// H
	case 1: // He
		return 1;
	case 2:	// Li
	case 3:	// Be
	case 4:	// B
	case 5: // C
	case 6: // N
	case 7: // O
	case 8: // F
	case 9: // Ne
		return 2;
	case 10: // Na
	case 11: // Mg
	case 12: // Al
	case 13: // Si
	case 14: // P
	case 15: // S
	case 16: // Cl
	case 17: // Ar
		return 3;
	case 18: // K
	case 19: // Ca
	case 20: // Sc
	case 21: // Ti
	case 22: // V
	case 23: // Cr
	case 24: // Mn
	case 25: // Fe
	case 26: // Co
	case 27: // Ni
	case 28: // Cu
	case 29: // Zn
	case 30: // Ga
	case 31: // Ge
	case 32: // As
	case 33: // Se
	case 34: // Br
	case 35: // Kr
		return 4;
	case 36: // Rb
	case 37: // Sr
	case 38: // Y
	case 39: // Zr
	case 40: // Nb
	case 41: // Mo
	case 42: // Tc
	case 43: // Ru
	case 44: // Rh
	case 45: // Pd
	case 46: // Ag
	case 47: // Cd
	case 48: // In
	case 49: // Sn
	case 50: // Sb
	case 51: // Te
	case 52: // I
	case 53: // Xe
		return 5;
	case 54: // Cs
	case 55: // Ba
	case 56: // La
	case 57: // Ce
	case 58: // Pr
	case 59: // Nb	
	case 60: // Pm
	case 61: // Sm
	case 62: // Eu
	case 63: // Gd
	case 64: // Tb
	case 65: // Dy
	case 66: // Ho
	case 67: // Er
	case 68: // Tm
	case 69: // Yb
	case 70: // Lu
	case 71: // Hf
	case 72: // Ta
	case 73: // W
	case 74: // Re
	case 75: // Os
	case 76: // Ir
	case 77: // Pt
	case 78: // Au
	case 79: // Hg
	case 80: // Tl
	case 81: // Pb
	case 82: // Bi
	case 83: // Po
	case 84: // At
	case 85: // Rn
		return 6;
	case 86: // Fr
	case 87: // Ra
	case 88: // Ac
	case 89: // Th
	case 90: // Pa
	case 91: // U
	case 92: // Np
	case 93: // Pu
	case 94: // Am
	case 95: // Cm
	case 96: // Bk
	case 97: // Cf
	case 98: // Es
	case 99: // Fm
	case 100: // Md
	case 101: // No
	case 102: // Lr
	case 103: // Rf
	case 104: // Db
	case 105: // Sg
	case 106: // Bh
	case 107: // Hs
	case 108: // Mt
	case 109: // UUn
	case 110: // Uuu
	case 111: // Uub
		return 7;
	default:
	{
		AfxMessageBox("Invalid atom number in CLattice",MB_OK);
		return 0;
	}
	}
}

//////////////////////////////////////////////////////////////////////////
//	returns the number of p-shell functions on atoms
//
int CLattice::pFunctions(int atomNumber)
{
	int size = this->GetSize();
	if(atomNumber < 0 || atomNumber >= size)
	{
		AfxMessageBox("Invalid atom number in CLattice",MB_OK);
		return 0;
	}
// atom type
	int type = this->m_type[atomNumber];

	switch(type)
	{
	case 0:	// H
	case 1: // He
		return 0;
	case 2:	// Li
	case 3:	// Be
	case 4:	// B
	case 5: // C
	case 6: // N
	case 7: // O
	case 8: // F
	case 9: // Ne
	case 10: // Na
	case 11: // Mg
		return 1;
	case 12: // Al
	case 13: // Si
	case 14: // P
	case 15: // S
	case 16: // Cl
	case 17: // Ar
	case 18: // K
	case 19: // Ca
	case 20: // Sc
	case 21: // Ti
	case 22: // V
	case 23: // Cr
	case 24: // Mn
	case 25: // Fe
	case 26: // Co
	case 27: // Ni
	case 28: // Cu
	case 29: // Zn
		return 2;
	case 30: // Ga
	case 31: // Ge
	case 32: // As
	case 33: // Se
	case 34: // Br
	case 35: // Kr
	case 36: // Rb
	case 37: // Sr
	case 38: // Y
	case 39: // Zr
	case 40: // Nb
	case 41: // Mo
	case 42: // Tc
	case 43: // Ru
	case 44: // Rh
	case 45: // Pd
	case 46: // Ag
	case 47: // Cd
		return 3;
	case 48: // In
	case 49: // Sn
	case 50: // Sb
	case 51: // Te
	case 52: // I
	case 53: // Xe
	case 54: // Cs
	case 55: // Ba
	case 56: // La
	case 57: // Ce
	case 58: // Pr
	case 59: // Nb	
	case 60: // Pm
	case 61: // Sm
	case 62: // Eu
	case 63: // Gd
	case 64: // Tb
	case 65: // Dy
	case 66: // Ho
	case 67: // Er
	case 68: // Tm
	case 69: // Yb
	case 70: // Lu
	case 71: // Hf
	case 72: // Ta
	case 73: // W
	case 74: // Re
	case 75: // Os
	case 76: // Ir
	case 77: // Pt
	case 78: // Au
	case 79: // Hg
		return 4;
	case 80: // Tl
	case 81: // Pb
	case 82: // Bi
	case 83: // Po
	case 84: // At
	case 85: // Rn
	case 86: // Fr
	case 87: // Ra
	case 88: // Ac
	case 89: // Th
	case 90: // Pa
	case 91: // U
	case 92: // Np
	case 93: // Pu
	case 94: // Am
	case 95: // Cm
	case 96: // Bk
	case 97: // Cf
	case 98: // Es
	case 99: // Fm
	case 100: // Md
	case 101: // No
		return 5;
	case 102: // Lr
		return 6;
	case 103: // Rf
	case 104: // Db
	case 105: // Sg
	case 106: // Bh
	case 107: // Hs
	case 108: // Mt
	case 109: // UUn
	case 110: // Uuu
	case 111: // Uub
		return 5;
	default:
	{
		AfxMessageBox("Invalid atom number in CLattice",MB_OK);
		return 0;
	}
	}
}
//////////////////////////////////////////////////////////////////////////
//	returns the number of d-shell functions on atoms
//
int CLattice::dFunctions(int atomNumber)
{
	int size = this->GetSize();
	if(atomNumber < 0 || atomNumber >= size)
	{
		AfxMessageBox("Invalid atom number in CLattice",MB_OK);
		return 0;
	}
// atom type
	int type = this->m_type[atomNumber];

	switch(type)
	{
	case 0:	// H
	case 1: // He
	case 2:	// Li
	case 3:	// Be
	case 4:	// B
	case 5: // C
	case 6: // N
	case 7: // O
	case 8: // F
	case 9: // Ne
	case 10: // Na
	case 11: // Mg
	case 12: // Al
	case 13: // Si
	case 14: // P
	case 15: // S
	case 16: // Cl
	case 17: // Ar
	case 18: // K
	case 19: // Ca
		return 0;
	case 20: // Sc
	case 21: // Ti
	case 22: // V
	case 23: // Cr
	case 24: // Mn
	case 25: // Fe
	case 26: // Co
	case 27: // Ni
	case 28: // Cu
	case 29: // Zn
	case 30: // Ga
	case 31: // Ge
	case 32: // As
	case 33: // Se
	case 34: // Br
	case 35: // Kr
	case 36: // Rb
	case 37: // Sr
		return 1;
	case 38: // Y
	case 39: // Zr
	case 40: // Nb
	case 41: // Mo
	case 42: // Tc
	case 43: // Ru
	case 44: // Rh
	case 45: // Pd
	case 46: // Ag
	case 47: // Cd
		return 2;
	case 48: // In
	case 49: // Sn
	case 50: // Sb
	case 51: // Te
	case 52: // I
	case 53: // Xe
	case 54: // Cs
	case 55: // Ba
	case 56: // La
	case 57: // Ce
	case 58: // Pr
	case 59: // Nb	
	case 60: // Pm
	case 61: // Sm
	case 62: // Eu
	case 63: // Gd
	case 64: // Tb
	case 65: // Dy
	case 66: // Ho
	case 67: // Er
	case 68: // Tm
	case 69: // Yb
	case 70: // Lu
	case 71: // Hf
	case 72: // Ta
	case 73: // W
	case 74: // Re
	case 75: // Os
	case 76: // Ir
	case 77: // Pt
	case 78: // Au
	case 79: // Hg
	case 80: // Tl
	case 81: // Pb
	case 82: // Bi
	case 83: // Po
	case 84: // At
	case 85: // Rn
	case 86: // Fr
	case 87: // Ra
		return 3;
	case 88: // Ac
	case 89: // Th
	case 90: // Pa
	case 91: // U
	case 92: // Np
		return 4;
	case 93: // Pu
	case 94: // Am
		return 3;
	case 95: // Cm
		return 4;
	case 96: // Bk
	case 97: // Cf
	case 98: // Es
	case 99: // Fm
	case 100: // Md
	case 101: // No
		return 3;
	case 102: // Lr
	case 103: // Rf
	case 104: // Db
	case 105: // Sg
	case 106: // Bh
	case 107: // Hs
	case 108: // Mt
	case 109: // UUn
	case 110: // Uuu
	case 111: // Uub
		return 4;
	default:
	{
		AfxMessageBox("Invalid atom number in CLattice",MB_OK);
		return 0;
	}
	}
}
