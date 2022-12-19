// PropertyPageSetupLattice.cpp : implementation file
//

#include <afx.h>
#include "stdafx.h"
#include "madelung.h"
#include "PropertyPageSetupLattice.h"
#include "EnumTypes.h"
#include "Resource.h"
#include "UnitCell.h"

#include "SimpleCubic.h"
#include "BodyCenteredCubic.h"
#include "FaceCenteredCubic.h"
#include "SimpleTetragonal.h"
#include "CenteredTetragonal.h"
#include "SimpleOrthorombic.h"
#include "BaseCenteredOrthorombic.h"
#include "BodyCenteredOrthorombic.h"
#include "FaceCenteredOrthorombic.h"
#include "SimpleMonoclinic.h"
#include "CenteredMonoclinic.h"
#include "Triclinic.h"
#include "Trigonal.h"
#include "Hexagonal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropertyPageSetupLattice property page

IMPLEMENT_DYNCREATE(PropertyPageSetupLattice, CPropertyPage)

PropertyPageSetupLattice::PropertyPageSetupLattice() : CPropertyPage(PropertyPageSetupLattice::IDD)
{
	//{{AFX_DATA_INIT(PropertyPageSetupLattice)
	m_a = 2.0;
	m_b = 3.0;
	m_c = 4.0;
	m_alpha = 80.0;
	m_beta = 100.0;
	m_gamma = 95.0;
	m_crystalType = 0;
	//}}AFX_DATA_INIT

	m_s_cubic = NULL;
	m_bc_cubic = NULL;
	m_fc_cubic = NULL;
	m_s_tetragonal = NULL;
	m_c_tetragonal = NULL;
	m_s_orthorombic = NULL;
	m_basec_orthorombic = NULL;
	m_bodyc_orthorombic = NULL;
	m_facec_orthorombic = NULL;
	m_s_monoclinic = NULL;
	m_c_monoclinic = NULL;
	m_triclinic = NULL;
	m_trigonal = NULL;
	m_hexagonal = NULL;

	m_LatticeDataIsOK = FALSE;
}

PropertyPageSetupLattice::~PropertyPageSetupLattice()
{
}

void PropertyPageSetupLattice::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropertyPageSetupLattice)
	DDX_Text(pDX, IDC_EDIT_A, m_a);
	DDV_MinMaxDouble(pDX, m_a, 0., 1000000.);
	DDX_Text(pDX, IDC_EDIT_B, m_b);
	DDV_MinMaxDouble(pDX, m_b, 0., 1000000.);
	DDX_Text(pDX, IDC_EDIT_C, m_c);
	DDV_MinMaxDouble(pDX, m_c, 0., 1000000.);
	DDX_Text(pDX, IDC_EDIT_ALPHA, m_alpha);
	DDV_MinMaxDouble(pDX, m_alpha, 0., 180.);
	DDX_Text(pDX, IDC_EDIT_BETA, m_beta);
	DDV_MinMaxDouble(pDX, m_beta, 0., 180.);
	DDX_Text(pDX, IDC_EDIT_GAMMA, m_gamma);
	DDV_MinMaxDouble(pDX, m_gamma, 0., 180.);
	DDX_Radio(pDX, IDC_CLS_TYPE_CUBIC, m_crystalType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropertyPageSetupLattice, CPropertyPage)
	//{{AFX_MSG_MAP(PropertyPageSetupLattice)
	ON_BN_CLICKED(IDC_CLS_TYPE_CUBIC, OnClsTypeCubic)
	ON_BN_CLICKED(IDC_CLS_TYPE_BCC, OnClsTypeBcc)
	ON_BN_CLICKED(IDC_CLS_TYPE_FCC, OnClsTypeFcc)
	ON_BN_CLICKED(IDC_CLS_TYPE_TETRAGONAL, OnClsTypeTetragonal)
	ON_BN_CLICKED(IDC_CLS_TYPE_CENTERED_TETRAGONAL, OnClsTypeCenteredTetragonal)
	ON_BN_CLICKED(IDC_CLS_TYPE_ORTHOROMBIC, OnClsTypeOrthorombic)
	ON_BN_CLICKED(IDC_CLS_TYPE_BC_ORTHOROMBIC, OnClsTypeBcOrthorombic)
	ON_BN_CLICKED(IDC_CLS_TYPE_BODYCENTERED_ORTHOROMBIC, OnClsTypeBodycenteredOrthorombic)
	ON_BN_CLICKED(IDC_CLS_TYPE_FC_ORTHOROMBIC, OnClsTypeFcOrthorombic)
	ON_BN_CLICKED(IDC_CLS_TYPE_MONOCLINIC, OnClsTypeMonoclinic)
	ON_BN_CLICKED(IDC_CLS_TYPE_CENTERED_MONOCLINIC, OnClsTypeCenteredMonoclinic)
	ON_BN_CLICKED(IDC_CLS_TYPE_TRICLINIC, OnClsTypeTriclinic)
	ON_BN_CLICKED(IDC_CLS_TYPE_TRIGONAL, OnClsTypeTrigonal)
	ON_BN_CLICKED(IDC_CLS_TYPE_HAXAGONAL, OnClsTypeHaxagonal)
	ON_EN_CHANGE(IDC_EDIT_A, OnChangeEditA)
	ON_EN_CHANGE(IDC_EDIT_ALPHA, OnChangeEditAlpha)
	ON_EN_KILLFOCUS(IDC_EDIT_B, OnKillfocusEditB)
	ON_EN_KILLFOCUS(IDC_EDIT_C, OnKillfocusEditC)
	ON_EN_KILLFOCUS(IDC_EDIT_ALPHA, OnKillfocusEditAlpha)
	ON_EN_KILLFOCUS(IDC_EDIT_BETA, OnKillfocusEditBeta)
	ON_EN_KILLFOCUS(IDC_EDIT_GAMMA, OnKillfocusEditGamma)
	ON_EN_KILLFOCUS(IDC_EDIT_A, OnKillfocusEditA)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropertyPageSetupLattice message handlers

