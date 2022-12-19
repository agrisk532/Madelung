§// PointGroupIrreps.cpp: implementation of the PointGroupIrreps class.
//
// Implemented point groups : Cs,Ci,C2,C2h,C2v,D2,D2h
//
// Recognized symmetry operators:
// E,sh,i,C2,C2(z),C2(y),C2(x),sv(xz),sv(yz),sv(xy)
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "madelung.h"
#include "PointGroupIrreps.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PointGroupIrreps::PointGroupIrreps() {}

PointGroupIrreps::~PointGroupIrreps()
{
	int size = m_Irreps.size();
	if(size)
	{
		for(int i=0;i<size;i++)
			delete m_Irreps[i];
		this->m_Irreps.clear();
	}
	if(m_irrepCoeffs) delete [] m_irrepCoeffs;
}
////////////////////////////////////////////////////////////////////////
//  Cs, with main axis direction 'dir'
////////////////////////////////////////////////////////////////////////
Cs::Cs()
{
	Cs::Cs('z');
}

Cs::Cs(char dir)
{
	this->m_pointGroupName = "Cs";
	this->m_numberOfIrreps = 2;
	this->m_numberOfSymmetryClasses = 2;
	this->m_numberOfSymmetryOperators = 2;
	this->m_mainAxisDirection = dir;

	this->m_symmetryOperatorNames.push_back("E");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("sh");
	this->m_symmetryOperatorMultiplicity.push_back(1);

	if(dir == 'z')
	{
//
// A'
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[0]->SetName("A'");
		m_Irreps[0]->SetFunctionSymmetry("x2,y2,z2,x,y,xy");
		m_Irreps[0]->SetCoefficient(0);
// A''
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[1]->SetName("A''");
		m_Irreps[1]->SetFunctionSymmetry("z,zx,zy");
		m_Irreps[1]->SetCoefficient(0);
	}
	else
	if(dir == 'y')
	{
//
// A'
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[0]->SetName("A'");
		m_Irreps[0]->SetFunctionSymmetry("x2,y2,z2,x,z,xz");
		m_Irreps[0]->SetCoefficient(0);
// A''
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[1]->SetName("A''");
		m_Irreps[1]->SetFunctionSymmetry("y,yx,yz");
		m_Irreps[1]->SetCoefficient(0);
	}
	else
	if(dir == 'x')
	{
//
// A'
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[0]->SetName("A'");
		m_Irreps[0]->SetFunctionSymmetry("x2,y2,z2,y,z,yz");
		m_Irreps[0]->SetCoefficient(0);
// A''
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[1]->SetName("A''");
		m_Irreps[1]->SetFunctionSymmetry("x,xy,xz");
		m_Irreps[1]->SetCoefficient(0);
	}
	else
	{
		AfxMessageBox("PointGroupIrreps : unknown main axis direction",MB_OK);
		exit(1);
	}

	m_irrepCoeffs = new int[this->m_numberOfIrreps * this->m_numberOfSymmetryClasses];
// fill in the irrep matrix, by rows
	int* p = m_irrepCoeffs;

// A'	
// 0,0
	*(p++) = 1;
// 0,1
	*(p++) = 1;

//	A''
// 1,0
	*(p++) = 1;
// 1,1
	*(p++) = -1;
}

