// P2dm2dm2dm.cpp: implementation of the P2dm2dm2dm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "P2dm2dm2dm.h"

#include "SimpleOrthorombic.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

P2dm2dm2dm::P2dm2dm2dm()
{
	this->m_pageInInternationalTables = "p.252";
	this->m_SpaceGroupNumber = 47;
	this->m_Short_notation = "Pmmm";
	this->m_Long_notation = "P 2/m 2/m 2/m";
	this->m_Schoenflies = "D2h(1)";
	this->m_Hall = "?";
	this->m_PointGroup = "mmm";
	this->m_CrystalSystem = "Orthorhombic";
	this->m_LatticeSymbol = "P - primitive";
	this->m_additionalInfo = "";
	this->m_pUnitCell = new SimpleOrthorombic;
	this->m_numberOfWyckoffSets = 27;
	this->m_numberOfCenteringTranslations = 1;
//------------------------------------------------------------------------
// multiplicity as in international tables
	this->m_WMultiplicity = new int[this->m_numberOfWyckoffSets];
	this->m_WMultiplicity[0] = 8;	// alpha (A)
	this->m_WMultiplicity[1] = 4;	// z
	this->m_WMultiplicity[2] = 4;	// y
	this->m_WMultiplicity[3] = 4;	// x
	this->m_WMultiplicity[4] = 4;	// w
	this->m_WMultiplicity[5] = 4;	// v
	this->m_WMultiplicity[6] = 4;	// u
	this->m_WMultiplicity[7] = 2;	// t
	this->m_WMultiplicity[8] = 2;	// s
	this->m_WMultiplicity[9] = 2;	// r
	this->m_WMultiplicity[10] = 2;	// q
	this->m_WMultiplicity[11] = 2;	// p
	this->m_WMultiplicity[12] = 2;	// o
	this->m_WMultiplicity[13] = 2;	// n
	this->m_WMultiplicity[14] = 2;	// m
	this->m_WMultiplicity[15] = 2;	// l
	this->m_WMultiplicity[16] = 2;	// k
	this->m_WMultiplicity[17] = 2;	// j
	this->m_WMultiplicity[18] = 2;	// i
	this->m_WMultiplicity[19] = 1;	// h
	this->m_WMultiplicity[20] = 1;	// g
	this->m_WMultiplicity[21] = 1;	// f
	this->m_WMultiplicity[22] = 1;	// e
	this->m_WMultiplicity[23] = 1;	// d
	this->m_WMultiplicity[24] = 1;	// c
	this->m_WMultiplicity[25] = 1;	// b
	this->m_WMultiplicity[26] = 1;	// a

// Wyckoff Symbol
	this->m_WSymbol = new char[this->m_numberOfWyckoffSets];
	this->m_WSymbol[0] = 'A';
	this->m_WSymbol[1] = 'z';
	this->m_WSymbol[2] = 'y';
	this->m_WSymbol[3] = 'x';
	this->m_WSymbol[4] = 'w';
	this->m_WSymbol[5] = 'v';
	this->m_WSymbol[6] = 'u';
	this->m_WSymbol[7] = 't';
	this->m_WSymbol[8] = 's';
	this->m_WSymbol[9] = 'r';
	this->m_WSymbol[10] = 'q';
	this->m_WSymbol[11] = 'p';
	this->m_WSymbol[12] = 'o';
	this->m_WSymbol[13] = 'n';
	this->m_WSymbol[14] = 'm';
	this->m_WSymbol[15] = 'l';
	this->m_WSymbol[16] = 'k';
	this->m_WSymbol[17] = 'j';
	this->m_WSymbol[18] = 'i';
	this->m_WSymbol[19] = 'h';
	this->m_WSymbol[20] = 'g';
	this->m_WSymbol[21] = 'f';
	this->m_WSymbol[22] = 'e';
	this->m_WSymbol[23] = 'd';
	this->m_WSymbol[24] = 'c';
	this->m_WSymbol[25] = 'b';
	this->m_WSymbol[26] = 'a';

// site symmetry
	this->m_WSymmetry = new string[this->m_numberOfWyckoffSets];
	this->m_WSymmetry[0] = "1";			// alpha (A)
	this->m_WSymmetry[1] = "..m";		// z
	this->m_WSymmetry[2] = "..m";		// y
	this->m_WSymmetry[3] = ".m.";		// x
	this->m_WSymmetry[4] = ".m.";		// w
	this->m_WSymmetry[5] = "m..";		// v
	this->m_WSymmetry[6] = "m..";		// u
	this->m_WSymmetry[7] = "mm2";		// t
	this->m_WSymmetry[8] = "mm2";		// s
	this->m_WSymmetry[9] = "mm2";		// r
	this->m_WSymmetry[10] = "mm2";		// q
	this->m_WSymmetry[11] = "m2m";		// p
	this->m_WSymmetry[12] = "m2m";		// o
	this->m_WSymmetry[13] = "m2m";		// n
	this->m_WSymmetry[14] = "m2m";		// m
	this->m_WSymmetry[15] = "2mm";		// l
	this->m_WSymmetry[16] = "2mm";		// k
	this->m_WSymmetry[17] = "2mm";		// j
	this->m_WSymmetry[18] = "2mm";		// i
	this->m_WSymmetry[19] = "mmm";		// h
	this->m_WSymmetry[20] = "mmm";		// g
	this->m_WSymmetry[21] = "mmm";		// f
	this->m_WSymmetry[22] = "mmm";		// e
	this->m_WSymmetry[23] = "mmm";		// d
	this->m_WSymmetry[24] = "mmm";		// c
	this->m_WSymmetry[25] = "mmm";		// b
	this->m_WSymmetry[26] = "mmm";		// a

	int pos=0;
	for(int i=0;i<m_numberOfWyckoffSets;i++)
		pos += this->m_WMultiplicity[i];
// symbolic coordinates
	this->m_Coordinates = new string[pos/this->m_numberOfCenteringTranslations];
// alpha (A)
	m_Coordinates[0] = "x,y,z";
	m_Coordinates[1] = "-x,-y,z";
	m_Coordinates[2] = "-x,y,-z";
	m_Coordinates[3] = "x,-y,-z";
	m_Coordinates[4] = "-x,-y,-z";
	m_Coordinates[5] = "x,y,-z";
	m_Coordinates[6] = "x,-y,z";
	m_Coordinates[7] = "-x,y,z";
// z
	m_Coordinates[8] = "x,y,1/2";
	m_Coordinates[9] = "-x,-y,1/2";
	m_Coordinates[10] = "-x,y,1/2";
	m_Coordinates[11] = "x,-y,1/2";
// y
	m_Coordinates[12] = "x,y,0";
	m_Coordinates[13] = "-x,-y,0";
	m_Coordinates[14] = "-x,y,0";
	m_Coordinates[15] = "x,-y,0";
// x
	m_Coordinates[16] = "x,1/2,z";
	m_Coordinates[17] = "-x,1/2,z";
	m_Coordinates[18] = "-x,1/2,-z";
	m_Coordinates[19] = "x,1/2,-z";
// w
	m_Coordinates[20] = "x,0,z";
	m_Coordinates[21] = "-x,0,z";
	m_Coordinates[22] = "-x,0,-z";
	m_Coordinates[23] = "x,0,-z";
// v
	m_Coordinates[24] = "1/2,y,z";
	m_Coordinates[25] = "1/2,-y,z";
	m_Coordinates[26] = "1/2,y,-z";
	m_Coordinates[27] = "1/2,-y,-z";
// u
	m_Coordinates[28] = "0,y,z";
	m_Coordinates[29] = "0,-y,z";
	m_Coordinates[30] = "0,y,-z";
	m_Coordinates[31] = "0,-y,-z";
// t
	m_Coordinates[32] = "1/2,1/2,z";
	m_Coordinates[33] = "1/2,1/2,-z";
// s
	m_Coordinates[34] = "1/2,0,z";
	m_Coordinates[35] = "1/2,0,-z";
// r
	m_Coordinates[36] = "0,1/2,z";
	m_Coordinates[37] = "0,1/2,-z";
// q
	m_Coordinates[38] = "0,0,z";
	m_Coordinates[39] = "0,0,-z";
// p
	m_Coordinates[40] = "1/2,y,1/2";
	m_Coordinates[41] = "1/2,-y,1/2";
// o
	m_Coordinates[42] = "1/2,y,0";
	m_Coordinates[43] = "1/2,-y,0";
// n
	m_Coordinates[44] = "0,y,1/2";
	m_Coordinates[45] = "0,-y,1/2";
// m
	m_Coordinates[46] = "0,y,0";
	m_Coordinates[47] = "0,-y,0";
// l
	m_Coordinates[48] = "x,1/2,1/2";
	m_Coordinates[49] = "-x,1/2,1/2";
// k
	m_Coordinates[50] = "x,1/2,0";
	m_Coordinates[51] = "-x,1/2,0";
// j
	m_Coordinates[52] = "x,0,1/2";
	m_Coordinates[53] = "-x,0,1/2";
// i
	m_Coordinates[54] = "x,0,0";
	m_Coordinates[55] = "-x,0,0";
// h
	m_Coordinates[56] = "1/2,1/2,1/2";
// g
	m_Coordinates[57] = "0,1/2,1/2";
// f
	m_Coordinates[58] = "1/2,1/2,0";
// e
	m_Coordinates[59] = "0,1/2,0";
// d
	m_Coordinates[60] = "1/2,0,1/2";
// c
	m_Coordinates[61] = "0,0,1/2";
// b
	m_Coordinates[62] = "1/2,0,0";
// a
	m_Coordinates[63] = "0,0,0";

	this->m_CenteringTranslations = new string[m_numberOfCenteringTranslations];
	m_CenteringTranslations[0] = "(0,0,0)+";
}

