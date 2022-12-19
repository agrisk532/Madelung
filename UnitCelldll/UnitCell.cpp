// UnitCell.cpp: implementation of the UnitCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnitCell.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UnitCell::UnitCell()
{
// number of ALL atoms in unit cell
	m_atomsInUnitCell = 0;
// number of Wyckoff positions in UnitCell (one atom per position)
	m_positionsInUnitCell = 0; 
	for(int i=0;i<3;i++)
	{
		m_unitCellVectorA_Cart[i] = 0.;	// cartesian coordinates of unit cell vectors
		m_unitCellVectorB_Cart[i] = 0.;
		m_unitCellVectorC_Cart[i] = 0.;
	}

	FACTOR = 3.141592653589793/180.;
}


UnitCell::~UnitCell()
{}

///////////////////////////////////////////////////////////////////////////
// Calculates the cartesian coordinates of an atom, given it's coordinates in
// UnitCell units (0<x,y,z<1)

void UnitCell::CalculateAtoms(double xyz[3],int index)
{
// xyz[] contains the coordinates of an atom in the units of unitcell
// 'index' is used to copy the charge and atom type values
	double c[3];
	this->CalcCartCoordFromUCCoord(xyz,c);
// push back the cartesian coordinates 
// atom charges and types will be picked out for CLattice
		this->m_unitCellAtomCartX.push_back(c[0]);
		this->m_unitCellAtomCartY.push_back(c[1]);
		this->m_unitCellAtomCartZ.push_back(c[2]);
		this->m_unitCellAtomCartCharge.push_back(this->m_unitCellAtomCharge[index]);
		this->m_unitCellAtomCartType.push_back(this->m_unitCellAtomType[index]);
		this->m_unitCellAtomCartWPos.push_back(this->m_WyckoffPosition[index]);
		this->m_unitCellAtomCartPotential.push_back(0.); // calculated potential on atom

// increase the number of atoms
		this->m_atomsInUnitCell++;
}

///////////////////////////////////////////////////////////////////////////
//

void UnitCell::Clear()
{
	m_positionsInUnitCell = 0;
	m_atomsInUnitCell = 0;
	m_WyckoffPosition.clear();
	m_unitCellAtomX.clear();
	m_unitCellAtomY.clear();
	m_unitCellAtomZ.clear();
	m_unitCellAtomCharge.clear();
	m_unitCellAtomType.clear();
	m_unitCellAtomCartX.clear();
	m_unitCellAtomCartY.clear();
	m_unitCellAtomCartZ.clear();
	m_unitCellAtomCartCharge.clear();
	m_unitCellAtomCartType.clear();
	m_unitCellAtomCartWPos.clear();

	for(int j=0;j<3;j++)
	{
		m_unitCellVectorA_Cart[j]=0.;
		m_unitCellVectorB_Cart[j]=0.;
		m_unitCellVectorC_Cart[j]=0.;
	}
}

//////////////////////////////////////////////////////////////////////////////
// calculates cartesian coordinates of all 8 vertices forming a unitCell
//
void UnitCell::CalculateVertices()
{
	int index = 0;
	for(int ia=0;ia<2;ia++)
		for(int ib=0;ib<2;ib++)
			for(int ic=0;ic<2;ic++)
			{
				this->m_unitCellVertices[index][0] =
								ia*this->m_unitCellVectorA_Cart[0] +
								ib*this->m_unitCellVectorB_Cart[0] +
								ic*this->m_unitCellVectorC_Cart[0];
				this->m_unitCellVertices[index][1] =
								ia*this->m_unitCellVectorA_Cart[1] +
								ib*this->m_unitCellVectorB_Cart[1] +
								ic*this->m_unitCellVectorC_Cart[1];
				this->m_unitCellVertices[index][2] =
								ia*this->m_unitCellVectorA_Cart[2] +
								ib*this->m_unitCellVectorB_Cart[2] +
								ic*this->m_unitCellVectorC_Cart[2];
				index++;
			}
}

