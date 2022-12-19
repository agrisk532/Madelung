// EditAtomsInMolecule.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "EditAtomsInMolecule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditAtomsInMolecule dialog


EditAtomsInMolecule::EditAtomsInMolecule(CWnd* pParent /*=NULL*/)
	: CDialog(EditAtomsInMolecule::IDD, pParent)
{
	//{{AFX_DATA_INIT(EditAtomsInMolecule)
	m_type = -1;
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	m_charge = 0.0;
	//}}AFX_DATA_INIT
}


void EditAtomsInMolecule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditAtomsInMolecule)
	DDX_Radio(pDX, IDC_RADIO_H, m_type);
	DDX_Text(pDX, IDC_EDITATOMS_X, m_x);
	DDV_MinMaxDouble(pDX, m_x, -1000., 1000.);
	DDX_Text(pDX, IDC_EDITATOMS_Y, m_y);
	DDV_MinMaxDouble(pDX, m_y, -1000., 1000.);
	DDX_Text(pDX, IDC_EDITATOMS_Z, m_z);
	DDV_MinMaxDouble(pDX, m_z, -1000., 1000.);
	DDX_Text(pDX, IDC_EDITATOMS_CHARGE, m_charge);
	DDV_MinMaxDouble(pDX, m_charge, -100., 100.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EditAtomsInMolecule, CDialog)
	//{{AFX_MSG_MAP(EditAtomsInMolecule)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditAtomsInMolecule message handlers
