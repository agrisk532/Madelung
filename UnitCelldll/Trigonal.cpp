// Trigonal.cpp: implementation of the Trigonal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Trigonal.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Trigonal::Trigonal()
{
	m_ID = 7;

	m_a = 2.;
	m_b = 2.;
	m_c = 2.;
	m_alpha = 100.;
	m_beta = 100.;
	m_gamma = 100.;

	m_enabledAEditing = true;	// shows whether editing enabled 
	m_enabledBEditing = false;	// b=a
    m_enabledCEditing = false;	// c=a
	m_enabledAlphaEditing = true;	// !=90
	m_enabledBetaEditing = false;	// =alpha
	m_enabledGammaEditing = false;	// =alpha

	m_abcRestrictions = "a=b=c";
	m_angleRestrictions = "alpha=beta=gamma!=90deg";
}

Trigonal::~Trigonal()
{

}

///////////////////////////////////////////////////////////////////////
//	calculates the unit cell vectors

void Trigonal::CalculateVectors()
{
// calculate the helper angle x
// m_a maximal is 120degrees. At larger angles structure impossible
	double cosx = sqrt((4*cos(m_alpha/2*FACTOR)*cos(m_alpha/2*FACTOR)-1)/3);
	double sinx = sqrt(1-cosx*cosx);
	this->m_unitCellVectorA_Cart[0] = m_a*sinx;
	this->m_unitCellVectorA_Cart[1] = 0;
	this->m_unitCellVectorA_Cart[2] = m_a*cosx;

	this->m_unitCellVectorB_Cart[0] = -m_a*sinx/2;
	this->m_unitCellVectorB_Cart[1] = m_a*sinx*sqrt(3.)/2.;
	this->m_unitCellVectorB_Cart[2] = m_unitCellVectorA_Cart[2];

	this->m_unitCellVectorC_Cart[0] = m_unitCellVectorB_Cart[0];
	this->m_unitCellVectorC_Cart[1] = -m_unitCellVectorB_Cart[1];
	this->m_unitCellVectorC_Cart[2] = m_unitCellVectorA_Cart[2];
}
