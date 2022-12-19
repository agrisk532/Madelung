// Madelung.h : main header file for the MADELUNG application
//

#if !defined(AFX_MADELUNG_H__2B7F59FD_201B_4CC7_A8E5_41CB1CFCD0DA__INCLUDED_)
#define AFX_MADELUNG_H__2B7F59FD_201B_4CC7_A8E5_41CB1CFCD0DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// custom definitions
#define NUMBER_OF_ATOMS_IN_PERIODIC_TABLE 76
// messages (sent from worker threads to CMainFrame)
#define WM_USER_CALC_IN_PROGRESS		(WM_USER+1)
#define WM_USER_CREATE_PROGRESS_BAR		(WM_USER+2)
#define WM_USER_TOGGLE_STATUS_BAR		(WM_USER+3)
#define WM_USER_DESTROY_PROGRESS_BAR	(WM_USER+4)
#define WM_USER_MODIFY_MAINFRAME_MENU	(WM_USER+5)	// include/remove the 'Cancel' menu item
// text window thread messages
#define WM_USER_TEXT_WINDOW_ADD_LINE	(WM_USER+6)
#define WM_USER_GET_WORK_THREAD_HANDLE	(WM_USER+7)
#define WM_USER_DESTROY_TEXT_WINDOW		(WM_USER+8)	// if user cancels work thread
#define WM_USER_RESTORE_TEXT_WINDOW		(WM_USER+9)	// restores text window from minimized
// used to set/reset 'calculation in progress' event (defined in CMadelungDoc)
#define WM_USER_CALCULATION_IN_PROGRESS	(WM_USER+10)
// worker thread handler message to MainFrm about status of jobs
#define WM_USER_POTENTIALS_OK			(WM_USER+11)
// worker thread sends this message to the main thread that an optimization job has ended
// when last has ended, main thread averages charges and submits another set of jobs which
// calculates el.stat. potential on selected atoms and displays results
#define	WM_USER_OPTJOB_DONE				(WM_USER+12)

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMadelungApp:
// See Madelung.cpp for the implementation of this class
//

class CMadelungApp : public CWinApp
{
public:
	CMadelungApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMadelungApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMadelungApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MADELUNG_H__2B7F59FD_201B_4CC7_A8E5_41CB1CFCD0DA__INCLUDED_)
