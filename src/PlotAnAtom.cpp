// PlotAnAtom.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "PlotAnAtom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PlotAnAtom dialog


PlotAnAtom::PlotAnAtom(CWnd* pParent /*=NULL*/)
	: CDialog(PlotAnAtom::IDD, pParent)
{
	//{{AFX_DATA_INIT(PlotAnAtom)
	m_atom = 0;
	//}}AFX_DATA_INIT
}


void PlotAnAtom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PlotAnAtom)
	DDX_Text(pDX, IDC_ADD_ATOM_EDIT, m_atom);
	DDV_MinMaxInt(pDX, m_atom, 0, 2147483647);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PlotAnAtom, CDialog)
	//{{AFX_MSG_MAP(PlotAnAtom)
	ON_COMMAND(ID_PLOT_ATOM_REMOVE, OnDeleteAtom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PlotAnAtom message handlers

void PlotAnAtom::OnDeleteAtom() 
{
	// TODO: Add your command handler code here
	
}
