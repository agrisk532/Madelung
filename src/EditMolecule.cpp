// EditMolecule.cpp : implementation file
//
#include "stdafx.h"
#include "madelung.h"
#include "MadelungDoc.h"
#include "EditAtomsInMolecule.h"
#include "EditMolecule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMadelungDoc* g_pDoc;

/////////////////////////////////////////////////////////////////////////////
// EditMolecule dialog

EditMolecule::EditMolecule(CWnd* pParent /*=NULL*/)
	: CDialog(EditMolecule::IDD, pParent)
{
	//{{AFX_DATA_INIT(EditMolecule)
	m_alpha = 0.0;
	m_beta = 0.0;
	m_gamma = 0.0;
	m_name = _T("");
	m_X = 0.0;
	m_Y = 0.0;
	m_Z = 0.0;
	//}}AFX_DATA_INIT

	m_pImageListSmall = NULL;
}

EditMolecule::~EditMolecule()
{
	if(m_pImageListSmall)
	{
		delete m_pImageListSmall;
		m_pImageListSmall = NULL;
	}
}
//////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditMolecule)
	DDX_Control(pDX, IDC_EDITMOLECULE_LISTCONTROL, m_atomsList);
	DDX_Text(pDX, IDC_EDITMOLECULE_ALPHA, m_alpha);
	DDV_MinMaxDouble(pDX, m_alpha, -360., 360.);
	DDX_Text(pDX, IDC_EDITMOLECULE_BETA, m_beta);
	DDV_MinMaxDouble(pDX, m_beta, -360., 360.);
	DDX_Text(pDX, IDC_EDITMOLECULE_GAMMA, m_gamma);
	DDV_MinMaxDouble(pDX, m_gamma, -360., 360.);
	DDX_Text(pDX, IDC_EDITMOLECULE_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 256);
	DDX_Text(pDX, IDC_EDITMOLECULE_X, m_X);
	DDV_MinMaxDouble(pDX, m_X, -1000., 1000.);
	DDX_Text(pDX, IDC_EDITMOLECULE_Y, m_Y);
	DDV_MinMaxDouble(pDX, m_Y, -1000., 1000.);
	DDX_Text(pDX, IDC_EDITMOLECULE_Z, m_Z);
	DDV_MinMaxDouble(pDX, m_Z, -1000., 1000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EditMolecule, CDialog)
	//{{AFX_MSG_MAP(EditMolecule)
	ON_BN_CLICKED(IDC_EDITMOLECULE_ADD, OnEditmoleculeAdd)
	ON_BN_CLICKED(IDC_EDITMOLECULE_EDIT, OnEditmoleculeEdit)
	ON_BN_CLICKED(IDC_EDITMOLECULE_REMOVEATOM, OnEditmoleculeRemoveatom)
	ON_BN_CLICKED(IDC_EDITMOLECULE_REMOVEALL, OnEditmoleculeRemoveall)
	ON_NOTIFY(NM_DBLCLK, IDC_EDITMOLECULE_LISTCONTROL, OnDblclkEditmoleculeListcontrol)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditMolecule message handlers

