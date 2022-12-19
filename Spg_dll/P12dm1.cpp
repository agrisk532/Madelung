// P12dm1.cpp: implementation of the P112dm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "P12dm1.h"
#include "SimpleMonoclinic_C.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

P12dm1::P12dm1()
{
	this->m_pageInInternationalTables = "p.150";
	this->m_SpaceGroupNumber = 10002;
	this->m_Short_notation = "P2/m";
	this->m_Long_notation = "P12/m1";
	this->m_Schoenflies = "C2h(1)";
	this->m_Hall = "?";
	this->m_PointGroup = "2/m";
	this->m_CrystalSystem = "Monoclinic";
	this->m_LatticeSymbol = "P - primitive";
	this->m_additionalInfo = "unique axis b";
	this->m_pUnitCell = new SimpleMonoclinic_C;
	this->m_numberOfWyckoffSets = 15;
	this->m_numberOfCenteringTranslations = 1;
//------------------------------------------------------------------------
// multiplicity as in international tables
	this->m_WMultiplicity = new int[this->m_numberOfWyckoffSets];
	this->m_WMultiplicity[0] = 4;	// o
	this->m_WMultiplicity[1] = 2;	// n
	this->m_WMultiplicity[2] = 2;	// m
	this->m_WMultiplicity[3] = 2;	// l
	this->m_WMultiplicity[4] = 2;	// k
	this->m_WMultiplicity[5] = 2;	// j
	this->m_WMultiplicity[6] = 2;	// i
	this->m_WMultiplicity[7] = 1;	// h
	this->m_WMultiplicity[8] = 1;	// g
	this->m_WMultiplicity[9] = 1;	// f
	this->m_WMultiplicity[10] = 1;	// e
	this->m_WMultiplicity[11] = 1;	// d
	this->m_WMultiplicity[12] = 1;	// c
	this->m_WMultiplicity[13] = 1;	// b
	this->m_WMultiplicity[14] = 1;	// a

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
	this->m_WSymmetry[0] = "1";		// o
	this->m_WSymmetry[1] = "m";		// n
	this->m_WSymmetry[2] = "m";		// m
	this->m_WSymmetry[3] = "2";		// l
	this->m_WSymmetry[4] = "2";		// k
	this->m_WSymmetry[5] = "2";		// j
	this->m_WSymmetry[6] = "2";		// i
	this->m_WSymmetry[7] = "2/m";	// h
	this->m_WSymmetry[8] = "2/m";	// g
	this->m_WSymmetry[9] = "2/m";	// f
	this->m_WSymmetry[10] = "2/m";	// e
	this->m_WSymmetry[11] = "2/m";	// d
	this->m_WSymmetry[12] = "2/m";	// c
	this->m_WSymmetry[13] = "2/m";	// b
	this->m_WSymmetry[14] = "2/m";	// a

	int pos=0;
	for(int i=0;i<m_numberOfWyckoffSets;i++)
		pos += this->m_WMultiplicity[i];
// symbolic coordinates
	this->m_Coordinates = new string[pos/this->m_numberOfCenteringTranslations];
// o
	m_Coordinates[0] = "x,y,z";
	m_Coordinates[1] = "-x,y,-z";
	m_Coordinates[2] = "-x,-y,-z";
	m_Coordinates[3] = "x,-y,z";
// n
	m_Coordinates[4] = "x,1/2,z";
	m_Coordinates[5] = "-x,1/2,z";
// m
	m_Coordinates[6] = "x,0,z";
	m_Coordinates[7] = "-x,0,-z";
// l
	m_Coordinates[8] = "1/2,y,1/2";
	m_Coordinates[9] = "1/2,-y,1/2";
// k
	m_Coordinates[10] = "0,y,1/2";
	m_Coordinates[11] = "0,-y,1/2";
// j
	m_Coordinates[12] = "1/2,y,0";
	m_Coordinates[13] = "1/2,-y,0";
// i
	m_Coordinates[14] = "0,y,0";
	m_Coordinates[15] = "0,-y,0";
// h
	m_Coordinates[16] = "1/2,1/2,1/2";
// g
	m_Coordinates[17] = "1/2,0,1/2";
// f
	m_Coordinates[18] = "0,1/2,1/2";
// e
	m_Coordinates[19] = "1/2,1/2,0";
// d
	m_Coordinates[20] = "1/2,0,0";
// c
	m_Coordinates[21] = "0,0,1/2";
// b
	m_Coordinates[22] = "0,1/2,0";
// a
	m_Coordinates[23] = "0,0,0";

	this->m_CenteringTranslations = new string[m_numberOfCenteringTranslations];
	m_CenteringTranslations[0] = "(0,0,0)+";
}

P12dm1::~P12dm1()
{
}


////////////////////////////////////////////////////////////////////////
//
EqPositions* P12dm1::GetEqPos(double xyz[3], char Wpos)
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

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'b':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'c':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'd':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = 0;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'e':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'f':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'g':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0;
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
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0;

		m_EqPos->m_X[1] = 0;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = 0;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;
	case 'j':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0;

		m_EqPos->m_X[1] = 0.5;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = 0;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;
	case 'k':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0.5;

		m_EqPos->m_X[1] = 0;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;
	case 'l':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0.5;

		m_EqPos->m_X[1] = 0.5;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;
	case 'm':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0;
		m_EqPos->m_Z[1] = -z;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;
	case 'n':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0.5;
		m_EqPos->m_Z[1] = -z;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;
	case 'o':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = y;
		m_EqPos->m_Z[1] = -z;

		m_EqPos->m_X[2] = -x;
		m_EqPos->m_Y[2] = -y;
		m_EqPos->m_Z[2] = -z;

		m_EqPos->m_X[3] = x;
		m_EqPos->m_Y[3] = -y;
		m_EqPos->m_Z[3] = z;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = true;
		break;
	default:
		return NULL;				
	}
	return m_EqPos;
}
