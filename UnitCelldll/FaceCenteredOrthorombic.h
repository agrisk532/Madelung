// FaceCenteredOrthorombic.h: interface for the FaceCenteredOrthorombic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __FaceCenteredOrthorombic_h
#define __FaceCenteredOrthorombic_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API FaceCenteredOrthorombic : public UnitCell  
{
public:
	FaceCenteredOrthorombic();
	virtual ~FaceCenteredOrthorombic();
	virtual void CalculateVectors();
};

#endif

