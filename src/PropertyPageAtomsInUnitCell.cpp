// PropertyPageAtomsInUnitCell.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "PropertyPageAtomsInUnitCell.h"
#include "EditAtom.h"
#include "MadelungDoc.h"
#include "MainFrm.h"
#include "Spg_dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropertyPageAtomsInUnitCell property page

IMPLEMENT_DYNCREATE(PropertyPageAtomsInUnitCell, CPropertyPage)

PropertyPageAtomsInUnitCell::PropertyPageAtomsInUnitCell() : CPropertyPage(PropertyPageAtomsInUnitCell::IDD)
{
	//{{AFX_DATA_INIT(PropertyPageAtomsInUnitCell)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_NumberOfAtomsInUnitCell = 0;
	m_pImageListSmall = NULL;
// used to enable/disable menu item
	m_AtomCoordinatesAreReady = FALSE;
}

PropertyPageAtomsInUnitCell::~PropertyPageAtomsInUnitCell()
{
	if(m_pImageListSmall)
	{
		delete m_pImageListSmall;
		m_pImageListSmall = NULL;
	}
}

void PropertyPageAtomsInUnitCell::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropertyPageAtomsInUnitCell)
	DDX_Control(pDX, IDC_PP_ATOMSINUNITCELL_LISTCONTROL, m_atomsList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropertyPageAtomsInUnitCell, CPropertyPage)
	//{{AFX_MSG_MAP(PropertyPageAtomsInUnitCell)
	ON_BN_CLICKED(IDC_PP_ATOMSINUNITCELL_ADD, OnPpAtomsinunitcellAdd)
	ON_BN_CLICKED(IDC_PP_EDITATOMS_REMOVEATOM, OnPpEditatomsRemoveatom)
	ON_BN_CLICKED(IDC_PP_ATOMSINUNITCELL_EDIT, OnPpAtomsinunitcellEdit)
	ON_BN_CLICKED(IDC_PP_EDITATOMS_REMOVEALL, OnPpEditatomsRemoveall)
	ON_NOTIFY(NM_DBLCLK, IDC_PP_ATOMSINUNITCELL_LISTCONTROL, OnDblclkPpAtomsinunitcellListcontrol)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropertyPageAtomsInUnitCell message handlers

