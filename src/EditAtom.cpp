// EditAtom.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "MadelungDoc.h"
#include "MainFrm.h"
#include "EditAtom.h"
#include "Spg_dll.h"
#include "AtomWindow.h"
#include "Molecule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMadelungDoc* g_pDoc;
/////////////////////////////////////////////////////////////////////////////
// EditAtom dialog


EditAtom::EditAtom(CWnd* pParent /*=NULL*/)
	: CDialog(EditAtom::IDD, pParent)
{
	//{{AFX_DATA_INIT(EditAtom)
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	m_charge = 0.0;
	m_atomType = 0;
	m_WPosition = _T("a");
	//}}AFX_DATA_INIT
	m_pAW = NULL;
}

EditAtom::~EditAtom()
{
	if(m_pAW) delete m_pAW;
}


void EditAtom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditAtom)
	DDX_Text(pDX, IDC_EDITATOMS_X, m_x);
	DDX_Text(pDX, IDC_EDITATOMS_Y, m_y);
	DDX_Text(pDX, IDC_EDITATOMS_Z, m_z);
	DDX_Text(pDX, IDC_EDITATOMS_CHARGE, m_charge);
	DDX_Radio(pDX, IDC_RADIO_H, m_atomType);
	DDX_LBString(pDX, IDC_LIST_EDITATOM_WYCK, m_WPosition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EditAtom, CDialog)
	//{{AFX_MSG_MAP(EditAtom)
	ON_BN_CLICKED(IDC_EDITATOM_BUTTON_OK, OnEditatomButtonOk)
	ON_BN_CLICKED(IDC_EDITATOM_BUTTON_CANCEL, OnEditatomButtonCancel)
	ON_BN_CLICKED(IDC_EDITATOM_VIEWCOORDINATES, OnEditatomViewcoordinates)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LIST_EDITATOM_WYCK, OnSelchangeListEditatomWyck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditAtom message handlers


void EditAtom::OnEditatomButtonOk() 
{
	OnOK();
	
}

void EditAtom::OnEditatomButtonCancel() 
{
	OnCancel();
	
}

////////////////////////////////////////////////////////////////////////
//	assumes that a SpaceGroup has been loaded

BOOL EditAtom::OnInitDialog() 
{
	CDialog::OnInitDialog();
// add Wyckoff symbols to the listbox
	CListBox* lb = (CListBox*)this->GetDlgItem(IDC_LIST_EDITATOM_WYCK);
//	lb->SetCurSel(0);	// top item selected

	int nmb = GetDoc()->m_pSpaceGroup->m_numberOfWyckoffSets;
	for(int i=0;i<nmb;i++)
	{
// listbox must be sorted alphabetically
		lb->AddString(CString(GetDoc()->m_pSpaceGroup->m_WSymbol[nmb-i-1]));
	}
// select the first listbox item
// m_WPosition is set in the PropertyPageAtomsInUnitCell
	char Wsymbol = this->m_WPosition.GetAt(0);	// get the Wyckoff symbol as the first symbol
	int lbpos = Wsymbol-97;	// 0-based position in the list box
	this->LoadWyckoffDescription(lbpos);	// loads text description 
	lb->SetCurSel(lbpos);
	this->EnableDisableEditXYZ(Wsymbol);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////////////////////////////////
//
CMadelungDoc* EditAtom::GetDoc()
{
	CMadelungApp* pApp = (CMadelungApp*)AfxGetApp();
	CWnd* wnd = pApp->m_pMainWnd;
	ASSERT(wnd->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CMainFrame* pFr = (CMainFrame*)wnd;
	CMadelungDoc* pDoc = (CMadelungDoc*) pFr->GetActiveDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CMadelungDoc)));
	return pDoc;
}

////////////////////////////////////////////////////////////////////////////
//
void EditAtom::OnEditatomViewcoordinates() 
{
	CListBox* lb = (CListBox*)this->GetDlgItem(IDC_LIST_EDITATOM_WYCK);
	int selectedItem = lb->GetCurSel();
// now display all wyckoff sets with symbolic coordinates
// construct the AtomWindow object
	if(m_pAW)
	{
		delete m_pAW;
		m_pAW = NULL;
	}
	m_pAW = new AtomWindow();
	m_pAW->CreateEx(NULL,m_pAW->m_plpszAtomWindowClass,"AtomCoordinates",
				WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,100,100,600,500,NULL,NULL,NULL);
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
	WPARAM wParam = BN_CLICKED | (900+selectedItem);
	m_pAW->SendMessage(WM_COMMAND,wParam,0);
}

