// BodyCenteredCubic.h: interface for the BodyCenteredCubic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __BodyCenteredCubic_h
#define __BodyCenteredCubic_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API BodyCenteredCubic : public UnitCell  
{
public:
	BodyCenteredCubic();
	virtual ~BodyCenteredCubic();
	virtual void CalculateVectors();
};

#endif

