// MadelungView.cpp : implementation of the CMadelungView class
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <vector>
using namespace std;

#include "stdafx.h"

#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkWin32OpenGLRenderWindow.h"
#include "vtkActor.h"
#include "vtkSphereSource.h"
#include "vtkLineSource.h"
#include "vtkWin32RenderWindowInteractor.h"
#include "InteractorStyleNorm.h"
#include "InteractorStyleSelect.h"
#include "vtkPolyDataMapper.h"
#include "vtkPicker.h"

#include "MadelungDoc.h"
#include "MadelungView.h"
#include "Madelung.h"
#include "MainFrm.h"
#include "Spg_dll.h"
#include "UnitCell.h"
#include "Lattice.h"
#include "AtomProperties.h"
#include "ExtendLattice.h"
#include "ExtendLatticeSphere.h"

#define LAYER_TOLERANCE 0.01

/////////////////////////////////////////////////////////////////////////////
// CMadelungView

IMPLEMENT_DYNCREATE(CMadelungView, CView)

BEGIN_MESSAGE_MAP(CMadelungView, CView)
	//{{AFX_MSG_MAP(CMadelungView)
	ON_COMMAND(ID_TOOLS_SELECTATOMS, OnToolsSelectatoms)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SELECTATOMS, OnUpdateToolsSelectatoms)
	ON_COMMAND(ID_TOOLS_ROTATEATOMS, OnToolsRotateatoms)
	ON_COMMAND(ID_VIEW_BONDS, OnViewBonds)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BONDS, OnUpdateViewBonds)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ROTATEATOMS, OnUpdateToolsRotateatoms)
	ON_COMMAND(ID_TOOLS_EXTEND_NORMAL, OnToolsExtendNormal)
	ON_COMMAND(ID_TOOLS_EXTEND_SPHERE, OnToolsExtendSphere)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EXTEND_NORMAL, OnUpdateToolsExtendNormal)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EXTEND_SPHERE, OnUpdateToolsExtendSphere)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_SETUP, OnUpdateFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_TOOLS_EXTEND_DELETE, OnToolsExtendDelete)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EXTEND_DELETE, OnUpdateToolsExtendDelete)
	ON_COMMAND(ID_VIEW_CELLBORDERS, OnViewCellborders)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CELLBORDERS, OnUpdateViewCellborders)
	ON_COMMAND(ID_TOOLS_UNSELECTALLATOMS, OnToolsUnselectallatoms)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_UNSELECTALLATOMS, OnUpdateToolsUnselectallatoms)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_POTENTIAL_ON_ATOMS, OnUpdateToolsPotentialOnAtoms)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SETUP_LATTICE, OnUpdateToolsSetupLattice)
	ON_COMMAND(IDR_POPUP_SELECTATOM, OnPopupSelectatom)
	ON_COMMAND(IDR_POPUP_UNSELECTATOM, OnPopupUnselectatom)
	ON_COMMAND(IDR_POPUP_HIDE, OnPopupHide)
	ON_COMMAND(IDR_POPUP_HIDEALL, OnPopupHideall)
	ON_COMMAND(IDR_POPUP_SHOWALL, OnPopupShowall)
	ON_COMMAND(IDR_POPUP_HIDELAYER_X, OnPopupHidelayerX)
	ON_COMMAND(IDR_POPUP_HIDELAYER_Y, OnPopupHidelayerY)
	ON_COMMAND(IDR_POPUP_HIDELAYER_Z, OnPopupHidelayerZ)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMadelungView construction/destruction

CMadelungView::CMadelungView()
{
	m_pRenderer = NULL;
	m_pRenWin = NULL;
	m_pIrenNorm = NULL;
	m_pIrenSelect = NULL;
	m_picker = NULL;
	m_pIrenStyle = NULL;
	m_pIrenStyleSelect = NULL;

	m_ClusterIsRendered = false; // disable the view->bonds menu, file->save, etc menus
	m_AtomsAreSelected = false;
	m_IsosurfaceDataIsReady = false;
	m_NumberOfAtomsInPlot = 0;
	m_BondsAreRendered = false;	// disable the check mark in the view->bonds menu
	m_CellBordersAreRendered = false;
	m_AtomsAreRotated = true;
	m_bRenderExtension = false; // do not render lattice extension
}


CMadelungView::~CMadelungView()
{
	if(m_pIrenNorm)
	{
		m_pIrenNorm->Delete();
		m_pIrenNorm = NULL;
	}
	if(m_pIrenSelect)
	{
		m_pIrenSelect->Delete();
		m_pIrenSelect = NULL;
	}
	if(m_picker)
	{
		m_picker->Delete();
		m_picker = NULL;
	}
	if(m_pIrenStyle)
	{
		m_pIrenStyle->Delete();
		m_pIrenStyle = NULL;
	}
	if(m_pIrenStyleSelect)
	{
		m_pIrenStyleSelect->Delete();
		m_pIrenStyleSelect = NULL;
	}

// remove vectors
	int size = m_patomActor.size();
	for(int i=0;i<size;i++)
	{
		m_pRenderer->RemoveActor(m_patomActor[i]);
		m_patomActor[i]->Delete();
	}
	m_patomActor.clear();

	size = m_pbondActor.size();
	for(i=0;i<size;i++)
	{
		m_pRenderer->RemoveActor(m_pbondActor[i]);
		m_pbondActor[i]->Delete();
	}
	m_pbondActor.clear();

	size = m_pCellBorderActor.size();
	for(i=0;i<size;i++)
	{
		m_pRenderer->RemoveActor(m_pCellBorderActor[i]);
		m_pCellBorderActor[i]->Delete();
	}
	m_pCellBorderActor.clear();

	size = m_sphere.size();
	for(i=0;i<size;i++) m_sphere[i]->Delete();
	m_sphere.clear();

	size = m_ppdmSpheres.size();
	for(i=0;i<size;i++) m_ppdmSpheres[i]->Delete();
	m_ppdmSpheres.clear();

	size = m_pLine.size();
	for(i=0;i<size;i++) m_pLine[i]->Delete();
	m_pLine.clear();

	size = m_pLineMapper.size();
	for(i=0;i<size;i++) m_pLineMapper[i]->Delete();
	m_pLineMapper.clear();

	if(m_pRenderer)
	{
		m_pRenderer->Delete();
		m_pRenderer = NULL;
	}
	if(m_pRenWin)
	{
		m_pRenWin->Delete();
		m_pRenWin = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////
//
BOOL CMadelungView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	LPCTSTR lpszFrameClass =
		  AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
							AfxGetApp()->LoadStandardCursor(IDC_ARROW),
							CreateSolidBrush(RGB(0,0,0)),
							LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)));

	cs.lpszClass = lpszFrameClass;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMadelungView drawing

