// myAtomActor.h: interface for the myAtomActor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYATOMACTOR_H__29EE9A5E_94D3_410C_887E_0B7FDB033782__INCLUDED_)
#define AFX_MYATOMACTOR_H__29EE9A5E_94D3_410C_887E_0B7FDB033782__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vtkActor.h"

class myAtomActor : public vtkActor  
{
public:
	static myAtomActor* New();
	myAtomActor();
	virtual ~myAtomActor();

	vtkActor* m_pActor;
	int number; // used for marking the actor
};

#endif // !defined(AFX_MYATOMACTOR_H__29EE9A5E_94D3_410C_887E_0B7FDB033782__INCLUDED_)
