// I2dm2dm2dm.cpp: implementation of the I2dm2dm2dm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "I2dm2dm2dm.h"

#include "SimpleOrthorombic.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

I2dm2dm2dm::I2dm2dm2dm()
{
	this->m_pageInInternationalTables = "p.314";
	this->m_SpaceGroupNumber = 71;
	this->m_Short_notation = "Immm";
	this->m_Long_notation = "I 2/m 2/m 2/m";
	this->m_Schoenflies = "D2h(25)";
	this->m_Hall = "?";
	this->m_PointGroup = "mmm";
	this->m_CrystalSystem = "Orthorhombic";
	this->m_LatticeSymbol = "I - body centered";
	this->m_additionalInfo = "";
	this->m_pUnitCell = new SimpleOrthorombic;
	this->m_numberOfWyckoffSets = 15;
	this->m_numberOfCenteringTranslations = 2;
//------------------------------------------------------------------------
// multiplicity as in international tables
	this->m_WMultiplicity = new int[this->m_numberOfWyckoffSets];
	this->m_WMultiplicity[0] = 16;	// o
	this->m_WMultiplicity[1] = 8;	// n
	this->m_WMultiplicity[2] = 8;	// m
	this->m_WMultiplicity[3] = 8;	// l
	this->m_WMultiplicity[4] = 8;	// k
	this->m_WMultiplicity[5] = 4;	// j
	this->m_WMultiplicity[6] = 4;	// i
	this->m_WMultiplicity[7] = 4;	// h
	this->m_WMultiplicity[8] = 4;	// g
	this->m_WMultiplicity[9] = 4;	// f
	this->m_WMultiplicity[10] = 4;	// e
	this->m_WMultiplicity[11] = 2;	// d
	this->m_WMultiplicity[12] = 2;	// c
	this->m_WMultiplicity[13] = 2;	// b
	this->m_WMultiplicity[14] = 2;	// a

// Wyckoff Symbol
	this->m_WSymbol = new char[this->m_numberOfWyckoffSets];
	this->m_WSymbol[0] = 'o';
	this->m_WSymbol[1] = 'n';
	this->m_WSymbol[2] = 'm';
	this->m_WSymbol[3] = 'l';
	this->m_WSymbol[4] = 'k';
	this->m_WSymbol[5] = 'j';
	this->m_WSymbol[6] = 'i';
	this->m_WSymbol[7] = 'h';
	this->m_WSymbol[8] = 'g';
	this->m_WSymbol[9] = 'f';
	this->m_WSymbol[10] = 'e';
	this->m_WSymbol[11] = 'd';
	this->m_WSymbol[12] = 'c';
	this->m_WSymbol[13] = 'b';
	this->m_WSymbol[14] = 'a';

// site symmetry
	this->m_WSymmetry = new string[this->m_numberOfWyckoffSets];
	this->m_WSymmetry[0] = "1";			// o
	this->m_WSymmetry[1] = "..m";		// n
	this->m_WSymmetry[2] = ".m.";		// m
	this->m_WSymmetry[3] = "m..";		// l
	this->m_WSymmetry[4] = "-1";		// k
	this->m_WSymmetry[5] = "mm2";		// j
	this->m_WSymmetry[6] = "mm2";		// i
	this->m_WSymmetry[7] = "m2m";		// h
	this->m_WSymmetry[8] = "m2m";		// g
	this->m_WSymmetry[9] = "2mm";		// f
	this->m_WSymmetry[10] = "2mm";		// e
	this->m_WSymmetry[11] = "mmm";		// d
	this->m_WSymmetry[12] = "mmm";		// c
	this->m_WSymmetry[13] = "mmm";		// b
	this->m_WSymmetry[14] = "mmm";		// a

	int pos=0;
	for(int i=0;i<m_numberOfWyckoffSets;i++)
		pos += this->m_WMultiplicity[i];
// symbolic coordinates
	this->m_Coordinates = new string[pos/this->m_numberOfCenteringTranslations];
// o
	m_Coordinates[0] = "x,y,z";
	m_Coordinates[1] = "-x,-y,z";
	m_Coordinates[2] = "-x,y,-z";
	m_Coordinates[3] = "x,-y,-z";
	m_Coordinates[4] = "-x,-y,-z";
	m_Coordinates[5] = "x,y,-z";
	m_Coordinates[6] = "x,-y,z";
	m_Coordinates[7] = "-x,y,z";

// n
	m_Coordinates[8] = "x,y,0";
	m_Coordinates[9] = "-x,-y,0";
	m_Coordinates[10] = "-x,y,0";
	m_Coordinates[11] = "x,-y,0";
// m
	m_Coordinates[12] = "x,0,z";
	m_Coordinates[13] = "-x,0,z";
	m_Coordinates[14] = "-x,0,-z";
	m_Coordinates[15] = "x,0,-z";
// l
	m_Coordinates[16] = "0,y,z";
	m_Coordinates[17] = "0,-y,z";
	m_Coordinates[18] = "0,y,-z";
	m_Coordinates[19] = "0,-y,-z";
// k
	m_Coordinates[20] = "1/4,1/4,1/4";
	m_Coordinates[21] = "3/4,3/4,1/4";
	m_Coordinates[22] = "3/4,1/4,3/4";
	m_Coordinates[23] = "1/4,3/4,3/4";
// j
	m_Coordinates[24] = "1/2,0,z";
	m_Coordinates[25] = "1/2,0,-z";
// i
	m_Coordinates[26] = "0,0,z";
	m_Coordinates[27] = "0,0,-z";
// h
	m_Coordinates[28] = "0,y,1/2";
	m_Coordinates[29] = "0,-y,1/2";
// g
	m_Coordinates[30] = "0,y,0";
	m_Coordinates[31] = "0,-y,0";
// f
	m_Coordinates[32] = "x,1/2,0";
	m_Coordinates[33] = "-x,1/2,0";
// e
	m_Coordinates[34] = "x,0,0";
	m_Coordinates[35] = "-x,0,0";
// d
	m_Coordinates[36] = "1/2,0,1/2";
// c
	m_Coordinates[37] = "1/2,1/2,0";
// b
	m_Coordinates[38] = "0,0,1/2";
// a
	m_Coordinates[39] = "0,0,0";

	this->m_CenteringTranslations = new string[m_numberOfCenteringTranslations];
	m_CenteringTranslations[0] = "(0,0,0)+";
	m_CenteringTranslations[0] = "(1/2,1/2,1/2)+";
}

