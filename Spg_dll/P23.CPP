// P23.cpp: implementation of the P23 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "P23.h"
#include "SimpleCubic.h"	// Bravais lattice type

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

P23::P23()
{
	this->m_pageInInternationalTables = "p.592";
	this->m_SpaceGroupNumber = 195;
	this->m_Short_notation = "P23";
	this->m_Long_notation = "P23";
	this->m_Schoenflies = "T(1)";
	this->m_Hall = "?";
	this->m_PointGroup = "23";
	this->m_CrystalSystem = "Cubic";
	this->m_LatticeSymbol = "P - primitive";
	this->m_additionalInfo = "";
	this->m_pUnitCell = new SimpleCubic;
	this->m_numberOfWyckoffSets = 10;
	this->m_numberOfCenteringTranslations = 1;
//------------------------------------------------------------------------
// multiplicity as in international tables
	this->m_WMultiplicity = new int[this->m_numberOfWyckoffSets];
	this->m_WMultiplicity[0] = 12;
	this->m_WMultiplicity[1] = 6;
	this->m_WMultiplicity[2] = 6;
	this->m_WMultiplicity[3] = 6;
	this->m_WMultiplicity[4] = 6;
	this->m_WMultiplicity[5] = 4;
	this->m_WMultiplicity[6] = 3;
	this->m_WMultiplicity[7] = 3;
	this->m_WMultiplicity[8] = 1;
	this->m_WMultiplicity[9] = 1;

// Wyckoff Symbol
	this->m_WSymbol = new char[this->m_numberOfWyckoffSets];
	this->m_WSymbol[0] = 'j';
	this->m_WSymbol[1] = 'i';
	this->m_WSymbol[2] = 'h';
	this->m_WSymbol[3] = 'g';
	this->m_WSymbol[4] = 'f';
	this->m_WSymbol[5] = 'e';
	this->m_WSymbol[6] = 'd';
	this->m_WSymbol[7] = 'c';
	this->m_WSymbol[8] = 'b';
	this->m_WSymbol[9] = 'a';

// site symmetry
	this->m_WSymmetry = new string[this->m_numberOfWyckoffSets];
	this->m_WSymmetry[0] = "1";
	this->m_WSymmetry[1] = "2..";
	this->m_WSymmetry[2] = "2..";
	this->m_WSymmetry[3] = "2..";
	this->m_WSymmetry[4] = "2..";
	this->m_WSymmetry[5] = ".3.";
	this->m_WSymmetry[6] = "222..";
	this->m_WSymmetry[7] = "222..";
	this->m_WSymmetry[8] = "23.";
	this->m_WSymmetry[9] = "23.";

	int pos=0;
	for(int i=0;i<m_numberOfWyckoffSets;i++)
		pos += this->m_WMultiplicity[i];
// symbolic coordinates
	this->m_Coordinates = new string[pos/this->m_numberOfCenteringTranslations];
// j
	m_Coordinates[0] = "x,y,z";
	m_Coordinates[1] = "-x,-y,z";
	m_Coordinates[2] = "-x,y,-z";
	m_Coordinates[3] = "x,-y,-z";
	m_Coordinates[4] = "z,x,y";
	m_Coordinates[5] = "z,-x,-y";
	m_Coordinates[6] = "-z,-x,y";
	m_Coordinates[7] = "-z,x,-y";
	m_Coordinates[8] = "y,z,x";
	m_Coordinates[9] = "-y,z,-x";
	m_Coordinates[10] = "y,-z,-x";
	m_Coordinates[11] = "-y,-z,x";

// i
	m_Coordinates[12] = "x,1/2,1/2";
	m_Coordinates[13] = "-x,1/2,1/2";
	m_Coordinates[14] = "1/2,x,1/2";
	m_Coordinates[15] = "1/2,-x,1/2";
	m_Coordinates[16] = "1/2,1/2,x";
	m_Coordinates[17] = "1/2,1/2,-x";
	
// h
	m_Coordinates[18] = "x,1/2,0";
	m_Coordinates[19] = "-x,1/2,0";
	m_Coordinates[20] = "0,x,1/2";
	m_Coordinates[21] = "0,-x,1/2";
	m_Coordinates[22] = "1/2,0,x";
	m_Coordinates[23] = "1/2,0,-x";

// g
	m_Coordinates[24] = "x,0,1/2";
	m_Coordinates[25] = "-x,0,1/2";
	m_Coordinates[26] = "1/2,x,0";
	m_Coordinates[27] = "1/2,-x,0";
	m_Coordinates[28] = "0,1/2,x";
	m_Coordinates[29] = "0,1/2,-x";

// f
	m_Coordinates[30] = "x,0,0";
	m_Coordinates[31] = "-x,0,0";
	m_Coordinates[32] = "0,x,0";
	m_Coordinates[33] = "0,-x,0";
	m_Coordinates[34] = "0,0,x";
	m_Coordinates[35] = "0,0,-x";
// e
	m_Coordinates[36] = "x,x,x";
	m_Coordinates[37] = "-x,-x,x";
	m_Coordinates[38] = "-x,x,-x";
	m_Coordinates[39] = "x,-x,-x";

// d
	m_Coordinates[40] = "1/2,0,0";
	m_Coordinates[41] = "0,1/2,0";
	m_Coordinates[42] = "0,0,1/2";

// c
	m_Coordinates[43] = "0,1/2,1/2";
	m_Coordinates[44] = "1/2,0,1/2";
	m_Coordinates[45] = "1/2,1/2,0";

// b
	m_Coordinates[46] = "1/2,1/2,1/2";

// a
	m_Coordinates[47] = "0,0,0";

	this->m_CenteringTranslations = new string[m_numberOfCenteringTranslations];
	m_CenteringTranslations[0] = "(0,0,0)+";

}

