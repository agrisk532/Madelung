// PointGroupIrreps.h: interface for the PointGroupIrreps class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTGROUPIRREPS_H__1E7C1CF4_390E_45AE_A811_E309E703E79B__INCLUDED_)
#define AFX_POINTGROUPIRREPS_H__1E7C1CF4_390E_45AE_A811_E309E703E79B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
using namespace std;

// keeps the info about an irrreducible representation of the point group
// used to calculate the 'OCCUPIED' keyword values for MOLCAS module SCF
class Irrep {
private:
	string m_name; // irrep symbolic name
	string m_function_symmetry;	// irrep function symmetry (last column in the point group tables)
// coefficient of this irrep in the reducible representation expansion (for ex.,Gamma3N[])
	int m_coefficient;
public:
	Irrep() {};
	~Irrep() {};
	string GetName() {return m_name;}
	string GetFunctionSymmetry() {return m_function_symmetry;}
	int GetCoefficient() {return m_coefficient;}
	void SetName(string name) {m_name = name;}
	void SetFunctionSymmetry(string symm) {m_function_symmetry = symm;}
	void SetCoefficient(int coeff) {m_coefficient = coeff;}
};

class PointGroupIrreps
{
protected:
// Schoenflies name of the point group
	string m_pointGroupName;
// contains the number of irreducible representations of the point group
	int m_numberOfIrreps;
// the number of symmetry operator classes (number of columns in irrep tables)
	int m_numberOfSymmetryClasses;
// total number of symmetry operators
	int m_numberOfSymmetryOperators;
// contains the multiplicity of symmetry operators in columns
	vector<int> m_symmetryOperatorMultiplicity;
// names of symmetry operators, symbolic, ONE for class
	vector<string> m_symmetryOperatorNames;
// the main symmetry direction, can be 'x','y' or 'z'
	char m_mainAxisDirection;
// irrep table coefficients
	int* m_irrepCoeffs;
public:
// Irreducible representation objects
// irrep name, function symmetry and coefficient in the reducible representation
	vector<Irrep*>m_Irreps;

public:
	PointGroupIrreps();			// default constructor
	virtual ~PointGroupIrreps();
//////////////////////////////////////////////////////////////
// returns a character from 'm_Irreps'
	int GetCharacter(string irrep,string symOperator)
	{
		int irrep_index = -1;
		int operator_index = -1;
		for(int i=0;i<m_numberOfIrreps;i++)
		{
			if(m_Irreps[i]->GetName() == irrep)
				irrep_index = i;
		}
		for(i=0;i<m_numberOfSymmetryClasses;i++)
		{
			if(m_symmetryOperatorNames[i] == symOperator)
				operator_index = i;
		}
		if(irrep_index == -1 || operator_index == -1)
		{
			AfxMessageBox("Irrep not found",MB_OK);
			exit(1);
		}
		return *(m_irrepCoeffs + irrep_index*m_numberOfSymmetryClasses + operator_index);
	}
//////////////////////////////////
	int GetNumberOfIrreps() {return m_numberOfIrreps;}
	int GetNumberOfSymmetryClasses() {return m_numberOfSymmetryClasses;}
	int GetNumberOfSymmetryOperators() {return m_numberOfSymmetryOperators;}
/////////////////////////////////
	string GetOperatorSymbol(int op)
	{
		if(op < this->m_numberOfSymmetryClasses && op >= 0)
		{
			return m_symmetryOperatorNames[op];
		}
		else
		{
			AfxMessageBox("OperatorSymbol out of array boundaries",MB_OK);
			return "error";
		}
	}
///////////////////////////////////
	string GetIrrepName(int op)
	{
		if(op < this->m_numberOfIrreps && op >= 0)
		{
			return m_Irreps[op]->GetName();
		}
		else
		{
			AfxMessageBox("Irreps out of array boundaries",MB_OK);
			return "error";
		}
	}
	int GetOperatorMultiplicity(int op)
	{
		if(op < this->m_numberOfSymmetryClasses && op >= 0)
		{
			return m_symmetryOperatorMultiplicity[op];
		}
		else
		{
			AfxMessageBox("OperatorSymbol out of array boundaries",MB_OK);
			return -1;
		}
	}
///////////////////////////////////////
	char GetMainAxisDirection() {return m_mainAxisDirection;}
//
// returns the Irrep function symmetry
//
	string GetIrrepFunctionSymmetry(string irrep)
	{
		int irrep_index = -1;
		for(int i=0;i<m_numberOfIrreps;i++)
		{
			if(m_Irreps[i]->GetName() == irrep)
				irrep_index = i;
		}
		if(irrep_index == -1)
		{
			AfxMessageBox("Irrep not found",MB_OK);
			exit(1);
		}
		else
			return m_Irreps[irrep_index]->GetFunctionSymmetry();
	}
//
// coefficient of this irrep in the reducible representation expansion (for ex.,Gamma3N[])
//
	int GetIrrepCoefficient(string irrep)
	{
		int irrep_index = -1;
		for(int i=0;i<m_numberOfIrreps;i++)
		{
			if(m_Irreps[i]->GetName() == irrep)
				irrep_index = i;
		}
		if(irrep_index == -1)
		{
			AfxMessageBox("Irrep not found",MB_OK);
			exit(1);
		}
		else
			return m_Irreps[i]->GetCoefficient();
	}
};

class Cs  : public PointGroupIrreps {
public:
	Cs();
	Cs(char direction);
	virtual ~Cs() {};
};
class Ci  : public PointGroupIrreps {
public:
	Ci();
	virtual ~Ci() {};
};
class C2  : public PointGroupIrreps {
public:
	C2();
	C2(char direction);
	virtual ~C2() {};
};
class D2  : public PointGroupIrreps {
public:
	D2();
	virtual ~D2() {};
};
class C2v : public PointGroupIrreps {
public:
	C2v();
	C2v(char direction);
	virtual ~C2v() {};
};
class C2h : public PointGroupIrreps {
public:
	C2h();
	C2h(char direction);
	virtual ~C2h() {};
};
class D2h : public PointGroupIrreps {
public:
	D2h();
	virtual ~D2h() {};
};

#endif // !defined(AFX_POINTGROUPIRREPS_H__1E7C1CF4_390E_45AE_A811_E309E703E79B__INCLUDED_)
