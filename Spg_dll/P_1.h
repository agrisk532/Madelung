// P_1.h: interface for the P_1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __P_1_h
#define __P_1_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Spg_dll.h"

class SPG_DLL_API P_1 : public SpaceGroup  
{
public:
	P_1();
	virtual ~P_1();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
