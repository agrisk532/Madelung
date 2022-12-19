// SimpleMonoclinic_C.h: interface for the SimpleMonoclinic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SimpleMonoclinic_C_h
#define __SimpleMonoclinic_C_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API SimpleMonoclinic_C : public UnitCell  
{
public:
	SimpleMonoclinic_C();
	virtual ~SimpleMonoclinic_C();
	virtual void CalculateVectors();
};

#endif

