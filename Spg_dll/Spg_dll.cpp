// Spg_dll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Spg_dll.h"
#include "AllSGHeaders.h"

///////////////////////////////////////////////////////////////////////
//	construction/destruction
//
SpaceGroup::SpaceGroup()
{
	this->m_EqPos = NULL;
	strcpy(m_outputText,"\n");
}

SpaceGroup::~SpaceGroup()
{
	if(m_WMultiplicity) delete [] m_WMultiplicity;
	if(m_WSymbol) delete [] m_WSymbol;
	if(m_WSymmetry) delete [] m_WSymmetry;
	if(m_EqPos) delete m_EqPos;
	if(m_Coordinates) delete [] m_Coordinates;
	if(m_CenteringTranslations) delete [] m_CenteringTranslations;
//	if(m_pUnitCell) delete m_pUnitCell;
}


SpaceGroup* SpaceGroup::New(int sgNumber)
{
	switch(sgNumber)
	{
	case 1:
		return new P1;				// p.102 in the International Tables
		break;
	case 2:
		return new P_1;				// p.104
		break;
	case 3:
	case 3001:
		return new P121;			// p.106
		break;
	case 3002:
		return new P112;			// p.108
		break;
	case 4:
	case 4001:
		return new P12s11;			// p.110
		break;
	case 4002:
		return new P112s1;			// p.112
		break;
	case 5:
	case 5001:
		return new C121;			// p.114
		break;
	case 5002:
		return new A121;			// p.117
		break;
	case 5003:
		return new I121;			// p.117
		break;
	case 5004:
		return new A112;			// p.118
		break;
	case 5005:
		return new B112;			// p.121
		break;
	case 5006:
		return new I112;			// p.121
		break;
	case 6:
	case 6001:
		return new P1m1;			// p.122
		break;
	case 6002:
		return new P11m;			// p.124
		break;
	case 63:
		return new C2dm2dc2s1dm;	// p.290
		break;
	case 195:
		return new P23;				// p.592
		break;
	case 47:						
		return new P2dm2dm2dm;		// p.252
		break;
	case 71:
		return new I2dm2dm2dm;		// p.314
		break;
	case 64:
		return new C2dm2dc2s1da;	// p.292
		break;
	case 10001:
		return new P112dm;			// p.152
		break;
	case 10002:
		return new P12dm1;			// p.150
		break;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////
//		returns the equivalent Wyckoff position count

int SpaceGroup::GetWMultiplicity(char Wpos)
{
	for(int i=0;i<this->m_numberOfWyckoffSets;i++)
	{
		if(!m_WSymbol[i] == Wpos)
			return this->m_WMultiplicity[i];
	}
	return -1;
}


///////////////////////////////////////////////////////////////////////
//

void SpaceGroup::ClearEqPos()
{
	if(m_EqPos)
	{
		delete m_EqPos;
		m_EqPos = NULL;
	}
}

////////////////////////////////////////////////////////////////////////
// calculates ALL atoms in ONE UnitCell (expands Wyckoff positions)
// returns 0 if allatoms appended,
// if not zero, returns the number of duplicate atoms, having the same cartesian coordinates
// in that case user should check for the text message in the m_outputText.
//
int SpaceGroup::CalculateUnitCellAtoms()
{
// text output file
// vectors for temporary coordinates
	vector<double>X;
	vector<double>Y;
	vector<double>Z;
// for AfxMessageBox()
	char buffer[1000];
	int duplicateAtoms = 0;
// erase previous message
	strcpy(this->m_outputText,"\n");
//
// number of wyckoff positions in UnitCell (not expanded)
	int numpos = this->m_pUnitCell->m_positionsInUnitCell;
	for(int i=0;i<numpos;i++)
	{
// find the Wyckoff position index in SpaceGroup arrays
//		int pos = this->FindWPositionIndex(this->m_pUnitCell->m_WyckoffPosition[i]);
//		if(pos == -1) printf("Spgdll - Wyckoff Symbol not found");	// symbol not found, failure!!!
		double xyz[3];
		xyz[0] = this->m_pUnitCell->m_unitCellAtomX[i];
		xyz[1] = this->m_pUnitCell->m_unitCellAtomY[i];
		xyz[2] = this->m_pUnitCell->m_unitCellAtomZ[i];
// now get ALL atom positions for the selected Wyckoff Symbol (in unit cell coordinates)
		m_EqPos = GetEqPos(xyz,this->m_pUnitCell->m_WyckoffPosition[i]);
// number of ALL atoms in the selected Wyckoff Position
		int size = m_EqPos->m_size;
// Calculate the cartesian coordinates for ALL atoms in the selected Wyckoff position
// store all positions in a vector, in order to check if any atoms are in the same positions
// in 3D space
		X.clear();
		Y.clear();
		Z.clear();
		for(int j=0;j<size;j++)
		{
			xyz[0] = m_EqPos->m_X[j];
			xyz[1] = m_EqPos->m_Y[j];
			xyz[2] = m_EqPos->m_Z[j];
// check if an atom has already the same cartesian coordinates.
			bool duplicateExists = false;
			for(int k=0;k<j;k++)
			{
				if(X[k]==xyz[0] && Y[k] == xyz[1] && Z[k] == xyz[2])
				{
					duplicateExists = true;
					sprintf(buffer,"atoms %d and %d of position %c are duplicate. Only one added\n",
									j,k,this->m_pUnitCell->m_WyckoffPosition[i]);
					strcat(this->m_outputText,buffer);
					duplicateAtoms++;
					break;
				}
				else
					continue;
// Calculate the cartesian coordinates of an atom AND
// store in this->m_pUnitCell->m_unitCellAtomCartX. Both are done in the following call
// atom charges and types are copied to this->m_pUnitCell->m_unitCellAtomCartCharge[] and
// this->m_pUnitCell->m_unitCellAtomCartType[] and later loaded to CLattice
			}
			if(!duplicateExists)
				this->m_pUnitCell->CalculateAtoms(xyz,i);

			X.push_back(xyz[0]);
			Y.push_back(xyz[1]);
			Z.push_back(xyz[2]);
		}
		this->ClearEqPos();
	}
	return duplicateAtoms;
}

////////////////////////////////////////////////////////////////////////
//	finds the index of Wyckoff symbol in this->m_WSymbol[]
int SpaceGroup::FindWPositionIndex(char pos)
{
	for(int i=0;i<this->m_numberOfWyckoffSets;i++)
		if(pos == this->m_WSymbol[i]) return i;
// failure, return -1
	return -1;
}
