// P_1.cpp: implementation of the P_1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "P_1.h"
#include "Triclinic.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

P_1::P_1()
{
	this->m_pageInInternationalTables = "p.104";
	this->m_SpaceGroupNumber = 2;
	this->m_Short_notation = "P_1";
	this->m_Long_notation = "P_1";
	this->m_Schoenflies = "Ci(1)";
	this->m_PointGroup = "-1";
	this->m_Hall = "?";
	this->m_CrystalSystem = "Triclinic";
	this->m_LatticeSymbol = "P - primitive";
	this->m_additionalInfo = "";
	this->m_pUnitCell = new Triclinic;
	this->m_numberOfWyckoffSets = 9;
	this->m_numberOfCenteringTranslations = 1;
//------------------------------------------------------------------------
// multiplicity as in international tables
	this->m_WMultiplicity = new int[this->m_numberOfWyckoffSets];
	this->m_WMultiplicity[0] = 2;	// i
	this->m_WMultiplicity[1] = 1;	// h
	this->m_WMultiplicity[2] = 1;	// g
	this->m_WMultiplicity[3] = 1;	// f
	this->m_WMultiplicity[4] = 1;	// e
	this->m_WMultiplicity[5] = 1;	// d
	this->m_WMultiplicity[6] = 1;	// c
	this->m_WMultiplicity[7] = 1;	// b
	this->m_WMultiplicity[8] = 1;	// a
// Wyckoff Symbols
	this->m_WSymbol = new char[this->m_numberOfWyckoffSets];
	this->m_WSymbol[0] = 'i';
	this->m_WSymbol[1] = 'h';
	this->m_WSymbol[2] = 'g';
	this->m_WSymbol[3] = 'f';
	this->m_WSymbol[4] = 'e';
	this->m_WSymbol[5] = 'd';
	this->m_WSymbol[6] = 'c';
	this->m_WSymbol[7] = 'b';
	this->m_WSymbol[8] = 'a';
// site symmetry
	this->m_WSymmetry = new string[this->m_numberOfWyckoffSets];
	this->m_WSymmetry[0] = "1";
	this->m_WSymmetry[1] = "-1";
	this->m_WSymmetry[2] = "-1";
	this->m_WSymmetry[3] = "-1";
	this->m_WSymmetry[4] = "-1";
	this->m_WSymmetry[5] = "-1";
	this->m_WSymmetry[6] = "-1";
	this->m_WSymmetry[7] = "-1";
	this->m_WSymmetry[8] = "-1";

	int pos = 0;
	for(int i=0;i<m_numberOfWyckoffSets;i++)
		pos += this->m_WMultiplicity[i];
// symbolic coordinates
	this->m_Coordinates = new string[pos/this->m_numberOfCenteringTranslations];
// i
	m_Coordinates[0] = "x,y,z";
	m_Coordinates[1] = "-x,-y,-z";
// h
	m_Coordinates[2] = "1/2,1/2,1/2";
// g
	m_Coordinates[3] = "0,1/2,1/2";
// f
	m_Coordinates[4] = "1/2,0,1/2";
// e
	m_Coordinates[5] = "1/2,1/2,0";
// d
	m_Coordinates[6] = "1/2,0,0";
// c
	m_Coordinates[7] = "0,1/2,0";
// b
	m_Coordinates[8] = "0,0,1/2";
// a
	m_Coordinates[9] = "0,0,0";

	this->m_CenteringTranslations = new string[this->m_numberOfCenteringTranslations];
	m_CenteringTranslations[0] = "(0,0,0)+";

}

P_1::~P_1()
{
}


////////////////////////////////////////////////////////////////////////
//
EqPositions* P_1::GetEqPos(double xyz[3], char Wpos)
{
	int pos;	// matches the Wyckoff symbol with an integer number
	double x = xyz[0];
	double y = xyz[1];
	double z = xyz[2];
// match the position symbol with number
	for(int i=0;i<m_numberOfWyckoffSets;i++)
		if(m_WSymbol[i] == Wpos) pos = i;

	ClearEqPos();

	m_EqPos = new EqPositions(this->m_WMultiplicity[pos]);

// construct the coordinate dimensions according to the selected Wyckoff symbol
	switch(Wpos)
	{
	case 'a':
		m_EqPos->m_X[0] = 0.;
		m_EqPos->m_Y[0] = 0.;
		m_EqPos->m_Z[0] = 0.;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'b':
		m_EqPos->m_X[0] = 0.;
		m_EqPos->m_Y[0] = 0.;
		m_EqPos->m_Z[0] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'c':
		m_EqPos->m_X[0] = 0.;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0.;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;	
	case 'd':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0.;
		m_EqPos->m_Z[0] = 0.;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'e':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0.;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'f':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0.;
		m_EqPos->m_Z[0] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'g':
		m_EqPos->m_X[0] = 0.;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'h':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'i':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = -z;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = true;
		break;
	default:
		return NULL;
	}
	return m_EqPos;
}
