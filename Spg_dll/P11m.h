// P11m.h: interface for the P11m class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __P11m_h
#define __P11m_h

#include "Spg_dll.h"

class SPG_DLL_API P11m : public SpaceGroup  
{
public:
	P11m();
	virtual ~P11m();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif
