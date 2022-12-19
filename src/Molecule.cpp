// Molecule.cpp: implementation of the Molecule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "madelung.h"
#include "Molecule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Molecule::Molecule()
{

}

Molecule::Molecule(string name)
{
	m_name = name;
}

Molecule::~Molecule()
{

}
///////////////////////////////////////////////////////////////////////
//
void Molecule::AddAtom(double coord[3],int type,double charge)
{
	m_atomX.push_back(coord[0]);
	m_atomY.push_back(coord[1]);
	m_atomZ.push_back(coord[2]);
	m_atomType.push_back(type);
	m_atomCharge.push_back(charge);
}
///////////////////////////////////////////////////////////////////////
//
int Molecule::GetAtom(int number, double coord[3], int &type, double &charge)
{
	if (number<0 || number>=GetSize()) return -1;
	else
	{
		coord[0] = m_atomX[number];
		coord[1] = m_atomY[number];
		coord[2] = m_atomZ[number];
		type = m_atomType[number];
		charge = m_atomCharge[number];
		return 0;
	}
}
////////////////////////////////////////////////////////////////////////
//
void Molecule::EditAtom(int atom,double coord[3],int type, double charge)
{
	m_atomX[atom] = coord[0];
	m_atomY[atom] = coord[1];
	m_atomZ[atom] = coord[2];
	m_atomCharge[atom] = charge;
	m_atomType[atom] = type;
}
//////////////////////////////////////////////////////////////////////////
//
Molecule Molecule::operator=(Molecule mol)
{
// clear current settings
	this->Clear();
// new settings
	this->SetName(mol.GetName());
	int size = mol.GetSize();
	for(int i=0;i<size;i++)
	{
		double coord[3];
		int type;
		double charge;
		mol.GetAtom(i,coord,type,charge);
		AddAtom(coord,type,charge);
	}
	for(i=0;i<3;i++)
	{
		double dim[3];
		mol.GetCenter(dim);
		this->SetCenter(dim);
		mol.GetOrientation(dim);
		this->SetOrientation(dim);
	}
	return *this;
}
////////////////////////////////////////////////////////////////////////////
//
void Molecule::Clear()
{
// clear current settings
	m_name = "";
	m_atomX.clear();
	m_atomY.clear();
	m_atomZ.clear();
	m_atomType.clear();
	m_atomCharge.clear();
	for(int i=0;i<3;i++)
	{
		m_center[i] = 0;
		m_orientation[i] = 0;
	}
}
