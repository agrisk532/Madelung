// SimpleCubic.cpp: implementation of the SimpleCubic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleCubic.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SimpleCubic::SimpleCubic()
{
	m_ID = 1;

	m_a = 2.;
	m_b = 2.;
	m_c = 2.;
	m_alpha = 90.;
	m_beta = 90.;
	m_gamma = 90.;
	m_enabledAEditing = true;	// shows whether editing enabled 
	m_enabledBEditing = false;
    m_enabledCEditing = false;
	m_enabledAlphaEditing = false;	// 90
	m_enabledBetaEditing = false;	// 90	
	m_enabledGammaEditing = false;	// 90

	m_abcRestrictions = "a=b=c";
	m_angleRestrictions = "alpha=beta=gamma=90deg";
}

SimpleCubic::~SimpleCubic()
{

}

////////////////////////////////////////////////////////////////////////////
//
void SimpleCubic::CalculateVectors()
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

