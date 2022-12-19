// P112dm.h: interface for the P112s1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __P112dm_h
#define __P112dm_h

#include "Spg_dll.h"

class SPG_DLL_API P112dm : public SpaceGroup  
{
public:
	P112dm();
	virtual ~P112dm();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
