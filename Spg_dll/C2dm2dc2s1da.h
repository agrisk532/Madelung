// C2dm2dc2s1da.h: interface for the C2dm2dc2s1dm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __C2dm2dc2s1da_h
#define __C2dm2dc2s1da_h


#include "Spg_dll.h"

class SPG_DLL_API C2dm2dc2s1da : public SpaceGroup  
{
public:
	C2dm2dc2s1da();
	virtual ~C2dm2dc2s1da();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
