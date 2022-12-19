// SimpleMonoclinic_B.h: interface for the SimpleMonoclinic class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SimpleMonoclinic_B_h
#define __SimpleMonoclinic_B_h

#include "win32Header.h"
#include "UnitCell.h"

class UNITCELLDLL_API SimpleMonoclinic_B : public UnitCell  
{
public:
	SimpleMonoclinic_B();
	virtual ~SimpleMonoclinic_B();
	virtual void CalculateVectors();
};

#endif