////////////////////////////////////////////////////////////////////////
//  Ci
////////////////////////////////////////////////////////////////////////
Ci::Ci()
{
	this->m_pointGroupName = "Ci";
	this->m_numberOfIrreps = 2;
	this->m_numberOfSymmetryClasses = 2;
	this->m_numberOfSymmetryOperators = 2;
	this->m_mainAxisDirection = ' '; // dummy direction

	this->m_symmetryOperatorNames.push_back("E");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("i");
	this->m_symmetryOperatorMultiplicity.push_back(1);
//
// Ag
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[0]->SetName("Ag");
	m_Irreps[0]->SetFunctionSymmetry("x2,y2,z2");
	m_Irreps[0]->SetCoefficient(0);
//
// Au
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[1]->SetName("Au");
	m_Irreps[1]->SetFunctionSymmetry("x,y,z,xy,zx,yz");
	m_Irreps[1]->SetCoefficient(0);

	m_irrepCoeffs = new int[this->m_numberOfIrreps * this->m_numberOfSymmetryClasses];
// fill in the irrep matrix, by rows
	int* p = m_irrepCoeffs;

// Ag
// 0,0
	*(p++) = 1;
// 0,1
	*(p++) = 1;

//	Au
// 1,0
	*(p++) = 1;
// 1,1
	*(p++) = -1;
}
////////////////////////////////////////////////////////////////////////
//  C2
////////////////////////////////////////////////////////////////////////
C2::C2()
{
	C2::C2('z');
}

C2::C2(char dir)
{
	this->m_pointGroupName = "C2";
	this->m_numberOfIrreps = 2;
	this->m_numberOfSymmetryClasses = 2;
	this->m_numberOfSymmetryOperators = 2;
	this->m_mainAxisDirection = dir;

	this->m_symmetryOperatorNames.push_back("E");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("C2");
	this->m_symmetryOperatorMultiplicity.push_back(1);

	if(dir == 'z')
	{
//
// A
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[0]->SetName("A");
		m_Irreps[0]->SetFunctionSymmetry("x2,y2,z2,z,xy");
		m_Irreps[0]->SetCoefficient(0);
// B
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[1]->SetName("B");
		m_Irreps[1]->SetFunctionSymmetry("x,y,zx,zy");
		m_Irreps[1]->SetCoefficient(0);
	}
	else
	if(dir == 'y')
	{
//
// A
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[0]->SetName("A");
		m_Irreps[0]->SetFunctionSymmetry("x2,y2,z2,y,zx");
		m_Irreps[0]->SetCoefficient(0);
// B
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[1]->SetName("B");
		m_Irreps[1]->SetFunctionSymmetry("x,z,yx,yz");
		m_Irreps[1]->SetCoefficient(0);
	}
	else
	if(dir == 'x')
	{
//
// A
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[0]->SetName("A");
		m_Irreps[0]->SetFunctionSymmetry("x2,y2,z2,x,yz");
		m_Irreps[0]->SetCoefficient(0);
// B
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[1]->SetName("B");
		m_Irreps[1]->SetFunctionSymmetry("y,z,xy,xz");
		m_Irreps[1]->SetCoefficient(0);
	}
	else
	{
		AfxMessageBox("PointGroupIrreps : unknown main axis direction",MB_OK);
		exit(1);
	}

	m_irrepCoeffs = new int[this->m_numberOfIrreps * this->m_numberOfSymmetryClasses];
// fill in the irrep matrix, by rows
	int* p = m_irrepCoeffs;

// A
// 0,0
	*(p++) = 1;
// 0,1
	*(p++) = 1;

//	B
// 1,0
	*(p++) = 1;
// 1,1
	*(p++) = -1;
}
////////////////////////////////////////////////////////////////////////
//  D2
////////////////////////////////////////////////////////////////////////
D2::D2()
{
	this->m_pointGroupName = "D2";
	this->m_numberOfIrreps = 4;
	this->m_numberOfSymmetryClasses = 4;
	this->m_numberOfSymmetryOperators = 4;
	this->m_mainAxisDirection = ' '; // dummy direction

	this->m_symmetryOperatorNames.push_back("E");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("C2(z)");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("C2(y)");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("C2(x)");
	this->m_symmetryOperatorMultiplicity.push_back(1);
//
// A
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[0]->SetName("A");
	m_Irreps[0]->SetFunctionSymmetry("x2,y2,z2");
	m_Irreps[0]->SetCoefficient(0);
//
// B1
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[1]->SetName("B1");
	m_Irreps[1]->SetFunctionSymmetry("z,xy");
	m_Irreps[1]->SetCoefficient(0);
//
// B2
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[2]->SetName("B2");
	m_Irreps[2]->SetFunctionSymmetry("y,zx");
	m_Irreps[2]->SetCoefficient(0);
//
// B3
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[3]->SetName("B3");
	m_Irreps[3]->SetFunctionSymmetry("x,yz");
	m_Irreps[3]->SetCoefficient(0);

	m_irrepCoeffs = new int[this->m_numberOfIrreps * this->m_numberOfSymmetryClasses];
// fill in the irrep matrix, by rows
	int* p = m_irrepCoeffs;

// A
// 0,0
	*(p++) = 1;
// 0,1
	*(p++) = 1;
// 0,2
	*(p++) = 1;
// 0,3
	*(p++) = 1;

//	B1
// 1,0
	*(p++) = 1;
// 1,1
	*(p++) = 1;
// 1,2
	*(p++) = -1;
// 1,3
	*(p++) = -1;

//	B2
// 2,0
	*(p++) = 1;
// 2,1
	*(p++) = -1;
// 2,2
	*(p++) = 1;
// 2,3
	*(p++) = -1;

//	B3
// 3,0
	*(p++) = 1;
// 3,1
	*(p++) = -1;
// 3,2
	*(p++) = -1;
// 3,3
	*(p++) = 1;
}

