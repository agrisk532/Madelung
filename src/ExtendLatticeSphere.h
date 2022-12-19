#if !defined(AFX_EXTENDLATTICESPHERE_H__7056BC3C_B8CF_421B_8701_AC97175B6857__INCLUDED_)
#define AFX_EXTENDLATTICESPHERE_H__7056BC3C_B8CF_421B_8701_AC97175B6857__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtendLatticeSphere.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ExtendLatticeSphere dialog

class ExtendLatticeSphere : public CDialog
{
// Construction
public:
	ExtendLatticeSphere(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ExtendLatticeSphere)
	enum { IDD = IDD_DIALOG_LATTICE_EXTEND_SPHERE };
	int		m_radius;
	BOOL	m_bRenderExtension;
	BOOL	m_bSymmetry;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ExtendLatticeSphere)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ExtendLatticeSphere)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSphereOk();
	afx_msg void OnButtonSphereCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTENDLATTICESPHERE_H__7056BC3C_B8CF_421B_8701_AC97175B6857__INCLUDED_)
