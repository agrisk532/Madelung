// MoleculeList.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "MadelungDoc.h"
#include "Molecule.h"
#include "MoleculeList.h"
#include "EditMolecule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMadelungDoc* g_pDoc;

/////////////////////////////////////////////////////////////////////////////
// MoleculeList dialog


MoleculeList::MoleculeList(CWnd* pParent /*=NULL*/)
	: CDialog(MoleculeList::IDD, pParent)
{
	//{{AFX_DATA_INIT(MoleculeList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	this->m_selectedMolecule = -1;
}


void MoleculeList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MoleculeList)
	DDX_Control(pDX, IDC_EDITMOLECULE_LISTCONTROL, m_moleculesList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MoleculeList, CDialog)
	//{{AFX_MSG_MAP(MoleculeList)
	ON_BN_CLICKED(IDC_MOLECULES_NEW, OnMoleculesNew)
	ON_BN_CLICKED(IDC_MOLECULES_ADD, OnMoleculesAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MoleculeList message handlers

BOOL MoleculeList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	MakeListHeaders();
	m_moleculesList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	if(g_pDoc->m_molecules.size() != 0)
	{
		EnableEditButton(TRUE);
		EnableRemoveButton(TRUE);
		EnableRemoveAllButton(TRUE);
		FillMoleculesList();
	}
	else
	{
		EnableEditButton(FALSE);
		EnableRemoveButton(FALSE);
		EnableRemoveAllButton(FALSE);
	}

// fill the list box with molecule ccordinates from g_pDoc


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
//
void MoleculeList::MakeListHeaders()
{
	LV_COLUMN col;
	col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	col.fmt = LVCFMT_CENTER;

	col.pszText = "molecule#";
	col.cx = 75;
	m_moleculesList.InsertColumn(0,&col);
	col.pszText = "name";
	col.cx = 150;
	m_moleculesList.InsertColumn(1,&col);

	col.pszText = "X";
	col.cx = 50;
	m_moleculesList.InsertColumn(2,&col);
	col.pszText = "Y";
	col.cx = 50;
	m_moleculesList.InsertColumn(3,&col);
	col.pszText = "Z";
	col.cx = 50;
	m_moleculesList.InsertColumn(4,&col);
	col.pszText = "alpha";
	col.cx = 50;
//	m_moleculesList.InsertColumn(5,&col);
	col.pszText = "beta";
	col.cx = 50;
//	m_moleculesList.InsertColumn(6,&col);
	col.pszText = "gamma";
	col.cx = 50;
//	m_moleculesList.InsertColumn(7,&col);
}
///////////////////////////////////////////////////////////////////////////////
//

void MoleculeList::EnableEditButton(bool flag)
{
	CButton* bt = (CButton*)this->GetDlgItem(IDC_MOLECULES_EDIT);
	bt->EnableWindow(flag);
}

///////////////////////////////////////////////////////////////////////////////
//

void MoleculeList::EnableRemoveButton(bool flag)
{
	CButton* bt = (CButton*)this->GetDlgItem(IDC_MOLECULES_REMOVE);
	bt->EnableWindow(flag);
}

///////////////////////////////////////////////////////////////////////////////
//

void MoleculeList::EnableRemoveAllButton(bool flag)
{
	CButton* bt = (CButton*)this->GetDlgItem(IDC_MOLECULES_REMOVEALL);
	bt->EnableWindow(flag);
}
///////////////////////////////////////////////////////////////////////////////
//
void MoleculeList::FillMoleculesList()
{
	LV_ITEM lvitem;
	CString cs;
	int iActualItem = 0;
// number of molecules, which has been created and can be inserted in the lattice
	int size = g_pDoc->m_molecules.size();
	if(size != 0)
	{
		for(int iItem=0; iItem<size;iItem++)
		{
			double center[3];
// molecule orientation not implemented at this stage. Molecule coordinates are read in the
// cartesian format, only center positions are specified.
//			double orientation[3];
			g_pDoc->m_molecules[iItem]->GetCenter(center);
//			g_pDoc->m_molecules[iItem]->GetOrientation(orientation);

//			for(int iSubItem = 0; iSubItem < 8; iSubItem++)
			for(int iSubItem = 0; iSubItem < 5; iSubItem++)
			{
//				lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
				lvitem.mask = LVIF_TEXT;
				lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
				lvitem.iSubItem = iSubItem;

				switch(iSubItem)
				{
				case 0:
					cs.Format("%d",iItem+1);
					break;
				case 1:
					cs.Format("%5.2lf",g_pDoc->m_molecules[iItem]->GetName());
					break;
				case 2:
					cs.Format("%5.2lf",center[0]);
					break;
				case 3:
					cs.Format("%5.2lf",center[1]);
					break;
				case 4:
					cs.Format("%5.2lf",center[2]);
					break;
				case 5:
//					cs.Format("%5.2lf",orientation[0]);
					break;
				case 6:
//					cs.Format("%5.2lf",orientation[1]);
					break;
				case 7:
//					cs.Format("%5.2lf",orientation[2]);
					break;
				}
				lvitem.pszText = cs.GetBuffer(20);
				if(iSubItem == 0) iActualItem = m_moleculesList.InsertItem(&lvitem); // insert new item
				else
					m_moleculesList.SetItem(&lvitem); // modify existing item (the sub-item text)
				cs.ReleaseBuffer();
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////
//
void MoleculeList::OnMoleculesNew() 
{
	EditMolecule em;
	em.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////
//
void MoleculeList::OnMoleculesAdd() 
{
	int nItem;

	CListCtrl* m_moleculesList = (CListCtrl*)GetDlgItem(IDC_EDITMOLECULE_LISTCONTROL);
	ASSERT(m_moleculesList != NULL);

	nItem = m_moleculesList->GetNextItem(-1,LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox("No items selected!",MB_OK);
		return;
	}
	else
	{
		g_pDoc->m_currentMolecule = nItem;
		this->m_selectedMolecule = nItem;
		OnOK();
	}
}
///////////////////////////////////////////////////////////////////////////////////
//