////////////////////////////////////////////////////////////////////////
//  C2v
////////////////////////////////////////////////////////////////////////
C2v::C2v()
{
	C2v::C2v('z');
}

C2v::C2v(char dir)
{
	this->m_pointGroupName = "C2v";
	this->m_numberOfIrreps = 4;
	this->m_numberOfSymmetryClasses = 4;
	this->m_numberOfSymmetryOperators = 4;
	this->m_mainAxisDirection = dir;

	if(dir == 'z')
	{
		this->m_symmetryOperatorNames.push_back("E");
		this->m_symmetryOperatorMultiplicity.push_back(1);
		this->m_symmetryOperatorNames.push_back("C2");
		this->m_symmetryOperatorMultiplicity.push_back(1);
		this->m_symmetryOperatorNames.push_back("sv(xz)");
		this->m_symmetryOperatorMultiplicity.push_back(1);
		this->m_symmetryOperatorNames.push_back("sv(yz)");
		this->m_symmetryOperatorMultiplicity.push_back(1);
//
// A1
//
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[0]->SetName("A1");
		m_Irreps[0]->SetFunctionSymmetry("z,x2,y2,z2");
		m_Irreps[0]->SetCoefficient(0);
// A2
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[1]->SetName("A2");
		m_Irreps[1]->SetFunctionSymmetry("xy");
		m_Irreps[1]->SetCoefficient(0);
//
// B1
//
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[2]->SetName("B1");
		m_Irreps[2]->SetFunctionSymmetry("x,zx");
		m_Irreps[2]->SetCoefficient(0);
// B2
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[3]->SetName("B2");
		m_Irreps[3]->SetFunctionSymmetry("y,yz");
		m_Irreps[3]->SetCoefficient(0);
	}
	else
	if(dir == 'y')
	{
		this->m_symmetryOperatorNames.push_back("E");
		this->m_symmetryOperatorMultiplicity.push_back(1);
		this->m_symmetryOperatorNames.push_back("C2");
		this->m_symmetryOperatorMultiplicity.push_back(1);
		this->m_symmetryOperatorNames.push_back("sv(xy)");
		this->m_symmetryOperatorMultiplicity.push_back(1);
		this->m_symmetryOperatorNames.push_back("sv(yz)");
		this->m_symmetryOperatorMultiplicity.push_back(1);
//
// A1
//
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[0]->SetName("A1");
		m_Irreps[0]->SetFunctionSymmetry("y,x2,y2,z2");
		m_Irreps[0]->SetCoefficient(0);
// A2
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[1]->SetName("A2");
		m_Irreps[1]->SetFunctionSymmetry("xz");
		m_Irreps[1]->SetCoefficient(0);
//
// B1
//
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[2]->SetName("B1");
		m_Irreps[2]->SetFunctionSymmetry("x,yx");
		m_Irreps[2]->SetCoefficient(0);
// B2
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[3]->SetName("B2");
		m_Irreps[3]->SetFunctionSymmetry("z,yz");
		m_Irreps[3]->SetCoefficient(0);
	}
	else
	if(dir == 'x')
	{
		this->m_symmetryOperatorNames.push_back("E");
		this->m_symmetryOperatorMultiplicity.push_back(1);
		this->m_symmetryOperatorNames.push_back("C2");
		this->m_symmetryOperatorMultiplicity.push_back(1);
		this->m_symmetryOperatorNames.push_back("sv(xy)");
		this->m_symmetryOperatorMultiplicity.push_back(1);
		this->m_symmetryOperatorNames.push_back("sv(xz)");
		this->m_symmetryOperatorMultiplicity.push_back(1);
//
// A1
//
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[0]->SetName("A1");
		m_Irreps[0]->SetFunctionSymmetry("x,x2,y2,z2");
		m_Irreps[0]->SetCoefficient(0);
// A2
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[1]->SetName("A2");
		m_Irreps[1]->SetFunctionSymmetry("yz");
		m_Irreps[1]->SetCoefficient(0);
//
// B1
//
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[2]->SetName("B1");
		m_Irreps[2]->SetFunctionSymmetry("y,yx");
		m_Irreps[2]->SetCoefficient(0);
// B2
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[3]->SetName("B2");
		m_Irreps[3]->SetFunctionSymmetry("z,xz");
		m_Irreps[3]->SetCoefficient(0);
	}
	else
	{
		AfxMessageBox("PointGroupIrreps : unknown main axis direction",MB_OK);
		exit(1);
	}

	m_irrepCoeffs = new int[this->m_numberOfIrreps * this->m_numberOfSymmetryClasses];
// fill in the irrep matrix, by rows
	int* p = m_irrepCoeffs;

// A1
// 0,0
	*(p++) = 1;
// 0,1
	*(p++) = 1;
// 0,2
	*(p++) = 1;
// 0,3
	*(p++) = 1;

// A2
// 1,0
	*(p++) = 1;
// 1,1
	*(p++) = 1;
// 1,2
	*(p++) = -1;
// 1,3
	*(p++) = -1;

// B1
// 2,0
	*(p++) = 1;
// 2,1
	*(p++) = -1;
// 2,2
	*(p++) = 1;
// 2,3
	*(p++) = -1;

// B2
// 3,0
	*(p++) = 1;
// 3,1
	*(p++) = -1;
// 3,2
	*(p++) = -1;
// 3,3
	*(p++) = 1;
}