BOOL PropertyPageSetupLattice::OnSetActive() 
{
	switch(m_crystalType)
	{
	case SIMPLECUBIC:
		OnClsTypeCubic();
		break;
	case BODYCENTEREDCUBIC:
		OnClsTypeBcc();
		break;
	case FACECENTEREDCUBIC:
		OnClsTypeFcc();
		break;
	case SIMPLETETRAGONAL:
		OnClsTypeTetragonal();
		break;
	case CENTEREDTETRAGONAL:
		OnClsTypeCenteredTetragonal();
		break;
	case SIMPLEORTHOROMBIC:
		OnClsTypeOrthorombic();
		break;
	case BASECENTEREDORTHOROMBIC:
		OnClsTypeBcOrthorombic();
		break;
	case BODYCENTEREDORTHOROMBIC:
		OnClsTypeBodycenteredOrthorombic();
		break;
	case FACECENTEREDORTHOROMBIC:
		OnClsTypeFcOrthorombic();
		break;
	case SIMPLEMONOCLINIC:
		OnClsTypeMonoclinic();
		break;
	case CENTEREDMONOCLINIC:
		OnClsTypeCenteredMonoclinic();
		break;
	case TRICLINIC:
		OnClsTypeTriclinic();
		break;
	case TRIGONAL:
		OnClsTypeTrigonal();
		break;
	case HEXAGONAL:
		OnClsTypeHaxagonal();
		break;
	}
	return CPropertyPage::OnSetActive();
}

/////////////////////////////////////////////////////////////////////////////
// function which enables/disables edit boxes depending on chosen crystal type

void PropertyPageSetupLattice::EnableDisableEditBoxes(UnitCell *crystalType)
{
		EnableEditABox(crystalType->m_enabledAEditing);
		EnableEditBBox(crystalType->m_enabledBEditing);
		EnableEditCBox(crystalType->m_enabledCEditing);
		EnableEditAlphaBox(crystalType->m_enabledAlphaEditing);
		EnableEditBetaBox(crystalType->m_enabledBetaEditing);
		EnableEditGammaBox(crystalType->m_enabledGammaEditing);
}

/////////////////////////////////////////////////////////////////////////////
// function which sets edit boxes values depending on chosen crystal type

void PropertyPageSetupLattice::SetEditBoxes( double a,	// lattice distances
											 double b,
											 double c,
											 double alpha,	// lattice angles
											 double beta,
											 double gamma)
{
		CString text;

		text.Format("%7.4f",a);
		SetEditABox(text);
		text.Format("%7.4f",b);
		SetEditBBox(text);
		text.Format("%7.4f",c);
		SetEditCBox(text);
		text.Format("%7.4f",alpha);
		SetEditAlphaBox(text);
		text.Format("%7.4f",beta);
		SetEditBetaBox(text);
		text.Format("%7.4f",gamma);
		SetEditGammaBox(text);
}

