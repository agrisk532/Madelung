// P1m1.cpp: implementation of the P1m1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "P1m1.h"
#include "SimpleMonoclinic_B.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

P1m1::P1m1()
{
	this->m_pageInInternationalTables = "p.122";
	this->m_SpaceGroupNumber = 6001;
	this->m_Short_notation = "Pm";
	this->m_Long_notation = "P1m1";
	this->m_Schoenflies = "Cs(1)";
	this->m_Hall = "?";
	this->m_PointGroup = "m";
	this->m_CrystalSystem = "Monoclinic";
	this->m_LatticeSymbol = "P - primitive";
	this->m_additionalInfo = "unique axis b";
	this->m_pUnitCell = new SimpleMonoclinic_B;
	this->m_numberOfWyckoffSets = 3;
	this->m_numberOfCenteringTranslations = 1;
//------------------------------------------------------------------------
// multiplicity as in international tables
	this->m_WMultiplicity = new int[this->m_numberOfWyckoffSets];
	this->m_WMultiplicity[0] = 2;
	this->m_WMultiplicity[1] = 1;
	this->m_WMultiplicity[2] = 1;
// Wyckoff Symbol
	this->m_WSymbol = new char[this->m_numberOfWyckoffSets];
	this->m_WSymbol[0] = 'c';
	this->m_WSymbol[1] = 'b';
	this->m_WSymbol[2] = 'a';
// site symmetry
	this->m_WSymmetry = new string[this->m_numberOfWyckoffSets];
	this->m_WSymmetry[0] = "1";
	this->m_WSymmetry[1] = "m";
	this->m_WSymmetry[2] = "m";

	int pos=0;
	for(int i=0;i<m_numberOfWyckoffSets;i++)
		pos += this->m_WMultiplicity[i];
// symbolic coordinates
	this->m_Coordinates = new string[pos/this->m_numberOfCenteringTranslations];
	m_Coordinates[0] = "x,y,z";
	m_Coordinates[1] = "x,-y,z";
	m_Coordinates[2] = "x,1/2,z";
	m_Coordinates[3] = "x,0,z";
// centering translations
	this->m_CenteringTranslations = new string[m_numberOfCenteringTranslations];
	m_CenteringTranslations[0] = "(0,0,0)+";

}

P1m1::~P1m1()
{
}


////////////////////////////////////////////////////////////////////////
//
EqPositions* P1m1::GetEqPos(double xyz[3], char Wpos)
{
	int pos;	// matches the Wyckoff symbol with an integer number
	int i;
	double x = xyz[0];
	double y = xyz[1];
	double z = xyz[2];
// match the position symbol with number
	for(i=0;i<m_numberOfWyckoffSets;i++)
		if(m_WSymbol[i] == Wpos) pos = i;

	ClearEqPos();

	m_EqPos = new EqPositions(this->m_WMultiplicity[pos]);

// construct the coordinate dimensions according to the selected Wyckoff symbol
	switch(Wpos)
	{
	case 'a':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = z;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;

	case 'b':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = z;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;

	case 'c':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = x;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = z;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = true;
		break;
	default:
		return NULL;				
	}
	return m_EqPos;
}
