// P1.h: interface for the P1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_P-1_H__73D5895B_E8AF_48BA_A3E9_A071480EC995__INCLUDED_)
#define AFX_P-1_H__73D5895B_E8AF_48BA_A3E9_A071480EC995__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Spg_dll.h"

class SPG_DLL_API P1 : public SpaceGroup  
{
public:
	P1();
	virtual ~P1();
	int GetEqPos(char Wpos);	// returns number of equivalent positions
	double* GetPos_X(double xyz[3], char Wpos);	// xyz contains point coordinates
	double* GetPos_Y(double xyz[3], char Wpos);	// Wpos contains Wyckoff set symbol
	double* GetPos_Z(double xyz[3], char Wpos);	// returns pointer to the dimension

};

#endif // !defined(AFX_P1_H__73D5895B_E8AF_48BA_A3E9_A071480EC995__INCLUDED_)