P23::~P23()
{
}


////////////////////////////////////////////////////////////////////////
//
EqPositions* P23::GetEqPos(double xyz[3], char Wpos)
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
		m_EqPos->m_X[0] = 0.;
		m_EqPos->m_Y[0] = 0.;
		m_EqPos->m_Z[0] = 0.;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'b':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'c':
		m_EqPos->m_X[0] = 0;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0.5;

		m_EqPos->m_X[1] = 0.5;
		m_EqPos->m_Y[1] = 0.;
		m_EqPos->m_Z[1] = 0.5;

		m_EqPos->m_X[2] = 0.5;
		m_EqPos->m_Y[2] = 0.5;
		m_EqPos->m_Z[2] = 0.;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'd':
		m_EqPos->m_X[0] = 0.5;
		m_EqPos->m_Y[0] = 0.;
		m_EqPos->m_Z[0] = 0.;

		m_EqPos->m_X[1] = 0.;
		m_EqPos->m_Y[1] = 0.5;
		m_EqPos->m_Z[1] = 0.;

		m_EqPos->m_X[2] = 0.;
		m_EqPos->m_Y[2] = 0.;
		m_EqPos->m_Z[2] = 0.5;

		m_WIsEditable_X = false;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'e':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = x;
		m_EqPos->m_Z[0] = x;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = -x;
		m_EqPos->m_Z[1] = x;

		m_EqPos->m_X[2] = -x;
		m_EqPos->m_Y[2] = x;
		m_EqPos->m_Z[2] = -x;

		m_EqPos->m_X[3] = x;
		m_EqPos->m_Y[3] = -x;
		m_EqPos->m_Z[3] = -x;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'f':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0.;
		m_EqPos->m_Z[0] = 0.;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0.;
		m_EqPos->m_Z[1] = 0.;

		m_EqPos->m_X[2] = 0.;
		m_EqPos->m_Y[2] = x;
		m_EqPos->m_Z[2] = 0.;

		m_EqPos->m_X[3] = 0.;
		m_EqPos->m_Y[3] = -x;
		m_EqPos->m_Z[3] = 0.;

		m_EqPos->m_X[4] = 0.;
		m_EqPos->m_Y[4] = 0.;
		m_EqPos->m_Z[4] = x;

		m_EqPos->m_X[5] = 0.;
		m_EqPos->m_Y[5] = 0.;
		m_EqPos->m_Z[5] = -x;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'g':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0.;
		m_EqPos->m_Z[0] = 0.5;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0.;
		m_EqPos->m_Z[1] = 0.5;

		m_EqPos->m_X[2] = 0.5;
		m_EqPos->m_Y[2] = x;
		m_EqPos->m_Z[2] = 0.;

		m_EqPos->m_X[3] = 0.5;
		m_EqPos->m_Y[3] = -x;
		m_EqPos->m_Z[3] = 0.;

		m_EqPos->m_X[4] = 0.;
		m_EqPos->m_Y[4] = 0.5;
		m_EqPos->m_Z[4] = x;

		m_EqPos->m_X[5] = 0.;
		m_EqPos->m_Y[5] = 0.5;
		m_EqPos->m_Z[5] = -x;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'h':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0.;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0.5;
		m_EqPos->m_Z[1] = 0.;

		m_EqPos->m_X[2] = 0.;
		m_EqPos->m_Y[2] = x;
		m_EqPos->m_Z[2] = 0.5;

		m_EqPos->m_X[3] = 0.;
		m_EqPos->m_Y[3] = -x;
		m_EqPos->m_Z[3] = 0.5;

		m_EqPos->m_X[4] = 0.5;
		m_EqPos->m_Y[4] = 0.;
		m_EqPos->m_Z[4] = x;

		m_EqPos->m_X[5] = 0.5;
		m_EqPos->m_Y[5] = 0.;
		m_EqPos->m_Z[5] = -x;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'i':
		m_EqPos->m_X[0] = x;
		m_EqPos->m_Y[0] = 0.5;
		m_EqPos->m_Z[0] = 0.5;

		m_EqPos->m_X[1] = -x;
		m_EqPos->m_Y[1] = 0.5;
		m_EqPos->m_Z[1] = 0.5;

		m_EqPos->m_X[2] = 0.5;
		m_EqPos->m_Y[2] = x;
		m_EqPos->m_Z[2] = 0.5;

		m_EqPos->m_X[3] = 0.5;
		m_EqPos->m_Y[3] = -x;
		m_EqPos->m_Z[3] = 0.5;

		m_EqPos->m_X[4] = 0.5;
		m_EqPos->m_Y[4] = 0.5;
		m_EqPos->m_Z[4] = x;

		m_EqPos->m_X[5] = 0.5;
		m_EqPos->m_Y[5] = 0.5;
		m_EqPos->m_Z[5] = -x;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = false;
		m_WIsEditable_Z = false;
		break;
	case 'j':
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

		m_EqPos->m_X[4] = z;
		m_EqPos->m_Y[4] = x;
		m_EqPos->m_Z[4] = y;

		m_EqPos->m_X[5] = z;
		m_EqPos->m_Y[5] = -x;
		m_EqPos->m_Z[5] = -y;

		m_EqPos->m_X[6] = -z;
		m_EqPos->m_Y[6] = -x;
		m_EqPos->m_Z[6] = y;

		m_EqPos->m_X[7] = -z;
		m_EqPos->m_Y[7] = x;
		m_EqPos->m_Z[7] = -y;

		m_EqPos->m_X[8] = y;
		m_EqPos->m_Y[8] = z;
		m_EqPos->m_Z[8] = x;

		m_EqPos->m_X[9] = -y;
		m_EqPos->m_Y[9] = z;
		m_EqPos->m_Z[9] = -x;

		m_EqPos->m_X[10] = y;
		m_EqPos->m_Y[10] = -z;
		m_EqPos->m_Z[10] = -x;

		m_EqPos->m_X[11] = -y;
		m_EqPos->m_Y[11] = -z;
		m_EqPos->m_Z[11] = x;

		m_WIsEditable_X = true;
		m_WIsEditable_Y = true;
		m_WIsEditable_Z = true;
		break;

	default:
		return NULL;				
	}
	return m_EqPos;
}
