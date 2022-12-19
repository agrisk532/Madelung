#if !defined(AFX_PLOTANATOM_H__868E9241_22FE_4BB2_A2FA_500E6C43C154__INCLUDED_)
#define AFX_PLOTANATOM_H__868E9241_22FE_4BB2_A2FA_500E6C43C154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlotAnAtom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PlotAnAtom dialog

class PlotAnAtom : public CDialog
{
// Construction
public:
	PlotAnAtom(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PlotAnAtom)
	enum { IDD = IDD_ADD_ATOM_TO_PLOT };
	int		m_atom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PlotAnAtom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PlotAnAtom)
	afx_msg void OnDeleteAtom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOTANATOM_H__868E9241_22FE_4BB2_A2FA_500E6C43C154__INCLUDED_)
