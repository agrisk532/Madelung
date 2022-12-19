// C2dm2dc2s1dm.h: interface for the C2dm2dc2s1dm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __C2dm2dc2s1dm_h
#define __C2dm2dc2s1dm_h


#include "Spg_dll.h"

class SPG_DLL_API C2dm2dc2s1dm : public SpaceGroup  
{
public:
	C2dm2dc2s1dm();
	virtual ~C2dm2dc2s1dm();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
