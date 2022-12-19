// CenteredTetragonal.h: interface for the CenteredTetragonal class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CenteredTetragonal_h
#define __CenteredTetragonal_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API CenteredTetragonal : public UnitCell  
{
public:
	CenteredTetragonal();
	virtual ~CenteredTetragonal();
	virtual void CalculateVectors();
};

#endif

