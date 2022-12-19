#if !defined(AFX_TEXTWINDOWTHREAD_H__45F5FBC5_03D7_4CA8_9615_271BBD1D8135__INCLUDED_)
#define AFX_TEXTWINDOWTHREAD_H__45F5FBC5_03D7_4CA8_9615_271BBD1D8135__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextWindowThread.h : header file
//

#include "TextWindow.h"
#include "StringArray.h"

/////////////////////////////////////////////////////////////////////////////
// CTextWindowThread thread

class CTextWindowThread : public CWinThread
{
	DECLARE_DYNCREATE(CTextWindowThread)
public:
	CTextWindowThread();           // protected constructor used by dynamic creation
	void operator delete(void *p);

// Attributes
public:

protected:
	TextWindow m_wndTextWindow;
// Operations
public:
	HWND GetTextWindow();	// returns handle to the textwindow
// signals when window has been created and is ready for painting
	HANDLE m_hEventWindowCreated;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextWindowThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTextWindowThread();

	// Generated message map functions
	//{{AFX_MSG(CTextWindowThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTWINDOWTHREAD_H__45F5FBC5_03D7_4CA8_9615_271BBD1D8135__INCLUDED_)
