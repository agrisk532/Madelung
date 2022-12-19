// P1.cpp: implementation of the P1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "P1.h"
#include "Triclinic.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

P1::P1()
{
	this->m_pageInInternationalTables = "p.102";
	this->m_SpaceGroupNumber = 1;
	this->m_Short_notation = "P1";
	this->m_Long_notation = "P1";
	this->m_Schoenflies = "1";
	this->m_Hall = "?";
	this->m_PointGroup = "1";
	this->m_CrystalSystem = "Triclinic";
	this->m_LatticeSymbol = "P - primitive";
	this->m_additionalInfo = "";
	this->m_pUnitCell = new Triclinic;
	this->m_numberOfWyckoffSets = 1;
	this->m_numberOfCenteringTranslations = 1;
//------------------------------------------------------------------------
// multiplicity as in international tables
	this->m_WMultiplicity = new int[this->m_numberOfWyckoffSets];
	this->m_WMultiplicity[0] = 1;
// Wyckoff Symbols
	this->m_WSymbol = new char[this->m_numberOfWyckoffSets];
	this->m_WSymbol[0] = 'a';
// site symmetry
	this->m_WSymmetry = new string[this->m_numberOfWyckoffSets];
	this->m_WSymmetry[0] = "1";

	int pos=0;
	for(int i=0;i<m_numberOfWyckoffSets;i++)
		pos += this->m_WMultiplicity[i];
// symbolic coordinates
	this->m_Coordinates = new string[pos/this->m_numberOfCenteringTranslations];
// a
	m_Coordinates[0] = "x,y,z";
// centering translations
	this->m_CenteringTranslations = new string[m_numberOfCenteringTranslations];
	m_CenteringTranslations[0] = "(0,0,0)+";
	
}

P1::~P1()
{
}


////////////////////////////////////////////////////////////////////////
//
EqPositions* P1::GetEqPos(double xyz[3], char Wpos)
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
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = z;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = true;
		break;
	default:
		return NULL;
	}
	return m_EqPos;
}
