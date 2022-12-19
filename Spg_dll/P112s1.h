// P112s1.h: interface for the P112s1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __P112s1_h
#define __P112s1_h

#include "Spg_dll.h"

class SPG_DLL_API P112s1 : public SpaceGroup  
{
public:
	P112s1();
	virtual ~P112s1();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
