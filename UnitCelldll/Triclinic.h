// Triclinic.h: interface for the Trigonal class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __Triclinic_h
#define __Triclinic_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API Triclinic : public UnitCell  
{
public:
	Triclinic();
	virtual ~Triclinic();
	virtual void CalculateVectors();
};

#endif