////////////////////////////////////////////////////////////////////////
//  C2h
////////////////////////////////////////////////////////////////////////
C2h::C2h()
{
	C2h::C2h('z');
}

C2h::C2h(char dir)
{
	this->m_pointGroupName = "C2h";
	this->m_numberOfIrreps = 4;
	this->m_numberOfSymmetryClasses = 4;
	this->m_numberOfSymmetryOperators = 4;
	this->m_mainAxisDirection = dir;

	this->m_symmetryOperatorNames.push_back("E");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("C2");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("i");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("sh");
	this->m_symmetryOperatorMultiplicity.push_back(1);

	if(dir == 'z')
	{
//
// Ag
//
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[0]->SetName("Ag");
		m_Irreps[0]->SetFunctionSymmetry("x2,y2,z2,xy");
		m_Irreps[0]->SetCoefficient(0);
// Bg
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[1]->SetName("Bg");
		m_Irreps[1]->SetFunctionSymmetry("yz,zx");
		m_Irreps[1]->SetCoefficient(0);
//
// Au
//
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[2]->SetName("Au");
		m_Irreps[2]->SetFunctionSymmetry("z");
		m_Irreps[2]->SetCoefficient(0);
// Bu
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[3]->SetName("Bu");
		m_Irreps[3]->SetFunctionSymmetry("x,y");
		m_Irreps[3]->SetCoefficient(0);
	}
	else
	if(dir == 'y')
	{
//
// Ag
//
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[0]->SetName("Ag");
		m_Irreps[0]->SetFunctionSymmetry("x2,y2,z2,xz");
		m_Irreps[0]->SetCoefficient(0);
// Bg
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[1]->SetName("Bg");
		m_Irreps[1]->SetFunctionSymmetry("yz,yx");
		m_Irreps[1]->SetCoefficient(0);
//
// Au
//
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[2]->SetName("Au");
		m_Irreps[2]->SetFunctionSymmetry("y");
		m_Irreps[2]->SetCoefficient(0);
// Bu
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[3]->SetName("Bu");
		m_Irreps[3]->SetFunctionSymmetry("x,z");
		m_Irreps[3]->SetCoefficient(0);
	}
	else
	if(dir == 'x')
	{
//
// Ag
//
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[0]->SetName("Ag");
		m_Irreps[0]->SetFunctionSymmetry("x2,y2,z2,yz");
		m_Irreps[0]->SetCoefficient(0);
// Bg
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[1]->SetName("Bg");
		m_Irreps[1]->SetFunctionSymmetry("xy,xz");
		m_Irreps[1]->SetCoefficient(0);
//
// Au
//
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[2]->SetName("Au");
		m_Irreps[2]->SetFunctionSymmetry("x");
		m_Irreps[2]->SetCoefficient(0);
// Bu
		this->m_Irreps.push_back(new Irrep);
		m_Irreps[3]->SetName("Bu");
		m_Irreps[3]->SetFunctionSymmetry("y,z");
		m_Irreps[3]->SetCoefficient(0);
	}
	else
	{
		AfxMessageBox("PointGroupIrreps : unknown main axis direction",MB_OK);
		exit(1);
	}

	m_irrepCoeffs = new int[this->m_numberOfIrreps * this->m_numberOfSymmetryClasses];
// fill in the irrep matrix, by rows
	int* p = m_irrepCoeffs;

// Ag
// 0,0
	*(p++) = 1;
// 0,1
	*(p++) = 1;
// 0,2
	*(p++) = 1;
// 0,3
	*(p++) = 1;

// Bg
// 1,0
	*(p++) = 1;
// 1,1
	*(p++) = -1;
// 1,2
	*(p++) = 1;
// 1,3
	*(p++) = -1;

// Au
// 2,0
	*(p++) = 1;
// 2,1
	*(p++) = 1;
// 2,2
	*(p++) = -1;
// 2,3
	*(p++) = -1;

// Bu
// 3,0
	*(p++) = 1;
// 3,1
	*(p++) = -1;
// 3,2
	*(p++) = -1;
// 3,3
	*(p++) = 1;
}

