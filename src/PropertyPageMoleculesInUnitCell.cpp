// PropertyPageMoleculesInUnitCell.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "MadelungDoc.h"
#include "PropertyPageMoleculesInUnitCell.h"
#include "Molecule.h"
#include "EditMolecule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMadelungDoc* g_pDoc;

/////////////////////////////////////////////////////////////////////////////
// PropertyPageMoleculesInUnitCell property page

IMPLEMENT_DYNCREATE(PropertyPageMoleculesInUnitCell, CPropertyPage)

PropertyPageMoleculesInUnitCell::PropertyPageMoleculesInUnitCell() : CPropertyPage(PropertyPageMoleculesInUnitCell::IDD)
{
	//{{AFX_DATA_INIT(PropertyPageMoleculesInUnitCell)
	//}}AFX_DATA_INIT
	m_MoleculesAreReady = FALSE;
}

PropertyPageMoleculesInUnitCell::~PropertyPageMoleculesInUnitCell()
{
	int size = this->size();
	if(size)
	{
		for(int i=0;i<size;i++)
			delete m_ObjMolecules[i];
	}
	m_ObjMolecules.clear();
}

void PropertyPageMoleculesInUnitCell::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropertyPageMoleculesInUnitCell)
	DDX_Control(pDX, IDC_PP_MOLECULESINUNITCELL_LISTCONTROL, m_moleculesList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropertyPageMoleculesInUnitCell, CPropertyPage)
	//{{AFX_MSG_MAP(PropertyPageMoleculesInUnitCell)
	ON_BN_CLICKED(IDC_PP_MOLECULESINUNITCELL_ADD, OnPpMoleculesinunitcellAdd)
	ON_BN_CLICKED(IDC_PP_MOLECULESINUNITCELL_EDIT, OnPpMoleculesinunitcellEdit)
	ON_BN_CLICKED(IDC_PP_MOLECULESINUNITCELL_REMOVE, OnPpMoleculesinunitcellRemove)
	ON_BN_CLICKED(IDC_PP_MOLECULESINUNITCELL_REMOVEALL, OnPpMoleculesinunitcellRemoveall)
	ON_NOTIFY(NM_DBLCLK, IDC_PP_MOLECULESINUNITCELL_LISTCONTROL, OnDblclkPpMoleculesinunitcellListcontrol)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropertyPageMoleculesInUnitCell message handlers

BOOL PropertyPageMoleculesInUnitCell::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
// copy molecules from g_pDoc
	int size = g_pDoc->m_molecules.size();
	if(this->m_ObjMolecules.size() == 0)
	{
		for(int i=0;i<size;i++)
		{
// create new Molecule objects, which keeps copies of Molecules from g_pDoc
			this->m_ObjMolecules.push_back(new Molecule);
			*this->m_ObjMolecules[i] = *g_pDoc->m_molecules[i];
		}
	}
	else
	{
		AfxMessageBox("PP_MoleculesInUnitCell:m_ObjMolecules not empty",MB_OK);
		return FALSE;
	}

	MakeListHeaders();
	m_moleculesList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	if(size != 0)
	{
		EnableEditButton(TRUE);
		EnableRemoveButton(TRUE);
		EnableRemoveAllButton(TRUE);
		FillMoleculesList();
		m_MoleculesAreReady = TRUE;
	}
	else
	{
		EnableEditButton(FALSE);
		EnableRemoveButton(FALSE);
		EnableRemoveAllButton(FALSE);
		m_MoleculesAreReady = FALSE;
	}

// fill the list box with molecule coordinates from g_pDoc

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
//
void PropertyPageMoleculesInUnitCell::MakeListHeaders()
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
	m_moleculesList.InsertColumn(5,&col);
	col.pszText = "beta";
	col.cx = 50;
	m_moleculesList.InsertColumn(6,&col);
	col.pszText = "gamma";
	col.cx = 50;
	m_moleculesList.InsertColumn(7,&col);
}
///////////////////////////////////////////////////////////////////////////////
//

void PropertyPageMoleculesInUnitCell::EnableEditButton(bool flag)
{
	CButton* bt = (CButton*)this->GetDlgItem(IDC_PP_MOLECULESINUNITCELL_EDIT);
	bt->EnableWindow(flag);
}

