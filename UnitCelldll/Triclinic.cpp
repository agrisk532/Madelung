// Triclinic.cpp: implementation of the Triclinic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Triclinic.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Triclinic::Triclinic()
{
	m_ID = 6;

	m_a = 2.;
	m_b = 3.;
	m_c = 4.;
	m_alpha = 70.;
	m_beta = 75.;
	m_gamma = 80.;

	m_enabledAEditing = true;	// shows whether editing enabled
	m_enabledBEditing = true;
    m_enabledCEditing = true;
	m_enabledAlphaEditing = true;
	m_enabledBetaEditing = true;
	m_enabledGammaEditing = true;

	m_abcRestrictions = "a!=b!=c";
	m_angleRestrictions = "alpha!=beta!=gamma!=90deg";
}

Triclinic::~Triclinic()
{

}

/////////////////////////////////////////////////////////////////////////////
//

void Triclinic::CalculateVectors()
{
// calculate the helper angle x
// m_a maximal is 120degrees. At larger angles structure impossible
	this->m_unitCellVectorA_Cart[0] = m_a;
	this->m_unitCellVectorA_Cart[1] = 0.;
	this->m_unitCellVectorA_Cart[2] = 0.;

	this->m_unitCellVectorB_Cart[0] = m_b*cos(m_gamma*FACTOR);
	this->m_unitCellVectorB_Cart[1] = m_b*sin(m_gamma*FACTOR);
	this->m_unitCellVectorB_Cart[2] = 0.;
// Gamma = Gamma1 + Gamma2 (two a,b in-plane angles)
	double tgGamma1 = (cos(m_alpha*FACTOR)-cos(m_beta*FACTOR)*cos(m_gamma*FACTOR))/
					(cos(m_beta*FACTOR)*sin(m_gamma*FACTOR));
	double cosGamma1 = sqrt(1./(1.+tgGamma1*tgGamma1));
	double sinGamma1 = sqrt(1.-cosGamma1*cosGamma1);

	this->m_unitCellVectorC_Cart[0] = m_c*cos(m_beta*FACTOR);
	this->m_unitCellVectorC_Cart[1] = this->m_unitCellVectorC_Cart[0]*
											sinGamma1/cosGamma1;
// Gamma3 - angle what z axis makes with a,b plane
	double cosGamma3 = this->m_unitCellVectorC_Cart[0]/cosGamma1/m_c;
	double sinGamma3 = sqrt(1.-cosGamma3*cosGamma3);
	this->m_unitCellVectorC_Cart[2] = m_c*sinGamma3;
}