////////////////////////////////////////////////////////////////////////
//

void PropertyPageSetupLattice::OnClsTypeCubic() 
{
	EnableDisableEditBoxes(m_s_cubic);
	SetEditBoxes(m_s_cubic->m_a,m_s_cubic->m_b,m_s_cubic->m_c,
					m_s_cubic->m_alpha,m_s_cubic->m_beta,m_s_cubic->m_gamma);
	this->m_crystalType = SIMPLECUBIC;
	SetHintBox("Note: a=b=c ; alpha=beta=gamma=90deg");
}

void PropertyPageSetupLattice::OnClsTypeBcc() 
{
	EnableDisableEditBoxes(m_bc_cubic);
	SetEditBoxes(m_bc_cubic->m_a,m_bc_cubic->m_b,m_bc_cubic->m_c,
					m_bc_cubic->m_alpha,m_bc_cubic->m_beta,m_bc_cubic->m_gamma);
	this->m_crystalType = BODYCENTEREDCUBIC;
	SetHintBox("Note: a=b=c ; alpha=beta=gamma=90deg");
}

void PropertyPageSetupLattice::OnClsTypeFcc() 
{
	EnableDisableEditBoxes(m_fc_cubic);
	SetEditBoxes(m_fc_cubic->m_a,m_fc_cubic->m_b,m_fc_cubic->m_c,
					m_fc_cubic->m_alpha,m_fc_cubic->m_beta,m_fc_cubic->m_gamma);
	this->m_crystalType = FACECENTEREDCUBIC;
	SetHintBox("Note: a=b=c ; alpha=beta=gamma=90deg");
}

void PropertyPageSetupLattice::OnClsTypeTetragonal() 
{
	EnableDisableEditBoxes(m_s_tetragonal);
	SetEditBoxes(m_s_tetragonal->m_a,m_s_tetragonal->m_b,m_s_tetragonal->m_c,
					m_s_tetragonal->m_alpha,m_s_tetragonal->m_beta,m_s_tetragonal->m_gamma);
	this->m_crystalType = SIMPLETETRAGONAL;
	SetHintBox("Note: a=b!=c ; alpha=beta=gamma=90deg");
}

void PropertyPageSetupLattice::OnClsTypeCenteredTetragonal() 
{
	EnableDisableEditBoxes(m_c_tetragonal);
	SetEditBoxes(m_c_tetragonal->m_a,m_c_tetragonal->m_b,m_c_tetragonal->m_c,
					m_c_tetragonal->m_alpha,m_c_tetragonal->m_beta,m_c_tetragonal->m_gamma);
	this->m_crystalType = CENTEREDTETRAGONAL;
	SetHintBox("Note: a=b!=c ; alpha=beta=gamma=90deg");
}

void PropertyPageSetupLattice::OnClsTypeOrthorombic() 
{
	EnableDisableEditBoxes(m_s_orthorombic);
	SetEditBoxes(m_s_orthorombic->m_a,m_s_orthorombic->m_b,m_s_orthorombic->m_c,
					m_s_orthorombic->m_alpha,m_s_orthorombic->m_beta,m_s_orthorombic->m_gamma);
	this->m_crystalType = SIMPLEORTHOROMBIC;
	SetHintBox("Note: a!=b!=c ; alpha=beta=gamma=90deg");
}

void PropertyPageSetupLattice::OnClsTypeBcOrthorombic() 
{
	EnableDisableEditBoxes(m_basec_orthorombic);
	SetEditBoxes(m_basec_orthorombic->m_a,m_basec_orthorombic->m_b,m_basec_orthorombic->m_c,
					m_basec_orthorombic->m_alpha,m_basec_orthorombic->m_beta,m_basec_orthorombic->m_gamma);
	this->m_crystalType = BASECENTEREDORTHOROMBIC;
	SetHintBox("Note: a!=b!=c ; alpha=beta=gamma=90deg");
}

void PropertyPageSetupLattice::OnClsTypeBodycenteredOrthorombic() 
{
	EnableDisableEditBoxes(m_bodyc_orthorombic);
	SetEditBoxes(m_bodyc_orthorombic->m_a,m_bodyc_orthorombic->m_b,m_bodyc_orthorombic->m_c,
					m_bodyc_orthorombic->m_alpha,m_bodyc_orthorombic->m_beta,
					m_bodyc_orthorombic->m_gamma);
	this->m_crystalType = BODYCENTEREDORTHOROMBIC;
	SetHintBox("Note: a!=b!=c ; alpha=beta=gamma=90deg");
}

