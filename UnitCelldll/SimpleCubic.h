// SimpleCubic.h: interface for the SimpleCubic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SimpleCubic_h
#define __SimpleCubic_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API SimpleCubic : public UnitCell  
{
public:
	SimpleCubic();
	virtual ~SimpleCubic();
	virtual void CalculateVectors();
};

#endif

