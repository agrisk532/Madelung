// TextWindowThread.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "TextWindowThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextWindowThread

IMPLEMENT_DYNCREATE(CTextWindowThread, CWinThread)

CTextWindowThread::CTextWindowThread()
{
	this->m_hEventWindowCreated = CreateEvent(NULL,FALSE,FALSE,NULL);
}

CTextWindowThread::~CTextWindowThread()
{
}

BOOL CTextWindowThread::InitInstance()
{
	CRect rect(100,50,750,250); // left,top,right,bottom
	BOOL bReturn = m_wndTextWindow.Create(_T("Calculation Progress"),
		WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, rect,NULL);
	if(bReturn)
	{
		m_pMainWnd = &m_wndTextWindow;
		m_wndTextWindow.ShowWindow(SW_SHOW);
	}

	::SetEvent(m_hEventWindowCreated);

	return bReturn;
}

int CTextWindowThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CTextWindowThread, CWinThread)
	//{{AFX_MSG_MAP(CTextWindowThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
//	returns the handle to the text window
//
HWND CTextWindowThread::GetTextWindow()
{
	return this->m_wndTextWindow.m_hWnd;
}

//////////////////////////////////////////////////////////////////////////////
//
//
void CTextWindowThread::operator delete(void* p)
{
	CWinThread::operator delete(p);
}


/////////////////////////////////////////////////////////////////////////////
// CTextWindowThread message handlers

