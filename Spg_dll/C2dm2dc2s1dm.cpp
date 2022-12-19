// C2dm2dc2s1dm.cpp: implementation of the C2dm2dc2s1dm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C2dm2dc2s1dm.h"
#include "SimpleOrthorombic.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C2dm2dc2s1dm::C2dm2dc2s1dm()
{
	this->m_pageInInternationalTables = "p.290";
	this->m_SpaceGroupNumber = 63;
	this->m_Short_notation = "Cmcm";
	this->m_Long_notation = "C 2/m 2/c 21/m";
	this->m_Schoenflies = "D2h(17)";
	this->m_Hall = "?";
	this->m_PointGroup = "mmm";
	this->m_CrystalSystem = "Orthorhombic";
	this->m_LatticeSymbol = "C - centered";
	this->m_additionalInfo = "unique axis c";
	this->m_pUnitCell = new SimpleOrthorombic;
	this->m_numberOfWyckoffSets = 8;
	this->m_numberOfCenteringTranslations = 2;
//------------------------------------------------------------------------
// multiplicity as in international tables
	this->m_WMultiplicity = new int[this->m_numberOfWyckoffSets];
	this->m_WMultiplicity[0] = 16;
	this->m_WMultiplicity[1] = 8;
	this->m_WMultiplicity[2] = 8;
	this->m_WMultiplicity[3] = 8;
	this->m_WMultiplicity[4] = 8;
	this->m_WMultiplicity[5] = 4;
	this->m_WMultiplicity[6] = 4;
	this->m_WMultiplicity[7] = 4;
// Wyckoff Symbol
	this->m_WSymbol = new char[this->m_numberOfWyckoffSets];
	this->m_WSymbol[0] = 'h';
	this->m_WSymbol[1] = 'g';
	this->m_WSymbol[2] = 'f';
	this->m_WSymbol[3] = 'e';
	this->m_WSymbol[4] = 'd';
	this->m_WSymbol[5] = 'c';
	this->m_WSymbol[6] = 'b';
	this->m_WSymbol[7] = 'a';
// site symmetry
	this->m_WSymmetry = new string[this->m_numberOfWyckoffSets];
	this->m_WSymmetry[0] = "1";
	this->m_WSymmetry[1] = "..m";
	this->m_WSymmetry[2] = "m..";
	this->m_WSymmetry[3] = "2..";
	this->m_WSymmetry[4] = "-1";
	this->m_WSymmetry[5] = "m2m";
	this->m_WSymmetry[6] = "2/m..";
	this->m_WSymmetry[7] = "2/m..";

	int pos=0;
	for(int i=0;i<m_numberOfWyckoffSets;i++)
		pos += this->m_WMultiplicity[i];
// symbolic coordinates
	this->m_Coordinates = new string[pos/this->m_numberOfCenteringTranslations];
// h
	m_Coordinates[0] = "x,y,z";
	m_Coordinates[1] = "-x,-y,z+1/2";
	m_Coordinates[2] = "-x,y,-z+1/2";
	m_Coordinates[3] = "x,-y,-z";
	m_Coordinates[4] = "-x,-y,-z";
	m_Coordinates[5] = "x,y,-z+1/2";
	m_Coordinates[6] = "x,-y,z+1/2";
	m_Coordinates[7] = "-x,y,z";
// g
	m_Coordinates[8] = "x,y,1/4";
	m_Coordinates[9] = "-x,-y,3/4";
	m_Coordinates[10] = "-x,y,1/4";
	m_Coordinates[11] = "x,-y,3/4";
// f
	m_Coordinates[12] = "0,y,z";
	m_Coordinates[13] = "0,-y,z+1/2";
	m_Coordinates[14] = "0,y,-z+1/2";
	m_Coordinates[15] = "0,-y,-z";
// e
	m_Coordinates[16] = "x,0,0";
	m_Coordinates[17] = "-x,0,1/2";
	m_Coordinates[18] = "-x,0,0";
	m_Coordinates[19] = "x,0,1/2";
// d
	m_Coordinates[20] = "1/4,1/4,0";
	m_Coordinates[21] = "3/4,3/4,1/2";
	m_Coordinates[22] = "3/4,1/4,1/2";
	m_Coordinates[23] = "1/4,3/4,0";
// c
	m_Coordinates[24] = "0,y,1/4";
	m_Coordinates[25] = "0,-y,3/4";
// b
	m_Coordinates[26] = "0,1/2,0";
	m_Coordinates[27] = "0,1/2,1/2";
// a
	m_Coordinates[28] = "0,0,0";
	m_Coordinates[29] = "0,0,1/2";

	this->m_CenteringTranslations = new string[m_numberOfCenteringTranslations];
	m_CenteringTranslations[0] = "(0,0,0)+";
	m_CenteringTranslations[1] = "(1/2,1/2,0)+";

}

