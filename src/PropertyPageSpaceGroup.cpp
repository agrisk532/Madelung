// PropertyPageSpaceGroup.cpp : implementation file
//

#include "stdafx.h"
#include <afx.h>
#include "Madelung.h"
#include "MadelungDoc.h"
#include "MadelungView.h"
#include "MainFrm.h"
#include "PropertyPageSpaceGroup.h"
#include "PropertyPageAtomsInUnitCell.h"
#include "Spg_dll.h"
#include "UnitCell.h"
#include "spg_names.h"
#include "TestReply.h"

// used for checking input fields of unitcell parameters(a,b,c,alpha,beta,gamma)
const int HEXAGONAL = 0;
const int CUBIC = 1;
const int MONOCLINIC_B = 2;
const int MONOCLINIC_C = 3;
const int ORTHOROMBIC = 4;
const int TETRAGONAL = 5;
const int TRICLINIC = 6;
const int TRIGONAL = 7;

/////////////////////////////////////////////////////////////////////////////
// PropertyPageSpaceGroup dialog

IMPLEMENT_DYNCREATE(PropertyPageSpaceGroup, CPropertyPage)

PropertyPageSpaceGroup::PropertyPageSpaceGroup() : CPropertyPage(PropertyPageSpaceGroup::IDD){
	//{{AFX_DATA_INIT(PropertyPageSpaceGroup)
	m_sg = 0;
	m_sg_edit = _T("");
	m_a = 0.0;
	m_alpha = 0.0;
	m_b = 0.0;
	m_beta = 0.0;
	m_c = 0.0;
	m_gamma = 0.0;
	m_pagenumber = _T("");
	m_sgnumber = _T("");
	m_shortValue = _T("");
	m_longValue = _T("");
	m_schValue = _T("");
	m_hallValue = _T("");
	m_pgValue = _T("");
	m_crysystValue = _T("");
	m_latticeSymbolValue = _T("");
	m_addInfoValue = _T("");
	m_distanceRestrictions = _T("");
	m_angleRestrictions = _T("");
	//}}AFX_DATA_INIT
	this->m_pSpaceGroup = NULL;

}

PropertyPageSpaceGroup::~PropertyPageSpaceGroup()
{
// do not delete m_pSpaceGroup here, it's used in CMadelungDoc
}

void PropertyPageSpaceGroup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropertyPageSpaceGroup)
	DDX_Radio(pDX, IDC_RADIO_SPGROUPS_SHORT_HM, m_sg);
	DDX_Text(pDX, IDC_EDIT_SG, m_sg_edit);
	DDV_MaxChars(pDX, m_sg_edit, 20);
	DDX_Text(pDX, IDC_EDIT_A, m_a);
	DDX_Text(pDX, IDC_EDIT_ALPHA, m_alpha);
	DDX_Text(pDX, IDC_EDIT_B, m_b);
	DDX_Text(pDX, IDC_EDIT_BETA, m_beta);
	DDX_Text(pDX, IDC_EDIT_C, m_c);
	DDX_Text(pDX, IDC_EDIT_GAMMA, m_gamma);
	DDX_Text(pDX, IDC_STATIC_SGSETUP_PAGENUMBER, m_pagenumber);
	DDX_Text(pDX, IDC_STATIC_SGSETUP_SGNUMBER, m_sgnumber);
	DDX_Text(pDX, IDC_STATIC_SGSETUP_SHORT_VALUE, m_shortValue);
	DDX_Text(pDX, IDC_STATIC_SGSETUP_LONG_VALUE, m_longValue);
	DDX_Text(pDX, IDC_STATIC_SGSETUP_SCH_VALUE, m_schValue);
	DDX_Text(pDX, IDC_STATIC_HALLSYMBOLVALUE, m_hallValue);
	DDX_Text(pDX, IDC_STATIC_SGSETUP_PGVALUE, m_pgValue);
	DDX_Text(pDX, IDC_STATIC_SGSETUP_CRYSYSTVALUE, m_crysystValue);
	DDX_Text(pDX, IDC_STATIC_SGSETUP_LATTICESYMBOLVALUE, m_latticeSymbolValue);
	DDX_Text(pDX, IDC_STATIC_SGSETUP_ADDITIONALINFOVALUE, m_addInfoValue);
	DDX_Text(pDX, IDC_STATIC_SGSETUP_ABC_RESTRICTIONS, m_distanceRestrictions);
	DDX_Text(pDX, IDC_STATIC_SGSETUP_ANGLE_RESTRICTIONS, m_angleRestrictions);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropertyPageSpaceGroup, CPropertyPage)
	//{{AFX_MSG_MAP(PropertyPageSpaceGroup)
	ON_BN_CLICKED(IDC_BUTTON_SGSETUP_LOAD, OnButtonSgsetupLoad)
	ON_BN_CLICKED(IDC_RADIO_SPGROUPS_FULL_HM, OnRadioSpgroupsFullHm)
	ON_BN_CLICKED(IDC_RADIO_SPGROUPS_HALL, OnRadioSpgroupsHall)
	ON_BN_CLICKED(IDC_RADIO_SPGROUPS_NUMBER, OnRadioSpgroupsNumber)
	ON_BN_CLICKED(IDC_RADIO_SPGROUPS_SCHOENFLIES, OnRadioSpgroupsSchoenflies)
	ON_BN_CLICKED(IDC_RADIO_SPGROUPS_SHORT_HM, OnRadioSpgroupsShortHm)
	ON_EN_CHANGE(IDC_EDIT_A, OnChangeEditA)
	ON_EN_CHANGE(IDC_EDIT_ALPHA, OnChangeEditAlpha)
	ON_EN_KILLFOCUS(IDC_EDIT_A, OnKillfocusEditA)
	ON_EN_KILLFOCUS(IDC_EDIT_B, OnKillfocusEditB)
	ON_EN_KILLFOCUS(IDC_EDIT_C, OnKillfocusEditC)
	ON_EN_KILLFOCUS(IDC_EDIT_ALPHA, OnKillfocusEditAlpha)
	ON_EN_KILLFOCUS(IDC_EDIT_BETA, OnKillfocusEditBeta)
	ON_EN_KILLFOCUS(IDC_EDIT_GAMMA, OnKillfocusEditGamma)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropertyPageSpaceGroup message handlers

