// P12s11.h: interface for the P12s11 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __P12s11_h
#define __P12s11_h

#include "Spg_dll.h"

class SPG_DLL_API P12s11 : public SpaceGroup  
{
public:
	P12s11();
	virtual ~P12s11();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