void CMadelungView::OnDraw(CDC* pDC)
{
	CMadelungDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

// finds out the logical size of the view and writes it to the member variable
// when I put pDC->GetClipBox(&rect), copying to clipboard doesn't work

	if(pDoc->GetCalculatedOK())	// document has data for plotting
	{
		CRect rect;
		this->GetClientRect(&rect);
		m_pRenWin->SetSize(rect.right,rect.bottom);
		m_pRenWin->Render();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMadelungView printing

BOOL CMadelungView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMadelungView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMadelungView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMadelungView diagnostics

#ifdef _DEBUG
void CMadelungView::AssertValid() const
{
	CView::AssertValid();
}

void CMadelungView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
//
CMadelungDoc* CMadelungView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMadelungDoc)));
	return (CMadelungDoc*)m_pDocument;
}
/////////////////////////////////////////////////////////////////////////////
// CMadelungView message handlers
void CMadelungView::PrepareForUnitCellPlot()
{
	CMadelungDoc *pDoc=GetDocument();

	if(!m_pRenderer)
	{
		m_pRenderer= vtkRenderer::New();
	}
	if(!m_pRenWin)
	{
		m_pRenWin = vtkWin32OpenGLRenderWindow::New();
		HWND hwnd = this->GetSafeHwnd();
// adds renderer to the list of renderers
		m_pRenWin->AddRenderer(m_pRenderer);
		m_pRenWin->SetParentId(hwnd);
	}
// calculate and add cartesian coordinates of ALL atoms in unit cell(expanding Wyckoff positions)
// calculate a unitcell vertices (for unitcell border plots)
// add unitcells to CLattice 
	pDoc->CalculateUnitCellVertices(); // this sets also pDoc->m_CalculatedOK = true;
	AddUnitCellVerticesToRenderer(pDoc->m_pLattice);
	this->m_ClusterIsRendered = true;
// interactor stuff
	this->OnToolsRotateatoms();
}

/////////////////////////////////////////////////////////////////////////////
//

void CMadelungView::AddUnitCellVerticesToRenderer(CLattice* pl)
{
	if(this->m_ClusterIsRendered)
	{
		AfxMessageBox("Cluster is already rendered",MB_OK);
		return;
	}
	CMadelungDoc* pDoc = this->GetDocument();
	int lsize = pl->GetSize();
	CString wrt;
	wrt.Format("atoms in plot = %d",lsize);
	WriteStatusBarMessage(wrt);
// make atom spheres. Do this only once at the beginning
	if(m_sphere.size() == 0 && m_ppdmSpheres.size() == 0)
	{
		for(int atomType=0;atomType<NUMBER_OF_ATOMS_IN_PERIODIC_TABLE;atomType++)
		{
			m_sphere.push_back(vtkSphereSource::New());
				m_sphere[atomType]->SetThetaResolution(8);
				m_sphere[atomType]->SetPhiResolution(8);
				m_sphere[atomType]->SetRadius(pDoc->m_pAtomProperties->m_Radius[atomType]);
				m_sphere[atomType]->SetCenter(0.,0.,0.);
			m_ppdmSpheres.push_back(vtkPolyDataMapper::New());
				m_ppdmSpheres[atomType]->SetInput(m_sphere[atomType]->GetOutput());
		}
	}
// now add atoms to the renderer
	m_NumberOfAtomsInPlot = 0;

	for(int i=0;i<lsize;i++)
	{
		AddAtomToPlot(i); // adds m_pLattice atom 'i' to the plot.
/*
		m_NumberOfAtomsInPlot++;
		m_patomActor.push_back(vtkActor::New());
			m_patomActor[i]->SetMapper(m_ppdmSpheres[pl->m_type[i]]);
			m_patomActor[i]->SetPosition(pl->m_X[i],
												pl->m_Y[i],
												pl->m_Z[i]);
			m_patomActor[i]->GetProperty()->
				SetColor(pDoc->m_pAtomProperties->m_Color[pl->m_type[i]][0],
						pDoc->m_pAtomProperties->m_Color[pl->m_type[i]][1],
						pDoc->m_pAtomProperties->m_Color[pl->m_type[i]][2]);
			m_pRenderer->AddActor(m_patomActor[i]);
*/
	}
}

////////////////////////////////////////////////////////////////////////////
// totally clears the view

void CMadelungView::Clear()
{
	if(this->m_pIrenStyleSelect)
		this->m_pIrenStyleSelect->HighlightActor(NULL);
// remove atoms
	if(this->m_ClusterIsRendered)
	{
		for(int i=0;i<m_NumberOfAtomsInPlot;i++)
		{
			m_pRenderer->RemoveActor(m_patomActor[i]);
			m_patomActor[i]->Delete();
		}
		m_patomActor.clear();
		this->m_ClusterIsRendered = false;
	}
// remove bonds
	if(this->m_BondsAreRendered)
	{
		int nbonds = m_pbondActor.size();
		for(int i=0;i<nbonds;i++)
		{
			m_pRenderer->RemoveActor(m_pbondActor[i]);
			m_pbondActor[i]->Delete();
		}
		m_pbondActor.clear();
		this->m_BondsAreRendered = false;
	}

	if(this->m_CellBordersAreRendered)
	{
		int nbonds = m_pCellBorderActor.size();
		for(int i=0;i<nbonds;i++)
		{
			m_pRenderer->RemoveActor(m_pCellBorderActor[i]);
			m_pCellBorderActor[i]->Delete();
		}
		m_pCellBorderActor.clear();
		this->m_CellBordersAreRendered = false;
	}

	this->m_AtomsAreSelected = false;
	this->m_IsosurfaceDataIsReady = false;
	this->m_NumberOfAtomsInPlot = 0;
// we don't delete the m_sphere and m_ppdmSpheres here but keep until the next reuse
}

