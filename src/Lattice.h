// Lattice.h: interface for the CLattice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LATTICE_H__FFB7DCB9_60C6_4029_8FA9_643867016B35__INCLUDED_)
#define AFX_LATTICE_H__FFB7DCB9_60C6_4029_8FA9_643867016B35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

class CMadelungDoc;

class CLattice : public CObject  
{
public:
// Construction / destruction
	CLattice();
	virtual ~CLattice();

// Member variables
public:
	vector<double>m_X;				// cartesian X coordinate of an atom
	vector<double>m_Y;				// cartesian Y coordinate of an atom
	vector<double>m_Z;				// cartesian Z coordinate of an atom
	vector<double>m_charge;			// charge of an atom
	vector<int>m_type;				// contains the type of an atom
	vector<char> m_WPosition;		// Wyckoff position of atom
	vector<bool>m_selected;			// true if atom sselected
	vector<bool>m_rendered;			// true if atom is rendered, ie, has an associated actor
// contains index of the atom in initial unit cell
// all translated atoms here contain index of the original atom in the UnitCell object
	vector<int>m_atomInUnitCell;
// Next items indicates to which unit cell an atom belongs.
// It is used in the UnitCell::AddToLattice() method to prevent drawing an atom two
// times in the same point of the (x,y,z) space.
	vector<int>m_unitCellA;			// unit cell location in the lattice (along A axis)
	vector<int>m_unitCellB;			// unit cell location in the lattice (along B axis)
	vector<int>m_unitCellC;			// unit cell location in the lattice (along C axis)
// 'true' if atom is symmetrically independent, 'false' if it can be obtained by a symmetry
//  operation from any symmetrically independent atom.
	vector<bool>m_SymmetricallyIndependent;
// contains a number of parent atom, from which this atom can be obtained by a
// refined (from Symmetry.dll) symmetry operation, consistent with Wyckoff site symmetry.
	vector<int>m_SymmetryParentAtom;

// Member functions
public:
// determines whether atoms are bonded. a,b,c - squared distances for check for bond
	bool AtomsAreBonded(int i,int j, double a,double b,double c);
	void Clear();	// used for new document initialization
// checks whether a unitcell already exists at (na,nb,nc)
	bool UnitCellExists(int na,int nb,int nc);
// returns the size of the CLattice object
	int GetSize() {return this->m_X.size();}
// returns the distance between two CLattice atoms
	double GetDistance(int i,int j);
// returns the number of selected atoms
	int SelectedAtoms();
// returns the number of symmetry independent atoms
	int SymmetryIndependentAtoms();
// returns the number of s,p,d function sets for an atom 'atomNumber' in CLattice
// example: 1s,2s,3s - 3 sets; 2p - 1 set; 3d,4d - 2 sets
	int sFunctions(int atomNumber);
	int pFunctions(int atomNumber);
	int dFunctions(int atomNumber);
};

#endif // !defined(AFX_LATTICE_H__FFB7DCB9_60C6_4029_8FA9_643867016B35__INCLUDED_)