void PropertyPageSpaceGroup::OnButtonSgsetupLoad() 
{
// check if space group was specified
	CString tmp1 = this->ConvertSGEditString();
	if(tmp1 == "")
	{
		int reply = AfxMessageBox("Specify a space group name!",MB_OK);
		if(reply == IDOK)
		{
//			PostMessage(WM_COMMAND,ID_TOOLS_SETUP_LATTICE,NULL);
			return;
		}
	}
// Checks whether the SpaceGroup has been already loaded and exists in pDoc
	if(this->IsSpaceGroupLoaded(tmp1))
		return;
// names are different. Attempt to load new group
	int SGnumber;	// SpaceGroup number
// delete the atom coordinates from the PropertyPageAtomsInUnitCell
	if((SGnumber = this->GetSGNumber()) != -1)	// spacegroup found
	{
		if(this->DeleteSpaceGroup()) // old SG deleted
		{
			this->CreateSpaceGroup(SGnumber);
			this->LoadSGDescription();	// loads spacegroup description in the propertypage
// save the spacegroup name to prevent it loading second time if user loads the same group
		}
		else
		{
			int reply = AfxMessageBox("Space Group not deleted",MB_OK);
			if(reply == IDOK)
			{
				return;
			}
			else // if canceled 
			{
				return;
			}
		}
	}
	else
	{
		int reply = AfxMessageBox("Space Group not found. Try again.",MB_OK);
			if(reply == IDOK)
			{
				return;
			}
			else // if canceled 
			{
				return;
			}
	}
}


/////////////////////////////////////////////////////////////////////////////
//
//	space group handling done only here, not in the document. Document receives
//	only pointer to it.
//	EditAtom and AtomWindow objects receive pointer from the document object
//

void PropertyPageSpaceGroup::CreateSpaceGroup(int SGnumber)
{
// check if space group exists. If exists, delete it.
	if(this->m_pSpaceGroup) 
		this->DeleteSpaceGroup();
// clear view - removes all actors
	ClearView();
// create new space group
	this->m_pSpaceGroup = SpaceGroup::New(SGnumber);
	GetDoc()->m_pSpaceGroup = this->m_pSpaceGroup;
}

