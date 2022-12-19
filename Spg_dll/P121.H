// P121.h: interface for the P1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __P121_h
#define __P121_h

#include "Spg_dll.h"

class SPG_DLL_API P121 : public SpaceGroup  
{
public:
	P121();
	virtual ~P121();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
