// AtomWindow.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "AtomWindow.h"
#include "Spg_dll.h"
#include "MadelungDoc.h"
#include "AtomView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AtomWindow construction/destruction
// assumes that SpaceGroup has been loaded

AtomWindow::AtomWindow()
{
}

AtomWindow::~AtomWindow()
{
	CMadelungDoc* pDoc = GetDoc();
// delete 'radiobuttons'
	if(pDoc->m_pSpaceGroup)
	{
		int nmb = pDoc->m_pSpaceGroup->m_numberOfWyckoffSets;
		for(int i=0;i<nmb;i++)
			delete this->m_pWRadio[i];
	}
// delete the AtomView
	if(pDoc->m_pAtomView)
	{
// delete GetDoc()->m_pAtomView here didn't work!!!
		pDoc->m_pAtomView = NULL;
	}
}

BEGIN_MESSAGE_MAP(AtomWindow, CWnd)
	//{{AFX_MSG_MAP(AtomWindow)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//	precreate window

BOOL AtomWindow::PreCreateWindow(CREATESTRUCT& cs) 
{
	CBrush brush;
	LPCTSTR m_plpszAtomWindowClass =
		  AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
							AfxGetApp()->LoadStandardCursor(IDC_ARROW),
//							CreateSolidBrush(RGB(192,192,192)),
							HBRUSH(brush.CreateStockObject(LTGRAY_BRUSH)),
							LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)));

	cs.lpszClass = m_plpszAtomWindowClass;
	return CWnd::PreCreateWindow(cs);
}

///////////////////////////////////////////////////////////////////////////
//	Create call from the EditAtom object
// assumes that the SpaceGroup object has been created

int AtomWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
// now test if the SpaceGroup is created
	CMadelungDoc* pDoc = GetDoc();;

	if(!pDoc->m_pSpaceGroup)	// SpaceGroup not created
	{
		AfxMessageBox("Load a Space Group first!",MB_OK);
		return -1;
	}
	else	// SpaceGroup created
	{
// make the groupbox
		CRect rect;
		this->GetClientRect(rect);
		CRect rectGB(rect.TopLeft().x+5,rect.TopLeft().y+5,rect.TopLeft().x+rect.Width()-5,
			rect.TopLeft().y+rect.Height()-5);
		m_GroupBox.Create("Select the Wyckoff symbol for coordinate lookup",
			BS_GROUPBOX | WS_VISIBLE | WS_CHILD,rectGB,this,ID_AW_GB);
		CFont font;
		font.CreateStockObject(DEFAULT_GUI_FONT);
		m_GroupBox.SetFont(&font,FALSE);
// now make radiobuttons
// Button ID's are chosen to start from 900
		int ID_BUTTON[30];
		for(int i=0;i<30;i++) ID_BUTTON[i] = 900 + i;

		const int buttonSize = 20;	// radiopushbutton size
		int nmb = pDoc->m_pSpaceGroup->m_numberOfWyckoffSets;
		int bwidth = nmb*buttonSize;
		CRect gb;
		this->m_GroupBox.GetClientRect(gb);
		int x0 = (gb.Width()-bwidth)/2;
		int y0 = 25;
		for(i=0;i<nmb;i++)
		{
			this->m_pWRadio.push_back(new CButton);
			this->m_pWRadio[i]->Create(CString(pDoc->m_pSpaceGroup->m_WSymbol[nmb-i-1]),
				BS_PUSHBUTTON | WS_VISIBLE,
				CRect(x0+i*buttonSize,y0,x0+i*buttonSize+buttonSize,y0+buttonSize),
				this,ID_BUTTON[i]);	
		}
// now make AtomView window
// get the available region for AtomView
		CRect rectAV(rectGB.TopLeft().x+5,rectGB.TopLeft().y+50,rectGB.TopLeft().x+
			rectGB.Width()-5,rectGB.TopLeft().y+rectGB.Height()-5);

		pDoc->m_pAtomView = new AtomView;
		pDoc->m_pAtomView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		rectAV, this, ID_WIN_ATOMWIN, NULL);

		CSize sizeTotal;
		sizeTotal.cx = sizeTotal.cy = 100;
		pDoc->m_pAtomView->SetScrollSizes(MM_TEXT, sizeTotal);
		pDoc->m_pAtomView->ShowWindow(SW_SHOW);
		pDoc->AddView(pDoc->m_pAtomView);	
		return 0;
	}
}

