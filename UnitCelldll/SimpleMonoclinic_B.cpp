// SimpleMonoclinic_B.cpp: implementation of the SimpleMonoclinic (unique axis b) class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleMonoclinic_B.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SimpleMonoclinic_B::SimpleMonoclinic_B()
{
	m_ID = 2;

	m_a = 2.;
	m_b = 4.;
	m_c = 3.;
	m_alpha = 90.;
	m_beta = 100.;
	m_gamma = 90.;

	m_enabledAEditing = true;	// shows whether editing enabled
	m_enabledBEditing = true;
    m_enabledCEditing = true;
	m_enabledAlphaEditing = false;	// 90
	m_enabledBetaEditing = true;	// >90
	m_enabledGammaEditing = false;	// 90

	m_abcRestrictions = "a!=b!=c";
	m_angleRestrictions = "alpha=gamma=90deg!=beta>90deg";
}

SimpleMonoclinic_B::~SimpleMonoclinic_B()
{

}

////////////////////////////////////////////////////////////////////////////
//
void SimpleMonoclinic_B::CalculateVectors()
{
	this->m_unitCellVectorA_Cart[0] = m_a;
	this->m_unitCellVectorA_Cart[1] = 0.;
	this->m_unitCellVectorA_Cart[2] = 0.;

	this->m_unitCellVectorB_Cart[0] = 0.;
	this->m_unitCellVectorB_Cart[1] = m_b;
	this->m_unitCellVectorB_Cart[2] = 0.;

	this->m_unitCellVectorC_Cart[0] = m_c*cos(m_beta*FACTOR);
	this->m_unitCellVectorC_Cart[1] = 0.;
	this->m_unitCellVectorC_Cart[2] = m_c*sin(m_beta*FACTOR);
}
