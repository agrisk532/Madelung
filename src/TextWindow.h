#if !defined(AFX_TEXTWINDOW_H__7AA0B221_B9C9_4573_BB6D_A5BF86928659__INCLUDED_)
#define AFX_TEXTWINDOW_H__7AA0B221_B9C9_4573_BB6D_A5BF86928659__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextWindow.h : header file
//

#define IDC_TEXT_WND 1
#include "StringArray.h"

/////////////////////////////////////////////////////////////////////////////
// TextWindow window
// it is the main window of another user interface thread
//

class TextWindow : public CWnd
{
// Construction
public:
	TextWindow();
	BOOL Create(LPCTSTR szTitle,LONG Style,const RECT& rect,CWnd* pParent);

	// Attributes
public:

	int m_LinesInDocument;			// Count of lines in the m_pStringArray
	int m_LinesInDisplay;			// count of lines in text window
	int m_LineHeight;				// Height of line in pixels
	int m_clHeight;					// window height line count
	int m_TopLine;					// zero based index of the topmost line
	int m_cxClient;					// window horizontal width in pixels
	int m_cyClient;					// window vertical height in pixels
	CFont m_Font;					// used font
	int m_leftMargin;				// left margin of the output text
	int m_cy;						// vertical position of the next for the output line
	bool m_bScrollBarClicked;		// true if user has clicked on the scrollbar
	bool m_bScrollBarAtBottom;		// true if ScrollBar at the bottom position
	bool m_bfirstOnSize;			// true for the first WM_SIZE message. false after
	HANDLE m_hWorkThread;			// work thread handle
	HANDLE m_hEventCalculationInProgress;	// 'calculation in progress' event handle

// Operations
public:
// displays ALL strings from m_pStringArray
	void DisplayAllStrings();
//	reset scroll values
	void ResetScrollValues();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TextWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TextWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(TextWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LRESULT OnUserCalcInProgress(WPARAM,LPARAM);
	afx_msg LRESULT OnUserGetWorkThreadHandle(WPARAM,LPARAM);
	afx_msg LRESULT OnDestroyTextWindow(WPARAM,LPARAM);
	afx_msg LRESULT OnRestoreTextWindow(WPARAM,LPARAM);
	afx_msg LRESULT OnCalculationInProgress(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTWINDOW_H__7AA0B221_B9C9_4573_BB6D_A5BF86928659__INCLUDED_)
