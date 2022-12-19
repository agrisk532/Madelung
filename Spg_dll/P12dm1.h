// P12dm1.h: interface for the P112s1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __P12dm1_h
#define __P12dm1_h

#include "Spg_dll.h"

class SPG_DLL_API P12dm1 : public SpaceGroup  
{
public:
	P12dm1();
	virtual ~P12dm1();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
