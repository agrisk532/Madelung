// ExtendLattice.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "ExtendLattice.h"
#include "MadelungDoc.h"
#include "MadelungView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMadelungDoc* g_pDoc;	// pointer to the MadelungDoc

/////////////////////////////////////////////////////////////////////////////
// ExtendLattice dialog


ExtendLattice::ExtendLattice(CWnd* pParent /*=NULL*/)
	: CDialog(ExtendLattice::IDD, pParent)
{
	//{{AFX_DATA_INIT(ExtendLattice)
	m_na = 0;
	m_nb = 0;
	m_nc = 0;
	m_bRenderExtension = FALSE;
	m_bSymmetry = FALSE;
	//}}AFX_DATA_INIT
}


void ExtendLattice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ExtendLattice)
	DDX_Text(pDX, IDC_EDIT_EXTEND_A, m_na);
	DDV_MinMaxInt(pDX, m_na, 0, 1000000);
	DDX_Text(pDX, IDC_EDIT_EXTEND_B, m_nb);
	DDV_MinMaxInt(pDX, m_nb, 0, 1000000);
	DDX_Text(pDX, IDC_EDIT_EXTEND_C, m_nc);
	DDV_MinMaxInt(pDX, m_nc, 0, 1000000);
	DDX_Check(pDX, IDC_EXTEND_PARAL_CHECK, m_bRenderExtension);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ExtendLattice, CDialog)
	//{{AFX_MSG_MAP(ExtendLattice)
	ON_BN_CLICKED(IDC_BUTTON_EXTEND_OK, OnButtonExtendOk)
	ON_BN_CLICKED(IDC_BUTTON_EXTEND_CANCEL, OnButtonExtendCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ExtendLattice message handlers
//////////////////////////////////////////////////////////////////////////

BOOL ExtendLattice::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CSpinButtonCtrl* spin;
	CEdit* ed;
	spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_EXTEND_A);
	ed = (CEdit*)GetDlgItem(IDC_EDIT_EXTEND_A);
	spin->SetBuddy(ed);
	spin->SetRange(1,1000000);

	spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_EXTEND_B);
	ed = (CEdit*)GetDlgItem(IDC_EDIT_EXTEND_B);
	spin->SetBuddy(ed);
	spin->SetRange(1,1000000);

	spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_EXTEND_C);
	ed = (CEdit*)GetDlgItem(IDC_EDIT_EXTEND_C);
	spin->SetBuddy(ed);
	spin->SetRange(1,1000000);

	POSITION pos = g_pDoc->GetFirstViewPosition();
	CMadelungView* pV = (CMadelungView*)g_pDoc->GetNextView(pos);
	ASSERT(pV->IsKindOf(RUNTIME_CLASS(CMadelungView)));

	CButton* chk;
	chk = (CButton*)GetDlgItem(IDC_EXTEND_PARAL_CHECK);
	chk->SetCheck(pV->m_bRenderExtension);

	CStatic* st;
	st = (CStatic*)GetDlgItem(IDC_STATIC_PARAL_SYMMETRY);
	if(g_pDoc->m_bUseSymmetry)
		st->SetWindowText("Symmetry will be used");
	else
		st->SetWindowText("No symmetry will be used");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void ExtendLattice::OnButtonExtendOk() 
{
	CDialog::OnOK();
}

void ExtendLattice::OnButtonExtendCancel() 
{
	CDialog::OnCancel();
}
