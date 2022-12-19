///////////////////////////////////////////////////////////////////////
//
// header file for the spacegroup class
//
///////////////////////////////////////////////////////////////////////

#if !defined __spgdll_h
#define __spgdll_h

#include "win32HeaderSG.h"	//	dll export/import stuff
#include "EqPositions.h"
#include "UnitCell.h"

#include <string>
using namespace std;

////////////////////////////////////////////////////////////////////////
// base class for all spacegroups
////////////////////////////////////////////////////////////////////////

class SPG_DLL_API SpaceGroup
{
public:
// general info
	int m_SpaceGroupNumber;	// according to the international tables
	string m_Short_notation;	// H-M international notation of the space group
	string m_Long_notation;
	string m_Schoenflies;
	string m_Hall;
	string m_PointGroup;
	string m_CrystalSystem;
	string m_LatticeSymbol;
	string m_additionalInfo;
	string m_pageInInternationalTables;		// on which page the spacegroup is given
	int m_numberOfCenteringTranslations;	// for I,C,F structures
	int m_numberOfWyckoffSets;
// Wyckoff parameters for all Wyckoff sets
	int* m_WMultiplicity;
	char* m_WSymbol;
	string* m_WSymmetry;
	string* m_Coordinates;	// symbolic coordinates
	string* m_CenteringTranslations;
// contains all eq positions (only coordinates)
	EqPositions* m_EqPos;
// the object containing coordinates of atoms and unitcell parameters
	UnitCell* m_pUnitCell;
// enable/disable the coordinate editing for a Wuckoff position
	bool m_WIsEditable_X;
	bool m_WIsEditable_Y;
	bool m_WIsEditable_Z;
// output text message buffer
	char m_outputText[1000];

public:
	SpaceGroup();
	virtual ~SpaceGroup();
// constructor accepts the number from the International Tables and creates corresponding
// space group
	static SpaceGroup* New(int sgNumber);	
// gets equivalent positions' coordinates for the particular Wyckoff symbol
	int GetWMultiplicity(char Wpos);
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos) = 0;
// calculates ALL atoms in ONE UnitCell (expands Wyckoff positions)
// returns 0 if no duplicate atoms, otherwise the number of duplicates
	int CalculateUnitCellAtoms();
// find the Wyckoff position index
	int FindWPositionIndex(char pos);
	void ClearEqPos();	// clears the EqPos object
};

#endif
