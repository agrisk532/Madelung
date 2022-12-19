#ifndef __UnitCelldll_h
#define __UnitCelldll_h

#include "win32Header.h"

// This class is exported from the UnitCelldll.dll
class UNITCELLDLL_API CUnitCelldll {
public:
	CUnitCelldll(void);
	// TODO: add your methods here.
};

extern UNITCELLDLL_API int nUnitCelldll;

UNITCELLDLL_API int fnUnitCelldll(void);

#endif