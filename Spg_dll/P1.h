// P1.h: interface for the P1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __P1_h
#define __P1_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Spg_dll.h"

class SPG_DLL_API P1 : public SpaceGroup  
{
public:
	P1();
	virtual ~P1();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
