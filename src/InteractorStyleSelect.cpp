// InteractorStyleSelect.cpp: implementation of the InteractorStyleSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "madelung.h"
#include "InteractorStyleSelect.h"
#include "vtkPicker.h"
#include "vtkOutlineSource.h"
#include "vtkPolyDataMapper.h"
#include "MainFrm.h"
#include "vtkActor.h"
#include "AtomProperties.h"
#include "Lattice.h"
#include "MadelungView.h"
#include "MadelungDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
InteractorStyleSelect *InteractorStyleSelect::New() 
{
  return new InteractorStyleSelect;
}

InteractorStyleSelect::InteractorStyleSelect()
{
	this->m_SelectedAtoms = 0;
// context menu
	m_pmenuContext = new CMenu();
	VERIFY(m_pmenuContext->LoadMenu(IDR_POPUP));
	m_number = 0;
	m_numberInLattice = 0;
	m_actr = NULL;
}

InteractorStyleSelect::~InteractorStyleSelect()
{
	if(m_pmenuContext)
		m_pmenuContext->DestroyMenu();
}

void InteractorStyleSelect::OnChar(int ctrl, int shift, 
                                char keycode, int vtkNotUsed(repeatcount)) 
{

}

void InteractorStyleSelect::OnMouseMove(int vtkNotUsed(ctrl), int vtkNotUsed(shift),
				    int X, int Y) 
{
	this->LastPos[0] = X;
	this->LastPos[1] = Y;
/*
	CMadelungApp* pApp = (CMadelungApp*) AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)pApp->m_pMainWnd;
	CStatusBar* sb = (CStatusBar*) pFrame->GetMessageBar();
	CString str;
	str.Format("%d %d",X,Y);
	sb->SetWindowText(str);
*/
}



//----------------------------------------------------------------------------
void InteractorStyleSelect::OnLeftButtonDown(int ctrl, int shift, 
                                          int X, int Y) 
{
	this->UpdateInternalState(ctrl, shift, X, Y);
  
	vtkRenderWindowInteractor *rwi = this->Interactor;

//	if (this->State == VTKIS_START) 

		this->FindPokedRenderer(this->LastPos[0],this->LastPos[1]);
        rwi->StartPickCallback();
        rwi->GetPicker()->Pick(this->LastPos[0],this->LastPos[1], 0.0, 
                               this->CurrentRenderer);
        this->ActorPicked = 0;
//        this->HighlightActor(rwi->GetPicker()->GetAssembly());
		vtkActor* actr;
		actr = rwi->GetPicker()->GetActor();
		if(actr)
		{
			CMadelungDoc* pDoc = this->GetDoc();
			CLattice* pl = pDoc->m_pLattice;
			CMadelungView* pV = GetView(pDoc);
			int number = GetActorNumber(actr,pV);
			if(number==-1) return; // returns when user clicks on bond
// 
			if(pl->m_selected[number])	// atom previously selected
			{
//				actr->GetProperty()->
//				SetColor(pDoc->m_pAtomProperties->m_Color[pl->m_type[number]][0],
//						pDoc->m_pAtomProperties->m_Color[pl->m_type[number]][1],
//						pDoc->m_pAtomProperties->m_Color[pl->m_type[number]][2]);
//				pl->m_selected[number] = false;
			}
			else
			{
				actr->GetProperty()->SetColor(0.,1.,0.);
				pl->m_selected[number] = true;
				pDoc->m_selectedAtoms++;
				this->m_SelectedAtoms++;
// write status bar message
			}
			this->HighlightActor(actr);
// used to remove the actor bounding box at CDocument->OnNewDocument()
			this->m_HighlitedActor = actr; 
			CMadelungApp* pApp = (CMadelungApp*) AfxGetApp();
			CMainFrame* pFrame = (CMainFrame*)pApp->m_pMainWnd;
			CStatusBar* sb = (CStatusBar*) pFrame->GetMessageBar();
			CString str;
			float coord[3];
			actr->GetPosition(coord);
			int numberInLattice = GetActorNumberInLattice(coord);
			if(numberInLattice == -1)
			{
				AfxMessageBox("Atom not found in lattice",MB_OK);
				exit(1);
			}
			str.Format("Selected atom = %s (%d) at x,y,z=(%f, %f, %f), Wyckoff position = %c, charge = %f",
										pDoc->m_pAtomProperties->m_Type[pl->m_type[numberInLattice]],
										numberInLattice,
										pl->m_X[numberInLattice],
										pl->m_Y[numberInLattice],
										pl->m_Z[numberInLattice],
										pl->m_WPosition[numberInLattice],
										pl->m_charge[numberInLattice]);
			sb->SetWindowText(str);
// used to enable the Calculate menu items
			pV->m_AtomsAreSelected = TRUE;
		}
        rwi->EndPickCallback();
}