////////////////////////////////////////////////////////////////////////////
//

void CMadelungView::OnToolsSelectatoms() 
{
	if(m_pIrenNorm)
	{
		m_pIrenNorm->Delete();
		m_pIrenNorm = NULL;
	}
	if(m_pIrenStyle)
	{
		m_pIrenStyle->Delete();
		m_pIrenStyle = NULL;
	}
	if(!m_picker)
	{
		m_picker = vtkPicker::New();
		m_picker->SetTolerance(0.001);
	}
	if(!m_pIrenStyleSelect) m_pIrenStyleSelect = InteractorStyleSelect::New();
	if(!m_pIrenSelect)
	{
		m_pIrenSelect = vtkWin32RenderWindowInteractor::New();
		m_pIrenSelect->SetRenderWindow(m_pRenWin);
		m_pIrenSelect->SetInteractorStyle(m_pIrenStyleSelect);
		m_pIrenSelect->SetPicker(m_picker);
	}
	m_AtomsAreRotated = false;
	m_pRenWin->Render();
}

/////////////////////////////////////////////////////////////////////////////
//
void CMadelungView::OnToolsRotateatoms() 
{
	if(m_pIrenSelect)
	{
		m_pIrenSelect->Delete();
		m_pIrenSelect = NULL;
	}
	if(m_pIrenStyleSelect)
	{
		m_pIrenStyleSelect->Delete();
		m_pIrenStyleSelect = NULL;
	}
	if(!m_pIrenStyle) m_pIrenStyle = InteractorStyleNorm::New();
	if(!m_pIrenNorm)
	{
		m_pIrenNorm = vtkWin32RenderWindowInteractor::New();
		m_pIrenNorm->SetRenderWindow(m_pRenWin);
		m_pIrenNorm->SetInteractorStyle(m_pIrenStyle);
	}
	m_AtomsAreRotated = true;
	m_pRenWin->Render();
// calculate the focal point of camera
	CMadelungDoc* pDoc = GetDocument();;
	vtkCamera* cam = m_pRenderer->GetActiveCamera();
	if(pDoc->m_bUseSymmetry)
		cam->SetFocalPoint(0,0,0);
	else
	{
// set focal point to the geometrical center of system
		int size = pDoc->m_pLattice->GetSize();
		double x=0;
		double y=0;
		double z=0;
		for(int i=0;i<size;i++)
		{
			x += pDoc->m_pLattice->m_X[i];
			y += pDoc->m_pLattice->m_Y[i];
			z += pDoc->m_pLattice->m_Z[i];
		}
		x /= size;
		y /= size;
		z /= size;
		cam->SetFocalPoint(x,y,z);
	}
}
/////////////////////////////////////////////////////////////////////////////
//

void CMadelungView::WriteStatusBarMessage(CString msg)
{
	CMainFrame* pFrame = (CMainFrame*)GetParentFrame();
	CStatusBar* sb = (CStatusBar*) pFrame->GetMessageBar();
	sb->SetWindowText(msg);
}

/////////////////////////////////////////////////////////////////////////////
//

