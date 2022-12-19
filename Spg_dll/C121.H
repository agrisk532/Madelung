// C121.h: interface for the C121 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __C121_h
#define __C121_h

#include "Spg_dll.h"

class SPG_DLL_API C121 : public SpaceGroup  
{
public:
	C121();
	virtual ~C121();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
