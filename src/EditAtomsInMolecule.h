#if !defined(AFX_EDITATOMSINMOLECULE_H__F896B8F3_D024_4732_BF18_4AD4E9AA3A3F__INCLUDED_)
#define AFX_EDITATOMSINMOLECULE_H__F896B8F3_D024_4732_BF18_4AD4E9AA3A3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditAtomsInMolecule.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EditAtomsInMolecule dialog

class EditAtomsInMolecule : public CDialog
{
// Construction
public:
	EditAtomsInMolecule(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EditAtomsInMolecule)
	enum { IDD = IDD_EDIT_ATOMS_IN_MOLECULE };
	int		m_type;
	double	m_x;
	double	m_y;
	double	m_z;
	double	m_charge;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditAtomsInMolecule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EditAtomsInMolecule)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITATOMSINMOLECULE_H__F896B8F3_D024_4732_BF18_4AD4E9AA3A3F__INCLUDED_)