/////////////////////////////////////////////////////////////////////////////
//	returns the number of the space group from the symbol
//	spacegroup taken from the edit window
//
int PropertyPageSpaceGroup::GetSGNumber()
{
	spg_names* sg = new spg_names;
	CString txt;
	string txt_s;
	CEdit* wnd = (CEdit*)this->GetDlgItem(IDC_EDIT_SG);
	wnd->GetWindowText(txt);
	txt_s = CStringToString(txt);
	int number = -1;	// dummy
// retrieve the dialog box data (for m_sg)
	this->UpdateData(TRUE);
/*	switch(m_sg)
	{
// all following functions are implemented in the SpaceGroupdll
	case 0:	// short HM symbol
		number = sg->sgdll_GetShort(txt_s);
		break;
	case 1:	// full HM symbol
		number = sg->sgdll_GetFull(txt_s);
		break;
	case 2:	// Schoenflies
		number = sg->sgdll_GetSchoenflies(txt_s);
		break;
	case 3:	// number
		number = sg->sgdll_GetNumber(txt_s);
		break;
	case 4:	// Hall
		number = sg->sgdll_GetHall(txt_s);
		break;
	default:
		delete sg;
		return -1;
	}
*/
// try to load spacegroup, ignoring radio button settings
	for(int i=0;i<1;i++)
	{
		number = sg->sgdll_GetShort(txt_s);
		if(number != -1)
		{
			m_sg = 0;
			break;
		}

		number = sg->sgdll_GetFull(txt_s);
		if(number != -1)
		{
			m_sg = 1;
			break;
		}

		number = sg->sgdll_GetSchoenflies(txt_s);
		if(number != -1)
		{
			m_sg = 2;
			break;
		}

		number = sg->sgdll_GetNumber(txt_s);
		if(number != -1)
		{
			m_sg = 3;
			break;
		}

		number = sg->sgdll_GetHall(txt_s);
		if(number != -1)
		{
			m_sg = 4;
			break;
		}
	}

	if(number != -1)	// space group found
	{
		this->UpdateData(FALSE);
		delete sg;
		return number;
	}
	else	// spacegroup not found
	{
		delete sg;
		return -1;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	warns user and deletes space group
bool PropertyPageSpaceGroup::DeleteSpaceGroup()
{
	if(this->m_pSpaceGroup)	// group exists
	{
		int reply = AfxMessageBox("Previous Space Group will be lost!",MB_OKCANCEL);
		if(reply == IDOK)
		{
			if(m_pSpaceGroup)
			{
				delete m_pSpaceGroup;
				m_pSpaceGroup = NULL;
				GetDoc()->m_pSpaceGroup = NULL;
// removes all items from the PropertyPageUnitCell
				this->ClearPropertyPageAtomsInUnitCell();
			}
			return true;
		}
		else // if canceled 
		{
			return false;
		}
	}
	else	// group doesnt exist
		return true;
}

/////////////////////////////////////////////////////////////////////////////
//
//
void PropertyPageSpaceGroup::LoadSGDescription()
{
	char buffer[20];

// load the description only if spacegroup has been created and exists
	if(this->m_pSpaceGroup)
	{
// set the page number in International Tables
		CStatic* wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_PAGENUMBER);
		wnd->SetWindowText(this->m_pSpaceGroup->m_pageInInternationalTables.c_str());
// set the spacegroup number window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_SGNUMBER);
		int sgnumber = this->m_pSpaceGroup->m_SpaceGroupNumber;
		if(sgnumber > 1000) sgnumber /= 1000;
		wnd->SetWindowText(itoa(sgnumber,buffer,10));
// set the short notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_SHORT_VALUE);
		wnd->SetWindowText(this->m_pSpaceGroup->m_Short_notation.c_str());
// set the full notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_LONG_VALUE);
		wnd->SetWindowText(this->m_pSpaceGroup->m_Long_notation.c_str());
// set the Schoenflies notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_SCH_VALUE);
		wnd->SetWindowText(this->m_pSpaceGroup->m_Schoenflies.c_str());
// set the Hall notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_HALLSYMBOLVALUE);
		wnd->SetWindowText(this->m_pSpaceGroup->m_Hall.c_str());
// set the pointgroup notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_PGVALUE);
		wnd->SetWindowText(this->m_pSpaceGroup->m_PointGroup.c_str());
// set the crystal system notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_CRYSYSTVALUE);
		wnd->SetWindowText(this->m_pSpaceGroup->m_CrystalSystem.c_str());
// set the lattice symbol notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_LATTICESYMBOLVALUE);
		wnd->SetWindowText(this->m_pSpaceGroup->m_LatticeSymbol.c_str());
// set the additional info notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_ADDITIONALINFOVALUE);
		wnd->SetWindowText(this->m_pSpaceGroup->m_additionalInfo.c_str());

//------  enable/disable unit cell parameter input windows --------

		CEdit* edit;
		CString str;
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_A);
		edit->EnableWindow(this->m_pSpaceGroup->m_pUnitCell->m_enabledAEditing);
		str.Format("%5.2f",this->m_pSpaceGroup->m_pUnitCell->m_a);
		edit->SetWindowText(str);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_B);
		edit->EnableWindow(this->m_pSpaceGroup->m_pUnitCell->m_enabledBEditing);
		str.Format("%5.2f",this->m_pSpaceGroup->m_pUnitCell->m_b);
		edit->SetWindowText(str);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_C);
		edit->EnableWindow(this->m_pSpaceGroup->m_pUnitCell->m_enabledCEditing);
		str.Format("%5.2f",this->m_pSpaceGroup->m_pUnitCell->m_c);
		edit->SetWindowText(str);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_ALPHA);
		edit->EnableWindow(this->m_pSpaceGroup->m_pUnitCell->m_enabledAlphaEditing);
		str.Format("%6.2f",this->m_pSpaceGroup->m_pUnitCell->m_alpha);
		edit->SetWindowText(str);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_BETA);
		edit->EnableWindow(this->m_pSpaceGroup->m_pUnitCell->m_enabledBetaEditing);
		str.Format("%6.2f",this->m_pSpaceGroup->m_pUnitCell->m_beta);
		edit->SetWindowText(str);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_GAMMA);
		edit->EnableWindow(this->m_pSpaceGroup->m_pUnitCell->m_enabledGammaEditing);
		str.Format("%6.2f",this->m_pSpaceGroup->m_pUnitCell->m_gamma);
		edit->SetWindowText(str);