I2dm2dm2dm::~I2dm2dm2dm()
{
}


////////////////////////////////////////////////////////////////////////
//
EqPositions* I2dm2dm2dm::GetEqPos(double xyz[3], char Wpos)
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

		m_EqPos->m_X[1] = 0+0.5;
		m_EqPos->m_Y[1] = y+0.5;
		m_EqPos->m_Z[1] = 0+0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;

	case 'b':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0.5;

		m_EqPos->m_X[1] = 0  +0.5;
		m_EqPos->m_Y[1] = 0.5+0.5;
		m_EqPos->m_Z[1] = 0.5+0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;

	case 'c':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0;

		m_EqPos->m_X[1] = 0.5+0.5;
		m_EqPos->m_Y[1] = 0.5+0.5;
		m_EqPos->m_Z[1] = 0  +0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;

	case 'd':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0.;
		m_EqPos->m_Z[0] = 0.5;

		m_EqPos->m_X[1] = 0.5+0.5;
		m_EqPos->m_Y[1] = 0. +0.5;
		m_EqPos->m_Z[1] = 0.5+0.5;

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
		m_EqPos->m_Z[1] = 0;

		for(i=2;i<4;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.5;
		}

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;

	case 'f':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0.5;
		m_EqPos->m_Z[1] = 0;

		for(i=2;i<4;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.5;
		}

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;

	case 'g':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0;

		m_EqPos->m_X[1] = 0;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = 0;

		for(i=2;i<4;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.5;
		}

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;

	case 'h':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0.5;

		m_EqPos->m_X[1] = 0;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = 0.5;

		for(i=2;i<4;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.5;
		}

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;

	case 'i':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = 0;
		m_EqPos->m_Y[1] = 0;
		m_EqPos->m_Z[1] = -z;

		for(i=2;i<4;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.5;
		}

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;

	case 'j':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = 0.5;
		m_EqPos->m_Y[1] = 0;
		m_EqPos->m_Z[1] = -z;

		for(i=2;i<4;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.5;
		}

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;

	case 'k':
		m_EqPos->m_X[0] = 0.25;
		m_EqPos->m_Y[0] = 0.25;
		m_EqPos->m_Z[0] = 0.25;

		m_EqPos->m_X[1] = 0.75;
		m_EqPos->m_Y[1] = 0.75;
		m_EqPos->m_Z[1] = 0.25;

		m_EqPos->m_X[2] = 0.75;
		m_EqPos->m_Y[2] = 0.25;
		m_EqPos->m_Z[2] = 0.75;

		m_EqPos->m_X[3] = 0.25;
		m_EqPos->m_Y[3] = 0.75;
		m_EqPos->m_Z[3] = 0.75;

		for(i=4;i<8;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.5;
		}

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;

	case 'l':
		m_EqPos->m_X[0] = 0.;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = 0.;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = z;

		m_EqPos->m_X[2] = 0.;
		m_EqPos->m_Y[2] = y;
		m_EqPos->m_Z[2] = -z;

		m_EqPos->m_X[3] = 0.;
		m_EqPos->m_Y[3] = -y;
		m_EqPos->m_Z[3] = -z;

		for(i=4;i<8;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.5;
		}

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = true;
		break;

	case 'm':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0.;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0.;
		m_EqPos->m_Z[1] = z;

		m_EqPos->m_X[2] = -x;
		m_EqPos->m_Y[2] = 0.;
		m_EqPos->m_Z[2] = -z;

		m_EqPos->m_X[3] = x;
		m_EqPos->m_Y[3] = 0.;
		m_EqPos->m_Z[3] = -z;

		for(i=4;i<8;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.5;
		}

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;

	case 'n':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0.;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = 0.;

		m_EqPos->m_X[2] = -x;
		m_EqPos->m_Y[2] = y;
		m_EqPos->m_Z[2] = 0.;

		m_EqPos->m_X[3] = x;
		m_EqPos->m_Y[3] = -y;
		m_EqPos->m_Z[3] = 0.;

		for(i=4;i<8;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-2] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-2] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-2] + 0.5;
		}

		m_WIsEditable_X = true;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;

	case 'o':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = z;

		m_EqPos->m_X[2] = -x;
		m_EqPos->m_Y[2] = y;
		m_EqPos->m_Z[2] = -z;

		m_EqPos->m_X[3] = x;
		m_EqPos->m_Y[3] = -y;
		m_EqPos->m_Z[3] = -z;

		m_EqPos->m_X[4] = -x;
		m_EqPos->m_Y[4] = -y;
		m_EqPos->m_Z[4] = -z;

		m_EqPos->m_X[5] = x;
		m_EqPos->m_Y[5] = y;
		m_EqPos->m_Z[5] = -z;

		m_EqPos->m_X[6] = x;
		m_EqPos->m_Y[6] = -y;
		m_EqPos->m_Z[6] = z;

		m_EqPos->m_X[7] = -x;
		m_EqPos->m_Y[7] = y;
		m_EqPos->m_Z[7] = z;

		for(i=8;i<16;i++)
		{
			m_EqPos->m_X[i] = m_EqPos->m_X[i-4] + 0.5;
			m_EqPos->m_Y[i] = m_EqPos->m_Y[i-4] + 0.5;
			m_EqPos->m_Z[i] = m_EqPos->m_Z[i-4] + 0.5;
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