BOOL EditMolecule::OnInitDialog() 
{
	CDialog::OnInitDialog();

	MakeListHeaders();
	m_atomsList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
// image list for atom icons
	CMadelungApp *pApp = (CMadelungApp*)AfxGetApp();
	if(!m_pImageListSmall) m_pImageListSmall = new CImageList();
	ASSERT(m_pImageListSmall != NULL);    // serious allocation failure checking
	m_pImageListSmall->Create(16, 16, TRUE, 4, 4);
	for(int i=135; i<211; i++) m_pImageListSmall->Add(pApp->LoadIcon(i));
	m_atomsList.SetImageList(m_pImageListSmall, LVSIL_SMALL);
	if(this->size()) // there are atoms to edit
	{
		EnableEditButton(TRUE);
		EnableRemoveButton(TRUE);
		EnableRemoveAllButton(TRUE);
	}
	else // no atoms to edit
	{
		EnableEditButton(FALSE);
		EnableRemoveButton(FALSE);
		EnableRemoveAllButton(FALSE);
	}

// fill the list box with atom ccordinates from pDoc

	FillAtomsList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////////
//	adds an atom to the molecule
//
void EditMolecule::OnEditmoleculeAdd() 
{
	EditAtomsInMolecule em(this->FromHandle(this->GetSafeHwnd()));
	int nResult = em.DoModal();
	if(nResult == IDOK)
	{
		m_type.push_back(em.m_type);
		m_x.push_back(em.m_x);
		m_y.push_back(em.m_y);
		m_z.push_back(em.m_z);
		m_charge.push_back(em.m_charge);

		AddAtomToList();
		if(this->size())
		{
			EnableEditButton(TRUE);
			EnableRemoveButton(TRUE);
			EnableRemoveAllButton(TRUE);
		}
	}
	else	// Cancel button pressed
	{
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::MakeListHeaders()
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
}
//////////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::OnOK() 
{
	CDialog::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::FillAtomsList()
{
	LV_ITEM lvitem;
	CString cs;
	int iActualItem = 0;
// atoms in molecule
	int size = this->size();
	for(int iItem=0; iItem<size;iItem++)
	{
		for (int iSubItem = 0; iSubItem < 5; iSubItem++)
		{
			lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
			lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
			lvitem.iSubItem = iSubItem;

			switch(iSubItem)
			{
			case 0:
// choose the icon and legend for the entry
				lvitem.iImage = this->m_type[iItem];
				cs.Format("%d",iItem+1);
				break;
			case 1:
				cs.Format("%5.2lf",this->m_x[iItem]);
				break;
			case 2:
				cs.Format("%5.2lf",this->m_y[iItem]);
				break;
			case 3:
				cs.Format("%5.2lf",this->m_z[iItem]);
				break;
			case 4:
				cs.Format("%5.2lf",this->m_charge[iItem]);
				break;
			}
			lvitem.pszText = cs.GetBuffer(20);
			if(iSubItem == 0) iActualItem = m_atomsList.InsertItem(&lvitem); // insert new item
			else
				m_atomsList.SetItem(&lvitem); // modify existing item (the sub-item text)
			cs.ReleaseBuffer();
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::AddAtomToList()
{
	LV_ITEM lvitem;
	CString cs;
	int iActualItem = 0;
	int atom = this->size() - 1;

	for(int iSubItem = 0; iSubItem < 6; iSubItem++)
	{
		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
		lvitem.iItem = (iSubItem == 0)? atom : iActualItem;
		lvitem.iSubItem = iSubItem;

		switch(iSubItem)
		{
		case 0:
// choose the icon and legend for the entry
			lvitem.iImage = this->m_type[atom];
			cs.Format("%d",atom+1);
			break;
		case 1:
			cs.Format("%5.2lf",this->m_x[atom]);
			break;
		case 2:
			cs.Format("%5.2lf",this->m_y[atom]);
			break;
		case 3:
			cs.Format("%5.2lf",this->m_z[atom]);
			break;
		case 4:
			cs.Format("%5.2lf",this->m_charge[atom]);
			break;
		}
		lvitem.pszText = cs.GetBuffer(20);
		if(iSubItem == 0) iActualItem = m_atomsList.InsertItem(&lvitem); // insert new item
		else
			m_atomsList.SetItem(&lvitem); // modify existing item (the sub-item text)
		cs.ReleaseBuffer();
	}
}
///////////////////////////////////////////////////////////////////////////////
//

void EditMolecule::EnableEditButton(bool flag)
{
	CButton* bt = (CButton*)this->GetDlgItem(IDC_EDITMOLECULE_EDIT);
	bt->EnableWindow(flag);
}

///////////////////////////////////////////////////////////////////////////////
//

void EditMolecule::EnableRemoveButton(bool flag)
{
	CButton* bt = (CButton*)this->GetDlgItem(IDC_EDITMOLECULE_REMOVEATOM);
	bt->EnableWindow(flag);
}

///////////////////////////////////////////////////////////////////////////////
//

void EditMolecule::EnableRemoveAllButton(bool flag)
{
	CButton* bt = (CButton*)this->GetDlgItem(IDC_EDITMOLECULE_REMOVEALL);
	bt->EnableWindow(flag);
}
///////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::OnEditmoleculeEdit() 
{
	int nItem;

	CListCtrl* m_atomsList = (CListCtrl*)GetDlgItem(IDC_EDITMOLECULE_LISTCONTROL);
	ASSERT(m_atomsList != NULL);

	nItem = m_atomsList->GetNextItem(-1,LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox("No items selected!",MB_OK);
		return;
	}
// invoke the edit box with selected item's values
	EditAtomsInMolecule editAtom(this->FromHandle(this->GetSafeHwnd()));
	editAtom.m_x = this->m_x[nItem];
	editAtom.m_y = this->m_y[nItem];
	editAtom.m_z = this->m_z[nItem];
	editAtom.m_charge = this->m_charge[nItem];
	editAtom.m_type = this->m_type[nItem];

	int nResult = editAtom.DoModal();
	if(nResult == IDOK)
	{
		m_x[nItem]=editAtom.m_x;
		m_y[nItem]=editAtom.m_y;
		m_z[nItem]=editAtom.m_z;
		m_charge[nItem]=editAtom.m_charge;
		m_type[nItem]=editAtom.m_type;
		
		LV_ITEM lvitem;
		CString cs;
		int iActualItem = 0;
	
		for(int iSubItem = 0; iSubItem < 6; iSubItem++)
		{
			lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
			lvitem.iItem = nItem;
			lvitem.iSubItem = iSubItem;

			switch(iSubItem)
			{	
			case 0:
// choose the icon and legend for the entry
				lvitem.iImage = this->m_type[nItem];
				cs.Format("%d",nItem+1);
				break;
			case 1:
				cs.Format("%5.2lf",m_x[nItem]);
				break;
			case 2:
				cs.Format("%5.2lf",m_y[nItem]);
				break;
			case 3:
				cs.Format("%5.2lf",m_z[nItem]);
				break;
			case 4:
				cs.Format("%5.2lf",m_charge[nItem]);
				break;
			}
			lvitem.pszText = cs.GetBuffer(20);
			m_atomsList->SetItem(&lvitem); // modify existing item (the sub-item text)
			cs.ReleaseBuffer();
		}
		m_atomsList->Update(nItem);
	}
	else	// Cancel button pressed
	{
	}
}
/////////////////////////////////////////////////////////////////////////////
//

void EditMolecule::OnEditmoleculeRemoveatom() 
{
	int nItem;
	
	CListCtrl* m_atomsList = (CListCtrl*)GetDlgItem(IDC_EDITMOLECULE_LISTCONTROL);
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
	vector<double>::iterator iterator_x;
	vector<double>::iterator iterator_y;
	vector<double>::iterator iterator_z;
	vector<double>::iterator iterator_charge;
	vector<int>::iterator iterator_type;

	iterator_x = m_x.begin() + nItem;
	iterator_y = m_y.begin() + nItem;
	iterator_z = m_z.begin() + nItem;
	iterator_charge = m_charge.begin() + nItem;
	iterator_type = m_type.begin() + nItem;
	m_x.erase(iterator_x);
	m_y.erase(iterator_y);
	m_z.erase(iterator_z);
	m_charge.erase(iterator_charge);
	m_type.erase(iterator_type);

	m_atomsList->DeleteItem(nItem);
	if(!this->size()) 
	{
		EnableEditButton(FALSE);
		EnableRemoveButton(FALSE);
		EnableRemoveAllButton(FALSE);
	}
	RenumberAllItems();
}
////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::RenumberAllItems()
{
	LV_ITEM lvitem;
	CString cs;
	int size = this->size(); // number of atoms in the molecule

	for(int nItem = 0; nItem < size; nItem++)
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
/////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::OnEditmoleculeRemoveall() 
{
	CListCtrl* m_atomsList = (CListCtrl*)GetDlgItem(IDC_EDITMOLECULE_LISTCONTROL);
	ASSERT(m_atomsList != NULL);
	if(AfxMessageBox("Are You sure to remove all items?\n",MB_OKCANCEL) == IDCANCEL)
		return;

	m_atomsList->DeleteAllItems();

// remove all items from the list
	this->Clear();

	EnableEditButton(FALSE);
	EnableRemoveButton(FALSE);
	EnableRemoveAllButton(FALSE);
}
//////////////////////////////////////////////////////////////////////////////
//
void EditMolecule::Clear()
{
	m_x.clear();
	m_y.clear();
	m_z.clear();
	m_charge.clear();
	m_type.clear();
}

void EditMolecule::OnDblclkEditmoleculeListcontrol(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(this->size())
		this->OnEditmoleculeEdit();
	*pResult = 0;
}