//--------set the hint windows next to edit windows --------------

		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_ABC_RESTRICTIONS);
		wnd->SetWindowText(this->m_pSpaceGroup->m_pUnitCell->m_abcRestrictions.c_str());

		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_ANGLE_RESTRICTIONS);
		wnd->SetWindowText(this->m_pSpaceGroup->m_pUnitCell->m_angleRestrictions.c_str());
	}
// SpaceGroup not loaded
	else
	{
// set the page number in International Tables
		CStatic* wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_PAGENUMBER);
		wnd->SetWindowText("N/A");
// set the spacegroup number window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_SGNUMBER);
		wnd->SetWindowText("N/A");
// set the short notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_SHORT_VALUE);
		wnd->SetWindowText("N/A");
// set the full notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_LONG_VALUE);
		wnd->SetWindowText("N/A");
// set the Schoenflies notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_SCH_VALUE);
		wnd->SetWindowText("N/A");
// set the Hall notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_HALLSYMBOLVALUE);
		wnd->SetWindowText("N/A");
// set the pointgroup notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_PGVALUE);
		wnd->SetWindowText("N/A");
// set the crystal system notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_CRYSYSTVALUE);
		wnd->SetWindowText("N/A");
// set the lattice symbol notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_LATTICESYMBOLVALUE);
		wnd->SetWindowText("N/A");
// set the additional info notation window
		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_ADDITIONALINFOVALUE);
		wnd->SetWindowText("N/A");

//------  enable/disable unit cell parameter input windows --------

		CEdit* edit;
		CString str;
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_A);
		edit->EnableWindow(FALSE);
		edit->SetWindowText("0");
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_B);
		edit->EnableWindow(FALSE);
		edit->SetWindowText("0");
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_C);
		edit->EnableWindow(FALSE);
		edit->SetWindowText("0");
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_ALPHA);
		edit->EnableWindow(FALSE);
		edit->SetWindowText("0");
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_BETA);
		edit->EnableWindow(FALSE);
		edit->SetWindowText("0");
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_GAMMA);
		edit->EnableWindow(FALSE);
		edit->SetWindowText("0");

//--------set the hint windows next to edit windows --------------

		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_ABC_RESTRICTIONS);
		wnd->SetWindowText("N/A");

		wnd = (CStatic*)this->GetDlgItem(IDC_STATIC_SGSETUP_ANGLE_RESTRICTIONS);
		wnd->SetWindowText("N/A");
	}
}
////////////////////////////////////////////////////////////////////////////
//	converts a CString object to C++ string object
// 
string PropertyPageSpaceGroup::CStringToString(CString txt)
{
// remove spaces
	txt.Remove(' ');
// make string to lowercase
	txt.MakeLower();
	const char* cstr;
	cstr = LPCTSTR(txt);
	return string(cstr);
}

////////////////////////////////////////////////////////////////////////////
//
CMadelungDoc* PropertyPageSpaceGroup::GetDoc()
{
	CMadelungApp* pApp = (CMadelungApp*)AfxGetApp();
	CWnd* wnd = pApp->m_pMainWnd;
	ASSERT(wnd->IsKindOf(RUNTIME_CLASS(CMainFrame)));
	CMainFrame* pFr = (CMainFrame*)wnd;
	CMadelungDoc* pDoc = (CMadelungDoc*) pFr->GetActiveDocument();
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CMadelungDoc)));
	return pDoc;
}

/////////////////////////////////////////////////////////////////////////////
//
BOOL PropertyPageSpaceGroup::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
// set the SpaceGroup pointer
	this->m_pSpaceGroup = GetDoc()->m_pSpaceGroup;
// initialize edit boxes and description windows
	this->EnableDisableEditBoxes();
	this->LoadSGDescription();
