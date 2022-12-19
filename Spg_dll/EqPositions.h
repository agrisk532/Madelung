// EqPositions.h: interface for the EqPositions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __EqPositions_h
#define __EqPositions_h

#include "win32HeaderSG.h"

class SPG_DLL_API EqPositions  
{
public:
	EqPositions(int count=0);	// constructs the object
	virtual ~EqPositions();
	double* m_X;
	double* m_Y;
	double* m_Z;
	int m_size;	// size of the vectors above
};

#endif
