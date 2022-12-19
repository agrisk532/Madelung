// InteractorStyleNorm.cpp: implementation of the InteractorStyleNorm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "madelung.h"
#include "InteractorStyleNorm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
InteractorStyleNorm *InteractorStyleNorm::New() 
{
  return new InteractorStyleNorm;
}


InteractorStyleNorm::InteractorStyleNorm()
{

}

InteractorStyleNorm::~InteractorStyleNorm()
{

}


void InteractorStyleNorm::OnChar(int ctrl, int shift, 
                                char keycode, int vtkNotUsed(repeatcount)) 
{

}
