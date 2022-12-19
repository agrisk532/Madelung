// B112.h: interface for the B112 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __B112_h
#define __B112_h

#include "Spg_dll.h"

class SPG_DLL_API B112 : public SpaceGroup  
{
public:
	B112();
	virtual ~B112();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
