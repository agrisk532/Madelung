// I112.h: interface for the I112 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __I112_h
#define __I112_h

#include "Spg_dll.h"

class SPG_DLL_API I112 : public SpaceGroup  
{
public:
	I112();
	virtual ~I112();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