////////////////////////////////////////////////////////////////////////////
//

void EditAtom::OnDestroy() 
{
	CDialog::OnDestroy();
	if(m_pAW)
	{
		m_pAW->DestroyWindow();
		delete m_pAW;
		m_pAW = NULL;
		GetDoc()->m_pAtomView = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////
// loads the description of the Wyckoff symbol 'pos' to the listbox
// positions in the listbox are created in increasing order (a,b,c,d,...)

void EditAtom::LoadWyckoffDescription(int pos)
{
	CMadelungDoc* pDoc = GetDoc();
	if(pDoc->m_pSpaceGroup)
	{
		char buffer[20];
		CStatic* cs = (CStatic*)this->GetDlgItem(IDC_STATIC_EDITATOM_WYMULT);
		int posMax = pDoc->m_pSpaceGroup->m_numberOfWyckoffSets-1;	// zero based index
		int value = pDoc->m_pSpaceGroup->m_WMultiplicity[posMax-pos];
		cs->SetWindowText(itoa(value,buffer,10));
		cs = (CStatic*)this->GetDlgItem(IDC_STATIC_EDITATOM_WYSYMM);
		cs->SetWindowText(pDoc->m_pSpaceGroup->m_WSymmetry[posMax-pos].c_str());
	}
	else
	{
		CStatic* cs = (CStatic*)this->GetDlgItem(IDC_STATIC_EDITATOM_WYMULT);
		cs->SetWindowText("N/A");
		cs = (CStatic*)this->GetDlgItem(IDC_STATIC_EDITATOM_WYSYMM);
		cs->SetWindowText("N/A");
	}
}

////////////////////////////////////////////////////////////////////////////
//

void EditAtom::OnSelchangeListEditatomWyck() 
{
	CListBox* lb = (CListBox*)this->GetDlgItem(IDC_LIST_EDITATOM_WYCK);
	int pos = lb->GetCurSel();	// zero based index
	char symbol;
	if(pos>25)
		symbol = pos + 39;	// ascii 'A' symbol
	else
		symbol = pos + 97;	// ascii 'a' symbol
	LoadWyckoffDescription(pos);
	this->EnableDisableEditXYZ(symbol);
}

//////////////////////////////////////////////////////////////////////////////
//
void EditAtom::EnableDisableEditXYZ(char Wsymbol)
{
	CMadelungDoc* pDoc = GetDoc();
	CEdit* edit;
	bool ena;

	if(pDoc->m_pSpaceGroup)
	{
		double xyz[3] = {0.,0.,0.};
// just to find out whether edit windows are enabled/disabled
// if disabled, set window text to zero.
		pDoc->m_pSpaceGroup->GetEqPos(xyz,Wsymbol);
// for edit_X box
		edit = (CEdit*)this->GetDlgItem(IDC_EDITATOMS_X);
		ena = pDoc->m_pSpaceGroup->m_WIsEditable_X;
// if editing not enabled, set to zero
		if(!ena)
		{
			this->m_x = 0.;
			edit->SetWindowText("0");
		}
		edit->EnableWindow(ena);
// for edit_Y box
		edit = (CEdit*)this->GetDlgItem(IDC_EDITATOMS_Y);
		ena = GetDoc()->m_pSpaceGroup->m_WIsEditable_Y;
// if editing not enabled, set to zero
		if(!ena)
		{
			this->m_y = 0.;
			edit->SetWindowText("0");
		}
		edit->EnableWindow(ena);
// for edit_Z box
		edit = (CEdit*)this->GetDlgItem(IDC_EDITATOMS_Z);
		ena	= GetDoc()->m_pSpaceGroup->m_WIsEditable_Z;
// if editing not enabled, set to zero
		if(!ena)
		{
			this->m_z = 0.;
			edit->SetWindowText("0");
		}
		edit->EnableWindow(ena);
// clear the EqPos object
		GetDoc()->m_pSpaceGroup->ClearEqPos();
	}
	else
	{
		edit = (CEdit*)this->GetDlgItem(IDC_EDITATOMS_X);
			this->m_x = 0.;
			edit->SetWindowText("0");
			edit->EnableWindow(FALSE);
// for edit_Y box
		edit = (CEdit*)this->GetDlgItem(IDC_EDITATOMS_Y);
			this->m_y = 0.;
			edit->SetWindowText("0");
			edit->EnableWindow(FALSE);
// for edit_Z box
		edit = (CEdit*)this->GetDlgItem(IDC_EDITATOMS_Z);
			this->m_z = 0.;
			edit->SetWindowText("0");
			edit->EnableWindow(FALSE);
	}
}