void PropertyPageAtomsInUnitCell::OnPpAtomsinunitcellAdd() 
{
// Wyckoff Position 'a' from the EditAtom constructor
	EditAtom editAtom(this->FromHandle(this->GetSafeHwnd()));
	int nResult = editAtom.DoModal();
	if(nResult == IDOK)
	{
		m_NumberOfAtomsInUnitCell++; // initial value in constructor is 0
		m_x.push_back(editAtom.m_x);
		m_y.push_back(editAtom.m_y);
		m_z.push_back(editAtom.m_z);
		m_charge.push_back(editAtom.m_charge);
		m_atomType.push_back(editAtom.m_atomType);
		m_WPosition.push_back(editAtom.m_WPosition);

		AddAtomToList();
		if(m_NumberOfAtomsInUnitCell)
		{
			EnableEditButton(TRUE);
			EnableRemoveButton(TRUE);
			EnableRemoveAllButton(TRUE);
			m_AtomCoordinatesAreReady = TRUE;
		}
	}
	else	// Cancel button pressed
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
//

void PropertyPageAtomsInUnitCell::AddAtomToList()
{
	LV_ITEM lvitem;
	CString cs;
	int iActualItem = 0;
	double xyz[3] = {0.,0.,0.};
// just to find out whether edit windows are enabled/disabled
// if disabled, set window text to zero.
// the following call sets m_pSpaceGroup->m_WIsEditable_XYZ
	GetDoc()->m_pSpaceGroup->GetEqPos(xyz,this->m_WPosition[m_NumberOfAtomsInUnitCell-1].GetAt(0));

	for (int iSubItem = 0; iSubItem < 6; iSubItem++)
	{
		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
		lvitem.iItem = (iSubItem == 0)? m_NumberOfAtomsInUnitCell-1 : iActualItem;
		lvitem.iSubItem = iSubItem;

		switch(iSubItem)
		{
		case 0:
// choose the icon and legend for the entry
			lvitem.iImage = this->m_atomType[m_NumberOfAtomsInUnitCell-1];
			cs.Format("%d",m_NumberOfAtomsInUnitCell);
			break;
		case 1:
			if(GetDoc()->m_pSpaceGroup->m_WIsEditable_X)
				cs.Format("%5.2lf",m_x[m_NumberOfAtomsInUnitCell-1]);
			else
				cs.Format("");
			break;
		case 2:
			if(GetDoc()->m_pSpaceGroup->m_WIsEditable_Y)
				cs.Format("%5.2lf",m_y[m_NumberOfAtomsInUnitCell-1]);
			else
				cs.Format("");
			break;
		case 3:
			if(GetDoc()->m_pSpaceGroup->m_WIsEditable_Z)
				cs.Format("%5.2lf",m_z[m_NumberOfAtomsInUnitCell-1]);
			else
				cs.Format("");
			break;
		case 4:
			cs.Format("%5.2lf",m_charge[m_NumberOfAtomsInUnitCell-1]);
			break;
		case 5:
			cs.Format("%s",m_WPosition[m_NumberOfAtomsInUnitCell-1]);
			break;

		}
		lvitem.pszText = cs.GetBuffer(20);
		if(iSubItem == 0) iActualItem = m_atomsList.InsertItem(&lvitem); // insert new item
		else
			m_atomsList.SetItem(&lvitem); // modify existing item (the sub-item text)
		cs.ReleaseBuffer();
// delete the EqPos object
		GetDoc()->m_pSpaceGroup->ClearEqPos();
	}
}

/////////////////////////////////////////////////////////////////////////////
//

BOOL PropertyPageAtomsInUnitCell::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	MakeListHeaders();
	m_atomsList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CMadelungApp *pApp = (CMadelungApp*)AfxGetApp();
	m_pImageListSmall = new CImageList();
	ASSERT(m_pImageListSmall != NULL);    // serious allocation failure checking
	m_pImageListSmall->Create(16, 16, TRUE, 4, 4);
	for(int i=135; i<211; i++) m_pImageListSmall->Add(pApp->LoadIcon(i));
	m_atomsList.SetImageList(m_pImageListSmall, LVSIL_SMALL);
	if(m_NumberOfAtomsInUnitCell)
	{
		EnableEditButton(TRUE);
		EnableRemoveButton(TRUE);
		EnableRemoveAllButton(TRUE);
		this->m_AtomCoordinatesAreReady = TRUE;
	}
	else
	{
		EnableEditButton(FALSE);
		EnableRemoveButton(FALSE);
		EnableRemoveAllButton(FALSE);
		this->m_AtomCoordinatesAreReady = FALSE;
	}

// fill the list box with atom ccordinates from pDoc

	FillAtomsList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
//

void PropertyPageAtomsInUnitCell::MakeListHeaders()
{

	LV_COLUMN col;
	col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	col.fmt = LVCFMT_CENTER;
	col.pszText = "Atom#";
	col.cx = 50;
	m_atomsList.InsertColumn(0,&col);
	col.pszText = "X";
	col.cx = 50;
	m_atomsList.InsertColumn(1,&col);
	col.pszText = "Y";
	col.cx = 50;
	m_atomsList.InsertColumn(2,&col);
	col.pszText = "Z";
	col.cx = 50;
	m_atomsList.InsertColumn(3,&col);
	col.pszText = "Charge";
	col.cx = 50;
	m_atomsList.InsertColumn(4,&col);
	col.pszText = "Wyckoff";
	col.cx = 60;
	m_atomsList.InsertColumn(5,&col);
}

///////////////////////////////////////////////////////////////////////////////
//

void PropertyPageAtomsInUnitCell::EnableEditButton(bool flag)
{
	CButton* bt = (CButton*)this->GetDlgItem(IDC_PP_ATOMSINUNITCELL_EDIT);
	bt->EnableWindow(flag);
}

///////////////////////////////////////////////////////////////////////////////
//

void PropertyPageAtomsInUnitCell::EnableRemoveButton(bool flag)
{
	CButton* bt = (CButton*)this->GetDlgItem(IDC_PP_EDITATOMS_REMOVEATOM);
	bt->EnableWindow(flag);
}

///////////////////////////////////////////////////////////////////////////////
//

void PropertyPageAtomsInUnitCell::EnableRemoveAllButton(bool flag)
{
	CButton* bt = (CButton*)this->GetDlgItem(IDC_PP_EDITATOMS_REMOVEALL);
	bt->EnableWindow(flag);
}

///////////////////////////////////////////////////////////////////////////////
//
void PropertyPageAtomsInUnitCell::OnPpEditatomsRemoveatom() 
{
	int nItem;
	
	CListCtrl* m_atomsList = (CListCtrl*)GetDlgItem(IDC_PP_ATOMSINUNITCELL_LISTCONTROL);
	ASSERT(m_atomsList != NULL);

	POSITION pos = m_atomsList->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("No items selected!\n");
		return;
	}
	else
		nItem = m_atomsList->GetNextSelectedItem(pos);
// remove the selected item from the list
	if(AfxMessageBox("Are You sure to remove the item?\n",MB_OKCANCEL) == IDCANCEL)
		return;

// now do actual removing of the item

	m_NumberOfAtomsInUnitCell--; // initial value in constructor is 0
	m_iterator_x = m_x.begin() + nItem;
	m_iterator_y = m_y.begin() + nItem;
	m_iterator_z = m_z.begin() + nItem;
	m_iterator_charge = m_charge.begin() + nItem;
	m_iterator_type = m_atomType.begin() + nItem;
	m_iterator_Wyckoff = m_WPosition.begin() + nItem;
	m_x.erase(m_iterator_x);
	m_y.erase(m_iterator_y);
	m_z.erase(m_iterator_z);
	m_charge.erase(m_iterator_charge);
	m_atomType.erase(m_iterator_type);
	m_WPosition.erase(m_iterator_Wyckoff);

	m_atomsList->DeleteItem(nItem);
	if(!m_NumberOfAtomsInUnitCell) 
	{
		EnableEditButton(FALSE);
		EnableRemoveButton(FALSE);
		EnableRemoveAllButton(FALSE);
		this->m_AtomCoordinatesAreReady = FALSE;
	}
	RenumberAllItems();
}