void CMadelungView::OnUpdateToolsSelectatoms(CCmdUI* pCmdUI) 
{
// atoms are not rendered, disable view->bond menu item
// disable it also if the work thread is running
	if(!m_ClusterIsRendered || GetDocument()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else	// cluster is rendered
	{
//		CMadelungDoc* pDoc = GetDocument();
//		if(pDoc->m_latticeExtended)	// enable selection only in the initial cluster
//		{
//			pCmdUI->Enable(FALSE);
//			return;
//		}
//		else
//		{
			if(this->m_AtomsAreRotated)
				pCmdUI->Enable(TRUE);
			else
				pCmdUI->Enable(FALSE);
//		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//	toggles between showing/hiding bonds

void CMadelungView::OnViewBonds() 
{
// remove bonds if already present
	if(this->m_BondsAreRendered)
	{
		int nbonds = m_pbondActor.size();
		for(int i=0;i<nbonds;i++)
		{
			m_pRenderer->RemoveActor(m_pbondActor[i]);
			m_pbondActor[i]->Delete();
		}
		m_pbondActor.clear();
		m_pRenWin->Render();
		this->m_BondsAreRendered = false;
		return;
	}
	else	// add bonds if not present
	{
		int bond = -1;
		CMadelungDoc* pDoc = this->GetDocument();
		CLattice* pl = pDoc->m_pLattice;
// squared lattice bond sizes
		double da = pDoc->m_pSpaceGroup->m_pUnitCell->m_a;
		double db = pDoc->m_pSpaceGroup->m_pUnitCell->m_b;
		double dc = pDoc->m_pSpaceGroup->m_pUnitCell->m_c;
		da *= da;
		db *= db;
		dc *= dc;
		for(int i=0;i<m_NumberOfAtomsInPlot;i++)
			for(int j=0;j<m_NumberOfAtomsInPlot;j++)
			{
				if(i<=j) continue; // to prevent drawing of bonds two times
				if(pl->AtomsAreBonded(i,j,da,db,dc))
				{
					bond++;
					m_pLine.push_back(vtkLineSource::New());
						m_pLine[bond]->SetPoint1(pDoc->m_pLattice->m_X[i],pDoc->m_pLattice->m_Y[i],
											pDoc->m_pLattice->m_Z[i]);
						m_pLine[bond]->SetPoint2(pDoc->m_pLattice->m_X[j],pDoc->m_pLattice->m_Y[j],
											pDoc->m_pLattice->m_Z[j]);
					m_pLineMapper.push_back(vtkPolyDataMapper::New());
						m_pLineMapper[bond]->SetInput(m_pLine[bond]->GetOutput());
	
					m_pbondActor.push_back(vtkActor::New());
						m_pbondActor[bond]->SetMapper(m_pLineMapper[bond]);
					m_pRenderer->AddActor(m_pbondActor[bond]);
//					m_pLine->Delete();
//					m_pLineMapper->Delete();
				}
			}
		m_pRenWin->Render();
		this->m_BondsAreRendered = true;
	}
}
/////////////////////////////////////////////////////////////////////////////
//

void CMadelungView::OnUpdateViewBonds(CCmdUI* pCmdUI)
{
	if(!m_ClusterIsRendered)	// atoms are rendered, enable view->bond menu item
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		if(this->m_BondsAreRendered)
		{
			pCmdUI->SetCheck(true);
			pCmdUI->Enable(TRUE);
		}
		else	// bonds not rendered
		{
// check whether only a single cell rendered
			if(GetDocument()->m_numberOfUnitCells == 1)	// only a single cell rendered
			{
				pCmdUI->SetCheck(false);
				pCmdUI->Enable(FALSE);
			}
			else	// multiple cells rendered
			{
				pCmdUI->SetCheck(false);
				pCmdUI->Enable(TRUE);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//

void CMadelungView::OnUpdateToolsRotateatoms(CCmdUI* pCmdUI) 
{
	if(!m_ClusterIsRendered)	// atoms are rendered, enable view->bond menu item
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		if(this->m_AtomsAreRotated)
			pCmdUI->Enable(FALSE);
		else
			pCmdUI->Enable(TRUE);
	}
}

//////////////////////////////////////////////////////////////////////////////
//	extend lattice as a parallelepiped

void CMadelungView::OnToolsExtendNormal() 
{
	CMadelungDoc* pDoc = GetDocument();
	ExtendLattice pEl; // lattice -> sphere extension dialog box

//	int choice = AfxMessageBox("Previous lattice extensions will be lost",MB_OKCANCEL);
//	if(choice == IDCANCEL) return;
//	else	// choice is OK
//	{
		pEl.m_na = pDoc->m_extend_a;
		pEl.m_nb = pDoc->m_extend_b;
		pEl.m_nc = pDoc->m_extend_c;

		int result = pEl.DoModal();
		if(result==IDOK)
		{
			pDoc->m_extend_a = pEl.m_na;
			pDoc->m_extend_b = pEl.m_nb;
			pDoc->m_extend_c = pEl.m_nc;
			this->m_bRenderExtension = pEl.m_bRenderExtension;
// now remove previous lattice extension
			pDoc->DeleteLatticeExtension();
			this->DeleteLatticeExtensionActors();
// extend lattice again, anew
			pDoc->ExtendLatticeAsParallelepiped(pDoc->m_bUseSymmetry);
			this->ExtendLatticeActors();
			this->Invalidate();
			return;
		}
		else
			return;
//	}
}

///////////////////////////////////////////////////////////////////////////////
//

void CMadelungView::OnToolsExtendSphere() 
{
	CMadelungDoc* pDoc = GetDocument();
	ExtendLatticeSphere pEl; // lattice -> sphere extension dialog box

//	int choice = AfxMessageBox("Previous lattice extensions will be lost",MB_OKCANCEL);
//	if(choice == IDCANCEL) return;
//	else	// proceed further
//	{
		pEl.m_radius = pDoc->m_extend_sphere;

		int result = pEl.DoModal();
		if(result==IDOK)
		{
			pDoc->m_extend_sphere = pEl.m_radius;
			this->m_bRenderExtension = pEl.m_bRenderExtension;
// now remove previous lattice extension
			pDoc->DeleteLatticeExtension();
			this->DeleteLatticeExtensionActors();
// extend lattice again, anew
			pDoc->ExtendLatticeAsSphere(pDoc->m_bUseSymmetry);
			this->ExtendLatticeActors();
			this->Invalidate();
			return;
		}
		else
			return;
//	}
}

//////////////////////////////////////////////////////////////////////////////
//
void CMadelungView::DeleteLatticeExtensionActors()
{

	CMadelungDoc* pDoc = GetDocument();
	CLattice* pl = pDoc->m_pLattice;
	ASSERT_VALID(pDoc);
	int atomsInCluster = pDoc->m_atomsInCluster;
	int atomsInExtension = pDoc->m_atomsInExtension;
	if(atomsInExtension == 0)
	{
//		AfxMessageBox("No atoms in extension",MB_OK);
		return;
	}
	else
	{
// remove actors only if they were rendered
		if(this->m_bRenderExtension)
		{
			int size = atomsInCluster+atomsInExtension;
			for(int i=atomsInCluster;i<size;i++)
			{
				this->m_pRenderer->RemoveActor(this->m_patomActor[i]);
				this->m_patomActor[i]->Delete();
				pl->m_rendered[i] = false;
			}

			vector<vtkActor*>::iterator p;
			p = m_patomActor.begin()+atomsInCluster;
			m_patomActor.erase(p,p+atomsInExtension);
			m_NumberOfAtomsInPlot -= atomsInExtension;
		}
	}
}	

//////////////////////////////////////////////////////////////////////////////
//
void CMadelungView::ExtendLatticeActors()
{
	CMadelungDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int atomsInCluster = pDoc->m_atomsInCluster;
	int atomsInExtension = pDoc->m_atomsInExtension;
	CLattice* pl = pDoc->m_pLattice;
	ASSERT_VALID(pl);

	if(atomsInCluster == 0)
	{
		int reply = AfxMessageBox("No atoms in cluster",MB_OKCANCEL);
		return;
	}
	else if(atomsInExtension == 0)	// nothing to plot in extension
	{
		int reply = AfxMessageBox("No atoms in extension",MB_OKCANCEL);
		return;
	}
	else
	{
// add extension actors only if user asks for it in PropertyPages for lattice extension
		if(this->m_bRenderExtension)
		{
			for(int i=atomsInCluster;i<atomsInCluster+atomsInExtension;i++)
			{
				m_NumberOfAtomsInPlot++;
				m_patomActor.push_back(vtkActor::New());
					m_patomActor[i]->SetMapper(m_ppdmSpheres[pl->m_type[i]]);
					m_patomActor[i]->SetPosition(pl->m_X[i],
													pl->m_Y[i],
													pl->m_Z[i]);
					m_patomActor[i]->GetProperty()->
						SetColor(pDoc->m_pAtomProperties->m_Color[pl->m_type[i]][0],
								pDoc->m_pAtomProperties->m_Color[pl->m_type[i]][1],
								pDoc->m_pAtomProperties->m_Color[pl->m_type[i]][2]);
					m_pRenderer->AddActor(m_patomActor[i]);
				pl->m_rendered[i] = true;
			}
		}
		else
		{
			AfxMessageBox("Lattice extended but no rendered",MB_OK);
		}
		pDoc->m_latticeExtended = true;	// to disable selection command
	}
}

//////////////////////////////////////////////////////////////////////////////
//

void CMadelungView::OnUpdateToolsExtendNormal(CCmdUI* pCmdUI) 
{
	if(GetDocument()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if(m_ClusterIsRendered && GetDocument()->m_atomsInExtension == 0)	// atoms are rendered, enable view->bond menu item
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	else
	{
		pCmdUI->Enable(FALSE);
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
//

void CMadelungView::OnUpdateToolsExtendSphere(CCmdUI* pCmdUI) 
{
	if(GetDocument()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if(m_ClusterIsRendered && GetDocument()->m_atomsInExtension == 0)	// atoms are rendered, enable view->bond menu item
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	else
	{
		pCmdUI->Enable(FALSE);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////
//

void CMadelungView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	if(!m_ClusterIsRendered)	// atoms are rendered, enable view->bond menu item
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CMadelungView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	if(!m_ClusterIsRendered)	// atoms are rendered, enable view->bond menu item
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}


void CMadelungView::OnUpdateFilePrintSetup(CCmdUI* pCmdUI) 
{
	if(!m_ClusterIsRendered)	// atoms are rendered, enable view->bond menu item
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void CMadelungView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	if(!m_ClusterIsRendered || GetDocument()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else	// atoms are rendered, enable view->bond menu item
	{
		pCmdUI->Enable(TRUE);
	}
}

void CMadelungView::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	if(!m_ClusterIsRendered || GetDocument()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else	// atoms are rendered, enable view->bond menu item
	{
		pCmdUI->Enable(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////////
//

void CMadelungView::OnToolsExtendDelete() 
{
	CMadelungDoc* pDoc = GetDocument();
	int choice = AfxMessageBox("All lattice extensions will be lost",MB_OKCANCEL);
	if(choice == IDCANCEL) return;
	else	// proceed further
	{
		this->DeleteLatticeExtensionActors();
		pDoc->DeleteLatticeExtension();
		this->Invalidate();
		return;
	}
}

///////////////////////////////////////////////////////////////////////
//
void CMadelungView::OnUpdateToolsExtendDelete(CCmdUI* pCmdUI) 
{
	CMadelungDoc* pDoc = GetDocument();

// atoms are rendered, enable view->bond menu item
	if(!m_ClusterIsRendered || pDoc->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		if(!pDoc->m_latticeExtended)
		{
			pCmdUI->Enable(FALSE);
			return;
		}
		else
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////
//
void CMadelungView::OnViewCellborders() 
{
// remove bonds if already present
	if(this->m_CellBordersAreRendered)
	{
		int nbonds = m_pCellBorderActor.size();
		for(int i=0;i<nbonds;i++)
		{
			m_pRenderer->RemoveActor(m_pCellBorderActor[i]);
			m_pCellBorderActor[i]->Delete();
		}
		m_pCellBorderActor.clear();
		m_pRenWin->Render();
		this->m_CellBordersAreRendered = false;
		return;
	}
	else	// add bonds if not present
	{
		int bond = -1;
		CMadelungDoc* pDoc = this->GetDocument();
// here we plot only the central unitcell borders
		for(int i=0;i<8;i++)
			for(int j=0;j<8;j++)
			{
				if(i<=j) continue; // to prevent drawing of bonds two times

				if(pDoc->m_pSpaceGroup->m_pUnitCell->VerticesAreBonded(i,j))
				{
					bond++;
					m_pLine.push_back(vtkLineSource::New());
						m_pLine[bond]->SetPoint1(
							pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellVertices[i][0],
							pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellVertices[i][1],
							pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellVertices[i][2]);
						m_pLine[bond]->SetPoint2(
							pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellVertices[j][0],
							pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellVertices[j][1],
							pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellVertices[j][2]);
					m_pLineMapper.push_back(vtkPolyDataMapper::New());
						m_pLineMapper[bond]->SetInput(m_pLine[bond]->GetOutput());
	
					m_pCellBorderActor.push_back(vtkActor::New());
						m_pCellBorderActor[bond]->SetMapper(m_pLineMapper[bond]);
					m_pRenderer->AddActor(m_pCellBorderActor[bond]);
//					m_pLine->Delete();
//					m_pLineMapper->Delete();
				}
			}
		m_pRenWin->Render();
		this->m_CellBordersAreRendered = true;
	}
}

////////////////////////////////////////////////////////////////////////
//	calculates the distance between two points
// 
double CMadelungView::Distance(double d1[3],double d2[3])
{
	double distance = 0.;
	double diff;
	for(int i=0;i<3;i++)
	{
		diff = d1[i]-d2[i];
		distance += diff*diff;
	}
	distance = sqrt(distance);
	return distance;
}

////////////////////////////////////////////////////////////////////////
//
void CMadelungView::OnUpdateViewCellborders(CCmdUI* pCmdUI) 
{
	if(!m_ClusterIsRendered)	// atoms are rendered, enable view->bond menu item
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		pCmdUI->Enable(TRUE);
		if(this->m_CellBordersAreRendered)
			pCmdUI->SetCheck(true);
		else
			pCmdUI->SetCheck(false);
	}
}

/////////////////////////////////////////////////////////////////////////
//
void CMadelungView::OnToolsUnselectallatoms() 
{
	CMadelungDoc* pDoc = this->GetDocument();
	CLattice* pl = pDoc->m_pLattice;
// number of atoms for plot
	int size = pl->GetSize();
	for(int i=0;i<size;i++)
	{
		if(pl->m_selected[i])
		{
			pl->m_selected[i] = false;
			this->m_patomActor[i]->GetProperty()->
				SetColor(pDoc->m_pAtomProperties->m_Color[pl->m_type[i]][0],
						pDoc->m_pAtomProperties->m_Color[pl->m_type[i]][1],
						pDoc->m_pAtomProperties->m_Color[pl->m_type[i]][2]);
		}
	}
// to disable the 'calculate potential' menu item
	if(this->m_pIrenStyleSelect)
	{
		if(this->m_pIrenStyleSelect->m_SelectedAtoms)
			this->m_pIrenStyleSelect->m_SelectedAtoms = 0;
	}
	this->m_AtomsAreSelected = FALSE;
	this->m_pRenWin->Render();
}

///////////////////////////////////////////////////////////////////////////
//
//
void CMadelungView::OnUpdateToolsUnselectallatoms(CCmdUI* pCmdUI) 
{
	CMadelungDoc* pDoc = this->GetDocument();

	if(pDoc->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	CLattice* pl = pDoc->m_pLattice;
// number of atoms for plot
	int size = pl->GetSize();
	for(int i=0;i<size;i++)
	{
		if(pl->m_selected[i])
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
// no atoms selected
	pCmdUI->Enable(FALSE);
	return;
}

////////////////////////////////////////////////////////////////////////
//
//
void CMadelungView::OnUpdateToolsPotentialOnAtoms(CCmdUI* pCmdUI) 
{
	CMadelungDoc* pDoc = this->GetDocument();

	if(pDoc->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(this->m_AtomsAreSelected && pDoc->m_latticeExtended)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	else
	{
		pCmdUI->Enable(FALSE);
		return;
	}
}

///////////////////////////////////////////////////////////////////////
//
//
void CMadelungView::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	if(GetDocument()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		pCmdUI->Enable(TRUE);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////
//
//

void CMadelungView::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	if(GetDocument()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		pCmdUI->Enable(TRUE);
		return;
	}
}

/////////////////////////////////////////////////////////////////////////
//
//

void CMadelungView::OnUpdateToolsSetupLattice(CCmdUI* pCmdUI) 
{
	if(GetDocument()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		pCmdUI->Enable(TRUE);
		return;
	}
}
//////////////////////////////////////////////////////////////////////////
// add the actor 'atom' from m_pLattice to the plot.
//
void CMadelungView::AddAtomToPlot(int atom)
{
	CMadelungDoc* pDoc = GetDocument();
	CLattice* pl = pDoc->m_pLattice;

	m_patomActor.push_back(vtkActor::New());
	int i = m_patomActor.size();
	m_patomActor[i-1]->SetMapper(m_ppdmSpheres[pl->m_type[atom]]);
	m_patomActor[i-1]->SetPosition(pl->m_X[atom],pl->m_Y[atom],pl->m_Z[atom]);
	m_patomActor[i-1]->GetProperty()->SetColor(
								pDoc->m_pAtomProperties->m_Color[pl->m_type[atom]][0],
								pDoc->m_pAtomProperties->m_Color[pl->m_type[atom]][1],
								pDoc->m_pAtomProperties->m_Color[pl->m_type[atom]][2]);
	pl->m_rendered[atom] = true;
	m_pRenderer->AddActor(m_patomActor[i-1]);
	m_NumberOfAtomsInPlot++;
}
//////////////////////////////////////////////////////////////////////////
// remove the actor 'atom' from the plot
//
void CMadelungView::RemoveAtomFromPlot(int atom)
{
}
//////////////////////////////////////////////////////////////////////////
//
//
void CMadelungView::OnPopupSelectatom() 
{
	if(m_pIrenStyleSelect)
	{
		CMadelungDoc* pDoc = GetDocument();
		CLattice* pl = pDoc->m_pLattice;

		if(!pl->m_selected[m_pIrenStyleSelect->m_number])	// atom not previously selected 
		{
			CMainFrame* pFr = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CStatusBar* sb = (CStatusBar*) pFr->GetMessageBar();
			m_pIrenStyleSelect->m_actr->GetProperty()->SetColor(0.,1.,0.);
			pDoc->m_selectedAtoms++;
// used to disable the Calculate menu items
//			if(!m_SelectedAtoms)
//				m_AtomsAreSelected = FALSE;
			CString str;
			str.Format("Selected atom = %s (%d) at x,y,z=(%f, %f, %f), Wyckoff position = %c, charge = %f",
						pDoc->m_pAtomProperties->m_Type[pl->m_type[m_pIrenStyleSelect->m_numberInLattice]],
						m_pIrenStyleSelect->m_numberInLattice,
						pl->m_X[m_pIrenStyleSelect->m_numberInLattice],
						pl->m_Y[m_pIrenStyleSelect->m_numberInLattice],
						pl->m_Z[m_pIrenStyleSelect->m_numberInLattice],
						pl->m_WPosition[m_pIrenStyleSelect->m_numberInLattice],
						pl->m_charge[m_pIrenStyleSelect->m_numberInLattice]);
			sb->SetWindowText(str);
			pl->m_selected[m_pIrenStyleSelect->m_number] = true;
		}
	}
}
///////////////////////////////////////////////////////////////////////////
//
void CMadelungView::OnPopupUnselectatom() 
{
	if(m_pIrenStyleSelect)
	{
		CMadelungDoc* pDoc = GetDocument();
		CLattice* pl = pDoc->m_pLattice;

		if(pl->m_selected[m_pIrenStyleSelect->m_number])	// atom previously selected 
		{
			CMainFrame* pFr = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CStatusBar* sb = (CStatusBar*) pFr->GetMessageBar();

			m_pIrenStyleSelect->m_actr->GetProperty()->
			SetColor(pDoc->m_pAtomProperties->m_Color[pl->m_type[m_pIrenStyleSelect->m_number]][0],
					pDoc->m_pAtomProperties->m_Color[pl->m_type[m_pIrenStyleSelect->m_number]][1],
					pDoc->m_pAtomProperties->m_Color[pl->m_type[m_pIrenStyleSelect->m_number]][2]);
			pDoc->m_selectedAtoms--;
// used to disable the Calculate menu items
//			if(!m_SelectedAtoms)
//				m_AtomsAreSelected = FALSE;
			CString str;
			str.Format("Unselected atom = %s (%d) at x,y,z=(%f, %f, %f), Wyckoff position = %c, charge = %f",
						pDoc->m_pAtomProperties->m_Type[pl->m_type[m_pIrenStyleSelect->m_numberInLattice]],
						m_pIrenStyleSelect->m_numberInLattice,
						pl->m_X[m_pIrenStyleSelect->m_numberInLattice],
						pl->m_Y[m_pIrenStyleSelect->m_numberInLattice],
						pl->m_Z[m_pIrenStyleSelect->m_numberInLattice],
						pl->m_WPosition[m_pIrenStyleSelect->m_numberInLattice],
						pl->m_charge[m_pIrenStyleSelect->m_numberInLattice]);
			sb->SetWindowText(str);
			pl->m_selected[m_pIrenStyleSelect->m_number] = false;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////
//	Hides only the actor of an atom from the cluster, ie, number <= pDoc->m_atomsInCluster
//  HIDES ONE ACTOR
//  Atoms are not deleted from the CLattice
//
void CMadelungView::OnPopupHide() 
{
	if(m_pIrenStyleSelect)
	{
		CMadelungDoc* pDoc = GetDocument();
		CLattice* pl = pDoc->m_pLattice;

		if(!pl->m_selected[m_pIrenStyleSelect->m_number] &&
			pl->m_rendered[m_pIrenStyleSelect->m_number] &&
			this->atomInCluster(m_pIrenStyleSelect->m_number))
// remove atom actor from renderer, but not delete it. Leave it in the CLattice
		{
			CMainFrame* pFr = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CStatusBar* sb = (CStatusBar*) pFr->GetMessageBar();

			m_pRenderer->RemoveActor(m_patomActor[m_pIrenStyleSelect->m_number]);
			pl->m_rendered[m_pIrenStyleSelect->m_number] = false;

// used to disable the Calculate menu items
//			if(!m_SelectedAtoms)
//				m_AtomsAreSelected = FALSE;
			CString str;
			str.Format("Hidden atom = %s (%d) at x,y,z=(%f, %f, %f), Wyckoff position = %c, charge = %f",
						pDoc->m_pAtomProperties->m_Type[pl->m_type[m_pIrenStyleSelect->m_numberInLattice]],
						m_pIrenStyleSelect->m_numberInLattice,
						pl->m_X[m_pIrenStyleSelect->m_numberInLattice],
						pl->m_Y[m_pIrenStyleSelect->m_numberInLattice],
						pl->m_Z[m_pIrenStyleSelect->m_numberInLattice],
						pl->m_WPosition[m_pIrenStyleSelect->m_numberInLattice],
						pl->m_charge[m_pIrenStyleSelect->m_numberInLattice]);
			sb->SetWindowText(str);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
//	Hides only actors of atoms from cluster, ie, number <= pDoc->m_atomsInCluster
//	HIDES ALL ACTORS OF THE SAME WYCKOFF POSITION OF THE SAME ATOM TYPE
//  Atoms are not deleted from the CLattice
//
void CMadelungView::OnPopupHideall() 
{
	if(m_pIrenStyleSelect)
	{
		CMadelungDoc* pDoc = GetDocument();
		CLattice* pl = pDoc->m_pLattice;

		char Wpos = pl->m_WPosition[m_pIrenStyleSelect->m_number];
		int type = pl->m_type[m_pIrenStyleSelect->m_number];
		double x = pl->m_X[m_pIrenStyleSelect->m_number];
		double y = pl->m_Y[m_pIrenStyleSelect->m_number];
		double z = pl->m_Z[m_pIrenStyleSelect->m_number];
		CString str("Removed atoms: ");
		CString str1;

		if(!pl->m_selected[m_pIrenStyleSelect->m_number] &&
			pl->m_rendered[m_pIrenStyleSelect->m_number] &&
			this->atomInCluster(m_pIrenStyleSelect->m_number))
// remove atom actor from renderer, but not delete it. Leave it in the CLattice
		{
			CMainFrame* pFr = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CStatusBar* sb = (CStatusBar*) pFr->GetMessageBar();

			m_pRenderer->RemoveActor(m_patomActor[m_pIrenStyleSelect->m_number]);
			for(int i=0;i<pDoc->m_atomsInCluster;i++)
			{
				if(pl->m_WPosition[i] == Wpos && pl->m_type[i] == type)
				{
					m_pRenderer->RemoveActor(m_patomActor[i]);
					pl->m_rendered[i] = false;
					str1.Format("%d ",i);
					str += str1;
				}
			}
// used to disable the Calculate menu items
//			if(!m_SelectedAtoms)
//				m_AtomsAreSelected = FALSE;
			sb->SetWindowText(str);
		}
	}
}
////////////////////////////////////////////////////////////////////////
//
void CMadelungView::OnPopupShowall() 
{
	CMadelungDoc* pDoc = GetDocument();
	CLattice* pl = pDoc->m_pLattice;

	for(int i=0;i<pDoc->m_atomsInCluster;i++)
		if(!pl->m_rendered[i])
		{
			m_pRenderer->AddActor(m_patomActor[i]);
			pl->m_rendered[i] = true;
		}
}
///////////////////////////////////////////////////////////////////////
//
bool CMadelungView::atomInCluster(int number) 
{
	CMadelungDoc* pDoc = GetDocument();
	if(number <= pDoc->m_atomsInCluster)
		return true;
	else
		return false;
}
///////////////////////////////////////////////////////////////////////
// shifts all actors by 'shift' vector
//
void CMadelungView::ShiftActors(double shift[3])
{
	float pos[3];
// ATOM actors
	int size = m_patomActor.size();
	for(int i=0;i<size;i++)
	{
		m_patomActor[i]->GetPosition(pos);
		for(int j=0;j<3;j++)
			pos[j] -= shift[j];
		m_patomActor[i]->SetPosition(pos);
	}
// BOND actors
	size = m_pbondActor.size();
	for(i=0;i<size;i++)
	{
		m_pbondActor[i]->GetPosition(pos);
		for(int j=0;j<3;j++)
			pos[j] -= shift[j];
		m_pbondActor[i]->SetPosition(pos);
	}
// CELL BORDER actors
	size = m_pCellBorderActor.size();
	for(i=0;i<size;i++)
	{
		m_pCellBorderActor[i]->GetPosition(pos);
		for(int j=0;j<3;j++)
			pos[j] -= shift[j];
		m_pCellBorderActor[i]->SetPosition(pos);
	}
	vtkCamera* cam = m_pRenderer->GetActiveCamera();
		cam->SetFocalPoint(0,0,0);
	this->UpdateWindow();
}
//////////////////////////////////////////////////////////////////////////
//	this function hides all atoms whose X coordinates are almost the same
//  and atom types are the same
//	Hides only actors of atoms from CLUSTER, ie, number <= pDoc->m_atomsInCluster
//
void CMadelungView::OnPopupHidelayerX() 
{
	if(m_pIrenStyleSelect)
	{
		CMadelungDoc* pDoc = GetDocument();
		CLattice* pl = pDoc->m_pLattice;

		int type = pl->m_type[m_pIrenStyleSelect->m_number];
		double x = pl->m_X[m_pIrenStyleSelect->m_number];
		CString str("Removed atoms: ");
		CString str1;

		if(!pl->m_selected[m_pIrenStyleSelect->m_number] &&
			pl->m_rendered[m_pIrenStyleSelect->m_number] &&
			this->atomInCluster(m_pIrenStyleSelect->m_number))
// remove atom actor from renderer, but not delete it. Leave it in the CLattice
		{
			CMainFrame* pFr = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CStatusBar* sb = (CStatusBar*) pFr->GetMessageBar();

//			m_pRenderer->RemoveActor(m_patomActor[m_pIrenStyleSelect->m_number]);
			for(int i=0;i<pDoc->m_atomsInCluster;i++)
			{
				if(fabs(pl->m_X[i] - x) < LAYER_TOLERANCE && pl->m_type[i] == type)
				{
					m_pRenderer->RemoveActor(m_patomActor[i]);
					pl->m_rendered[i] = false;
					str1.Format("%d ",i);
					str += str1;
				}
			}
// used to disable the Calculate menu items
//			if(!m_SelectedAtoms)
//				m_AtomsAreSelected = FALSE;
			sb->SetWindowText(str);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//	this function hides all atoms whose Y coordinates are almost the same
//  and atom types are the same
//	Hides only actors of atoms from CLUSTER, ie, number <= pDoc->m_atomsInCluster
//
void CMadelungView::OnPopupHidelayerY() 
{
	if(m_pIrenStyleSelect)
	{
		CMadelungDoc* pDoc = GetDocument();
		CLattice* pl = pDoc->m_pLattice;

		int type = pl->m_type[m_pIrenStyleSelect->m_number];
		double y = pl->m_Y[m_pIrenStyleSelect->m_number];
		CString str("Removed atoms: ");
		CString str1;

		if(!pl->m_selected[m_pIrenStyleSelect->m_number] &&
			pl->m_rendered[m_pIrenStyleSelect->m_number] &&
			this->atomInCluster(m_pIrenStyleSelect->m_number))
// remove atom actor from renderer, but not delete it. Leave it in the CLattice
		{
			CMainFrame* pFr = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CStatusBar* sb = (CStatusBar*) pFr->GetMessageBar();

//			m_pRenderer->RemoveActor(m_patomActor[m_pIrenStyleSelect->m_number]);
			for(int i=0;i<pDoc->m_atomsInCluster;i++)
			{
				if(fabs(pl->m_Y[i] - y) < LAYER_TOLERANCE && pl->m_type[i] == type)
				{
					m_pRenderer->RemoveActor(m_patomActor[i]);
					pl->m_rendered[i] = false;
					str1.Format("%d ",i);
					str += str1;
				}
			}
// used to disable the Calculate menu items
//			if(!m_SelectedAtoms)
//				m_AtomsAreSelected = FALSE;
			sb->SetWindowText(str);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//	this function hides all atoms whose Z coordinates are almost the same
//  and atom types are the same
//	Hides only actors of atoms from CLUSTER, ie, number <= pDoc->m_atomsInCluster
//
void CMadelungView::OnPopupHidelayerZ() 
{
	if(m_pIrenStyleSelect)
	{
		CMadelungDoc* pDoc = GetDocument();
		CLattice* pl = pDoc->m_pLattice;

		int type = pl->m_type[m_pIrenStyleSelect->m_number];
		double z = pl->m_Z[m_pIrenStyleSelect->m_number];
		CString str("Removed atoms: ");
		CString str1;

		if(!pl->m_selected[m_pIrenStyleSelect->m_number] &&
			pl->m_rendered[m_pIrenStyleSelect->m_number] &&
			this->atomInCluster(m_pIrenStyleSelect->m_number))
// remove atom actor from renderer, but not delete it. Leave it in the CLattice
		{
			CMainFrame* pFr = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CStatusBar* sb = (CStatusBar*) pFr->GetMessageBar();

//			m_pRenderer->RemoveActor(m_patomActor[m_pIrenStyleSelect->m_number]);
			for(int i=0;i<pDoc->m_atomsInCluster;i++)
			{
				if(fabs(pl->m_Z[i] - z) < LAYER_TOLERANCE && pl->m_type[i] == type)
				{
					m_pRenderer->RemoveActor(m_patomActor[i]);
					pl->m_rendered[i] = false;
					str1.Format("%d ",i);
					str += str1;
				}
			}
// used to disable the Calculate menu items
//			if(!m_SelectedAtoms)
//				m_AtomsAreSelected = FALSE;
			sb->SetWindowText(str);
		}
	}
}
