// InteractorStyleSelect.h: interface for the InteractorStyleSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERACTORSTYLESELECT_H__8B64739B_DB8B_4F37_AC1C_926FA46D95D7__INCLUDED_)
#define AFX_INTERACTORSTYLESELECT_H__8B64739B_DB8B_4F37_AC1C_926FA46D95D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMadelungDoc;
class CMadelungView;
class vtkActor;

#include "vtkInteractorStyle.h"

class InteractorStyleSelect : public vtkInteractorStyle  
{
// construction / destruction
public:
	InteractorStyleSelect();
	virtual ~InteractorStyleSelect();
// Member functions
public:
	static InteractorStyleSelect *New();
	void HighlightActor(vtkActor *actor);
	CMadelungDoc* GetDoc();
	CMadelungView* GetView(CMadelungDoc* pDoc);
	int GetActorNumber(vtkActor* ac, CMadelungView* pV);
	int m_SelectedAtoms;	// how many atoms are selected
	int m_number;	// keeps the selected atom's number
	vtkActor* m_actr; // keeps pointer to the actor of the selected atom
	vtkActor* m_HighlitedActor;	// the actor which has been highlited with bounding box around it
	int m_numberInLattice; // keeps the selected atom's number in CLattice
protected:
	CMenu* m_pmenuContext;
// Message handlers
protected:
	void OnChar(int ctrl, int shift, char keycode, int repeatcount);
	void OnMouseMove(int ctrl, int shift, int X, int Y);
	void OnLeftButtonDown(int ctrl, int shift, int X, int Y);
	void OnRightButtonDown(int ctrl, int shift, int X, int Y);
	int GetActorNumberInLattice(float* coord); // returns -1 if not found
};

#endif // !defined(AFX_INTERACTORSTYLESELECT_H__8B64739B_DB8B_4F37_AC1C_926FA46D95D7__INCLUDED_)
