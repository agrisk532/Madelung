#if !defined(AFX_ATOMVIEW_H__EE0966B0_251F_4B53_97E3_A6BB29EE9243__INCLUDED_)
#define AFX_ATOMVIEW_H__EE0966B0_251F_4B53_97E3_A6BB29EE9243__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AtomView.h : header file
//

#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// AtomView view

class AtomView : public CScrollView
{
public:
	AtomView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(AtomView)

//	Member variables
public:
	vector<CString> m_text;	// contains text for output

// Member functions
	void Clear();	// clears the window

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AtomView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~AtomView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(AtomView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATOMVIEW_H__EE0966B0_251F_4B53_97E3_A6BB29EE9243__INCLUDED_)
