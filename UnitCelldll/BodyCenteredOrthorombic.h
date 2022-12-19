// BodyCenteredOrthorombic.h: interface for the BodyCenteredOrthorombic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __BodyCenteredOrthorombic_h
#define __BodyCenteredOrthorombic_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API BodyCenteredOrthorombic : public UnitCell  
{
public:
	BodyCenteredOrthorombic();
	virtual ~BodyCenteredOrthorombic();
	virtual void CalculateVectors();
};

#endif

