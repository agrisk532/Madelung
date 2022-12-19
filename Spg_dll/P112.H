// P112.h: interface for the P1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __P112_h
#define __P112_h

#include "Spg_dll.h"

class SPG_DLL_API P112 : public SpaceGroup  
{
public:
	P112();
	virtual ~P112();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
