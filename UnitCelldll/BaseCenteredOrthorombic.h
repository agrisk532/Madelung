// BaseCenteredOrthorombic.h: interface for the BaseCenteredOrthorombic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __BaseCenteredOrthorombic_h
#define __BaseCenteredOrthorombic_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API BaseCenteredOrthorombic : public UnitCell  
{
public:
	BaseCenteredOrthorombic();
	virtual ~BaseCenteredOrthorombic();
	virtual void CalculateVectors();
};

#endif

