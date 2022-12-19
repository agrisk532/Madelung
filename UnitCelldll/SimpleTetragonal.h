// Triclinic.h: interface for the Trigonal class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SimpleTetragonal_h
#define __SimpleTetragonal_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API SimpleTetragonal : public UnitCell  
{
public:
	SimpleTetragonal();
	virtual ~SimpleTetragonal();
	virtual void CalculateVectors();
};

#endif