void PropertyPageSetupLattice::OnClsTypeFcOrthorombic() 
{
	EnableDisableEditBoxes(m_facec_orthorombic);
	SetEditBoxes(m_facec_orthorombic->m_a,m_facec_orthorombic->m_b,m_facec_orthorombic->m_c,
					m_facec_orthorombic->m_alpha,m_facec_orthorombic->m_beta,
					m_facec_orthorombic->m_gamma);
	this->m_crystalType = FACECENTEREDORTHOROMBIC;
	SetHintBox("Note: a!=b!=c ; alpha=beta=gamma=90deg");
}

void PropertyPageSetupLattice::OnClsTypeMonoclinic() 
{
	EnableDisableEditBoxes(m_s_monoclinic);
	SetEditBoxes(m_s_monoclinic->m_a,m_s_monoclinic->m_b,m_s_monoclinic->m_c,
					m_s_monoclinic->m_alpha,m_s_monoclinic->m_beta,m_s_monoclinic->m_gamma);
	this->m_crystalType = SIMPLEMONOCLINIC;
	SetHintBox("Note: a!=b!=c ; alpha=gamma=90deg ; beta>90deg");
}


void PropertyPageSetupLattice::OnClsTypeCenteredMonoclinic() 
{
	EnableDisableEditBoxes(m_c_monoclinic);
	SetEditBoxes(m_c_monoclinic->m_a,m_c_monoclinic->m_b,m_c_monoclinic->m_c,
					m_c_monoclinic->m_alpha,m_c_monoclinic->m_beta,m_c_monoclinic->m_gamma);
	this->m_crystalType = CENTEREDMONOCLINIC;
	SetHintBox("Note: a!=b!=c ; alpha=gamma=90deg ; beta>90deg");
}


void PropertyPageSetupLattice::OnClsTypeTriclinic() 
{
	EnableDisableEditBoxes(m_triclinic);
	SetEditBoxes(m_triclinic->m_a,m_triclinic->m_b,m_triclinic->m_c,
					m_triclinic->m_alpha,m_triclinic->m_beta,m_triclinic->m_gamma);
	this->m_crystalType = TRICLINIC;
	SetHintBox("Note: a!=b!=c ; alpha!=beta!=gamma!=90deg");
}


void PropertyPageSetupLattice::OnClsTypeTrigonal() 
{
	EnableDisableEditBoxes(m_trigonal);
	SetEditBoxes(m_trigonal->m_a,m_trigonal->m_b,m_trigonal->m_b,
					m_trigonal->m_alpha,m_trigonal->m_beta,m_trigonal->m_gamma);
	this->m_crystalType = TRIGONAL;
	SetHintBox("Note: a=b=c ; alpha=beta=gamma!=90deg");
}

void PropertyPageSetupLattice::OnClsTypeHaxagonal() 
{
	EnableDisableEditBoxes(m_hexagonal);
	SetEditBoxes(m_hexagonal->m_a,m_hexagonal->m_b,m_hexagonal->m_c,
					m_hexagonal->m_alpha,m_hexagonal->m_beta,m_hexagonal->m_gamma);
	this->m_crystalType = HEXAGONAL;
	SetHintBox("Note: a=b!=c ; alpha=beta=90deg ; gamma=120deg");
}

///////////////////////////////////////////////////////////////////
//

void PropertyPageSetupLattice::EnableEditABox(bool status)
{
	CWnd* item;
	item=GetDlgItem(IDC_EDIT_A);
	item->EnableWindow(status);
}

void PropertyPageSetupLattice::EnableEditBBox(bool status)
{
	CWnd* item;
	item=GetDlgItem(IDC_EDIT_B);
	item->EnableWindow(status);
}

void PropertyPageSetupLattice::EnableEditCBox(bool status)
{
	CWnd* item;
	item=GetDlgItem(IDC_EDIT_C);
	item->EnableWindow(status);
}

void PropertyPageSetupLattice::EnableEditAlphaBox(bool status)
{
	CWnd* item;
	item=GetDlgItem(IDC_EDIT_ALPHA);
	item->EnableWindow(status);
}

void PropertyPageSetupLattice::EnableEditBetaBox(bool status)
{
	CWnd* item;
	item=GetDlgItem(IDC_EDIT_BETA);
	item->EnableWindow(status);
}