////////////////////////////////////////////////////////////////////////
//  D2h
////////////////////////////////////////////////////////////////////////
D2h::D2h()
{
	this->m_pointGroupName = "D2h";
	this->m_numberOfIrreps = 8;
	this->m_numberOfSymmetryClasses = 8;
	this->m_numberOfSymmetryOperators = 8;
	this->m_mainAxisDirection = ' '; // dummy direction

	this->m_symmetryOperatorNames.push_back("E");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("C2(z)");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("C2(y)");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("C2(x)");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("i");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("sv(xy)");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("sv(xz)");
	this->m_symmetryOperatorMultiplicity.push_back(1);
	this->m_symmetryOperatorNames.push_back("sv(yz)");
	this->m_symmetryOperatorMultiplicity.push_back(1);

//
// Ag
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[0]->SetName("Ag");
	m_Irreps[0]->SetFunctionSymmetry("x2,y2,z2");
	m_Irreps[0]->SetCoefficient(0);
//
// B1g
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[1]->SetName("B1g");
	m_Irreps[1]->SetFunctionSymmetry("xy");
	m_Irreps[1]->SetCoefficient(0);
//
// B2g
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[2]->SetName("B2g");
	m_Irreps[2]->SetFunctionSymmetry("zx");
	m_Irreps[2]->SetCoefficient(0);
//
// B3g
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[3]->SetName("B3g");
	m_Irreps[3]->SetFunctionSymmetry("yz");
	m_Irreps[3]->SetCoefficient(0);

//
// Au
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[4]->SetName("Au");
	m_Irreps[4]->SetFunctionSymmetry(" ");
	m_Irreps[4]->SetCoefficient(0);
//
// B1u
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[5]->SetName("B1u");
	m_Irreps[5]->SetFunctionSymmetry("z");
	m_Irreps[5]->SetCoefficient(0);
//
// B2u
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[6]->SetName("B2u");
	m_Irreps[6]->SetFunctionSymmetry("y");
	m_Irreps[6]->SetCoefficient(0);
//
// B3u
	this->m_Irreps.push_back(new Irrep);
	m_Irreps[7]->SetName("B3u");
	m_Irreps[7]->SetFunctionSymmetry("x");
	m_Irreps[7]->SetCoefficient(0);

	m_irrepCoeffs = new int[this->m_numberOfIrreps * this->m_numberOfSymmetryClasses];
// fill in the irrep matrix, by rows
	int* p = m_irrepCoeffs;
// Ag
// 0,0
	*(p++) = 1;
// 0,1
	*(p++) = 1;
// 0,2
	*(p++) = 1;
// 0,3
	*(p++) = 1;
// 0,4
	*(p++) = 1;
// 0,5
	*(p++) = 1;
// 0,6
	*(p++) = 1;
// 0,7
	*(p++) = 1;

// B1g
// 1,0
	*(p++) = 1;
// 1,1
	*(p++) = 1;
// 1,2
	*(p++) = -1;
// 1,3
	*(p++) = -1;
// 1,4
	*(p++) = 1;
// 1,5
	*(p++) = 1;
// 1,6
	*(p++) = -1;
// 1,7
	*(p++) = -1;

// B2g
// 2,0
	*(p++) = 1;
// 2,1
	*(p++) = -1;
// 2,2
	*(p++) = 1;
// 2,3
	*(p++) = -1;
// 2,4
	*(p++) = 1;
// 2,5
	*(p++) = -1;
// 2,6
	*(p++) = 1;
// 2,7
	*(p++) = -1;

// B3g
// 3,0
	*(p++) = 1;
// 3,1
	*(p++) = -1;
// 3,2
	*(p++) = -1;
// 3,3
	*(p++) = 1;
// 3,4
	*(p++) = 1;
// 3,5
	*(p++) = -1;
// 3,6
	*(p++) = -1;
// 3,7
	*(p++) = 1;

// Au
// 4,0
	*(p++) = 1;
// 4,1
	*(p++) = 1;
// 4,2
	*(p++) = 1;
// 4,3
	*(p++) = 1;
// 4,4
	*(p++) = -1;
// 4,5
	*(p++) = -1;
// 4,6
	*(p++) = -1;
// 4,7
	*(p++) = -1;

// B1u
// 5,0
	*(p++) = 1;
// 5,1
	*(p++) = 1;
// 5,2
	*(p++) = -1;
// 5,3
	*(p++) = -1;
// 5,4
	*(p++) = -1;
// 5,5
	*(p++) = -1;
// 5,6
	*(p++) = 1;
// 5,7
	*(p++) = 1;

// B2u
// 6,0
	*(p++) = 1;
// 6,1
	*(p++) = -1;
// 6,2
	*(p++) = 1;
// 6,3
	*(p++) = -1;
// 6,4
	*(p++) = -1;
// 6,5
	*(p++) = 1;
// 6,6
	*(p++) = -1;
// 6,7
	*(p++) = 1;

// B3u
// 7,0
	*(p++) = 1;
// 7,1
	*(p++) = -1;
// 7,2
	*(p++) = -1;
// 7,3
	*(p++) = 1;
// 7,4
	*(p++) = -1;
// 7,5
	*(p++) = 1;
// 7,6
	*(p++) = 1;
// 7,7
	*(p++) = -1;
}