P2dm2dm2dm::~P2dm2dm2dm()
{
}


////////////////////////////////////////////////////////////////////////
//
EqPositions* P2dm2dm2dm::GetEqPos(double xyz[3], char Wpos)
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
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0;
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
		m_EqPos->m_Z[0] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'e':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'f':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'g':
		m_EqPos->m_X[0] = 0;
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
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = 0;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0;
		m_EqPos->m_Z[1] = 0;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'j':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = 0.5;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0;
		m_EqPos->m_Z[1] = 0.5;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'k':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0.5;
		m_EqPos->m_Z[1] = 0;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'l':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0.5;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0.5;
		m_EqPos->m_Z[1] = 0.5;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'm':
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
	case 'n':
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
	case 'o':
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
	case 'p':
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
	case 'q':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = 0;
		m_EqPos->m_Y[1] = 0;
		m_EqPos->m_Z[1] = -z;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;
	case 'r':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = 0;
		m_EqPos->m_Y[1] = 0.5;
		m_EqPos->m_Z[1] = -z;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;
	case 's':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = 0.5;
		m_EqPos->m_Y[1] = 0;
		m_EqPos->m_Z[1] = -z;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;
	case 't':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = 0.5;
		m_EqPos->m_Y[1] = 0.5;
		m_EqPos->m_Z[1] = -z;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;
	case 'u':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = 0;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = z;

		m_EqPos->m_X[2] = 0;
		m_EqPos->m_Y[2] = y;
		m_EqPos->m_Z[2] = -z;

		m_EqPos->m_X[3] = 0;
		m_EqPos->m_Y[3] = -y;
		m_EqPos->m_Z[3] = -z;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = true;
		break;
	case 'v':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = 0.5;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = z;

		m_EqPos->m_X[2] = 0.5;
		m_EqPos->m_Y[2] = y;
		m_EqPos->m_Z[2] = -z;

		m_EqPos->m_X[3] = 0.5;
		m_EqPos->m_Y[3] = -y;
		m_EqPos->m_Z[3] = -z;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = true;
		break;
	case 'w':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0;
		m_EqPos->m_Z[1] = z;

		m_EqPos->m_X[2] = -x;
		m_EqPos->m_Y[2] = 0;
		m_EqPos->m_Z[2] = -z;

		m_EqPos->m_X[3] = x;
		m_EqPos->m_Y[3] = 0;
		m_EqPos->m_Z[3] = -z;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;
	case 'x':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = z;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0.5;
		m_EqPos->m_Z[1] = z;

		m_EqPos->m_X[2] = -x;
		m_EqPos->m_Y[2] = 0.5;
		m_EqPos->m_Z[2] = -z;

		m_EqPos->m_X[3] = x;
		m_EqPos->m_Y[3] = 0.5;
		m_EqPos->m_Z[3] = -z;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = true;
		break;
	case 'y':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = 0;

		m_EqPos->m_X[2] = -x;
		m_EqPos->m_Y[2] = y;
		m_EqPos->m_Z[2] = 0;

		m_EqPos->m_X[3] = x;
		m_EqPos->m_Y[3] = -y;
		m_EqPos->m_Z[3] = 0;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;
	case 'z':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = y;
		m_EqPos->m_Z[0] = 0.5;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = -y;
		m_EqPos->m_Z[1] = 0.5;

		m_EqPos->m_X[2] = -x;
		m_EqPos->m_Y[2] = y;
		m_EqPos->m_Z[2] = 0.5;

		m_EqPos->m_X[3] = x;
		m_EqPos->m_Y[3] = -y;
		m_EqPos->m_Z[3] = 0.5;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = false;
		break;
	case 'A':
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

		m_WIsEditable_X = true;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = true;
		break;
	default:
		return NULL;				
	}
	return m_EqPos;
}