////////////////////////////////////////////////////////////////
//

CMadelungDoc* AtomWindow::GetDoc()
{
	CMadelungApp* pApp = (CMadelungApp*)AfxGetApp();
	CWnd* wnd = pApp->m_pMainWnd;
	ASSERT(wnd->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CMainFrame* pFr = (CMainFrame*)wnd;
	CMadelungDoc* pDoc = (CMadelungDoc*) pFr->GetActiveDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CMadelungDoc)));
	return pDoc;
}

////////////////////////////////////////////////////////////////
// Command handler 
// assumes that the SpaceGroup object has been created

BOOL AtomWindow::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if(HIWORD(wParam) == BN_CLICKED)
	{
		int wID = LOWORD(wParam);
		if(wID >= 900 && wID <930)	// assigned above button ID's
		{
			CMadelungDoc* pDoc = GetDoc();
// clears the atom coordinate view
			pDoc->m_pAtomView->Clear();
// write out the space group
			string str;
			str = "Space group = " + pDoc->m_pSpaceGroup->m_Long_notation;
			AddStringToView(-1,str);
// write out centering translation vectors
			str = "Centering translations (along a,b,c axes)";
			AddStringToView(-1,str);
			for(int i=0;i<pDoc->m_pSpaceGroup->m_numberOfCenteringTranslations;i++)
				AddStringToView(-1,pDoc->m_pSpaceGroup->m_CenteringTranslations[i]);
			str="Coordinates";
			AddStringToView(-1,str);
// highlite the pushed button
			int buttons = pDoc->m_pSpaceGroup->m_numberOfWyckoffSets;
			CButton* btn;
			for(i=0;i<buttons;i++)
			{
				btn = (CButton*)this->GetDlgItem(900+i);
				btn->SetState(0);
			}
			btn = (CButton*)this->GetDlgItem(wID);
			btn->SetState(1);
// number of the item in the spacegroup class
			int nmb = pDoc->m_pSpaceGroup->m_numberOfWyckoffSets-(wID-900)-1;
			int start=0;
			for(i=0;i<nmb;i++)
// add previous sets multiplicities to find the start for the required Symbol in the
// m_WSymbol array
				start += pDoc->m_pSpaceGroup->m_WMultiplicity[i]/
					pDoc->m_pSpaceGroup->m_numberOfCenteringTranslations;
// multiplicity of the required item, divided by the number of centering translations
			int multp = pDoc->m_pSpaceGroup->m_WMultiplicity[nmb]/
						pDoc->m_pSpaceGroup->m_numberOfCenteringTranslations;
			for(i=0;i<multp;i++)
				AddStringToView(i+1,pDoc->m_pSpaceGroup->m_Coordinates[start+i]);

			pDoc->m_pAtomView->Invalidate();
			pDoc->m_pAtomView->UpdateWindow();
		}
	}
	return CWnd::OnCommand(wParam, lParam);
}

//////////////////////////////////////////////////////////////////
//
void AtomWindow::AddStringToView(int i,string str)
{
	CString cstr;
	if(i == -1) cstr.Format("%s",str.c_str());
	else
		cstr.Format("%d) %s",i,str.c_str());
	GetDoc()->m_pAtomView->m_text.push_back(cstr);
	return;
}

///////////////////////////////////////////////////////////////////
//
void AtomWindow::OnDestroy() 
{
	CWnd::OnDestroy();
	GetDoc()->RemoveView(GetDoc()->m_pAtomView);	
}
