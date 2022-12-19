// A121.h: interface for the A121 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __A121_h
#define __A121_h

#include "Spg_dll.h"

class SPG_DLL_API A121 : public SpaceGroup  
{
public:
	A121();
	virtual ~A121();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
