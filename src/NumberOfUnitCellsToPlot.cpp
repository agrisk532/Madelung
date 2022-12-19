// NumberOfUnitCellsToPlot.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "NumberOfUnitCellsToPlot.h"
#include "MadelungDoc.h"
#include "MainFrm.h"
#include "UnitCell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NumberOfUnitCellsToPlot dialog


NumberOfUnitCellsToPlot::NumberOfUnitCellsToPlot(CWnd* pParent /*=NULL*/)
	: CDialog(NumberOfUnitCellsToPlot::IDD, pParent)
{
	//{{AFX_DATA_INIT(NumberOfUnitCellsToPlot)
	m_a = 1;
	m_b = 1;
	m_c = 1;
	//}}AFX_DATA_INIT
}


void NumberOfUnitCellsToPlot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NumberOfUnitCellsToPlot)
	DDX_Text(pDX, IDC_EDIT_UNITCELLS_A, m_a);
	DDV_MinMaxInt(pDX, m_a, 1, 1000000);
	DDX_Text(pDX, IDC_EDIT_UNITCELLS_B, m_b);
	DDV_MinMaxInt(pDX, m_b, 1, 1000000);
	DDX_Text(pDX, IDC_EDIT_UNITCELLS_C, m_c);
	DDV_MinMaxInt(pDX, m_c, 1, 1000000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NumberOfUnitCellsToPlot, CDialog)
	//{{AFX_MSG_MAP(NumberOfUnitCellsToPlot)
	ON_BN_CLICKED(IDC_BUTTON_UNITCELLS_OK, OnButtonUnitcellsOk)
	ON_BN_CLICKED(IDC_BUTTON_UNITCELLS_CANCEL, OnButtonUnitcellsCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NumberOfUnitCellsToPlot message handlers

void NumberOfUnitCellsToPlot::OnButtonUnitcellsOk() 
{
	OnOK();
}

void NumberOfUnitCellsToPlot::OnButtonUnitcellsCancel() 
{
	OnCancel();	
}

//////////////////////////////////////////////////////////////////////////
//
BOOL NumberOfUnitCellsToPlot::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CMadelungDoc* pDoc = GetDoc();
	CSpinButtonCtrl* spin;
	CWnd* ed;
	CString str;
	int na = pDoc->m_NumberOfUnitCellsForPlotAlongA;
	int nb = pDoc->m_NumberOfUnitCellsForPlotAlongB;
	int nc = pDoc->m_NumberOfUnitCellsForPlotAlongC;
	spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_UNITCELLS_A);
	ed = GetDlgItem(IDC_EDIT_UNITCELLS_A);
	spin->SetBuddy(ed);
	spin->SetRange(1,1000000);
	str.Format("%d",na);
	ed->SetWindowText(str);
	spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_UNITCELLS_B);
	ed = GetDlgItem(IDC_EDIT_UNITCELLS_B);
	spin->SetBuddy(ed);
	spin->SetRange(1,1000000);
	str.Format("%d",nb);
	ed->SetWindowText(str);
	spin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_UNITCELLS_C);
	ed = GetDlgItem(IDC_EDIT_UNITCELLS_C);
	spin->SetBuddy(ed);
	spin->SetRange(1,1000000);
	str.Format("%d",nc);
	ed->SetWindowText(str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////
//
CMadelungDoc* NumberOfUnitCellsToPlot::GetDoc()
{
	CMadelungApp* pApp = (CMadelungApp*)AfxGetApp();
	ASSERT_VALID(pApp);
	CFrameWnd* pw = (CFrameWnd*)pApp->m_pMainWnd;
	ASSERT_VALID(pw);
	CMadelungDoc* pDoc = (CMadelungDoc*)pw->GetActiveDocument();
	ASSERT_VALID(pDoc);
	return pDoc;
}
