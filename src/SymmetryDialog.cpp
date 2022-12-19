// SymmetryDialog.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "MadelungDoc.h"
#include "SymmetryDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMadelungDoc* g_pDoc;

/////////////////////////////////////////////////////////////////////////////
// SymmetryDialog dialog


SymmetryDialog::SymmetryDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SymmetryDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(SymmetryDialog)
	m_symmetryText = _T("");
	//}}AFX_DATA_INIT
	this->OK_ClickedTimes = 1;
}


void SymmetryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SymmetryDialog)
	DDX_Text(pDX, IDC_SYMMETRY_STATIC, m_symmetryText);
	DDV_MaxChars(pDX, m_symmetryText, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SymmetryDialog, CDialog)
	//{{AFX_MSG_MAP(SymmetryDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SymmetryDialog message handlers

void SymmetryDialog::OnOK() 
{
	if(this->OK_ClickedTimes == 2)
	{
		CDialog::OnOK();
		return;
	}
	g_pDoc->FindPointGroupForSelectedAtoms();
	CListCtrl* lc = (CListCtrl*)this->GetDlgItem(IDC_SYMMETRY_LIST);
	FILE* f = fopen("Symmetry.txt","r");
	char buffer[200];
	int item = 0;

	LV_COLUMN col;
	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	col.fmt = LVCFMT_LEFT;
	col.cx = 550;
	col.pszText = "Point group symmetry determination";
	lc->InsertColumn(0,&col);

	while(fgets(buffer,200,f))
	{
		int counter = 0;
		char* c = buffer;
		while(*c++)
			counter++;
		buffer[counter-1] = NULL;
		lc->InsertItem(item++,buffer);
	}
	fclose(f);
// REFINE SYMMETRY ELEMENTS (Select only those, consistent with Wyckoff site symmetry)
	g_pDoc->RefineSymmetryElements();
	sprintf(buffer,"%d symmetry elements refined",g_pDoc->m_numberOfRefinedSymmetryElements);
	lc->InsertItem(item++,buffer);
	for(int i=0;i<g_pDoc->m_numberOfRefinedSymmetryElements;i++)
	{
		SYMMETRY_ELEMENT* ic = g_pDoc->m_RefinedElements[i];
		string str;
		if(ic->transform_atom == invert_atom)
			str = "Inversion Center";
		else
			if(ic->transform_atom == rotate_atom)
				str = "Axis";
		else
			if(ic->transform_atom == mirror_atom)
				str = "Plane";
		else
			str = "Unknown";

		sprintf(buffer,"%s %d %d %lf %lf %lf %lf %lf %lf %lf %lf",str.c_str(),
				ic->order,ic->nparam,ic->maxdev,ic->distance,
				ic->normal[0],ic->normal[1],ic->normal[2],
				ic->direction[0],ic->direction[1],ic->direction[2]);
		lc->InsertItem(item++,buffer);
	}
	g_pDoc->m_bSymmetryElementsFound = true;
	OK_ClickedTimes = 2;
}