///////////////////////////////////////////////////////////////////////////////
//

void PropertyPageMoleculesInUnitCell::EnableRemoveButton(bool flag)
{
	CButton* bt = (CButton*)this->GetDlgItem(IDC_PP_MOLECULESINUNITCELL_REMOVE);
	bt->EnableWindow(flag);
}

///////////////////////////////////////////////////////////////////////////////
//

void PropertyPageMoleculesInUnitCell::EnableRemoveAllButton(bool flag)
{
	CButton* bt = (CButton*)this->GetDlgItem(IDC_PP_MOLECULESINUNITCELL_REMOVEALL);
	bt->EnableWindow(flag);
}
///////////////////////////////////////////////////////////////////////////////
//
void PropertyPageMoleculesInUnitCell::FillMoleculesList()
{
	LV_ITEM lvitem;
	CString cs;
	int iActualItem = 0;
// number of molecules, which has been created
	int size = this->size();
	if(size != 0)
	{
		for(int iItem=0; iItem<size;iItem++)
		{
			double center[3];
			double orientation[3];
// molecule ORIENTATION NOT IMPLEMENTED AT THIS STAGE. Molecule coordinates are read in the
// cartesian format, only center positions are specified.
			this->m_ObjMolecules[iItem]->GetCenter(center);
			this->m_ObjMolecules[iItem]->GetOrientation(orientation);

			for(int iSubItem = 0; iSubItem < 8; iSubItem++)
			{
				lvitem.mask = LVIF_TEXT;
				lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
				lvitem.iSubItem = iSubItem;

				switch(iSubItem)
				{
				case 0:
					cs.Format("%d",iItem+1);
					break;
				case 1:
					cs.Format("%s",this->m_ObjMolecules[iItem]->GetName().c_str());
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
					cs.Format("%5.2lf",orientation[0]);
					break;
				case 6:
					cs.Format("%5.2lf",orientation[1]);
					break;
				case 7:
					cs.Format("%5.2lf",orientation[2]);
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
//////////////////////////////////////////////////////////////////////////////////
//
void PropertyPageMoleculesInUnitCell::OnPpMoleculesinunitcellAdd() 
{
	EditMolecule editMolecule(this->FromHandle(this->GetSafeHwnd()));
	int nResult = editMolecule.DoModal();
	if(nResult == IDOK)
	{
// set up a new molecule
		this->m_ObjMolecules.push_back(new Molecule);
		int mol = this->size(); // number of molecules
		m_ObjMolecules[mol-1]->SetName(LPCTSTR(editMolecule.m_name));
		m_ObjMolecules[mol-1]->SetCenter(editMolecule.m_X,editMolecule.m_Y,editMolecule.m_Z);
		m_ObjMolecules[mol-1]->SetOrientation(editMolecule.m_alpha,
											  editMolecule.m_beta,
											  editMolecule.m_gamma);
// set up atoms in the new molecule
		int size = editMolecule.size();
		for(int i=0;i<size;i++)
		{
			double coord[3];
			coord[0] = editMolecule.m_x[i];
			coord[1] = editMolecule.m_y[i];
			coord[2] = editMolecule.m_z[i];
			m_ObjMolecules[mol-1]->AddAtom(coord,editMolecule.m_type[i],
												editMolecule.m_charge[i]);
		}
		AddMoleculeToList();
		if(this->size())
		{
			EnableEditButton(TRUE);
			EnableRemoveButton(TRUE);
			EnableRemoveAllButton(TRUE);
		}
		m_MoleculesAreReady = TRUE;
	}
	else	// Cancel button pressed
	{
	}
}
//////////////////////////////////////////////////////////////////////////////////
//
void PropertyPageMoleculesInUnitCell::OnPpMoleculesinunitcellEdit() 
{
	int nItem;

	CListCtrl* m_moleculesList = (CListCtrl*)GetDlgItem(IDC_PP_MOLECULESINUNITCELL_LISTCONTROL);
	ASSERT(m_moleculesList != NULL);

	nItem = m_moleculesList->GetNextItem(-1,LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox("No items selected!",MB_OK);
		return;
	}
// set the current molecule in pDoc. It is used in EditAtomsInMolecule()
	g_pDoc->m_currentMolecule = nItem;
// invoke the edit box with selected item's values
	EditMolecule editMolecule(this->FromHandle(this->GetSafeHwnd()));

	double center[3];
	this->m_ObjMolecules[nItem]->GetCenter(center);
	editMolecule.m_X = center[0];
	editMolecule.m_Y = center[1];
	editMolecule.m_Z = center[2];

	double orientation[3];
	this->m_ObjMolecules[nItem]->GetOrientation(orientation);
	editMolecule.m_alpha = orientation[0];
	editMolecule.m_beta = orientation[1];
	editMolecule.m_gamma = orientation[2];

	editMolecule.m_name = this->m_ObjMolecules[nItem]->GetName().c_str();
// atoms in the molecule
	int size = this->m_ObjMolecules[nItem]->GetSize();
	for(int i=0;i<size;i++)
	{
		double dim[3];
		int type;
		double charge;
		this->m_ObjMolecules[nItem]->GetAtom(i,dim,type,charge);
		editMolecule.m_x.push_back(dim[0]);
		editMolecule.m_y.push_back(dim[1]);
		editMolecule.m_z.push_back(dim[2]);
		editMolecule.m_type.push_back(type);
		editMolecule.m_charge.push_back(charge);
	}

	int nResult = editMolecule.DoModal();
	if(nResult == IDOK)
	{
		m_ObjMolecules[nItem]->Clear();
		m_ObjMolecules[nItem]->SetName(LPCTSTR(editMolecule.m_name));
		m_ObjMolecules[nItem]->SetCenter(editMolecule.m_X,editMolecule.m_Y,editMolecule.m_Z);
		m_ObjMolecules[nItem]->SetOrientation(editMolecule.m_alpha,
											  editMolecule.m_beta,
											  editMolecule.m_gamma);
// set up atoms in the new molecule
		int size = editMolecule.size();
		for(int i=0;i<size;i++)
		{
			double coord[3];
			coord[0] = editMolecule.m_x[i];
			coord[1] = editMolecule.m_y[i];
			coord[2] = editMolecule.m_z[i];
			m_ObjMolecules[nItem]->AddAtom(coord,editMolecule.m_type[i],
												editMolecule.m_charge[i]);
		}
// change the corresponding entry in the list box
		LV_ITEM lvitem;
		CString cs;
		int iActualItem = 0;

		double cnt[3];
		this->m_ObjMolecules[nItem]->GetCenter(cnt);

		double or[3];
		this->m_ObjMolecules[nItem]->GetOrientation(or);


		for(int iSubItem = 0; iSubItem < 8; iSubItem++)
		{
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = nItem;
			lvitem.iSubItem = iSubItem;

			switch(iSubItem)
			{	
			case 0:
				cs.Format("%d",nItem+1);
				break;
			case 1:
				cs.Format("%s",this->m_ObjMolecules[nItem]->GetName().c_str());
				break;
			case 2:
				cs.Format("%5.2lf",cnt[0]);
				break;
			case 3:
				cs.Format("%5.2lf",cnt[1]);
				break;
			case 4:
				cs.Format("%5.2lf",cnt[2]);
				break;
			case 5:
				cs.Format("%5.2lf",or[0]);
				break;
			case 6:
				cs.Format("%5.2lf",or[1]);
				break;
			case 7:
				cs.Format("%5.2lf",or[2]);
				break;
			}
			lvitem.pszText = cs.GetBuffer(20);
			m_moleculesList->SetItem(&lvitem); // modify existing item (the sub-item text)
			cs.ReleaseBuffer();
		}
		m_moleculesList->Update(nItem);
	}
	else	// Cancel button pressed
	{
	}
}
///////////////////////////////////////////////////////////////////////////////
//
void PropertyPageMoleculesInUnitCell::OnPpMoleculesinunitcellRemove() 
{
	int nItem;
	
	CListCtrl* m_moleculesList = (CListCtrl*)GetDlgItem(IDC_PP_MOLECULESINUNITCELL_LISTCONTROL);
	ASSERT(m_moleculesList != NULL);

	POSITION pos = m_moleculesList->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("No items selected!\n");
		return;
	}
	else
		nItem = m_moleculesList->GetNextSelectedItem(pos);
// remove the selected item from the list
	if(AfxMessageBox("Are You sure to remove the item?\n",MB_OKCANCEL) == IDCANCEL)
		return;

// now do actual removing of the item

	delete this->m_ObjMolecules[nItem];
	vector<Molecule*>::iterator p = this->m_ObjMolecules.begin();
	this->m_ObjMolecules.erase(p+nItem);

	m_moleculesList->DeleteItem(nItem);

	if(!this->size()) 
	{
		EnableEditButton(FALSE);
		EnableRemoveButton(FALSE);
		EnableRemoveAllButton(FALSE);
		m_MoleculesAreReady = FALSE;
	}
	RenumberAllItems();
// items in pDoc will be removed in CMainFrm() - function which called PropertySheet
}
//////////////////////////////////////////////////////////////////////////////
//
void PropertyPageMoleculesInUnitCell::RenumberAllItems()
{
	LV_ITEM lvitem;
	CString cs;
	int size = this->size();

	for (int nItem = 0; nItem < size; nItem++)
	{
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = nItem;
		lvitem.iSubItem = 0;
		cs.Format("%d",nItem+1);
		lvitem.pszText = cs.GetBuffer(20);
		m_moleculesList.SetItem(&lvitem); // modify existing item (the sub-item text)
		cs.ReleaseBuffer();
	}
}
////////////////////////////////////////////////////////////////////////////////
//

void PropertyPageMoleculesInUnitCell::OnPpMoleculesinunitcellRemoveall() 
{
	CListCtrl* m_moleculesList = (CListCtrl*)GetDlgItem(IDC_PP_MOLECULESINUNITCELL_LISTCONTROL);
	ASSERT(m_moleculesList != NULL);
	if(AfxMessageBox("Are You sure to remove all items?\n",MB_OKCANCEL) == IDCANCEL)
		return;

	m_moleculesList->DeleteAllItems();

// remove all items from the list
	this->Clear();

	EnableEditButton(FALSE);
	EnableRemoveButton(FALSE);
	EnableRemoveAllButton(FALSE);
	m_MoleculesAreReady = FALSE;
}
///////////////////////////////////////////////////////////////////////////
// clears all items from m_moleculesList
//
void PropertyPageMoleculesInUnitCell::Clear()
{
	int size = this->size();
	if(size)
	{
// delete all Molecule* objects
		for(int i=0;i<size;i++)
			delete this->m_ObjMolecules[i];
// clear vector
		this->m_ObjMolecules.clear();
	}
}
////////////////////////////////////////////////////////////////////////////
//
void PropertyPageMoleculesInUnitCell::AddMoleculeToList()
{
	LV_ITEM lvitem;
	CString cs;
	int iActualItem = 0;
	int mol = this->size() - 1;

	for (int iSubItem = 0; iSubItem < 8; iSubItem++)
	{
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = (iSubItem == 0)? mol : iActualItem;
		lvitem.iSubItem = iSubItem;
	
		double crd[3];
		double or[3];
		m_ObjMolecules[mol]->GetCenter(crd);
		m_ObjMolecules[mol]->GetOrientation(or);

		switch(iSubItem)
		{
		case 0:
// choose the icon and legend for the entry
			cs.Format("%d",mol+1);
			break;
		case 1:
			cs.Format("%s",m_ObjMolecules[mol]->GetName().c_str());
			break;
		case 2:
			cs.Format("%5.2lf",crd[0]);
			break;
		case 3:
			cs.Format("%5.2lf",crd[1]);
			break;
		case 4:
			cs.Format("%5.2lf",crd[2]);
			break;
		case 5:
			cs.Format("%5.2lf",or[0]);
			break;
		case 6:
			cs.Format("%5.2lf",or[1]);
			break;
		case 7:
			cs.Format("%5.2lf",or[2]);
			break;
		}
		lvitem.pszText = cs.GetBuffer(20);
		if(iSubItem == 0) iActualItem = m_moleculesList.InsertItem(&lvitem); // insert new item
		else
			m_moleculesList.SetItem(&lvitem); // modify existing item (the sub-item text)
		cs.ReleaseBuffer();
	}
}
////////////////////////////////////////////////////////////////////////////////////
//
void PropertyPageMoleculesInUnitCell::OnDblclkPpMoleculesinunitcellListcontrol(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(this->m_ObjMolecules.size())
		this->OnPpMoleculesinunitcellEdit();
	*pResult = 0;
}
