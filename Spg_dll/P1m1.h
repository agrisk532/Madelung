// P1m1.h: interface for the P1m1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __P1m1_h
#define __P1m1_h

#include "Spg_dll.h"

class SPG_DLL_API P1m1 : public SpaceGroup  
{
public:
	P1m1();
	virtual ~P1m1();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
