// PeriodicTable.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "PeriodicTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PeriodicTable dialog


PeriodicTable::PeriodicTable(CWnd* pParent /*=NULL*/)
	: CDialog(PeriodicTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(PeriodicTable)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void PeriodicTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PeriodicTable)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PeriodicTable, CDialog)
	//{{AFX_MSG_MAP(PeriodicTable)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PeriodicTable message handlers