void PropertyPageSetupLattice::EnableEditGammaBox(bool status)
{
	CWnd* item;
	item=GetDlgItem(IDC_EDIT_GAMMA);
	item->EnableWindow(status);
}


////////////////////////////////////////////////////////////////////////
//

void PropertyPageSetupLattice::SetEditABox(CString text)
{
	CWnd* item;
	item=GetDlgItem(IDC_EDIT_A);
	item->SetWindowText(text);
}

void PropertyPageSetupLattice::SetEditBBox(CString text)
{
	CWnd* item;
	item=GetDlgItem(IDC_EDIT_B);
	item->SetWindowText(text);
}

void PropertyPageSetupLattice::SetEditCBox(CString text)
{
	CWnd* item;
	item=GetDlgItem(IDC_EDIT_C);
	item->SetWindowText(text);
}

void PropertyPageSetupLattice::SetEditAlphaBox(CString text)
{
	CWnd* item;
	item=GetDlgItem(IDC_EDIT_ALPHA);
	item->SetWindowText(text);
}

void PropertyPageSetupLattice::SetEditBetaBox(CString text)
{
	CWnd* item;
	item=GetDlgItem(IDC_EDIT_BETA);
	item->SetWindowText(text);
}

void PropertyPageSetupLattice::SetEditGammaBox(CString text)
{
	CWnd* item;
	item=GetDlgItem(IDC_EDIT_GAMMA);
	item->SetWindowText(text);
}

///////////////////////////////////////////////////////////////////
//

