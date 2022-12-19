// A112.h: interface for the A112 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __A112_h
#define __A112_h

#include "Spg_dll.h"

class SPG_DLL_API A112 : public SpaceGroup  
{
public:
	A112();
	virtual ~A112();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
