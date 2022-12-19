// SimpleOrthorombic.h: interface for the Trigonal class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SimpleOrthorombic_h
#define __SimpleOrthorombic_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API SimpleOrthorombic : public UnitCell  
{
public:
	SimpleOrthorombic();
	virtual ~SimpleOrthorombic();
	virtual void CalculateVectors();
};

#endif

