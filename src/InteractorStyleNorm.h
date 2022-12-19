// InteractorStyleNorm.h: interface for the InteractorStyleNorm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERACTORSTYLENORM_H__681FC41A_1404_427A_968A_AD8566FA6245__INCLUDED_)
#define AFX_INTERACTORSTYLENORM_H__681FC41A_1404_427A_968A_AD8566FA6245__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vtkInteractorStyle.h"

class InteractorStyleNorm : public vtkInteractorStyle  
{
public:
// construction / destruction
	InteractorStyleNorm();
	virtual ~InteractorStyleNorm();
// Member functions
	static InteractorStyleNorm *New();
	void OnChar(int ctrl, int shift, char keycode, int vtkNotUsed(repeatcount));
};

#endif // !defined(AFX_INTERACTORSTYLENORM_H__681FC41A_1404_427A_968A_AD8566FA6245__INCLUDED_)
