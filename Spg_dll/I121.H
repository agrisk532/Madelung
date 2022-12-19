// I121.h: interface for the I121 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __I121_h
#define __I121_h

#include "Spg_dll.h"

class SPG_DLL_API I121 : public SpaceGroup  
{
public:
	I121();
	virtual ~I121();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
