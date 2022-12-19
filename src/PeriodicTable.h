#if !defined(AFX_PERIODICTABLE_H__C3D322D4_4F6F_4ECA_B4F0_856ED84078F8__INCLUDED_)
#define AFX_PERIODICTABLE_H__C3D322D4_4F6F_4ECA_B4F0_856ED84078F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PeriodicTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PeriodicTable dialog

class PeriodicTable : public CDialog
{
// Construction
public:
	PeriodicTable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PeriodicTable)
	enum { IDD = IDD_DIALOG_PERIODICTABLE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PeriodicTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PeriodicTable)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERIODICTABLE_H__C3D322D4_4F6F_4ECA_B4F0_856ED84078F8__INCLUDED_)
