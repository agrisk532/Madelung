// TextWindow.cpp : implementation file
//

#include "stdafx.h"
#include "madelung.h"
#include "MadelungDoc.h"
#include "MainFrm.h"
#include "TextWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern StringArray g_TextOutput;

/////////////////////////////////////////////////////////////////////////////
// TextWindow

TextWindow::TextWindow()
{
	m_LinesInDocument = 0;			// Count of lines in the g_TextOutput
	m_LinesInDisplay = 0;			// Count of lines in display
	m_LineHeight = 0;				// Height of line in pixels
	m_clHeight = 0;					// window height line count
	m_TopLine = 0;					// zero based index of the topmost line
	m_cxClient = 0;					// window horizontal width in pixels
	m_cyClient = 0;					// window vertical height in pixels
	m_leftMargin = 0;				// left margin of the output text
	m_cy = 0;						// vertical position of the next for the output line
	m_bScrollBarClicked = false;	// true if user has clicked on the scrollbar
	m_bScrollBarAtBottom = true;	// true if ScrollBar at the bottom position
	m_bfirstOnSize = true;
}

TextWindow::~TextWindow()
{
}


BEGIN_MESSAGE_MAP(TextWindow, CWnd)
	//{{AFX_MSG_MAP(TextWindow)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_USER_TEXT_WINDOW_ADD_LINE,OnUserCalcInProgress)
	ON_THREAD_MESSAGE(WM_USER_GET_WORK_THREAD_HANDLE,OnUserGetWorkThreadHandle)
	ON_THREAD_MESSAGE(WM_USER_DESTROY_TEXT_WINDOW,OnDestroyTextWindow)
	ON_THREAD_MESSAGE(WM_USER_RESTORE_TEXT_WINDOW,OnRestoreTextWindow)
	ON_THREAD_MESSAGE(WM_USER_CALCULATION_IN_PROGRESS,OnCalculationInProgress)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
BOOL TextWindow::Create(LPCTSTR szTitle, LONG style, const RECT& rect, CWnd* parent)
{
	CBrush brush;
	LPCTSTR lpszTextWindowClass =
			AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
							AfxGetApp()->LoadStandardCursor(IDC_ARROW),
							HBRUSH(brush.CreateStockObject(LTGRAY_BRUSH)),
							LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)));

	HWND parenthWnd = parent->GetSafeHwnd();
	return CWnd::CreateEx(NULL,lpszTextWindowClass,szTitle,style,
				rect.left,rect.top,rect.right,rect.bottom,parenthWnd,NULL,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// TextWindow message handlers

int TextWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreateStockObject(SYSTEM_FONT);
// calculate line height, etc, based on font properties
	CClientDC dc(this);
	TEXTMETRIC tm;
	dc.SelectObject(&m_Font);
	dc.GetTextMetrics(&tm);
	m_LineHeight = tm.tmHeight + tm.tmExternalLeading;
	m_leftMargin = tm.tmAveCharWidth * 2;

	m_LinesInDocument = g_TextOutput.Size();
	return 0;
}


//////////////////////////////////////////////////////////////////////////////
//	displays lines from m_TopLine position
//
void TextWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	dc.SetBkMode(TRANSPARENT);
// select font
	dc.SelectObject(&m_Font);
	int iLine = m_TopLine;	// zero based index of the topline
	int cLastLine = m_TopLine + m_clHeight + 1;
	cLastLine = min(cLastLine,m_LinesInDocument);
// loop through client area coordinates to draw
	int cyLine = 0;
	while(iLine < cLastLine)
	{
// draw line of text
		dc.TextOut(m_leftMargin,cyLine,g_TextOutput.GetString(iLine));
		cyLine += m_LineHeight;
		iLine++;
	}
}


//////////////////////////////////////////////////////////////////////////////////
//
void TextWindow::OnSize(UINT nType, int cx, int cy) 
{
//	if(!m_bfirstOnSize) SuspendThread(m_hWorkThread);
	CWnd::OnSize(nType, cx, cy);
// save new client area
	m_cxClient = cx;
	m_cyClient = cy;
	ResetScrollValues();
//	if(!m_bfirstOnSize) ResumeThread(m_hWorkThread);
	m_bfirstOnSize = FALSE;
}

////////////////////////////////////////////////////////////////////////////////////
//

void TextWindow::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_bScrollBarAtBottom = false;
	m_bScrollBarClicked = true;
	int iTop = m_TopLine;
// modify topline index based on particular scroll button clicked
	switch(nSBCode)
	{
		case SB_BOTTOM:
			iTop = m_LinesInDocument - m_clHeight;
			break;
		case SB_ENDSCROLL:
			break;
		case SB_LINEDOWN:
			iTop++;
			break;
		case SB_LINEUP:
			iTop--;
			break;
		case SB_PAGEDOWN:
			iTop += m_clHeight;
			break;
		case SB_PAGEUP:
			iTop -= m_clHeight;
			break;
		case SB_THUMBPOSITION:
			iTop = nPos;
			break;
		case SB_THUMBTRACK:
			iTop = nPos;
			break;
		case SB_TOP:
			iTop = 0;
			break;
	}
