#if !defined(AFX_NUMBEROFUNITCELLSTOPLOT_H__916D930A_314D_4F18_8C63_F652034E2813__INCLUDED_)
#define AFX_NUMBEROFUNITCELLSTOPLOT_H__916D930A_314D_4F18_8C63_F652034E2813__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NumberOfUnitCellsToPlot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NumberOfUnitCellsToPlot dialog

class CMadelungDoc;

class NumberOfUnitCellsToPlot : public CDialog
{
// Construction
public:
	NumberOfUnitCellsToPlot(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NumberOfUnitCellsToPlot)
	enum { IDD = IDD_PLOT_UNITCELLS };
	int		m_a;
	int		m_b;
	int		m_c;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NumberOfUnitCellsToPlot)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NumberOfUnitCellsToPlot)
	afx_msg void OnButtonUnitcellsOk();
	afx_msg void OnButtonUnitcellsCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CMadelungDoc* NumberOfUnitCellsToPlot::GetDoc();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMBEROFUNITCELLSTOPLOT_H__916D930A_314D_4F18_8C63_F652034E2813__INCLUDED_)
