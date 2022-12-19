#if !defined(AFX_ATOMWINDOW_H__8E8D1D21_A4A2_4B1C_B70E_7525E994631D__INCLUDED_)
#define AFX_ATOMWINDOW_H__8E8D1D21_A4A2_4B1C_B70E_7525E994631D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AtomWindow.h : header file
//

#include <vector>
using namespace std;

class SpaceGroup;
class CMadelungDoc;

/////////////////////////////////////////////////////////////////////////////
// AtomWindow window

class AtomWindow : public CWnd
{
// Construction
public:
	AtomWindow();

// Member variables
public:
	LPCTSTR m_plpszAtomWindowClass;	// points to the window class

protected:
	CButton m_GroupBox;	// group box
	vector<CButton*> m_pWRadio;

// Member functions
public:
	CMadelungDoc* GetDoc();
	void AddStringToView(int i,string str);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AtomWindow)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~AtomWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(AtomWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATOMWINDOW_H__8E8D1D21_A4A2_4B1C_B70E_7525E994631D__INCLUDED_)