///////////////////////////////////////////////////////////////////////////////
//
void PropertyPageAtomsInUnitCell::OnPpAtomsinunitcellEdit() 
{
	int nItem;

	CListCtrl* m_atomsList = (CListCtrl*)GetDlgItem(IDC_PP_ATOMSINUNITCELL_LISTCONTROL);
	ASSERT(m_atomsList != NULL);

	nItem = m_atomsList->GetNextItem(-1,LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox("No items selected!",MB_OK);
		return;
	}
// invoke the edit box with selected item's values
	EditAtom editAtom(this->FromHandle(this->GetSafeHwnd()));
	editAtom.m_x = this->m_x[nItem];
	editAtom.m_y = this->m_y[nItem];
	editAtom.m_z = this->m_z[nItem];
	editAtom.m_charge = this->m_charge[nItem];
	editAtom.m_atomType = this->m_atomType[nItem];
	editAtom.m_WPosition = this->m_WPosition[nItem];

	int nResult = editAtom.DoModal();
	if(nResult == IDOK)
	{
			m_x[nItem]=editAtom.m_x;
			m_y[nItem]=editAtom.m_y;
			m_z[nItem]=editAtom.m_z;
			m_charge[nItem]=editAtom.m_charge;
			m_atomType[nItem]=editAtom.m_atomType;
			m_WPosition[nItem]=editAtom.m_WPosition;
		
			LV_ITEM lvitem;
			CString cs;
			int iActualItem = 0;
			double xyz[3] = {0.,0.,0.};
// just to find out whether edit windows are enabled/disabled
// if disabled, set window text to zero.
// the following call sets m_pSpaceGroup->m_WIsEditable_XYZ
			GetDoc()->m_pSpaceGroup->GetEqPos(xyz,this->m_WPosition[nItem].GetAt(0));
	
			for(int iSubItem = 0; iSubItem < 6; iSubItem++)
			{
				lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
				lvitem.iItem = nItem;
				lvitem.iSubItem = iSubItem;

				switch(iSubItem)
				{	
				case 0:
// choose the icon and legend for the entry
					lvitem.iImage = editAtom.m_atomType;
					cs.Format("%d",nItem+1);
					break;
				case 1:
					if(GetDoc()->m_pSpaceGroup->m_WIsEditable_X)
						cs.Format("%5.2lf",m_x[nItem]);
					else
						cs.Format("");
					break;
				case 2:
					if(GetDoc()->m_pSpaceGroup->m_WIsEditable_Y)
						cs.Format("%5.2lf",m_y[nItem]);
					else
						cs.Format("");
					break;
				case 3:
					if(GetDoc()->m_pSpaceGroup->m_WIsEditable_Z)
						cs.Format("%5.2lf",m_z[nItem]);
					else
						cs.Format("");
					break;
				case 4:
					cs.Format("%5.2lf",m_charge[nItem]);
					break;
				case 5:
					cs.Format("%s",m_WPosition[nItem]);
					break;
				}
				lvitem.pszText = cs.GetBuffer(20);
				m_atomsList->SetItem(&lvitem); // modify existing item (the sub-item text)
				cs.ReleaseBuffer();
			}
			m_atomsList->Update(nItem);
// delete the EqPos object
			GetDoc()->m_pSpaceGroup->ClearEqPos();
	}
	else	// Cancel button pressed
	{
	}

}