///////////////////////////////////////////////////////////////////////////////
//	checks whether unitcell vertices are bonded
//
bool UnitCell::VerticesAreBonded(int i,int j)
{
	if(abs(Distance(i,j)-this->m_a)<1.e-3 || abs(Distance(i,j)-this->m_b)<1.e-3 ||
		abs(Distance(i,j)-this->m_c)<1.e-3)
	return true;
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
//	calculates the distance between two points
// 
double UnitCell::Distance(int i,int j)
{
	double distance = 0.;
	double diff;
	for(int k=0;k<3;k++)
	{
		diff = this->m_unitCellVertices[i][k]-this->m_unitCellVertices[j][k];
		distance += diff*diff;
	}
	distance = sqrt(distance);
	return distance;
}

////////////////////////////////////////////////////////////////////////////////
//
void UnitCell::CalculateReciprocalLatticeVectors()
{
// get volume of the unit cell
	double volume = this->GetVolume();
	VectProd(this->m_unitCellVectorB_Cart,this->m_unitCellVectorC_Cart,this->m_recLatticeVectorA);
	VectProd(this->m_unitCellVectorC_Cart,this->m_unitCellVectorA_Cart,this->m_recLatticeVectorB);
	VectProd(this->m_unitCellVectorA_Cart,this->m_unitCellVectorB_Cart,this->m_recLatticeVectorC);
	for(int i=0;i<3;i++)
	{
		this->m_recLatticeVectorA[i] /= volume;
		this->m_recLatticeVectorB[i] /= volume;
		this->m_recLatticeVectorC[i] /= volume;
	}
}

////////////////////////////////////////////////////////////////////////////////
//	calculates vector product of two vectors a,b and stores it in c
// 
void UnitCell::VectProd(double a[3],double b[3],double c[3])
{
	c[0] = a[1]*b[2]-b[1]*a[2];
	c[1] = a[2]*b[0]-a[0]*b[2];
	c[2] = a[0]*b[1]-b[0]*a[1];
	return;
}

/////////////////////////////////////////////////////////////////////////////////
// calculates volume of the unit cell
double UnitCell::GetVolume()
{
	double volume;
	double vProd[3];
	this->VectProd(this->m_unitCellVectorB_Cart,this->m_unitCellVectorC_Cart,vProd);
	volume = this->m_unitCellVectorA_Cart[0]*vProd[0] +
			 this->m_unitCellVectorA_Cart[1]*vProd[1] +
			 this->m_unitCellVectorA_Cart[2]*vProd[2];
	return volume;
}

/////////////////////////////////////////////////////////////////////////////////
// compute the unitcell coordinates from the cartesian coordinates for geomCenter[]
// (coordinate origins must be the same)
int UnitCell::CalcUCCoordFromCartCoord(double geomCenter[3],double geomCenter_UnitCell[3])
{
// transformation matrix between bases
	double P[3][3];
	double P_inv[3][3];
// tmp array used for debugging only
	double tmp[3];
	for(int i=0;i<3;i++)
		tmp[i] = geomCenter[i];

	P[0][0] = m_unitCellVectorA_Cart[0];
	P[1][0] = m_unitCellVectorA_Cart[1];
	P[2][0] = m_unitCellVectorA_Cart[2];
	P[0][1] = m_unitCellVectorB_Cart[0];
	P[1][1] = m_unitCellVectorB_Cart[1];
	P[2][1] = m_unitCellVectorB_Cart[2];
	P[0][2] = m_unitCellVectorC_Cart[0];
	P[1][2] = m_unitCellVectorC_Cart[1];
	P[2][2] = m_unitCellVectorC_Cart[2];

	int ret = this->InvertMatrix(P,P_inv);
	if (ret == -1) return ret;	// -1 if error
// find the coordinates
	for(i=0;i<3;i++)
	{
		geomCenter_UnitCell[i] = 0.;
		for(int j=0;j<3;j++)
			geomCenter_UnitCell[i] += P_inv[i][j] * geomCenter[j];
	}
// check if the geomCenter_UnitCell[] is within the first unitcell. Otherwise translate it there.
// it is necessary for the Wyckoff position checking in CMadelungDoc
	bool b = true;
	double expr;
	do
	{
		if(fabs(geomCenter_UnitCell[0]) > 1.0)
		{
			if(geomCenter_UnitCell[0] > 0.0)
			{
				geomCenter_UnitCell[0] -= 1.;
				continue;
			}
			if(geomCenter_UnitCell[0] < 0.0)
			{
				geomCenter_UnitCell[0] += 1.;
				continue;
			}
		}
		else
		{
			expr = fabs(fabs(geomCenter_UnitCell[0]) - 1.0);
			if(expr < 0.001)
			{
				geomCenter_UnitCell[0] = 0.0;
				b = false;
			}
			else
				b = false;
		}
	} while(b);
//
	b = true;
	do
	{
		if(fabs(geomCenter_UnitCell[1]) > 1.0)
		{
			if(geomCenter_UnitCell[1] > 0.0)
			{
				geomCenter_UnitCell[1] -= 1.;
				continue;
			}
			if(geomCenter_UnitCell[1] < 0.0)
			{
				geomCenter_UnitCell[1] += 1.;
				continue;
			}
		}
		else
		{
			expr = fabs(fabs(geomCenter_UnitCell[1]) - 1.0);
			if(expr < 0.001)
			{
				geomCenter_UnitCell[1] = 0.0;
				b = false;
			}
			else
				b = false;
		}
	} while(b);
//
	b = true;
	do
	{
		if(fabs(geomCenter_UnitCell[2]) > 1.0)
		{
			if(geomCenter_UnitCell[2] > 0.0)
			{
				geomCenter_UnitCell[2] -= 1.;
				continue;
			}
			if(geomCenter_UnitCell[2] < 0.0)
			{
				geomCenter_UnitCell[2] += 1.;
				continue;
			}
		}
		else
		{
			expr = fabs(fabs(geomCenter_UnitCell[2]) - 1.0);
			if(expr < 0.001)
			{
				geomCenter_UnitCell[2] = 0.0;
				b = false;
			}
			else
				b = false;
		}
	} while(b);
// if geomCenter_UnitCell[] contains a number, 0 < number < -1, translate it into the unitcell
		if(geomCenter_UnitCell[0] < 0.) geomCenter_UnitCell[0] += 1.;
		if(geomCenter_UnitCell[1] < 0.) geomCenter_UnitCell[1] += 1.;
		if(geomCenter_UnitCell[2] < 0.) geomCenter_UnitCell[2] += 1.;

	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////
// Inverts matrix P[3][3] and stores into P_inv[3][3]
// returns -1 if det(P) == 0, else returns 0
//
int UnitCell::InvertMatrix(double P[3][3],double P_inv[3][3])
{
// calculate the determinant
	double det_P =	P[0][0]*(P[1][1]*P[2][2]-P[2][1]*P[1][2])-
					P[0][1]*(P[1][0]*P[2][2]-P[2][0]*P[1][2])+
					P[0][2]*(P[1][0]*P[2][1]-P[2][0]*P[1][1]);
	if(det_P == 0) return -1;	// error
// inverse matrix
	P_inv[0][0] = (P[1][1]*P[2][2]-P[2][1]*P[1][2]) / det_P;
	P_inv[0][1] = (P[2][1]*P[0][2]-P[0][1]*P[2][2]) / det_P;
	P_inv[0][2] = (P[0][1]*P[1][2]-P[1][1]*P[0][2]) / det_P;
	P_inv[1][0] = (P[2][0]*P[1][2]-P[1][0]*P[2][2]) / det_P;
	P_inv[1][1] = (P[0][0]*P[2][2]-P[2][0]*P[0][2]) / det_P;
	P_inv[1][2] = (P[1][0]*P[0][2]-P[0][0]*P[1][2]) / det_P;
	P_inv[2][0] = (P[1][0]*P[2][1]-P[2][0]*P[1][1]) / det_P;
	P_inv[2][1] = (P[2][0]*P[0][1]-P[0][0]*P[2][1]) / det_P;
	P_inv[2][2] = (P[0][0]*P[1][1]-P[0][1]*P[1][0]) / det_P;
// check by multiplying P and P_inv
	double t[3][3];
	MultiplyMatrices(P,P_inv,t);
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		{
			if(i==j)
			{
				if(fabs(t[i][j] - 1.) > 1.e-8) return -1;
				else
					continue;
			}
			else
			{
				if(fabs(t[i][j]) > 1.e-8) return -1;
				else
					continue;
			}
		}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////
//	multiplies 3x3 matrices a and b and stores in c
//
void UnitCell::MultiplyMatrices(double a[3][3], double b[3][3], double c[3][3])
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		{
			c[i][j] = 0.;
			for(int k=0;k<3;k++)
				c[i][j] += a[i][k]*b[k][j];
		}
}
/////////////////////////////////////////////////////////////////////////////////
// compute the cartesian coordinates from the unitcell coordinates
// (coordinate origins must be the same)
void UnitCell::CalcCartCoordFromUCCoord(double a_UnitCell[3],double a_Cartesian[3])
{
// a_UnitCell[] contains the coordinates of an atom in the units of unitcell
// a_Cartesian[] contains 
		a_Cartesian[0] =
					a_UnitCell[0]*this->m_unitCellVectorA_Cart[0]+
					a_UnitCell[1]*this->m_unitCellVectorB_Cart[0]+
					a_UnitCell[2]*this->m_unitCellVectorC_Cart[0];
		a_Cartesian[1] = 
					a_UnitCell[0]*this->m_unitCellVectorA_Cart[1]+
					a_UnitCell[1]*this->m_unitCellVectorB_Cart[1]+
					a_UnitCell[2]*this->m_unitCellVectorC_Cart[1];
		a_Cartesian[2] =
					a_UnitCell[0]*this->m_unitCellVectorA_Cart[2]+
					a_UnitCell[1]*this->m_unitCellVectorB_Cart[2]+
					a_UnitCell[2]*this->m_unitCellVectorC_Cart[2];
}
///////////////////////////////////////////////////////////////////////////////////
// center[] is in the units of unit cell lengths
// coord[] in the units of cartesian displacements from center[]
void UnitCell::AddAtomCart(double center[3],double coord[3],int type,double charge)
{
	double center_cart[3];

	this->CalcCartCoordFromUCCoord(center,center_cart);
	for(int i=0;i<3;i++)
		center_cart[i] += coord[i];

	this->m_unitCellAtomCartX.push_back(center_cart[0]);
	this->m_unitCellAtomCartY.push_back(center_cart[1]);
	this->m_unitCellAtomCartZ.push_back(center_cart[2]);
	this->m_unitCellAtomCartCharge.push_back(charge);
	this->m_unitCellAtomCartType.push_back(type);
	this->m_unitCellAtomCartWPos.push_back('%');
	this->m_unitCellAtomCartPotential.push_back(0.); // calculated potential on atom

// increase the number of atoms
	this->m_atomsInUnitCell++;
}
