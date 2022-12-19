// P2dm2dm2dm.h: interface for the P2dm2dm2dm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_P2DM2DM2DM_H__0FD158A6_BC12_4803_A930_3E2FCD15CAFD__INCLUDED_)
#define AFX_P2DM2DM2DM_H__0FD158A6_BC12_4803_A930_3E2FCD15CAFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Spg_dll.h"

class SPG_DLL_API P2dm2dm2dm : public SpaceGroup  
{
public:
	P2dm2dm2dm();
	virtual ~P2dm2dm2dm();
	virtual EqPositions* GetEqPos(double xyz[3], char Wpos);
};

#endif // !defined(AFX_P2DM2DM2DM_H__0FD158A6_BC12_4803_A930_3E2FCD15CAFD__INCLUDED_)