////////////////////////////////////////////////////////////////////////////
//

void PropertyPageAtomsInUnitCell::RenumberAllItems()
{
	LV_ITEM lvitem;
	CString cs;

	for (int nItem = 0; nItem < m_NumberOfAtomsInUnitCell; nItem++)
	{
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = nItem;
		lvitem.iSubItem = 0;
		cs.Format("%d",nItem+1);
		lvitem.pszText = cs.GetBuffer(20);
		m_atomsList.SetItem(&lvitem); // modify existing item (the sub-item text)
		cs.ReleaseBuffer();
	}
}

////////////////////////////////////////////////////////////////////////////
//

void PropertyPageAtomsInUnitCell::FillAtomsList()
{
	LV_ITEM lvitem;
	CString cs;
	int iActualItem = 0;
// m_NumberOfAtomsInUnitCell is set to 0 in constructor and in MainFrm.cpp
	for(int iItem=0; iItem<m_NumberOfAtomsInUnitCell;iItem++)
	{
// find out which columns to fill according to Wyckoff Position
		double xyz[3] = {0.,0.,0.};
// just to find out whether edit windows are enabled/disabled
// if disabled, set window text to zero.
// the following call sets m_pSpaceGroup->m_WIsEditable_XYZ
		GetDoc()->m_pSpaceGroup->GetEqPos(xyz,this->m_WPosition[iItem].GetAt(0));
// for edit_X box

		for (int iSubItem = 0; iSubItem < 6; iSubItem++)
		{
			lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
			lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
			lvitem.iSubItem = iSubItem;
	
			switch(iSubItem)
			{
			case 0:
// choose the icon and legend for the entry
				lvitem.iImage = this->m_atomType[iItem];
				cs.Format("%d",iItem+1);
				break;
			case 1:
				if(GetDoc()->m_pSpaceGroup->m_WIsEditable_X)
					cs.Format("%5.2lf",m_x[iItem]);
				else
					cs.Format("");
				break;
			case 2:
				if(GetDoc()->m_pSpaceGroup->m_WIsEditable_Y)
					cs.Format("%5.2lf",m_y[iItem]);
				else
					cs.Format("");
				break;
			case 3:
				if(GetDoc()->m_pSpaceGroup->m_WIsEditable_Z)
					cs.Format("%5.2lf",m_z[iItem]);
				else
					cs.Format("");
				break;
			case 4:
				cs.Format("%5.2lf",m_charge[iItem]);
				break;
			case 5:
				cs.Format("%s",m_WPosition[iItem]);
				break;
			}
			lvitem.pszText = cs.GetBuffer(20);
			if(iSubItem == 0) iActualItem = m_atomsList.InsertItem(&lvitem); // insert new item
			else
				m_atomsList.SetItem(&lvitem); // modify existing item (the sub-item text)
			cs.ReleaseBuffer();
		}
// clear the EqPos object, created in GetDoc()->m_pSpaceGroup->GetEqPos()
		GetDoc()->m_pSpaceGroup->ClearEqPos();
	}
}
///////////////////////////////////////////////////////////////////
//
CMadelungDoc* PropertyPageAtomsInUnitCell::GetDoc()
{
	CMadelungApp* pApp = (CMadelungApp*)AfxGetApp();
	CWnd* wnd = pApp->m_pMainWnd;
	ASSERT(wnd->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CMainFrame* pFr = (CMainFrame*)wnd;
	CMadelungDoc* pDoc = (CMadelungDoc*) pFr->GetActiveDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CMadelungDoc)));
	return pDoc;
}

