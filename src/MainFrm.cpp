// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Madelung.h"
#include "MadelungDoc.h"
#include "MadelungView.h"
#include "MainFrm.h"
#include "PropertyPageAtomsInUnitCell.h"
#include "PropertyPageMoleculesInUnitCell.h"
#include "PropertyPageSpaceGroup.h"
#include "NumberOfUnitCellsToPlot.h"
#include "Spg_dll.h"
#include "UnitCell.h"
#include "AtomWindow.h"
#include "PlotAnAtom.h"
#include "Molecule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOOLS_SETUP_LATTICE, OnToolsSetupLattice)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PCDF_1, OnUpdateToolsPcdf1)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PCDF_2, OnUpdateToolsPcdf2)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PCDF_3, OnUpdateToolsPcdf3)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PCDF_4, OnUpdateToolsPcdf4)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PCDF_5, OnUpdateToolsPcdf5)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PCDF_6, OnUpdateToolsPcdf6)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PCDF_7, OnUpdateToolsPcdf7)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PCDF_8, OnUpdateToolsPcdf8)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PCDF_9, OnUpdateToolsPcdf9)
	ON_COMMAND(ID_TOOLS_PCDF_1, OnToolsPcdf1)
	ON_COMMAND(ID_TOOLS_PCDF_2, OnToolsPcdf2)
	ON_COMMAND(ID_TOOLS_PCDF_3, OnToolsPcdf3)
	ON_COMMAND(ID_TOOLS_PCDF_4, OnToolsPcdf4)
	ON_COMMAND(ID_TOOLS_PCDF_5, OnToolsPcdf5)
	ON_COMMAND(ID_TOOLS_PCDF_6, OnToolsPcdf6)
	ON_COMMAND(ID_TOOLS_PCDF_7, OnToolsPcdf7)
	ON_COMMAND(ID_TOOLS_PCDF_8, OnToolsPcdf8)
	ON_COMMAND(ID_TOOLS_PCDF_9, OnToolsPcdf9)
	ON_COMMAND(ID_PLOT_UNITCELL, OnPlotUnitcell)
	ON_UPDATE_COMMAND_UI(ID_PLOT_UNITCELL, OnUpdatePlotUnitcell)
	ON_COMMAND(ID_VIEW_WYCKOFF, OnViewWyckoff)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WYCKOFF, OnUpdateViewWyckoff)
	ON_WM_SIZE()
	ON_COMMAND(ID_PLOT_ATOM, OnPlotAtom)
	ON_COMMAND(ID_PLOT_ATOM_REMOVE, OnDeleteAtom)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
	ON_MESSAGE(WM_USER_CALC_IN_PROGRESS, OnCalcInProgress)
	ON_MESSAGE(WM_USER_TOGGLE_STATUS_BAR, OnToggleStatusBar)
	ON_MESSAGE(WM_USER_CREATE_PROGRESS_BAR, OnCreateProgressBar)
	ON_MESSAGE(WM_USER_DESTROY_PROGRESS_BAR, OnDestroyProgressBar)
	ON_MESSAGE(WM_USER_MODIFY_MAINFRAME_MENU, OnModifyMainframeMenu)
	ON_MESSAGE(WM_USER_POTENTIALS_OK, OnPotentialsOK)
	ON_MESSAGE(WM_USER_OPTJOB_DONE, OnOptJobDone)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL
};

static UINT indicators1[] =
{
	ID_SEPARATOR,           // status line indicator
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
	ID_PROGRESS_BAR
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
// indicators for menu items, when they are checked, when not
	m_FitFunction_1 = false;
	m_FitFunction_2 = false;
	m_FitFunction_3 = false;
	m_FitFunction_4 = false;
	m_FitFunction_5 = false;
	m_FitFunction_6 = false;
	m_FitFunction_7 = true;
	m_FitFunction_8 = false;
	m_FitFunction_9 = false;

	m_pAW = NULL;	// AtomWindow not created
	m_ProgressBarCreated = FALSE;
	m_StatusBarWithProgressBar = 0;
}


