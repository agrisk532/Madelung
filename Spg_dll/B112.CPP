// B112.cpp: implementation of the B112 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "B112.h"
#include "SimpleMonoclinic_C.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

B112::B112()
{
	this->m_pageInInternationalTables = "p.121";
	this->m_SpaceGroupNumber = 5005;
	this->m_Short_notation = "C2";
	this->m_Long_notation = "B112";
	this->m_Schoenflies = "C2(3)";
	this->m_Hall = "?";
	this->m_PointGroup = "2";
	this->m_CrystalSystem = "Monoclinic";
	this->m_LatticeSymbol = "B - centered";
	this->m_additionalInfo = "unique axis c, cell choice 2";
	this->m_pUnitCell = new SimpleMonoclinic_C;
	this->m_numberOfWyckoffSets = 3;
	this->m_numberOfCenteringTranslations = 2;
//------------------------------------------------------------------------
// multiplicity as in international tables
	this->m_WMultiplicity = new int[this->m_numberOfWyckoffSets];
	this->m_WMultiplicity[0] = 4;
	this->m_WMultiplicity[1] = 2;
	this->m_WMultiplicity[2] = 2;
// Wyckoff symbols
	this->m_WSymbol = new char[this->m_numberOfWyckoffSets];
	this->m_WSymbol[0] = 'c';
	this->m_WSymbol[1] = 'b';
	this->m_WSymbol[2] = 'a';
// site symmetry
	this->m_WSymmetry = new string[this->m_numberOfWyckoffSets];
	this->m_WSymmetry[0] = "1";
	this->m_WSymmetry[1] = "2";
	this->m_WSymmetry[2] = "2";

	int pos=0;
	for(int i=0;i<m_numberOfWyckoffSets;i++)
		pos += this->m_WMultiplicity[i];
// symbolic coordinates
	this->m_Coordinates = new string[pos*m_numberOfCenteringTranslations];
// c
	m_Coordinates[0] = "x,y,z";
	m_Coordinates[1] = "-x,-y,z";
// b
	m_Coordinates[2] = "1/2,1/2,z";
// a
	m_Coordinates[3] = "0,0,z";
// centering translations
	this->m_CenteringTranslations = new string[m_numberOfCenteringTranslations];
	m_CenteringTranslations[0] = "(0,0,0)+";
	m_CenteringTranslations[1] = "(1/2,0,1/2)+";

}

B112::~B112()
{
}


////////////////////////////////////////////////////////////////////////
//
EqPositions* B112::GetEqPos(double xyz[3], char Wpos)
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
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = 0+0.5;
		m_EqPos->m_Y[1] = 0;
		m_EqPos->m_Z[1] = z+0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;

	case 'b':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = 0.5+0.5;
		m_EqPos->m_Y[1] = 0.5;
		m_EqPos->m_Z[1] = z+0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;

	case 'c':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = z;

		for(i=2;i<4;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2];
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.5;
		}

		m_WIsEditable_X = true;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = true;
		break;
	default:
		return NULL;				
	}
	return m_EqPos;
}
