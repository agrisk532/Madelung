#if !defined(AFX_SYMMETRYDIALOG_H__E812A6C7_3822_43D4_958D_1966FD9B10FC__INCLUDED_)
#define AFX_SYMMETRYDIALOG_H__E812A6C7_3822_43D4_958D_1966FD9B10FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SymmetryDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SymmetryDialog dialog

class SymmetryDialog : public CDialog
{
// Construction
public:
	SymmetryDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SymmetryDialog)
	enum { IDD = IDD_DIALOG_SYMMETRY };
	CString	m_symmetryText;
	//}}AFX_DATA
// how many times OK clicked? 1 - display listbox with CSymmetry output,
// 2 - close window and return OK to set 'g_pDoc->m_bUseSymmetry == true'
	int OK_ClickedTimes;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SymmetryDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SymmetryDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMMETRYDIALOG_H__E812A6C7_3822_43D4_958D_1966FD9B10FC__INCLUDED_)
