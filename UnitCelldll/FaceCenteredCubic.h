// FaceCenteredCubic.h: interface for the FaceCenteredCubic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __FaceCenteredCubic_h
#define __FaceCenteredCubic_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API FaceCenteredCubic : public UnitCell  
{
public:
	FaceCenteredCubic();
	virtual ~FaceCenteredCubic();
	virtual void CalculateVectors();
};

#endif