// check range of new index
	iTop = max(iTop,0);
	iTop = min(iTop,m_LinesInDocument - m_clHeight);
// if no change, ignore
	if(iTop == m_TopLine)
		return;
// pixels to scroll = (lines to scroll)*height-per-line
	int cyScroll = (m_TopLine - iTop) * m_LineHeight;
// define new top line value
	m_TopLine = iTop;
// Scroll pixels
	ScrollWindow(0,cyScroll);
// adjust scroll thumb position
	SetScrollPos(SB_VERT,m_TopLine,TRUE);
//	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
// the next lines doesn't work. I do not know why.....
	if(iTop == m_LinesInDocument - m_clHeight + 2)
		m_bScrollBarAtBottom = true;
	else
		m_bScrollBarAtBottom = false;
}

/////////////////////////////////////////////////////////////////////////
//
void TextWindow::ResetScrollValues()
{
// set count of lines in window client height
	m_clHeight = m_cyClient / m_LineHeight;
// hide scroll bars when not needed
	if(m_LinesInDocument <= m_clHeight)
	{
		SetScrollRange(SB_VERT,0,0,TRUE);
		m_TopLine = 0;
// initially m_LinesInDocument == 0 (work thread starts after this window created)
		m_cy = m_LinesInDocument * m_LineHeight;
		return;
	}
// adjust scroll range for new window size
	SetScrollRange(SB_VERT,0,m_LinesInDocument - m_clHeight,TRUE);
// adjust scroll thumb position
	SetScrollPos(SB_VERT,m_TopLine,TRUE);
}

//////////////////////////////////////////////////////////////////////////
// handler for message from the CalculateElStatEnergy thread
// wParam contains the number of text lines in g_TextOutput
//
LRESULT TextWindow::OnUserCalcInProgress(WPARAM wParam,LPARAM lParam)
{
	this->m_LinesInDocument = wParam;
	CClientDC dc(this);
// set text color to COLORREF 0x00000000 = black
	dc.SetTextColor(0x00000000);
// set background mode to transparent
	dc.SetBkMode(TRANSPARENT);
// display lines
	if(m_LinesInDocument<=m_clHeight)
	{
		dc.TextOut(m_leftMargin,m_cy,g_TextOutput.GetString(m_LinesInDocument-1));
		m_cy += m_LineHeight;
		m_LinesInDisplay++;
	}
	else	// m_LinesInDocument > m_cMaxLinesInDisplay
	{
// adjust scroll range for new window size
		SetScrollRange(SB_VERT,0,m_LinesInDocument - m_clHeight,FALSE);
		if(m_bScrollBarClicked)
		{
			if(m_bScrollBarAtBottom)	// this section cannot be reached. I do not know why...
			{
				dc.TextOut(m_leftMargin,m_cy,g_TextOutput.GetString(m_LinesInDocument-1));
				ScrollWindow(0,-m_LineHeight);	// scroll up one line
				// scroll bar to the bottom position
				SetScrollPos(SB_VERT,m_LinesInDocument - m_clHeight,TRUE);
				m_TopLine++;
			}
			else
			{
				SetScrollPos(SB_VERT,m_TopLine,TRUE);
				return 0;
			}
		}
		else	// scroll bar has not been clicked
		{
			dc.TextOut(m_leftMargin,m_cy,g_TextOutput.GetString(m_LinesInDocument-1));
			ScrollWindow(0,-m_LineHeight);
// scroll bar to the bottom position
			SetScrollPos(SB_VERT,m_LinesInDocument - m_clHeight,TRUE);
			m_TopLine++;
			m_bScrollBarAtBottom = true;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
//
//
LRESULT TextWindow::OnUserGetWorkThreadHandle(WPARAM wParam, LPARAM lParam)
{
	this->m_hWorkThread = (HANDLE)wParam;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
void TextWindow::OnClose() 
{
// test if calculation is in progress
	if(WaitForSingleObject(this->m_hEventCalculationInProgress,0) == WAIT_OBJECT_0)
	{
// get the mainframe window handle
		HWND wnd = AfxGetApp()->m_pMainWnd->GetSafeHwnd();
		this->ShowWindow(SW_MINIMIZE);
		::PostMessage(wnd,WM_COMMAND,ID_CANCEL_CALCULATION,0);
	}
	else
	{
		CWnd::OnClose();
	}
}

/////////////////////////////////////////////////////////////////////////////////
//
LRESULT TextWindow::OnDestroyTextWindow(WPARAM wParam, LPARAM lParam)
{
	this->DestroyWindow();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
//
LRESULT TextWindow::OnRestoreTextWindow(WPARAM wParam, LPARAM lParam)
{
	this->ShowWindow(SW_RESTORE);
	this->BringWindowToTop();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
//
LRESULT TextWindow::OnCalculationInProgress(WPARAM wParam, LPARAM lParam)
{
	this->m_hEventCalculationInProgress = (HANDLE) wParam;
	return 0;
}
