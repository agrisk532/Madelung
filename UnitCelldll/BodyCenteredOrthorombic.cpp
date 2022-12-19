// BodyCenteredOrthorombic.cpp: implementation of the BodyCenteredOrthorombic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BodyCenteredOrthorombic.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BodyCenteredOrthorombic::BodyCenteredOrthorombic()
{
	m_a = 2.;
	m_b = 3.;
	m_c = 4.;
	m_alpha = 90.;
	m_beta = 90.;
	m_gamma = 90.;

	m_enabledAEditing = true;	// shows whether editing enabled 
	m_enabledBEditing = true;
    m_enabledCEditing = true;
	m_enabledAlphaEditing = false;	// 90
	m_enabledBetaEditing = false;	// 90	
	m_enabledGammaEditing = false;	// 90
}

BodyCenteredOrthorombic::~BodyCenteredOrthorombic()
{

}

////////////////////////////////////////////////////////////////////////////
//
void BodyCenteredOrthorombic::CalculateVectors()
{
	this->m_unitCellVectorA_Cart[0] = m_a;
	this->m_unitCellVectorA_Cart[1] = 0;
	this->m_unitCellVectorA_Cart[2] = 0;

	this->m_unitCellVectorB_Cart[0] = 0;
	this->m_unitCellVectorB_Cart[1] = m_b;
	this->m_unitCellVectorB_Cart[2] = 0;

	this->m_unitCellVectorC_Cart[0] = 0;
	this->m_unitCellVectorC_Cart[1] = 0;
	this->m_unitCellVectorC_Cart[2] = m_c;
}