// return TRUE unless you set the focus to a control
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// enable / disable edit boxes
//
void PropertyPageSpaceGroup::EnableDisableEditBoxes()
{
	CEdit* edit;
	if(this->m_pSpaceGroup)
	{
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_A);
		edit->EnableWindow(this->m_pSpaceGroup->m_pUnitCell->m_enabledAEditing);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_B);
		edit->EnableWindow(this->m_pSpaceGroup->m_pUnitCell->m_enabledBEditing);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_C);
		edit->EnableWindow(this->m_pSpaceGroup->m_pUnitCell->m_enabledCEditing);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_ALPHA);
		edit->EnableWindow(this->m_pSpaceGroup->m_pUnitCell->m_enabledAlphaEditing);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_BETA);
		edit->EnableWindow(this->m_pSpaceGroup->m_pUnitCell->m_enabledBetaEditing);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_GAMMA);
		edit->EnableWindow(this->m_pSpaceGroup->m_pUnitCell->m_enabledGammaEditing);
	}
	else	// no SpaceGroup loaded
	{
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_A);
		edit->EnableWindow(FALSE);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_B);
		edit->EnableWindow(FALSE);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_C);
		edit->EnableWindow(FALSE);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_ALPHA);
		edit->EnableWindow(FALSE);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_BETA);
		edit->EnableWindow(FALSE);
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_GAMMA);
		edit->EnableWindow(FALSE);
	}
}

////////////////////////////////////////////////////////////////////////
//
//
void PropertyPageSpaceGroup::OnRadioSpgroupsFullHm() 
{
	CEdit* edit;
	if(this->m_pSpaceGroup)
	{
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_SG);
		edit->SetWindowText(this->m_pSpaceGroup->m_Long_notation.c_str());
	}
}

/////////////////////////////////////////////////////////////////////////
//
//

void PropertyPageSpaceGroup::OnRadioSpgroupsHall() 
{
	CEdit* edit;
	if(this->m_pSpaceGroup)
	{
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_SG);
		edit->SetWindowText(this->m_pSpaceGroup->m_Hall.c_str());
	}
}

void PropertyPageSpaceGroup::OnRadioSpgroupsNumber() 
{
	CEdit* edit;
	if(this->m_pSpaceGroup)
	{
		char buffer[10];
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_SG);
		int number = this->m_pSpaceGroup->m_SpaceGroupNumber;
		if(number>1000) number/=1000;
		edit->SetWindowText(itoa(number,buffer,10));
	}
}

void PropertyPageSpaceGroup::OnRadioSpgroupsSchoenflies() 
{
	CEdit* edit;
	if(this->m_pSpaceGroup)
	{
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_SG);
		edit->SetWindowText(this->m_pSpaceGroup->m_Schoenflies.c_str());
	}
}

void PropertyPageSpaceGroup::OnRadioSpgroupsShortHm() 
{
	CEdit* edit;
	if(this->m_pSpaceGroup)
	{
		edit = (CEdit*)this->GetDlgItem(IDC_EDIT_SG);
		edit->SetWindowText(this->m_pSpaceGroup->m_Short_notation.c_str());
	}
}


void PropertyPageSpaceGroup::OnOK() 
{
	CString tmp = this->ConvertSGEditString();
	if(!this->m_pSpaceGroup)
	{
		this->OnButtonSgsetupLoad();
		return;
	}
	else
	{
		if(IsSpaceGroupLoaded(tmp))	// if spacegroup already loaded or exists, continue
			return;
		else
		{
			this->OnButtonSgsetupLoad();
			return;
		}
	}

	CPropertyPage::OnOK();
}

////////////////////////////////////////////////////////////////////////
//	Removes blank characters and converts string to lowercase
//
CString PropertyPageSpaceGroup::ConvertSGEditString()
{
// check if space group was specified
	this->UpdateData();
// get the spacegroup name
	CString tmp1 = this->m_sg_edit;
	tmp1.Remove(' ');
	tmp1.MakeLower();
	return tmp1;
}

/////////////////////////////////////////////////////////////////////////
//	clears all items in the PropertyPageAtomsInUnitCell
//	used when loading new SpaceGroup
//
void PropertyPageSpaceGroup::ClearPropertyPageAtomsInUnitCell()
{
	CPropertySheet* pSh = (CPropertySheet*)this->GetParent();
	ASSERT_VALID(pSh);
	CPropertyPage* pP = pSh->GetPage(1);
	ASSERT_VALID(pP);
	PropertyPageAtomsInUnitCell* pPPa = (PropertyPageAtomsInUnitCell*)pP;
	ASSERT_VALID(pPPa);
	pPPa->Clear();
}

////////////////////////////////////////////////////////////////////////
//	clears view
//
void PropertyPageSpaceGroup::ClearView()
{
	CMadelungDoc* pDoc = GetDoc();
	POSITION pos = GetDoc()->GetFirstViewPosition();
	CMadelungView* pV = (CMadelungView*)pDoc->GetNextView(pos);
	ASSERT(pV->IsKindOf(RUNTIME_CLASS(CMadelungView)));
	pV->Clear();
	pV->Invalidate();
}

