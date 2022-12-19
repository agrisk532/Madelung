// Hexagonal.h: interface for the Hexagonal class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __Hexagonal_h
#define __Hexagonal_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API Hexagonal : public UnitCell  
{
public:
	Hexagonal();
	virtual ~Hexagonal();
	virtual void CalculateVectors();
};

#endif

