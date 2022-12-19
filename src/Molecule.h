// Molecule.h: interface for the Molecule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOLECULE_H__9FD4E5E7_0CE5_4912_998B_E7345C1C6D14__INCLUDED_)
#define AFX_MOLECULE_H__9FD4E5E7_0CE5_4912_998B_E7345C1C6D14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
using namespace std;

class Molecule  
{
public:
	Molecule();
	Molecule(string);
	virtual ~Molecule();
private:
// all atoms of a molecule are specified by their cartesian coordinates (x,y,z) in the LOCAL
// coordinate system. To put a molecule in the lattice we must specify the coordinates of 
// the geometrical center of a molecule in the lattice and the orientation of the LOCAL
// coordinate system axes with the axes of the lattice.
//
// name of the molecule
	string m_name;
// atoms in the molecule
	vector<double>m_atomX;
	vector<double>m_atomY;
	vector<double>m_atomZ;
	vector<int>m_atomType;
	vector<double>m_atomCharge;
// geom. center of the MOLECULE
	double m_center[3];
// angles between the 'local coordinate system axes of molecule' and the lattice axes.
	double m_orientation[3]; 
public:
	void SetName(string name) {m_name = name;}
	string GetName() {return m_name;}
	void AddAtom(double coord[3],int type,double charge);
	void EditAtom(int atom,double coord[3],int type, double charge);
	int GetSize() {return m_atomX.size();}
	int GetAtom(int number, double coord[3], int &type, double &charge);
// sets/gets the geom center of the molecule in the lattice
	void SetCenter(double center[3])
	{
		for(int i=0;i<3;i++)
			m_center[i] = center[i];
	}
	void SetCenter(double x,double y, double z)
	{
		m_center[0] = x;
		m_center[1] = y;
		m_center[2] = z;
	}
	void GetCenter(double center[3])
	{
		for(int i=0;i<3;i++)
			center[i] = m_center[i];
	}
// sets/gets the orientation of the molecule in the lattice
	void SetOrientation(double orientation[3])
	{
		for(int i=0;i<3;i++)
			m_orientation[i] = orientation[i];
	}
	void SetOrientation(double x,double y, double z)
	{
		m_orientation[0] = x;
		m_orientation[1] = y;
		m_orientation[2] = z;
	}
	void GetOrientation(double orientation[3])
	{
		for(int i=0;i<3;i++)
			orientation[i] = m_orientation[i];
	}
// operator overloading
	Molecule operator=(Molecule mol);
// clears all parameters of molecule
	void Clear();
};

#endif // !defined(AFX_MOLECULE_H__9FD4E5E7_0CE5_4912_998B_E7345C1C6D14__INCLUDED_)
