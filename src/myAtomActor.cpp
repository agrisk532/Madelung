// myAtomActor.cpp: implementation of the myAtomActor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "madelung.h"
#include "myAtomActor.h"
#include "vtkActor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

myAtomActor *myAtomActor::New() 
{
	return new myAtomActor;
}


myAtomActor::myAtomActor()
{
	m_pActor = vtkActor::New();
	number = 0;
}

myAtomActor::~myAtomActor()
{
	m_pActor->Delete();
}
