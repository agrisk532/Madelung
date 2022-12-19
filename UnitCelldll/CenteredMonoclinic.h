// CenteredMonoclinic.h: interface for the CenteredMonoclinic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CenteredMonoclinic_h
#define __CenteredMonoclinic_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API CenteredMonoclinic : public UnitCell  
{
public:
	CenteredMonoclinic();
	virtual ~CenteredMonoclinic();
	virtual void CalculateVectors();
};

#endif

