// P23.h: interface for the C2dm2dc2s1dm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __P23_h
#define __P23_h


#include "Spg_dll.h"

class SPG_DLL_API P23 : public SpaceGroup  
{
public:
	P23();
	virtual ~P23();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
