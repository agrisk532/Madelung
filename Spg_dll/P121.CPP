// P1.cpp: implementation of the P1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "P121.h"
#include "SimpleMonoclinic_B.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

P121::P121()
{
	this->m_pageInInternationalTables = "p.106";
	this->m_SpaceGroupNumber = 3001;
	this->m_Short_notation = "P2";
	this->m_Long_notation = "P121";
	this->m_Schoenflies = "C2(1)";
	this->m_Hall = "?";
	this->m_PointGroup = "2";
	this->m_CrystalSystem = "Monoclinic";
	this->m_LatticeSymbol = "P - primitive";
	this->m_additionalInfo = "unique axis b";
	this->m_pUnitCell = new SimpleMonoclinic_B;
	this->m_numberOfWyckoffSets = 5;
	this->m_numberOfCenteringTranslations = 1;
//------------------------------------------------------------------------
// multiplicity as in International tables
	this->m_WMultiplicity = new int[this->m_numberOfWyckoffSets];
	this->m_WMultiplicity[0] = 2;
	this->m_WMultiplicity[1] = 1;
	this->m_WMultiplicity[2] = 1;
	this->m_WMultiplicity[3] = 1;
	this->m_WMultiplicity[4] = 1;
// Wyckoff Symbols
	this->m_WSymbol = new char[this->m_numberOfWyckoffSets];
	this->m_WSymbol[0] = 'e';
	this->m_WSymbol[1] = 'd';
	this->m_WSymbol[2] = 'c';
	this->m_WSymbol[3] = 'b';
	this->m_WSymbol[4] = 'a';
// site symmetry
	this->m_WSymmetry = new string[this->m_numberOfWyckoffSets];
	this->m_WSymmetry[0] = "1";
	this->m_WSymmetry[1] = "2";
	this->m_WSymmetry[2] = "2";
	this->m_WSymmetry[3] = "2";
	this->m_WSymmetry[4] = "2";

	int pos=0;
	for(int i=0;i<m_numberOfWyckoffSets;i++)
		pos += this->m_WMultiplicity[i];
// symbolic coordinates
	this->m_Coordinates = new string[pos/this->m_numberOfCenteringTranslations];
// e
	m_Coordinates[0] = "x,y,z";
	m_Coordinates[1] = "-x,y,-z";
// d
	m_Coordinates[2] = "1/2,y,1/2";
// c
	m_Coordinates[3] = "1/2,y,0";
// b
	m_Coordinates[4] = "0,y,1/2";
// a
	m_Coordinates[5] = "0,y,0";
// centering translations
	this->m_CenteringTranslations = new string[this->m_numberOfCenteringTranslations];
	m_CenteringTranslations[0] = "(0,0,0)+";

}

P121::~P121()
{
}


////////////////////////////////////////////////////////////////////////
//
EqPositions* P121::GetEqPos(double xyz[3], char Wpos)
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
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;
	case 'b':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;
	case 'c':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;
	case 'd':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;
	case 'e':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = y;
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