C2dm2dc2s1dm::~C2dm2dc2s1dm()
{
}


////////////////////////////////////////////////////////////////////////
//
EqPositions* C2dm2dc2s1dm::GetEqPos(double xyz[3], char Wpos)
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
		m_EqPos->m_Z[0] = 0;

		m_EqPos->m_X[1] = 0;
		m_EqPos->m_Y[1] = 0;
		m_EqPos->m_Z[1] = 0.5;

		for(i=2;i<4;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.;
		}
		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'b':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0;

		m_EqPos->m_X[1] = 0;
		m_EqPos->m_Y[1] = 0.5;
		m_EqPos->m_Z[1] = 0.5;

		for(i=2;i<4;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.;
		}
		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'c':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0.25;

		m_EqPos->m_X[1] = 0;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = 0.75;

		for(i=2;i<4;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.;
		}
		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;
	case 'd':
		m_EqPos->m_X[0] = 0.25;
		m_EqPos->m_Y[0] = 0.25;
		m_EqPos->m_Z[0] = 0;

		m_EqPos->m_X[1] = 0.75;
		m_EqPos->m_Y[1] = 0.75;
		m_EqPos->m_Z[1] = 0.5;

		m_EqPos->m_X[2] = 0.75;
		m_EqPos->m_Y[2] = 0.25;
		m_EqPos->m_Z[2] = 0.5;

		m_EqPos->m_X[3] = 0.25;
		m_EqPos->m_Y[3] = 0.75;
		m_EqPos->m_Z[3] = 0;

		for(i=4;i<8;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-4] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-4] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-4] + 0.;
		}
		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'e':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = 0;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0;
		m_EqPos->m_Z[1] = 0.5;

		m_EqPos->m_X[2] = -x;
		m_EqPos->m_Y[2] = 0;
		m_EqPos->m_Z[2] = 0;

		m_EqPos->m_X[3] = x;
		m_EqPos->m_Y[3] = 0;
		m_EqPos->m_Z[3] = 0.5;

		for(i=4;i<8;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-4] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-4] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-4] + 0.;
		}
		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'f':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = 0;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = z+0.5;

		m_EqPos->m_X[2] = 0;
		m_EqPos->m_Y[2] = y;
		m_EqPos->m_Z[2] = -z+0.5;

		m_EqPos->m_X[3] = 0;
		m_EqPos->m_Y[3] = -y;
		m_EqPos->m_Z[3] = -z;

		for(i=4;i<8;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-4] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-4] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-4] + 0.;
		}
		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = true;
		break;
	case 'g':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0.25;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = 0.75;

		m_EqPos->m_X[2] = -x;
		m_EqPos->m_Y[2] = y;
		m_EqPos->m_Z[2] = 0.25;

		m_EqPos->m_X[3] = x;
		m_EqPos->m_Y[3] = -y;
		m_EqPos->m_Z[3] = 0.75;

		for(i=4;i<8;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-4] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-4] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-4] + 0.;
		}
		m_WIsEditable_X = true;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;
	case 'h':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = z+0.5;

		m_EqPos->m_X[2] = -x;
		m_EqPos->m_Y[2] = y;
		m_EqPos->m_Z[2] = -z+0.5;

		m_EqPos->m_X[3] = x;
		m_EqPos->m_Y[3] = -y;
		m_EqPos->m_Z[3] = -z;

		m_EqPos->m_X[4] = -x;
		m_EqPos->m_Y[4] = -y;
		m_EqPos->m_Z[4] = -z;

		m_EqPos->m_X[5] = x;
		m_EqPos->m_Y[5] = y;
		m_EqPos->m_Z[5] = -z+0.5;

		m_EqPos->m_X[6] = x;
		m_EqPos->m_Y[6] = -y;
		m_EqPos->m_Z[6] = z+0.5;

		m_EqPos->m_X[7] = -x;
		m_EqPos->m_Y[7] = y;
		m_EqPos->m_Z[7] = z;

		for(i=8;i<16;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-4] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-4] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-4] + 0.;
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