void PropertyPageSetupLattice::OnChangeEditA() 
{
	CString string;
	CEdit* editABox;
	CEdit* editBBox;
	CEdit* editCBox;
	editABox = (CEdit*)GetDlgItem(IDC_EDIT_A);
	editBBox = (CEdit*)GetDlgItem(IDC_EDIT_B);
	editCBox = (CEdit*)GetDlgItem(IDC_EDIT_C);
	
	switch(m_crystalType)
	{
	case SIMPLECUBIC:
	case BODYCENTEREDCUBIC:
	case FACECENTEREDCUBIC:
		editABox->GetWindowText(string);
		editBBox->SetWindowText(string);
		editCBox->SetWindowText(string);
		break;
	case SIMPLETETRAGONAL:
	case CENTEREDTETRAGONAL:
		editABox->GetWindowText(string);
		editBBox->SetWindowText(string);
		break;
	case SIMPLEORTHOROMBIC:
	case BASECENTEREDORTHOROMBIC:
	case BODYCENTEREDORTHOROMBIC:
	case FACECENTEREDORTHOROMBIC:
		break;
	case SIMPLEMONOCLINIC:
	case CENTEREDMONOCLINIC:
		break;
	case TRICLINIC:
		break;
	case TRIGONAL:
		editABox->GetWindowText(string);
		editBBox->SetWindowText(string);
		editCBox->SetWindowText(string);
		break;
	case HEXAGONAL:
		editABox->GetWindowText(string);
		editBBox->SetWindowText(string);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////
//

void PropertyPageSetupLattice::OnChangeEditAlpha() 
{
	CString string;
	CEdit* editAlphaBox;
	CEdit* editBetaBox;
	CEdit* editGammaBox;
	editAlphaBox = (CEdit*)GetDlgItem(IDC_EDIT_ALPHA);
	editBetaBox = (CEdit*)GetDlgItem(IDC_EDIT_BETA);
	editGammaBox = (CEdit*)GetDlgItem(IDC_EDIT_GAMMA);
	
	switch(m_crystalType)
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
/////////////////////////////////////////////////////////////////////////
//
void PropertyPageSetupLattice::OnKillfocusEditA() 
{
	UpdateData(TRUE);

	switch(m_crystalType)
	{
	case SIMPLETETRAGONAL:
	case CENTEREDTETRAGONAL:
	case HEXAGONAL:
		if(m_a==m_c)
		{
			MessageBox("a is not equal to c in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_A);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		break;
	case SIMPLEORTHOROMBIC:
	case BASECENTEREDORTHOROMBIC:
	case BODYCENTEREDORTHOROMBIC:
	case FACECENTEREDORTHOROMBIC:
	case SIMPLEMONOCLINIC:
	case CENTEREDMONOCLINIC:
	case TRICLINIC:
		if(m_a==m_c)
		{
			MessageBox("a is not equal to c in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_A);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		if(m_a==m_b)
		{
			MessageBox("a is not equal to b in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_A);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		break;
	default:
		break;
	}
}


/////////////////////////////////////////////////////////////////////////
//
void PropertyPageSetupLattice::OnKillfocusEditB() 
{
	UpdateData(TRUE);
	switch(m_crystalType)
	{
	case SIMPLEORTHOROMBIC:
	case BASECENTEREDORTHOROMBIC:
	case BODYCENTEREDORTHOROMBIC:
	case FACECENTEREDORTHOROMBIC:
	case SIMPLEMONOCLINIC:
	case CENTEREDMONOCLINIC:
	case TRICLINIC:
		if(m_a==m_b)
		{
			MessageBox("b is not equal to a in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_B);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		if(m_c==m_b)
		{
			MessageBox("b is not equal to c in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_B);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////
//
void PropertyPageSetupLattice::OnKillfocusEditC() 
{
// retrieve values from edit controls
	UpdateData(TRUE);

	switch(m_crystalType)
	{
	case SIMPLETETRAGONAL:
	case CENTEREDTETRAGONAL:
	case HEXAGONAL:
		if(m_c==m_a)
		{
			MessageBox("c is not equal to a in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_C);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		break;
	case SIMPLEORTHOROMBIC:
	case BASECENTEREDORTHOROMBIC:
	case BODYCENTEREDORTHOROMBIC:
	case FACECENTEREDORTHOROMBIC:
	case SIMPLEMONOCLINIC:
	case CENTEREDMONOCLINIC:
	case TRICLINIC:
		if(m_c==m_a)
		{
			MessageBox("c is not equal to a in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_C);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		if(m_c==m_b)
		{
			MessageBox("c is not equal to b in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_C);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////
//
void PropertyPageSetupLattice::OnKillfocusEditAlpha() 
{
// retrieve values from edit controls
	UpdateData(TRUE);

	switch(m_crystalType)
	{
	case TRIGONAL:
		if(m_alpha>=120.0)
		{
			MessageBox("alpha larger than 120 degrees impossible in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_ALPHA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		if(m_alpha==90.0)
		{
			MessageBox("alpha not equal 90 degrees in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_ALPHA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		break;

	case TRICLINIC:
		if(m_alpha==90.0)
		{
			MessageBox("alpha is not equal 90 degrees in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_ALPHA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		if(m_alpha==m_beta)
		{
			MessageBox("alpha is not equal beta in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_ALPHA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		if(m_alpha==m_gamma)
		{
			MessageBox("alpha is not equal gamma in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_ALPHA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////////////
//
void PropertyPageSetupLattice::OnKillfocusEditBeta() 
{
	UpdateData(TRUE);

	switch(m_crystalType)
	{
	case TRIGONAL:
		if(m_beta==90.0)
		{
			MessageBox("beta is not equal 90 degrees in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_BETA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		break;
	case SIMPLEMONOCLINIC:
	case CENTEREDMONOCLINIC:
		if(m_beta<=90.0)
		{
			MessageBox("beta is greater than 90 degrees in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_BETA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		break;
	case TRICLINIC:
		if(m_beta==90.0)
		{
			MessageBox("beta is not equal 90 degrees in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_BETA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		if(m_beta==m_alpha)
		{
			MessageBox("beta is not equal alpha in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_BETA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		if(m_beta==m_gamma)
		{
			MessageBox("beta is not equal gamma in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_BETA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
//

void PropertyPageSetupLattice::OnKillfocusEditGamma() 
{
	UpdateData(TRUE);

	switch(m_crystalType)
	{
	case TRIGONAL:
		if(m_gamma==90.0)
		{
			MessageBox("gamma is not equal 90 degrees in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_GAMMA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		break;
	case TRICLINIC:
		if(m_gamma==90.0)
		{
			MessageBox("gamma is not equal 90 degrees in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_GAMMA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		if(m_gamma==m_alpha)
		{
			MessageBox("gamma is not equal alpha in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_GAMMA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		if(m_gamma==m_beta)
		{
			MessageBox("gamma is not equal beta in this structure");
			CWnd* item;
			item=(CWnd*)GetDlgItem(IDC_EDIT_GAMMA);
			ASSERT_VALID(item);
			GotoDlgCtrl(item);
		}
		break;
	default:
		break;
	}
}


////////////////////////////////////////////////////////////////////////////
//
void PropertyPageSetupLattice::SetHintBox(CString str)
{
	CWnd* item;
	item=(CWnd*)GetDlgItem(IDC_STATIC_LATTICEHINT);
	ASSERT_VALID(item);
	item->SetWindowText(str);
}