CMainFrame::~CMainFrame()
{
	if(m_pAW) delete m_pAW;	// delete the AtomWindow object
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
//	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
//		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
//	{
//		TRACE0("Failed to create dialogbar\n");
//		return -1;		// fail to create
//	}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar))
//		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnToolsSetupLattice() 
{
	CPropertySheet dlgPropertySheet("Crystal Lattice Setup");
	PropertyPageSpaceGroup sgPage;
	PropertyPageAtomsInUnitCell atomsInUnitCellPage;
	PropertyPageMoleculesInUnitCell moleculesInUnitCellPage;

	CMadelungDoc* pDoc = GetDoc();

	sgPage.m_sg = 0; // radio button selected to the short HM symbol

// now load the group description (if created)
	if(pDoc->m_pSpaceGroup)
	{
		sgPage.m_sg_edit = pDoc->m_pSpaceGroup->m_Short_notation.c_str();
// load the spacegroup description
		sgPage.m_a = pDoc->m_pSpaceGroup->m_pUnitCell->m_a;
		sgPage.m_b = pDoc->m_pSpaceGroup->m_pUnitCell->m_b;
		sgPage.m_c = pDoc->m_pSpaceGroup->m_pUnitCell->m_c;
		sgPage.m_alpha = pDoc->m_pSpaceGroup->m_pUnitCell->m_alpha;
		sgPage.m_beta = pDoc->m_pSpaceGroup->m_pUnitCell->m_beta;
		sgPage.m_gamma = pDoc->m_pSpaceGroup->m_pUnitCell->m_gamma;

		sgPage.m_pSpaceGroup = pDoc->m_pSpaceGroup;
		LoadSGDescription(sgPage);

// WITHOUT symmetric Wyckoff positions (one atom per position)
		int numat = pDoc->m_pSpaceGroup->m_pUnitCell->m_positionsInUnitCell;
		atomsInUnitCellPage.m_NumberOfAtomsInUnitCell = numat;
// for enabling/disabling menu item
		if(numat) atomsInUnitCellPage.m_AtomCoordinatesAreReady = true;
		else
			atomsInUnitCellPage.m_AtomCoordinatesAreReady = false;

		for(int i=0; i<numat;i++)
		{
			char type = pDoc->m_pSpaceGroup->m_pUnitCell->m_WyckoffPosition[i];
			atomsInUnitCellPage.m_WPosition.push_back(type);

			atomsInUnitCellPage.m_x.push_back(
						pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomX[i]
											);
			atomsInUnitCellPage.m_y.push_back(
						pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomY[i]
											);
			atomsInUnitCellPage.m_z.push_back(
						pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomZ[i]
										);
			atomsInUnitCellPage.m_charge.push_back(
						pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomCharge[i]
											);
			atomsInUnitCellPage.m_atomType.push_back(
						pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomType[i]
												);
		}
// molecules in unit cell page is filled in the 'PropertyPageMoleculesInUnitCell'
	}
	else	// SpaceGroup class not created
	{
		sgPage.m_pSpaceGroup = NULL;
		sgPage.m_sg_edit = "";
		sgPage.m_a = 0;
		sgPage.m_b = 0;
		sgPage.m_c = 0;
		sgPage.m_alpha = 0;
		sgPage.m_beta = 0;
		sgPage.m_gamma = 0;
// set space group description fields to blank
		atomsInUnitCellPage.m_NumberOfAtomsInUnitCell = 0;
		atomsInUnitCellPage.m_AtomCoordinatesAreReady = false;
	}
// add pages to the sheet
	dlgPropertySheet.AddPage(&sgPage);
	dlgPropertySheet.AddPage(&atomsInUnitCellPage);
	dlgPropertySheet.AddPage(&moleculesInUnitCellPage);

	if(dlgPropertySheet.DoModal() == IDOK)
	{
// check if spacegroup was specified
		CString tmp = sgPage.m_sg_edit;
		tmp.Remove(' ');
		if(tmp == "")
		{
			int reply = AfxMessageBox("Load Space group!",MB_OKCANCEL);
			if(reply == IDOK)
			{
				PostMessage(WM_COMMAND,ID_TOOLS_SETUP_LATTICE,NULL);
				return;
			}
			else // if canceled 
				return;
		}
// not ALL atoms, Just one from each Wyckoff set
		int numpos = atomsInUnitCellPage.m_NumberOfAtomsInUnitCell;
// check if atoms or molecules has been specified
		if(!atomsInUnitCellPage.m_AtomCoordinatesAreReady &&
		   !moleculesInUnitCellPage.m_MoleculesAreReady)
		{
			int reply = AfxMessageBox("Atoms not specified",MB_OKCANCEL);
			if(reply == IDOK)
			{
				PostMessage(WM_COMMAND,ID_TOOLS_SETUP_LATTICE,NULL);
				return;
			}
			else // if canceled 
				return;
		}
// SPACE GROUP IS CREATED in PropertyPageSpaceGroup BY CLICKING THE 'LOAD' BUTTON
// and assigned to the pDoc->m_pSpaceGroup
// SpaceGroup symbol validation done in the SpaceGroup property page
		pDoc->ReinitializeDocument();	// do not pDoc->m_pSpaceGroup = NULL !!!
		pDoc->m_pSpaceGroup->m_pUnitCell->Clear();
		pDoc->m_pSpaceGroup->m_pUnitCell->m_a = sgPage.m_a;
		pDoc->m_pSpaceGroup->m_pUnitCell->m_b = sgPage.m_b;
		pDoc->m_pSpaceGroup->m_pUnitCell->m_c = sgPage.m_c;
		pDoc->m_pSpaceGroup->m_pUnitCell->m_alpha = sgPage.m_alpha;
		pDoc->m_pSpaceGroup->m_pUnitCell->m_beta = sgPage.m_beta;
		pDoc->m_pSpaceGroup->m_pUnitCell->m_gamma = sgPage.m_gamma;
// only symmetrically independent atoms, not all atoms
		pDoc->m_pSpaceGroup->m_pUnitCell->m_positionsInUnitCell = numpos;
		for(int i=0; i<numpos;i++)
		{
			char type = atomsInUnitCellPage.m_WPosition[i].GetAt(0);
			pDoc->m_pSpaceGroup->m_pUnitCell->m_WyckoffPosition.push_back(type);
			pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomX.push_back(
					atomsInUnitCellPage.m_x[i]);
			pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomY.push_back(			
					atomsInUnitCellPage.m_y[i]);
			pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomZ.push_back(
					atomsInUnitCellPage.m_z[i]);
			pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomCharge.push_back(
					atomsInUnitCellPage.m_charge[i]);
			pDoc->m_pSpaceGroup->m_pUnitCell->m_unitCellAtomType.push_back(
					atomsInUnitCellPage.m_atomType[i]);
		}
// MOLECULES IN THE UNIT CELL
// delete previously stored molecules
		int size = pDoc->m_molecules.size();
		if(size)
		{
			for(int i=0;i<size;i++)
				delete pDoc->m_molecules[i];
			pDoc->m_molecules.clear();
		}
// store new molecules
		size = moleculesInUnitCellPage.size();
		if(size)
		{
			for(int i=0;i<size;i++)
			{
				pDoc->m_molecules.push_back(new Molecule);
				*pDoc->m_molecules[i] = *moleculesInUnitCellPage.m_ObjMolecules[i];
			}
		}

// change the menu from start
		pDoc->SetMenu(IDR_MAINFRAME);
//	calculate the unit cell vectors, they are used in atom coordinate calculations
//		pDoc->m_pSpaceGroup->m_pUnitCell->CalculateVectors();
// calculate the cartesian coordinates of symmetrically independent atoms
// only ONE atom from a Wyckoff position
//		pDoc->m_pSpaceGroup->m_pUnitCell->CalculateAtoms();
	}
	else
		return;
}

///////////////////////////////////////////////////////////////////////////////
//

void CMainFrame::OnUpdateToolsPcdf1(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	if(GetDoc()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
	}
	if(m_FitFunction_1) pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CMainFrame::OnUpdateToolsPcdf2(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	if(GetDoc()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
	}
	if(m_FitFunction_2) pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CMainFrame::OnUpdateToolsPcdf3(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	if(GetDoc()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
	}
	if(m_FitFunction_3) pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CMainFrame::OnUpdateToolsPcdf4(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	if(GetDoc()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
	}
	if(m_FitFunction_4) pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CMainFrame::OnUpdateToolsPcdf5(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	if(GetDoc()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
	}
	if(m_FitFunction_5) pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CMainFrame::OnUpdateToolsPcdf6(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	if(GetDoc()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
	}
	if(m_FitFunction_6) pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CMainFrame::OnUpdateToolsPcdf7(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	if(GetDoc()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
	}
	if(m_FitFunction_7) pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CMainFrame::OnUpdateToolsPcdf8(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	if(GetDoc()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
	}
	if(m_FitFunction_8) pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CMainFrame::OnUpdateToolsPcdf9(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	if(GetDoc()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
	}
	if(m_FitFunction_9) pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

///////////////////////////////////////////////////////////////////////
//

void CMainFrame::OnToolsPcdf1() 
{
	CMadelungDoc* pDoc = GetDoc();
	m_FitFunction_1 = true;
	m_FitFunction_2 = false;
	m_FitFunction_3 = false;
	m_FitFunction_4 = false;
	m_FitFunction_5 = false;
	m_FitFunction_6 = false;
	m_FitFunction_7 = false;
	m_FitFunction_8 = false;
	m_FitFunction_9 = false;
	pDoc->m_FitFunction = 1;
	CMadelungView* pv = (CMadelungView*)GetActiveView();
	ASSERT_VALID(pv);
	pv->Invalidate();
}

void CMainFrame::OnToolsPcdf2() 
{
	CMadelungDoc* pDoc = GetDoc();
	m_FitFunction_1 = false;
	m_FitFunction_2 = true;
	m_FitFunction_3 = false;
	m_FitFunction_4 = false;
	m_FitFunction_5 = false;
	m_FitFunction_6 = false;
	m_FitFunction_7 = false;
	m_FitFunction_8 = false;
	m_FitFunction_9 = false;
	pDoc->m_FitFunction = 2;
	CMadelungView* pv = (CMadelungView*)GetActiveView();
	ASSERT_VALID(pv);
	pv->Invalidate();
}

void CMainFrame::OnToolsPcdf3() 
{
	CMadelungDoc* pDoc = GetDoc();
	m_FitFunction_1 = false;
	m_FitFunction_2 = false;
	m_FitFunction_3 = true;
	m_FitFunction_4 = false;
	m_FitFunction_5 = false;
	m_FitFunction_6 = false;
	m_FitFunction_7 = false;
	m_FitFunction_8 = false;
	m_FitFunction_9 = false;
	pDoc->m_FitFunction = 3;
	CMadelungView* pv = (CMadelungView*)GetActiveView();
	ASSERT_VALID(pv);
	pv->Invalidate();
}

void CMainFrame::OnToolsPcdf4() 
{
	CMadelungDoc* pDoc = GetDoc();
	m_FitFunction_1 = false;
	m_FitFunction_2 = false;
	m_FitFunction_3 = false;
	m_FitFunction_4 = true;
	m_FitFunction_5 = false;
	m_FitFunction_6 = false;
	m_FitFunction_7 = false;
	m_FitFunction_8 = false;
	m_FitFunction_9 = false;
	pDoc->m_FitFunction = 4;
	CMadelungView* pv = (CMadelungView*)GetActiveView();
	ASSERT_VALID(pv);
	pv->Invalidate();
}

void CMainFrame::OnToolsPcdf5() 
{
	CMadelungDoc* pDoc = GetDoc();
	m_FitFunction_1 = false;
	m_FitFunction_2 = false;
	m_FitFunction_3 = false;
	m_FitFunction_4 = false;
	m_FitFunction_5 = true;
	m_FitFunction_6 = false;
	m_FitFunction_7 = false;
	m_FitFunction_8 = false;
	m_FitFunction_9 = false;
	pDoc->m_FitFunction = 5;
	CMadelungView* pv = (CMadelungView*)GetActiveView();
	ASSERT_VALID(pv);
	pv->Invalidate();
}

void CMainFrame::OnToolsPcdf6() 
{
	CMadelungDoc* pDoc = GetDoc();
	m_FitFunction_1 = false;
	m_FitFunction_2 = false;
	m_FitFunction_3 = false;
	m_FitFunction_4 = false;
	m_FitFunction_5 = false;
	m_FitFunction_6 = true;
	m_FitFunction_7 = false;
	m_FitFunction_8 = false;
	m_FitFunction_9 = false;
	pDoc->m_FitFunction = 6;
	CMadelungView* pv = (CMadelungView*)GetActiveView();
	ASSERT_VALID(pv);
	pv->Invalidate();
}

void CMainFrame::OnToolsPcdf7() 
{
	CMadelungDoc* pDoc = GetDoc();
	m_FitFunction_1 = false;
	m_FitFunction_2 = false;
	m_FitFunction_3 = false;
	m_FitFunction_4 = false;
	m_FitFunction_5 = false;
	m_FitFunction_6 = false;
	m_FitFunction_7 = true;
	m_FitFunction_8 = false;
	m_FitFunction_9 = false;
	pDoc->m_FitFunction = 7;
	CMadelungView* pv = (CMadelungView*)GetActiveView();
	ASSERT_VALID(pv);
	pv->Invalidate();
}

void CMainFrame::OnToolsPcdf8() 
{
	CMadelungDoc* pDoc = GetDoc();
	m_FitFunction_1 = false;
	m_FitFunction_2 = false;
	m_FitFunction_3 = false;
	m_FitFunction_4 = false;
	m_FitFunction_5 = false;
	m_FitFunction_6 = false;
	m_FitFunction_7 = false;
	m_FitFunction_8 = true;
	m_FitFunction_9 = false;
	pDoc->m_FitFunction = 8;
	CMadelungView* pv = (CMadelungView*)GetActiveView();
	ASSERT_VALID(pv);
	pv->Invalidate();
}

void CMainFrame::OnToolsPcdf9() 
{
	CMadelungDoc* pDoc = GetDoc();
	m_FitFunction_1 = false;
	m_FitFunction_2 = false;
	m_FitFunction_3 = false;
	m_FitFunction_4 = false;
	m_FitFunction_5 = false;
	m_FitFunction_6 = false;
	m_FitFunction_7 = false;
	m_FitFunction_8 = false;
	m_FitFunction_9 = true;
	pDoc->m_FitFunction = 9;
	CMadelungView* pv = (CMadelungView*)GetActiveView();
	ASSERT_VALID(pv);
	pv->Invalidate();
}

///////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnPlotUnitcell() 
{
	CMadelungDoc* pDoc = GetDoc();
// read how many unit cells along a,b,c needs to be plot
	NumberOfUnitCellsToPlot d;
// initialize the dialog box
		d.m_a = pDoc->m_NumberOfUnitCellsForPlotAlongA;
		d.m_b = pDoc->m_NumberOfUnitCellsForPlotAlongB;
		d.m_c = pDoc->m_NumberOfUnitCellsForPlotAlongC;

	int result = d.DoModal();
	if(result == IDOK)
	{
		if(d.m_a>0 && d.m_b>0 && d.m_c>0)
		{
			pDoc->m_NumberOfUnitCellsForPlotAlongA = d.m_a;
			pDoc->m_NumberOfUnitCellsForPlotAlongB = d.m_b;
			pDoc->m_NumberOfUnitCellsForPlotAlongC = d.m_c;
		}
		else
		{
			int reply;
			reply = AfxMessageBox("Number of unit cells in plot must be > 0",MB_OKCANCEL);
			if(reply == IDOK)
			{
				PostMessage(WM_COMMAND,ID_PLOT_UNITCELL,NULL);
				return;
			}
			else
				return;
		}
	}
	else
		return;

	CMadelungView* pv;
	pv = (CMadelungView*)GetActiveView();
	pv->PrepareForUnitCellPlot();
//	pv->Invalidate();
}

////////////////////////////////////////////////////////////////////////////
//

CMadelungDoc* CMainFrame::GetDoc()
{
	CMadelungDoc* pDoc = (CMadelungDoc*) GetActiveDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CMadelungDoc)));
	return pDoc;
}

////////////////////////////////////////////////////////////////////////////
//

void CMainFrame::OnUpdatePlotUnitcell(CCmdUI* pCmdUI) 
{
	CMadelungView* pv;
	pv = (CMadelungView*)GetActiveView();
	if(pv->m_ClusterIsRendered || GetDoc()->m_bCalculationInProgress)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

//////////////////////////////////////////////////////////////////////////////
//	fills the static windows with spacegroup description (uses DDX)

void CMainFrame::LoadSGDescription(PropertyPageSpaceGroup &sgPage)
{
	if(sgPage.m_pSpaceGroup)
	{
		char buffer[10];
		sgPage.m_pagenumber = sgPage.m_pSpaceGroup->m_pageInInternationalTables.c_str();
		sgPage.m_sgnumber = itoa(sgPage.m_pSpaceGroup->m_SpaceGroupNumber,buffer,10);
		sgPage.m_shortValue = sgPage.m_pSpaceGroup->m_Short_notation.c_str();
		sgPage.m_longValue = sgPage.m_pSpaceGroup->m_Long_notation.c_str();
		sgPage.m_schValue = sgPage.m_pSpaceGroup->m_Schoenflies.c_str();
		sgPage.m_hallValue = sgPage.m_pSpaceGroup->m_Hall.c_str();
		sgPage.m_pgValue = sgPage.m_pSpaceGroup->m_PointGroup.c_str();
		sgPage.m_crysystValue = sgPage.m_pSpaceGroup->m_CrystalSystem.c_str();
		sgPage.m_latticeSymbolValue = sgPage.m_pSpaceGroup->m_LatticeSymbol.c_str();
		sgPage.m_addInfoValue = sgPage.m_pSpaceGroup->m_additionalInfo.c_str();
		sgPage.m_distanceRestrictions = sgPage.m_pSpaceGroup->m_pUnitCell->m_abcRestrictions.c_str();
		sgPage.m_angleRestrictions = sgPage.m_pSpaceGroup->m_pUnitCell->m_angleRestrictions.c_str();
	}
}

void CMainFrame::OnViewWyckoff() 
{
	if(m_pAW)
	{
		delete m_pAW;
		m_pAW = NULL;
	}
	m_pAW = new AtomWindow();
	m_pAW->CreateEx(NULL,m_pAW->m_plpszAtomWindowClass,"AtomCoordinates",
				WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,100,100,400,500,NULL,NULL,NULL);
// here follows window region settings
/*
	CRect rect;
	CRgn rgn;
	m_pAW->GetWindowRect(rect);
	rect.NormalizeRect();
	rgn.CreateRoundRectRgn(0,0,rect.Width(),rect.Height(),30,30);
	HRGN hrgn = HRGN(rgn);
	m_pAW->SetWindowRgn(hrgn,FALSE);
*/
	m_pAW->ShowWindow(SW_SHOWNORMAL);
	m_pAW->UpdateWindow();
// ID=900 is used also in the AtomWindow for buttons
// show coordinates for the selected Wyckoff set
	WPARAM wParam = BN_CLICKED | 900;
	m_pAW->SendMessage(WM_COMMAND,wParam,0);
}

//////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnUpdateViewWyckoff(CCmdUI* pCmdUI) 
{
	if(GetDoc()->m_pSpaceGroup)
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

////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnCalcInProgress(WPARAM WParam, LPARAM LParam)
{
	this->m_wndProgressBar.SetPos(WParam);
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnCreateProgressBar(WPARAM WParam, LPARAM LParam)
{
	if(!m_ProgressBarCreated)
	{
		CRect progrRect;
		m_wndStatusBar.GetItemRect(1,&progrRect);

		if (!m_wndProgressBar.Create(WS_VISIBLE|WS_CHILD,progrRect,&m_wndStatusBar,ID_PROGRESS_BAR))
		{
			TRACE0("Failed to create progress bar\n");
			return -1;      // fail to create
		}
		m_wndProgressBar.SetRange(0,100);
		m_ProgressBarCreated = TRUE;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::ResetProgressBar()
{
	if(m_ProgressBarCreated)
	{
		m_wndProgressBar.SetPos(0);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnToggleStatusBar(WPARAM wParam, LPARAM lParam)
{
	if(m_StatusBarWithProgressBar == wParam)
		return 0;
	else
	{
		if(wParam == 1)	// progressBar required
		{
			m_wndStatusBar.SetIndicators(indicators1,sizeof(indicators1)/sizeof(UINT));
			m_StatusBarWithProgressBar = 1;
		}
		else	// progress bar not required
		{
			m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));
			m_StatusBarWithProgressBar = 0;
		}
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnDestroyProgressBar(WPARAM wParam, LPARAM lParam)
{
	if(m_ProgressBarCreated)
	{
		m_wndProgressBar.DestroyWindow();
		m_ProgressBarCreated = FALSE;
		this->m_wndStatusBar.UpdateWindow();
		GetDoc()->m_bCalculationInProgress = FALSE;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	if(m_ProgressBarCreated)
	{
		RECT rc;
		m_wndStatusBar.GetItemRect(1,&rc);
	// Reposition the progress control correctly!
		m_wndProgressBar.SetWindowPos(&wndTop, rc.left, rc.top, rc.right - rc.left,
		rc.bottom - rc.top, 0); 
	}
}

////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnModifyMainframeMenu(WPARAM wParam,LPARAM lParam)
{
	if(wParam == 0)
	{
		CMenu menu;
		menu.LoadMenu(IDR_MAINFRAME);
		this->SetMenu(&menu);
		menu.Detach();
	}
	else
	{
		CMenu menu;
		menu.LoadMenu(IDR_MAINFRAME_CANCEL);
		this->SetMenu(&menu);
		menu.Detach();
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnPotentialsOK(WPARAM wParam,LPARAM lParam)
{
	if(wParam == 1)	 // all jobs done OK
		GetDoc()->SetCalculatedOK(TRUE);
	else	// worker thread terminated
		GetDoc()->SetCalculatedOK(FALSE);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//
LRESULT CMainFrame::OnOptJobDone(WPARAM wParam,LPARAM lParam)
{
	return GetDoc()->OnOptJobDone(wParam,lParam);
}

//////////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnPlotAtom() 
{
	PlotAnAtom paa;
	paa.m_atom = 0;
	int result = paa.DoModal();
	if(result == IDOK)
	{
		CMadelungView* pv = (CMadelungView*)GetActiveView();
		pv->AddAtomToPlot(paa.m_atom);
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::OnDeleteAtom()
{
	PlotAnAtom paa;
	paa.m_atom = 0;
	int result = paa.DoModal();
	if(result == IDOK)
	{
		CMadelungView* pv = (CMadelungView*)GetActiveView();
		pv->RemoveAtomFromPlot(paa.m_atom);
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
