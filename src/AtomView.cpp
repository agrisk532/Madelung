// AtomView.cpp : implementation file
//
//	This is a view in the AtomWindow
//
// !!!!!! This must be implemented not as a view, but listbox !!!!
//

#include "stdafx.h"
#include "AtomView.h"
//#include "MadelungDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AtomView

IMPLEMENT_DYNCREATE(AtomView, CScrollView)

AtomView::AtomView()
{
}

AtomView::~AtomView()
{
}


BEGIN_MESSAGE_MAP(AtomView, CScrollView)
	//{{AFX_MSG_MAP(AtomView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AtomView drawing

void AtomView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

//	CSize sizeTotal;
	// TODO: calculate the total size of this view
//	sizeTotal.cx = sizeTotal.cy = 100;
//	SetScrollSizes(MM_TEXT, sizeTotal);
}

void AtomView::OnDraw(CDC* pDC)
{
	int x = 20;
	int y = 20;
	int lines = this->m_text.size();
	CSize size = pDC->GetTextExtent(CString("Agris"));
	for(int i=0;i<lines;i++)
	{
		pDC->TextOut(x,y,this->m_text[i]);
		y += size.cy;
	}
}

/////////////////////////////////////////////////////////////////////////////
// AtomView diagnostics

#ifdef _DEBUG
void AtomView::AssertValid() const
{
	CScrollView::AssertValid();
}

void AtomView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// AtomView message handlers

void AtomView::Clear()
{
	this->m_text.clear();
//	this->Invalidate();
	return;
}
