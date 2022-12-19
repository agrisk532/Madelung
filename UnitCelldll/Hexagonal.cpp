// Hexagonal.cpp: implementation of the Hexagonal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Hexagonal.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Hexagonal::Hexagonal()
{
	m_ID = 0;

	m_a = 2.;
	m_b = 2.;
	m_c = 3.;
	m_alpha = 90.;
	m_beta = 90.;
	m_gamma = 120.;

	m_enabledAEditing = true;	// shows whether editing enabled 
	m_enabledBEditing = false;	// b=a
    m_enabledCEditing = true;
	m_enabledAlphaEditing = false;	// 90
	m_enabledBetaEditing = false;	// 90	
	m_enabledGammaEditing = false;	// 120

	m_abcRestrictions = "a=b!=c";
	m_angleRestrictions = "alpha=beta=90deg;gamma=120deg";
}

Hexagonal::~Hexagonal()
{

}

////////////////////////////////////////////////////////////////////////
//

void Hexagonal::CalculateVectors()
{
// calculate the helper angle x
// m_a maximal is 120degrees. At larger angles structure impossible
	this->m_unitCellVectorA_Cart[0] = m_a*sqrt(3.)/2.;
	this->m_unitCellVectorA_Cart[1] = -m_a/2.;
	this->m_unitCellVectorA_Cart[2] = 0;

	this->m_unitCellVectorB_Cart[0] = 0;
	this->m_unitCellVectorB_Cart[1] = m_a;
	this->m_unitCellVectorB_Cart[2] = 0;

	this->m_unitCellVectorC_Cart[0] = 0;
	this->m_unitCellVectorC_Cart[1] = 0;
	this->m_unitCellVectorC_Cart[2] = m_c;
}