///////////////////////////////////////////////////////////////////////////
//	checks whether specified SpaceGroup is already loaded
//	from tmp all spaces must be removed and characters lowercase
bool PropertyPageSpaceGroup::IsSpaceGroupLoaded(CString str)
{
	CMadelungDoc* pDoc = GetDoc();
	if(pDoc->m_pSpaceGroup)
	{
		CString tmp;
		tmp = pDoc->m_pSpaceGroup->m_Short_notation.c_str();
		tmp.Remove(' ');
		tmp.MakeLower();
		if(tmp == str)
			return true;

		tmp = pDoc->m_pSpaceGroup->m_Long_notation.c_str();
		tmp.Remove(' ');
		tmp.MakeLower();
		if(tmp == str)
			return true;

		tmp = pDoc->m_pSpaceGroup->m_Schoenflies.c_str();
		tmp.Remove(' ');
		tmp.MakeLower();
		if(tmp == str)
			return true;

		tmp = pDoc->m_pSpaceGroup->m_Hall.c_str();
		tmp.Remove(' ');
		tmp.MakeLower();
		if(tmp == str)
			return true;

		char buffer[8];
		tmp = str;
		tmp.Remove(' ');
		tmp.MakeLower();
		int number = pDoc->m_pSpaceGroup->m_SpaceGroupNumber;
		if(number>1000) number/=1000;
		if(CString(itoa(number,buffer,10)) == tmp)
			return true;

		return false;
	}
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////////
//
//
void PropertyPageSpaceGroup::OnChangeEditA() 
{
	CMadelungDoc* pDoc = GetDoc();

	if(pDoc->m_pSpaceGroup)
	{
		CString string;
		CEdit* editABox;
		CEdit* editBBox;
		CEdit* editCBox;
		editABox = (CEdit*)GetDlgItem(IDC_EDIT_A);
		editBBox = (CEdit*)GetDlgItem(IDC_EDIT_B);
		editCBox = (CEdit*)GetDlgItem(IDC_EDIT_C);
	
		switch(pDoc->m_pSpaceGroup->m_pUnitCell->m_ID)
		{
		case CUBIC:
		case TRIGONAL:
			editABox->GetWindowText(string);
			editBBox->SetWindowText(string);
			editCBox->SetWindowText(string);
			break;
		case TETRAGONAL:
		case HEXAGONAL:
			editABox->GetWindowText(string);
			editBBox->SetWindowText(string);
			break;
		default:
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//
void PropertyPageSpaceGroup::OnChangeEditAlpha() 
{
	CMadelungDoc* pDoc = GetDoc();

	if(pDoc->m_pSpaceGroup)
	{
		CString string;
		CEdit* editAlphaBox;
		CEdit* editBetaBox;
		CEdit* editGammaBox;
		editAlphaBox = (CEdit*)GetDlgItem(IDC_EDIT_ALPHA);
		editBetaBox = (CEdit*)GetDlgItem(IDC_EDIT_BETA);
		editGammaBox = (CEdit*)GetDlgItem(IDC_EDIT_GAMMA);
	
		switch(pDoc->m_pSpaceGroup->m_pUnitCell->m_ID)
		{
		case TRIGONAL:
			editAlphaBox->GetWindowText(string);
			editBetaBox->SetWindowText(string);
			editGammaBox->SetWindowText(string);
			break;
		default:
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//
void PropertyPageSpaceGroup::OnKillfocusEditA() 
{
	CMadelungDoc* pDoc = GetDoc();

	if(pDoc->m_pSpaceGroup)
	{
		UpdateData(TRUE);

		switch(pDoc->m_pSpaceGroup->m_pUnitCell->m_ID)
		{
		case TETRAGONAL:
			if(m_a==m_c)
				ShowMessageBox("'a' is not equal to 'c' in the Tetragonal crystal system",
					IDC_EDIT_C);
			break;
		case HEXAGONAL:
			if(m_a==m_c)
				ShowMessageBox("'a' is not equal to 'c' in the Hexagonal crystal system",
					IDC_EDIT_C);
			break;
		case MONOCLINIC_B:
			if(m_a==m_c)
			{
				ShowMessageBox("'a' is not equal to 'c' in the Monoclinic(b) crystal system",
					IDC_EDIT_C);
				break;
			}
			if(m_a==m_b)
				ShowMessageBox("a is not equal to b in the Monoclinic(b) crystal system",
					IDC_EDIT_B);
			break;
		case MONOCLINIC_C:
			if(m_a==m_c)
			{
				ShowMessageBox("'a' is not equal to 'c' in the Monoclinic(c) crystal system",
					IDC_EDIT_C);
				break;
			}
			if(m_a==m_b)
				ShowMessageBox("'a' is not equal to 'b' in the Monoclinic(c) crystal system",
					IDC_EDIT_B);
			break;
		case TRICLINIC:
			if(m_a==m_c)
			{
				ShowMessageBox("'a' is not equal to 'c' in the Triclinic crystal system",
					IDC_EDIT_C);
				break;
			}
			if(m_a==m_b)
				ShowMessageBox("'a' is not equal to 'b' in the Triclinic crystal system",
					IDC_EDIT_B);
			break;
		case ORTHOROMBIC:
			if(m_a==m_c)
			{
				ShowMessageBox("'a' is not equal to 'c' in the Orthorombic crystal system",
					IDC_EDIT_C);
				break;
			}
			if(m_a==m_b)
				ShowMessageBox("'a' is not equal to 'b' in the Orthorombic crystal system",
					IDC_EDIT_B);
			break;
		default:
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//
void PropertyPageSpaceGroup::OnKillfocusEditB() 
{
	CMadelungDoc* pDoc = GetDoc();

	if(pDoc->m_pSpaceGroup)
	{
		UpdateData(TRUE);
		switch(pDoc->m_pSpaceGroup->m_pUnitCell->m_ID)
		{
		case TRICLINIC:
			if(m_a==m_b)
			{
				ShowMessageBox("'b' is not equal to 'a' in the Triclinic crystal system",
					IDC_EDIT_A);
				break;
			}
			if(m_c==m_b)
				ShowMessageBox("'b' is not equal to 'c' in the Triclinic crystal system",
					IDC_EDIT_C);
			break;
		case MONOCLINIC_C:
			if(m_a==m_b)
			{
				ShowMessageBox("'b' is not equal to 'a' in the Monoclinic(c) crystal system",
					IDC_EDIT_A);
				break;
			}
			if(m_c==m_b)
				ShowMessageBox("'b' is not equal to 'c' in the Monoclinic(c) crystal system",
					IDC_EDIT_C);
			break;
		case MONOCLINIC_B:
			if(m_a==m_b)
			{
				ShowMessageBox("'b' is not equal to 'a' in the Monoclinic(b) crystal system",
					IDC_EDIT_A);
				break;
			}
			if(m_c==m_b)
				ShowMessageBox("'b' is not equal to 'c' in the Monoclinic(b) crystal system",
					IDC_EDIT_C);
			break;
		case ORTHOROMBIC:
			if(m_a==m_b)
			{
				ShowMessageBox("'b' is not equal to 'a' in the Orthorombic crystal system",
					IDC_EDIT_A);
				break;
			}
			if(m_c==m_b)
				ShowMessageBox("'b' is not equal to 'c' in the Orthorombic crystal system",
					IDC_EDIT_C);
			break;
		default:
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//
void PropertyPageSpaceGroup::OnKillfocusEditC() 
{
	CMadelungDoc* pDoc = GetDoc();

	if(pDoc->m_pSpaceGroup)
	{
		UpdateData(TRUE);

		switch(pDoc->m_pSpaceGroup->m_pUnitCell->m_ID)
		{
		case TETRAGONAL:
			if(m_c==m_a)
				ShowMessageBox("'c' is not equal to 'a' in the Tetragonal crystal system",
					IDC_EDIT_A);
			break;
		case HEXAGONAL:
			if(m_c==m_a)
				ShowMessageBox("'c' is not equal to 'a' in the Hexagonal crystal system",
					IDC_EDIT_A);
			break;
		case MONOCLINIC_C:
			if(m_c==m_a)
			{
				ShowMessageBox("'c' is not equal to 'a' in the Monoclinic(c) crystal system",
					IDC_EDIT_A);
				break;
			}
			if(m_c==m_b)
				ShowMessageBox("'c' is not equal to 'b' in the Monoclinic(c) crystal system",
					IDC_EDIT_B);
			break;
		case TRICLINIC:
			if(m_c==m_a)
			{
				ShowMessageBox("'c' is not equal to 'a' in the Triclinic crystal system",
					IDC_EDIT_A);
				break;
			}
			if(m_c==m_b)
				ShowMessageBox("'c' is not equal to 'b' in the Triclinic crystal system",
					IDC_EDIT_B);
			break;
		case MONOCLINIC_B:
			if(m_c==m_a)
			{
				ShowMessageBox("'c' is not equal to 'a' in the Monoclinic(b) crystal system",
					IDC_EDIT_A);
				break;
			}
			if(m_c==m_b)
				ShowMessageBox("'c' is not equal to 'b' in the Monoclinic(b) crystal system",
					IDC_EDIT_B);
			break;
		case ORTHOROMBIC:
			if(m_c==m_a)
			{
				ShowMessageBox("'c' is not equal to 'a' in the Orthorombic crystal system",
					IDC_EDIT_A);
				break;
			}
			if(m_c==m_b)
				ShowMessageBox("'c' is not equal to 'b' in the Orthorombic crystal system",
					IDC_EDIT_B);
			break;
		default:
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//
void PropertyPageSpaceGroup::OnKillfocusEditAlpha() 
{
	CMadelungDoc* pDoc = GetDoc();

	if(pDoc->m_pSpaceGroup)
	{
// retrieve values from edit controls
		UpdateData(TRUE);

		switch(pDoc->m_pSpaceGroup->m_pUnitCell->m_ID)
		{
		case TRIGONAL:
			if(m_alpha>=120.0)
			{
				ShowMessageBox("'alpha' larger than 120 degrees impossible in the Trigonal crystal system",
					IDC_EDIT_ALPHA);
				break;
			}
			if(m_alpha==90.0)
				ShowMessageBox("'alpha' not equal 90 degrees in the Trigonal crystal system",
					IDC_EDIT_ALPHA);
			break;
		case TRICLINIC:
			if(m_alpha==90.0)
			{
				ShowMessageBox("'alpha' not equal 90 degrees in the Triclinic crystal system",
					IDC_EDIT_ALPHA);
				break;
			}
			if(m_alpha==m_beta)
			{
				ShowMessageBox("'alpha' is not equal 'beta' in the Triclinic crystal system",
					IDC_EDIT_BETA);
				break;
			}
			if(m_alpha==m_gamma)
				ShowMessageBox("'alpha' is not equal 'gamma' in the Triclinic crystal system",
					IDC_EDIT_GAMMA);
			break;
		default:
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
//
//
void PropertyPageSpaceGroup::OnKillfocusEditBeta() 
{
	CMadelungDoc* pDoc = GetDoc();

	if(pDoc->m_pSpaceGroup)
	{
		UpdateData(TRUE);

		switch(pDoc->m_pSpaceGroup->m_pUnitCell->m_ID)
		{
		case TRIGONAL:
			if(m_beta==90.0)
				ShowMessageBox("'beta' is not equal 90 degrees in the Trigonal crystal system",
					IDC_EDIT_BETA);
			break;
		case MONOCLINIC_B:
			if(m_beta<=90.0)
				ShowMessageBox("'beta' is greater than 90 degrees in the Monoclinic(b) crystal system",
					IDC_EDIT_BETA);
			break;
		case TRICLINIC:
			if(m_beta==90.0)
			{
				ShowMessageBox("'beta' is not equal 90 degrees in the Triclinic crystal system",
					IDC_EDIT_BETA);
				break;
			}
			if(m_beta==m_alpha)
			{
				ShowMessageBox("'beta' is not equal 'alpha' in the Triclinic crystal system",
					IDC_EDIT_ALPHA);
				break;
			}
			if(m_beta==m_gamma)
				ShowMessageBox("'beta' is not equal 'gamma' in the Triclinic crystal system",
					IDC_EDIT_GAMMA);
			break;
		default:
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////
//
//
void PropertyPageSpaceGroup::OnKillfocusEditGamma() 
{
	CMadelungDoc* pDoc = GetDoc();

	if(pDoc->m_pSpaceGroup)
	{
		UpdateData(TRUE);

		switch(pDoc->m_pSpaceGroup->m_pUnitCell->m_ID)
		{
		case TRIGONAL:
			if(m_gamma==90.0)
				ShowMessageBox("'gamma' is not equal 90 degrees in the Trigonal crystal system",
					IDC_EDIT_GAMMA);
			break;
		case TRICLINIC:
			if(m_gamma==90.0)
			{
				ShowMessageBox("'gamma' is not equal 90 degrees in the Triclinic crystal system",
					IDC_EDIT_GAMMA);
				break;
			}
			if(m_gamma==m_alpha)
			{
				ShowMessageBox("'gamma' is not equal 'alpha' in the Triclinic crystal system",
					IDC_EDIT_ALPHA);
				break;
			}
			if(m_gamma==m_beta)
				ShowMessageBox("'gamma' is not equal 'beta' in the Triclinic crystal system",
					IDC_EDIT_BETA);
			break;
		case MONOCLINIC_C:
			if(m_gamma<=90.0)
				ShowMessageBox("'gamma' is greater than 90 degrees in the Monoclinic(c) crystal system",
					IDC_EDIT_GAMMA);
			break;
		default:
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////
//	shows msg and returns input focus to dialog box ID
//
void PropertyPageSpaceGroup::ShowMessageBox(string msg,int ID)
{
	AfxMessageBox(msg.c_str());
	CWnd* item;
	item=(CWnd*)GetDlgItem(ID);
	ASSERT_VALID(item);
	GotoDlgCtrl(item);
}