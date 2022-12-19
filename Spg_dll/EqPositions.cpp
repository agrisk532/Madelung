// EqPositions.cpp: implementation of the EqPositions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EqPositions.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

EqPositions::EqPositions(int count)
{
	m_X = new double[count];
	m_Y = new double[count];
	m_Z = new double[count];
	m_size = count;
}

EqPositions::~EqPositions()
{
	delete [] m_X;
	delete [] m_Y;
	delete [] m_Z;
}
