#ifndef __UnitCell_h
#define __UnitCell_h

#include "win32Header.h"

#include <vector>
using namespace std;

class UNITCELLDLL_API UnitCell
{

public:
	UnitCell();
	virtual ~UnitCell();

// Member variables
protected:
// conversion factor between degrees and radians
	double FACTOR;
public:
// ID of unit cell (used in PropertyPageSpaceGroup for input field validation)
	int m_ID;

	double m_a;
	double m_b;
	double m_c;
	double m_alpha;
	double m_beta;
	double m_gamma;

//	number of positions in UnitCell (one atom for each Wyckoff position)
	int m_positionsInUnitCell;
// number of atoms in unit cell (ALL atoms in UnitCell. Wyckoff positions expanded)
	int m_atomsInUnitCell;	
// unit cell parameters for atoms in units of unit cell lengths (a,b,c)
// stored are only the symmetrically independent atoms (1 atom for the Wyckoff set)
// the rest is calculated in the spaceGroup->GetEqPos() and filled in the Lattice object
	vector<char> m_WyckoffPosition;	// contains the Wyckoff position in the unit cell
	vector<double> m_unitCellAtomX;
	vector<double> m_unitCellAtomY;
	vector<double> m_unitCellAtomZ;
	vector<double> m_unitCellAtomCharge;
	vector<int> m_unitCellAtomType;
// contains cartesian coordinates and parameters of ALL atoms in a unit cell
	vector<double> m_unitCellAtomCartX;	
	vector<double> m_unitCellAtomCartY;
	vector<double> m_unitCellAtomCartZ;
	vector<double> m_unitCellAtomCartCharge;
	vector<int> m_unitCellAtomCartType;
	vector<char> m_unitCellAtomCartWPos;	// Wyckoff position
// electrostatic potential on unit cell atom from lattice. Calculated in work threads
	vector<double> m_unitCellAtomCartPotential;
// cartesian coordinates of unit cell vectors
	double m_unitCellVectorA_Cart[3];
	double m_unitCellVectorB_Cart[3];
	double m_unitCellVectorC_Cart[3];
// reciprocal lattice vectors
	double m_recLatticeVectorA[3];
	double m_recLatticeVectorB[3];
	double m_recLatticeVectorC[3];
// shows whether editing enabled
	bool m_enabledAEditing;
	bool m_enabledBEditing;
    bool m_enabledCEditing;
	bool m_enabledAlphaEditing;
	bool m_enabledBetaEditing;
	bool m_enabledGammaEditing;
//	hint strings for editing a,b,c, alpha,beta,gamma
	string m_abcRestrictions;
	string m_angleRestrictions;
// cartesian coordinates of vertices forming a unitcell
	double m_unitCellVertices[8][3];
// volume of the unit cell
	double m_volume;

// Member Functions
public:
// calculates the unit cell vector cartesian coordinates and stores in m_unitCellVectorCart
	virtual void CalculateVectors() = 0;
// calculates reciprocal latice unit vectors
	void CalculateReciprocalLatticeVectors();
// calculates volume of the unit cell
	double GetVolume();
// calculates a unitcell atom cartesian coordinates and stores in m_unitCellAtomCart.
// 'index' is to copy the correct atom charge and type values from
// m_unitCellAtomCharge to m_unitCellAtomCartCharge
	void CalculateAtoms(double xyz[3],int index);
// used for file->new document
	void Clear();
// calculate cartesian coordinates of all 8 vertices forming a unitCell
	void CalculateVertices();
// checks whether unitcell vertices are bonded
	bool VerticesAreBonded(int i,int j);
// calculates the distance between two points in m_unitCellVertices[][]
	double Distance(int i,int j);
// compute the unitcell coordinates from the cartesian coordinates for geomCenter[]
// (coordinate origins must be the same)
	int  CalcUCCoordFromCartCoord(double geomCenter[3],double geomCenter_UnitCell[3]);
// calculates the cartesian coordinates from the unitcell coordinates
	void CalcCartCoordFromUCCoord(double a_UnitCell[3],double a_Cartesian[3]);
// Adds an atom to the unitcell. coord[3] contains the center of the molecule, in the
// units of unitcell axis lengths. Coord[3] contains cartesian shifts of atoms from the center[3]
	void AddAtomCart(double center[3],double coord[3],int type,double charge);
protected:
// vector product function
	void VectProd(double a[3],double b[3],double c[3]);
// invert 3x3 matrix
	int InvertMatrix(double P[3][3],double P_inv[3][3]);
// multiply 3x3 matrices
	void MultiplyMatrices(double a[3][3], double b[3][3], double c[3][3]);
};

#endif
