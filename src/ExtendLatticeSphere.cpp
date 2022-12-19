// ExtendLatticeSphere.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "ExtendLatticeSphere.h"
#include "MadelungDoc.h"
#include "MadelungView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMadelungDoc* g_pDoc;

/////////////////////////////////////////////////////////////////////////////
// ExtendLatticeSphere dialog


ExtendLatticeSphere::ExtendLatticeSphere(CWnd* pParent /*=NULL*/)
	: CDialog(ExtendLatticeSphere::IDD, pParent)
{
	//{{AFX_DATA_INIT(ExtendLatticeSphere)
	m_radius = 0;
	m_bRenderExtension = FALSE;
	m_bSymmetry = FALSE;
	//}}AFX_DATA_INIT
}


void ExtendLatticeSphere::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ExtendLatticeSphere)
	DDX_Text(pDX, IDC_EDIT_SPHERE_RADIUS, m_radius);
	DDV_MinMaxInt(pDX, m_radius, 0, 1000000);
	DDX_Check(pDX, IDC_EXTEND_SPHERE_CHECK, m_bRenderExtension);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ExtendLatticeSphere, CDialog)
	//{{AFX_MSG_MAP(ExtendLatticeSphere)
	ON_BN_CLICKED(IDC_BUTTON_SPHERE_OK, OnButtonSphereOk)
	ON_BN_CLICKED(IDC_BUTTON_SPHERE_CANCEL, OnButtonSphereCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ExtendLatticeSphere message handlers

BOOL ExtendLatticeSphere::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CSpinButtonCtrl* spin;
	spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_SPHERE_RADIUS);
	CEdit* ed = (CEdit*)GetDlgItem(IDC_EDIT_SPHERE_RADIUS);
	spin->SetBuddy(ed);
	spin->SetRange(1,1000000);

// set the render extension checkbox	
	POSITION pos = g_pDoc->GetFirstViewPosition();
	CMadelungView* pV = (CMadelungView*)g_pDoc->GetNextView(pos);
	ASSERT(pV->IsKindOf(RUNTIME_CLASS(CMadelungView)));

	CButton* chk;
	chk = (CButton*)GetDlgItem(IDC_EXTEND_SPHERE_CHECK);
	chk->SetCheck(pV->m_bRenderExtension);

	CStatic* st;
	st = (CStatic*)GetDlgItem(IDC_STATIC_SPHERE_SYMMETRY);
	if(g_pDoc->m_bUseSymmetry)
		st->SetWindowText("Symmetry will be used");
	else
		st->SetWindowText("No symmetry will be used");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ExtendLatticeSphere::OnButtonSphereOk() 
{
	CDialog::OnOK();
}

void ExtendLatticeSphere::OnButtonSphereCancel() 
{
	CDialog::OnCancel();	
}
