#if !defined(AFX_EXTENDLATTICE_H__2CC11CAE_58B7_4CD9_A8BB_4C6E7FD95163__INCLUDED_)
#define AFX_EXTENDLATTICE_H__2CC11CAE_58B7_4CD9_A8BB_4C6E7FD95163__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtendLattice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ExtendLattice dialog

class ExtendLattice : public CDialog
{
// Construction
public:
	ExtendLattice(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ExtendLattice)
	enum { IDD = IDD_DIALOG_LATTICE_EXTEND };
	int		m_na;
	int		m_nb;
	int		m_nc;
	BOOL	m_bRenderExtension;
	BOOL	m_bSymmetry;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ExtendLattice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ExtendLattice)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonExtendOk();
	afx_msg void OnButtonExtendCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTENDLATTICE_H__2CC11CAE_58B7_4CD9_A8BB_4C6E7FD95163__INCLUDED_)