////////////////////////////////////////////////////////////////////
//
void PropertyPageAtomsInUnitCell::OnPpEditatomsRemoveall() 
{
	CListCtrl* m_atomsList = (CListCtrl*)GetDlgItem(IDC_PP_ATOMSINUNITCELL_LISTCONTROL);
	ASSERT(m_atomsList != NULL);
	if(AfxMessageBox("Are You sure to remove all items?\n",MB_OKCANCEL) == IDCANCEL)
		return;

	m_atomsList->DeleteAllItems();

// remove all items from the list
	this->Clear();

	EnableEditButton(FALSE);
	EnableRemoveButton(FALSE);
	EnableRemoveAllButton(FALSE);
	this->m_AtomCoordinatesAreReady = FALSE;
}

//////////////////////////////////////////////////////////////////////////
//
void PropertyPageAtomsInUnitCell::OnDblclkPpAtomsinunitcellListcontrol(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_NumberOfAtomsInUnitCell)
		this->OnPpAtomsinunitcellEdit();
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////
//
BOOL PropertyPageAtomsInUnitCell::OnSetActive() 
{
	if(!GetDoc()->m_pSpaceGroup)
	{
		AfxMessageBox("Load a Space group first!!!",MB_OK);
		CWnd* pDlgSheet = this->GetParent();
		ASSERT(pDlgSheet->IsKindOf(RUNTIME_CLASS(CPropertySheet)));
		CPropertySheet* ps = DYNAMIC_DOWNCAST(CPropertySheet,pDlgSheet);
		ps->SetActivePage(0);
		return 0;
	}
	return CPropertyPage::OnSetActive();
}

///////////////////////////////////////////////////////////////////////////
// clears all items from m_atomsList
//
void PropertyPageAtomsInUnitCell::Clear()
{
	m_NumberOfAtomsInUnitCell=0; // initial value in constructor is 0
	m_x.clear();
	m_y.clear();
	m_z.clear();
	m_charge.clear();
	m_atomType.clear();
	m_WPosition.clear();
}
