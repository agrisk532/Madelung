// AtomProperties.cpp: implementation of the AtomProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "madelung.h"
#include "AtomProperties.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AtomProperties::AtomProperties()
{
// fill in atom ionic radii
	m_Radius[0] = 0.25;		// H
	m_Radius[1] = 0.30;		// He
	m_Radius[2] = 0.90;		// Li
	m_Radius[3] = 0.50;		// Be
	m_Radius[4] = 0.32;		// B
	m_Radius[5] = 0.30;		// C
	m_Radius[6] = 0.30;		// N
//	m_Radius[7] = 1.25;		// O
	m_Radius[7] = 0.75;		// O
	m_Radius[8] = 1.20;		// F
	m_Radius[9] = 1.12;		// Ne
	m_Radius[10] = 1.20;	// Na
	m_Radius[11] = 0.80;	// Mg
	m_Radius[12] = 0.60;	// Al
	m_Radius[13] = 0.45;	// Si
	m_Radius[14] = 0.45;	// P
	m_Radius[15] = 0.40;	// S
	m_Radius[16] = 1.70;	// Cl
	m_Radius[17] = 1.54;	// Ar
	m_Radius[18] = 1.55;	// K
	m_Radius[19] = 1.05;	// Ca
	m_Radius[20] = 0.95;	// Sc
	m_Radius[21] = 0.80;	// Ti
	m_Radius[22] = 0.75;	// V
	m_Radius[23] = 0.70;	// Cr
	m_Radius[24] = 0.70;	// Mn
	m_Radius[25] = 0.80;	// Fe
	m_Radius[26] = 0.80;	// Co
	m_Radius[27] = 0.70;	// Ni
	m_Radius[28] = 0.80;	// Cu
	m_Radius[29] = 0.90;	// Zn
	m_Radius[30] = 0.70;	// Ga
	m_Radius[31] = 0.70;	// Ge
	m_Radius[32] = 0.60;	// As
	m_Radius[33] = 0.55;	// Se
	m_Radius[34] = 1.80;	// Br
	m_Radius[35] = 1.70;	// Kr
	m_Radius[36] = 1.70;	// Rb
	m_Radius[37] = 1.36;	// Sr
	m_Radius[38] = 1.10;	// Y
	m_Radius[39] = 0.85;	// Zr
	m_Radius[40] = 0.80;	// Nb
	m_Radius[41] = 0.70;	// Mo
	m_Radius[42] = 0.68;	// Tc
	m_Radius[43] = 0.75;	// Ru
	m_Radius[44] = 0.75;	// Rh
	m_Radius[45] = 0.85;	// Pd
	m_Radius[46] = 1.00;	// Ag
	m_Radius[47] = 1.10;	// Cd
	m_Radius[48] = 1.00;	// In
	m_Radius[49] = 0.85;	// Sn
	m_Radius[50] = 0.82;	// Sb
	m_Radius[51] = 0.80;	// Te
	m_Radius[52] = 2.06;	// I
	m_Radius[53] = 1.90;	// Xe
	m_Radius[54] = 1.85;	// Cs
	m_Radius[55] = 1.52;	// Ba
	m_Radius[56] = 1.20;	// La
	m_Radius[57] = 1.05;	// Lu
	m_Radius[58] = 0.85;	// Hf
	m_Radius[59] = 0.82;	// Ta
	m_Radius[60] = 0.70;	// W
	m_Radius[61] = 0.70;	// Re
	m_Radius[62] = 0.70;	// Os
	m_Radius[63] = 0.75;	// Ir
	m_Radius[64] = 0.78;	// Pt
	m_Radius[65] = 1.00;	// Au
	m_Radius[66] = 1.20;	// Hg
	m_Radius[67] = 1.30;	// Tl
	m_Radius[68] = 1.10;	// Pb
	m_Radius[69] = 1.10;	// Bi
	m_Radius[70] = 1.05;	// Po
	m_Radius[71] = 0.76;	// At
	m_Radius[72] = 1.20;	// Rn
	m_Radius[73] = 1.94;	// Fr
	m_Radius[74] = 1.62;	// Ra
	m_Radius[75] = 1.26;	// Ac
// molecule
	m_Radius[76] = 0.;		// molecule

// fill in common oxidation state (charge) (see symbol-number table for m_Radius[])

	m_Charge[0] = 0.;
	m_Charge[1] = 0.;
	m_Charge[2] = 0.;
	m_Charge[3] = 0.;
	m_Charge[4] = 0.;
	m_Charge[5] = 0.;
	m_Charge[6] = 0.;
	m_Charge[7] = 0.;
	m_Charge[8] = 0.;
	m_Charge[9] = 0.;
	m_Charge[10] = 0.;
	m_Charge[11] = 0.;
	m_Charge[12] = 0.;
	m_Charge[13] = 0.;
	m_Charge[14] = 0.;
	m_Charge[15] = 0.;
	m_Charge[16] = 0.;
	m_Charge[17] = 0.;
	m_Charge[18] = 0.;
	m_Charge[19] = 0.;
	m_Charge[20] = 0.;
	m_Charge[21] = 0.;
	m_Charge[22] = 0.;
	m_Charge[23] = 0.;
	m_Charge[24] = 0.;
	m_Charge[25] = 0.;
	m_Charge[26] = 0.;
	m_Charge[27] = 0.;
	m_Charge[28] = 0.;
	m_Charge[29] = 0.;
	m_Charge[30] = 0.;
	m_Charge[31] = 0.;
	m_Charge[32] = 0.;
	m_Charge[33] = 0.;
	m_Charge[34] = 0.;
	m_Charge[35] = 0.;
	m_Charge[36] = 0.;
	m_Charge[37] = 0.;
	m_Charge[38] = 0.;
	m_Charge[39] = 0.;
	m_Charge[40] = 0.;
	m_Charge[41] = 0.;
	m_Charge[42] = 0.;
	m_Charge[43] = 0.;
	m_Charge[44] = 0.;
	m_Charge[45] = 0.;
	m_Charge[46] = 0.;
	m_Charge[47] = 0.;
	m_Charge[48] = 0.;
	m_Charge[49] = 0.;
	m_Charge[50] = 0.;
	m_Charge[51] = 0.;
	m_Charge[52] = 0.;
	m_Charge[53] = 0.;
	m_Charge[54] = 0.;
	m_Charge[55] = 0.;
	m_Charge[56] = 0.;
	m_Charge[57] = 0.;
	m_Charge[58] = 0.;
	m_Charge[59] = 0.;
	m_Charge[60] = 0.;
	m_Charge[61] = 0.;
	m_Charge[62] = 0.;
	m_Charge[63] = 0.;
	m_Charge[64] = 0.;
	m_Charge[65] = 0.;
	m_Charge[66] = 0.;
	m_Charge[67] = 0.;
	m_Charge[68] = 0.;
	m_Charge[69] = 0.;
	m_Charge[70] = 0.;
	m_Charge[71] = 0.;
	m_Charge[72] = 0.;
	m_Charge[73] = 0.;
	m_Charge[74] = 0.;
	m_Charge[75] = 0.;
// molecule
	m_Charge[76] = 0.;

// fill in atom colors (r,g,b) (see symbol-number table for m_Radius[])

	m_Color[0][0] = 1.0; m_Color[0][1] = 1.0; m_Color[0][2] = 1.0;		// H
	m_Color[1][0] = 1.0; m_Color[1][1] = 1.0; m_Color[1][2] = 1.0;
	m_Color[2][0] = 1.0; m_Color[2][1] = 1.0; m_Color[2][2] = 1.0;
	m_Color[3][0] = 1.0; m_Color[3][1] = 1.0; m_Color[3][2] = 1.0;
	m_Color[4][0] = 1.0; m_Color[4][1] = 1.0; m_Color[4][2] = 1.0;
	m_Color[5][0] = 1.0; m_Color[5][1] = 1.0; m_Color[5][2] = 1.0;
	m_Color[6][0] = 0.0; m_Color[6][1] = 0.0; m_Color[6][2] = 1.0;		// N
	m_Color[7][0] = 1.0; m_Color[7][1] = 0.0; m_Color[7][2] = 0.0;		// O
	m_Color[8][0] = 1.0; m_Color[8][1] = 1.0; m_Color[8][2] = 1.0;
	m_Color[9][0] = 1.0; m_Color[9][1] = 1.0; m_Color[9][2] = 1.0;
	m_Color[10][0] = 1.0; m_Color[10][1] = 1.0; m_Color[10][2] = 1.0;
	m_Color[11][0] = 1.0; m_Color[11][1] = 1.0; m_Color[11][2] = 1.0;
	m_Color[12][0] = 1.0; m_Color[12][1] = 1.0; m_Color[12][2] = 1.0;
	m_Color[13][0] = 1.0; m_Color[13][1] = 1.0; m_Color[13][2] = 1.0;
	m_Color[14][0] = 1.0; m_Color[14][1] = 1.0; m_Color[14][2] = 1.0;
	m_Color[15][0] = 1.0; m_Color[15][1] = 1.0; m_Color[15][2] = 1.0;
	m_Color[16][0] = 1.0; m_Color[16][1] = 1.0; m_Color[16][2] = 1.0;
	m_Color[17][0] = 1.0; m_Color[17][1] = 1.0; m_Color[17][2] = 1.0;
	m_Color[18][0] = 1.0; m_Color[18][1] = 1.0; m_Color[18][2] = 1.0;
	m_Color[19][0] = 1.0; m_Color[19][1] = 1.0; m_Color[19][2] = 1.0;
	m_Color[20][0] = 1.0; m_Color[20][1] = 1.0; m_Color[20][2] = 1.0;
	m_Color[21][0] = 1.0; m_Color[21][1] = 1.0; m_Color[21][2] = 1.0;
	m_Color[22][0] = 1.0; m_Color[22][1] = 1.0; m_Color[22][2] = 1.0;
	m_Color[23][0] = 1.0; m_Color[23][1] = 1.0; m_Color[23][2] = 1.0;
	m_Color[24][0] = 1.0; m_Color[24][1] = 1.0; m_Color[24][2] = 1.0;
	m_Color[25][0] = 1.0; m_Color[25][1] = 1.0; m_Color[25][2] = 1.0;
	m_Color[26][0] = 1.0; m_Color[26][1] = 1.0; m_Color[26][2] = 1.0;
	m_Color[27][0] = 1.0; m_Color[27][1] = 1.0; m_Color[27][2] = 1.0;
	m_Color[28][0] = 1.0; m_Color[28][1] = 1.0; m_Color[28][2] = 1.0;
	m_Color[29][0] = 1.0; m_Color[29][1] = 1.0; m_Color[29][2] = 1.0;
	m_Color[30][0] = 1.0; m_Color[30][1] = 1.0; m_Color[30][2] = 1.0;
	m_Color[31][0] = 1.0; m_Color[31][1] = 1.0; m_Color[31][2] = 1.0;
	m_Color[32][0] = 1.0; m_Color[32][1] = 1.0; m_Color[32][2] = 1.0;
	m_Color[33][0] = 1.0; m_Color[33][1] = 1.0; m_Color[33][2] = 1.0;
	m_Color[34][0] = 1.0; m_Color[34][1] = 1.0; m_Color[34][2] = 1.0;
	m_Color[35][0] = 1.0; m_Color[35][1] = 1.0; m_Color[35][2] = 1.0;
	m_Color[36][0] = 1.0; m_Color[36][1] = 1.0; m_Color[36][2] = 1.0;
	m_Color[37][0] = 1.0; m_Color[37][1] = 1.0; m_Color[37][2] = 1.0;
	m_Color[38][0] = 1.0; m_Color[38][1] = 1.0; m_Color[38][2] = 1.0;
	m_Color[39][0] = 1.0; m_Color[39][1] = 1.0; m_Color[39][2] = 1.0;
	m_Color[40][0] = 1.0; m_Color[40][1] = 1.0; m_Color[40][2] = 1.0;
	m_Color[41][0] = 1.0; m_Color[41][1] = 1.0; m_Color[41][2] = 1.0;
	m_Color[42][0] = 1.0; m_Color[42][1] = 1.0; m_Color[42][2] = 1.0;
	m_Color[43][0] = 1.0; m_Color[43][1] = 1.0; m_Color[43][2] = 1.0;
	m_Color[44][0] = 1.0; m_Color[44][1] = 1.0; m_Color[44][2] = 1.0;
	m_Color[45][0] = 1.0; m_Color[45][1] = 1.0; m_Color[45][2] = 1.0;
	m_Color[46][0] = 1.0; m_Color[46][1] = 1.0; m_Color[46][2] = 1.0;
	m_Color[47][0] = 1.0; m_Color[47][1] = 1.0; m_Color[47][2] = 1.0;
	m_Color[48][0] = 1.0; m_Color[48][1] = 1.0; m_Color[48][2] = 1.0;
	m_Color[49][0] = 1.0; m_Color[49][1] = 1.0; m_Color[49][2] = 1.0;
	m_Color[50][0] = 1.0; m_Color[50][1] = 1.0; m_Color[50][2] = 1.0;
	m_Color[51][0] = 1.0; m_Color[51][1] = 1.0; m_Color[51][2] = 1.0;
	m_Color[52][0] = 1.0; m_Color[52][1] = 1.0; m_Color[52][2] = 1.0;
	m_Color[53][0] = 1.0; m_Color[53][1] = 1.0; m_Color[53][2] = 1.0;
	m_Color[54][0] = 1.0; m_Color[54][1] = 1.0; m_Color[54][2] = 1.0;
	m_Color[55][0] = 1.0; m_Color[55][1] = 1.0; m_Color[55][2] = 1.0;
	m_Color[56][0] = 1.0; m_Color[56][1] = 1.0; m_Color[56][2] = 1.0;
	m_Color[57][0] = 1.0; m_Color[57][1] = 1.0; m_Color[57][2] = 1.0;
	m_Color[58][0] = 1.0; m_Color[58][1] = 1.0; m_Color[58][2] = 1.0;
	m_Color[59][0] = 1.0; m_Color[59][1] = 1.0; m_Color[59][2] = 1.0;
	m_Color[60][0] = 1.0; m_Color[60][1] = 1.0; m_Color[60][2] = 1.0;
	m_Color[61][0] = 1.0; m_Color[61][1] = 1.0; m_Color[61][2] = 1.0;
	m_Color[62][0] = 1.0; m_Color[62][1] = 1.0; m_Color[62][2] = 1.0;
	m_Color[63][0] = 1.0; m_Color[63][1] = 1.0; m_Color[63][2] = 1.0;
	m_Color[64][0] = 1.0; m_Color[64][1] = 1.0; m_Color[64][2] = 1.0;
	m_Color[65][0] = 1.0; m_Color[65][1] = 1.0; m_Color[65][2] = 1.0;
	m_Color[66][0] = 1.0; m_Color[66][1] = 1.0; m_Color[66][2] = 1.0;
	m_Color[67][0] = 1.0; m_Color[67][1] = 1.0; m_Color[67][2] = 1.0;
	m_Color[68][0] = 1.0; m_Color[68][1] = 1.0; m_Color[68][2] = 1.0;
	m_Color[69][0] = 1.0; m_Color[69][1] = 1.0; m_Color[69][2] = 1.0;
	m_Color[70][0] = 1.0; m_Color[70][1] = 1.0; m_Color[70][2] = 1.0;
	m_Color[71][0] = 1.0; m_Color[71][1] = 1.0; m_Color[71][2] = 1.0;
	m_Color[72][0] = 1.0; m_Color[72][1] = 1.0; m_Color[72][2] = 1.0;
	m_Color[73][0] = 1.0; m_Color[73][1] = 1.0; m_Color[73][2] = 1.0;
	m_Color[74][0] = 1.0; m_Color[74][1] = 1.0; m_Color[74][2] = 1.0;
	m_Color[75][0] = 1.0; m_Color[75][1] = 1.0; m_Color[75][2] = 1.0;
// molecule
	m_Color[76][0] = 0.0; m_Color[76][1] = 0.0; m_Color[76][2] = 0.0;

// atom types

	m_Type[0] = "H";		// H
	m_Type[1] = "He";		// He
	m_Type[2] = "Li";		// Li
	m_Type[3] = "Be";		// Be
	m_Type[4] = "B";		// B
	m_Type[5] = "C";		// C
	m_Type[6] = "N";		// N
	m_Type[7] = "O";		// O
	m_Type[8] = "F";		// F
	m_Type[9] = "Ne";		// Ne
	m_Type[10] = "Na";	// Na
	m_Type[11] = "Mg";	// Mg
	m_Type[12] = "Al";	// Al
	m_Type[13] = "Si";	// Si
	m_Type[14] = "P";	// P
	m_Type[15] = "S";	// S
	m_Type[16] = "Cl";	// Cl
	m_Type[17] = "Ar";	// Ar
	m_Type[18] = "K";	// K
	m_Type[19] = "Ca";	// Ca
	m_Type[20] = "Sc";	// Sc
	m_Type[21] = "Ti";	// Ti
	m_Type[22] = "V";	// V
	m_Type[23] = "Cr";	// Cr
	m_Type[24] = "Mn";	// Mn
	m_Type[25] = "Fe";	// Fe
	m_Type[26] = "Co";	// Co
	m_Type[27] = "Ni";	// Ni
	m_Type[28] = "Cu";	// Cu
	m_Type[29] = "Zn";	// Zn
	m_Type[30] = "Ga";	// Ga
	m_Type[31] = "Ge";	// Ge
	m_Type[32] = "As";	// As
	m_Type[33] = "Se";	// Se
	m_Type[34] = "Br";	// Br
	m_Type[35] = "Kr";	// Kr
	m_Type[36] = "Rb";	// Rb
	m_Type[37] = "Sr";	// Sr
	m_Type[38] = "Y";	// Y
	m_Type[39] = "Zr";	// Zr
	m_Type[40] = "Nb";	// Nb
	m_Type[41] = "Mo";	// Mo
	m_Type[42] = "Tc";	// Tc
	m_Type[43] = "Ru";	// Ru
	m_Type[44] = "Rh";	// Rh
	m_Type[45] = "Pd";	// Pd
	m_Type[46] = "Ag";	// Ag
	m_Type[47] = "Cd";	// Cd
	m_Type[48] = "In";	// In
	m_Type[49] = "Sn";	// Sn
	m_Type[50] = "Sb";	// Sb
	m_Type[51] = "Te";	// Te
	m_Type[52] = "I";	// I
	m_Type[53] = "Xe";	// Xe
	m_Type[54] = "Cs";	// Cs
	m_Type[55] = "Ba";	// Ba
	m_Type[56] = "La";	// La
	m_Type[57] = "Lu";	// Lu
	m_Type[58] = "Hf";	// Hf
	m_Type[59] = "Ta";	// Ta
	m_Type[60] = "W";	// W
	m_Type[61] = "Re";	// Re
	m_Type[62] = "Os";	// Os
	m_Type[63] = "Ir";	// Ir
	m_Type[64] = "Pt";	// Pt
	m_Type[65] = "Au";	// Au
	m_Type[66] = "Hg";	// Hg
	m_Type[67] = "Tl";	// Tl
	m_Type[68] = "Pb";	// Pb
	m_Type[69] = "Bi";	// Bi
	m_Type[70] = "Po";	// Po
	m_Type[71] = "At";	// At
	m_Type[72] = "Rn";	// Rn
	m_Type[73] = "Fr";	// Fr
	m_Type[74] = "Ra";	// Ra
	m_Type[75] = "Ac";	// Ac
// molecule
	m_Type[76] = "MOL";	// molecule
}

AtomProperties::~AtomProperties()
{

}