/////////////////////////////////////////////////////////////////////////////////
void InteractorStyleSelect::OnRightButtonDown(int ctrl, int shift, 
										       int X, int Y) 
{
	this->UpdateInternalState(ctrl, shift, X, Y);

	CMadelungApp* pApp = (CMadelungApp*) AfxGetApp();
	CMainFrame* pFr = (CMainFrame*)pApp->m_pMainWnd;
	CMadelungDoc* pDoc = this->GetDoc();
	CMadelungView* pV = GetView(pDoc);
	CLattice* pl = pDoc->m_pLattice;

	CRect cRect;
	pV->GetWindowRect(&cRect);

	vtkRenderWindowInteractor *rwi = this->Interactor;

//	if (this->State == VTKIS_START) 

		this->FindPokedRenderer(this->LastPos[0],this->LastPos[1]);
        rwi->StartPickCallback();
        rwi->GetPicker()->Pick(this->LastPos[0],this->LastPos[1], 0.0, 
                               this->CurrentRenderer);
        this->ActorPicked = 0;
        this->HighlightActor(rwi->GetPicker()->GetAssembly());

		m_actr = NULL;

		m_actr = rwi->GetPicker()->GetActor();
		if(m_actr)
		{
			m_number = GetActorNumber(m_actr,pV);
			if(m_number==-1) return; // returns when user clicks on bond

			this->HighlightActor(m_actr);
			CStatusBar* sb = (CStatusBar*) pFr->GetMessageBar();
			CString str;
			float coord[3];
			m_actr->GetPosition(coord);
			m_numberInLattice = GetActorNumberInLattice(coord);
			if(m_numberInLattice == -1)
			{
				AfxMessageBox("Atom not found in lattice",MB_OK);
				exit(1);
			}
			str.Format("Atom = %s (%d) at x,y,z=(%f, %f, %f), Wyckoff position = %c, charge = %f",
										pDoc->m_pAtomProperties->m_Type[pl->m_type[m_numberInLattice]],
										m_numberInLattice,
										pl->m_X[m_numberInLattice],
										pl->m_Y[m_numberInLattice],
										pl->m_Z[m_numberInLattice],
										pl->m_WPosition[m_numberInLattice],
										pl->m_charge[m_numberInLattice]);
			sb->SetWindowText(str);
// popup menus
			CPoint point(cRect.left+X,cRect.bottom-Y);
			CMenu* psubmenu = m_pmenuContext->GetSubMenu(0);
			ASSERT(psubmenu != NULL);
			psubmenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,pV);
		}
        rwi->EndPickCallback();
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// When pick action successfully selects actor, this method highlights the
// actor appropriately. Currently this is done by placing a bounding box
// around the actor.
void InteractorStyleSelect::HighlightActor(vtkActor *actor) 
{
  if ( ! this->OutlineActor ) 
    {
    // have to defer creation to get right type
    this->OutlineActor = vtkActor::New();
    this->OutlineActor->PickableOff();
    this->OutlineActor->DragableOff();
    this->OutlineActor->SetMapper(this->OutlineMapper);
    this->OutlineActor->GetProperty()->SetColor(1.0,1.0,1.0);
    this->OutlineActor->GetProperty()->SetAmbient(1.0);
    this->OutlineActor->GetProperty()->SetDiffuse(0.0);
    }
  if ( this->PickedRenderer ) 
    {
    this->PickedRenderer->RemoveActor(this->OutlineActor);
    }
  
  if ( ! actor ) 
    {
    this->PickedRenderer = NULL;
    }
  else 
    {
    this->PickedRenderer = this->CurrentRenderer;
    this->CurrentRenderer->AddActor(this->OutlineActor);
    this->Outline->SetBounds(actor->GetBounds());
    this->CurrentActor = actor;
    }
  this->Interactor->Render();
}

///////////////////////////////////////////////////////////////////////////////
//

CMadelungDoc* InteractorStyleSelect::GetDoc()
{
	CMadelungApp* pApp = (CMadelungApp*)AfxGetApp();
	ASSERT_VALID(pApp);
	CFrameWnd* pw = (CFrameWnd*)pApp->m_pMainWnd;
	ASSERT_VALID(pw);
	CMadelungDoc* pDoc = (CMadelungDoc*)pw->GetActiveDocument();
	ASSERT_VALID(pDoc);
	return pDoc;
}

///////////////////////////////////////////////////////////////////////////////
//

int InteractorStyleSelect::GetActorNumber(vtkActor* ac, CMadelungView* pV)
{
	for(int i=0;i<pV->m_NumberOfAtomsInPlot;i++)
	{
		if(ac == pV->m_patomActor[i])
			return i;
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////////////
//

CMadelungView* InteractorStyleSelect::GetView(CMadelungDoc* pDoc)
{
	POSITION pos = pDoc->GetFirstViewPosition();
	CMadelungView* pV = (CMadelungView*) pDoc->GetNextView(pos);
	return pV;
}
///////////////////////////////////////////////////////////////////////////////
//
int InteractorStyleSelect::GetActorNumberInLattice(float coord[3])
{
	CMadelungDoc* pDoc = this->GetDoc();
	CLattice* pl = pDoc->m_pLattice;
	int size = pl->GetSize();
	for(int i=0;i<size;i++)
	{
		if(fabs(coord[0] - pl->m_X[i]) < 1.e-4 &&
		   fabs(coord[1] - pl->m_Y[i]) < 1.e-4 &&
		   fabs(coord[2] - pl->m_Z[i]) < 1.e-4)
			return i;
		else
			continue;
	}
	return -1;
}
