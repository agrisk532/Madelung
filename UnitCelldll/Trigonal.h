// Trigonal.h: interface for the Trigonal class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __Trigonal_h
#define __Trigonal_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API Trigonal : public UnitCell  
{
public:
	Trigonal();
	virtual ~Trigonal();
	virtual void CalculateVectors();
};

#endif

