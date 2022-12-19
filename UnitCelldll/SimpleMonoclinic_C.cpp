// SimpleMonoclinic_C.cpp: implementation of the SimpleMonoclinic unique axis C class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleMonoclinic_C.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SimpleMonoclinic_C::SimpleMonoclinic_C()
{
	m_ID = 3;

	m_a = 2.;
	m_b = 3.;
	m_c = 4.;
	m_alpha = 90.;
	m_beta = 90.;
	m_gamma = 100.;

	m_enabledAEditing = true;	// shows whether editing enabled 
	m_enabledBEditing = true;
    m_enabledCEditing = true;
	m_enabledAlphaEditing = false;	// 90
	m_enabledBetaEditing = false;	// 90
	m_enabledGammaEditing = true;	// >90

	m_abcRestrictions = "a!=b!=c";
	m_angleRestrictions = "alpha=beta=90deg!=gamma>90deg";
}

SimpleMonoclinic_C::~SimpleMonoclinic_C()
{

}

////////////////////////////////////////////////////////////////////////////
//
void SimpleMonoclinic_C::CalculateVectors()
{
	this->m_unitCellVectorA_Cart[0] = m_a;	// x
	this->m_unitCellVectorA_Cart[1] = 0.;	// y
	this->m_unitCellVectorA_Cart[2] = 0.;	// z

	this->m_unitCellVectorB_Cart[0] = m_b*cos(m_gamma*FACTOR);	// x
	this->m_unitCellVectorB_Cart[1] = m_b*sin(m_gamma*FACTOR);	// y
	this->m_unitCellVectorB_Cart[2] = 0.;						// z

	this->m_unitCellVectorC_Cart[0] = 0.;	// x
	this->m_unitCellVectorC_Cart[1] = 0.;	// y
	this->m_unitCellVectorC_Cart[2] = m_c;	// z
}
