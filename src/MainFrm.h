// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__BC470DA7_9756_4031_A0B9_BFB76ACC19A4__INCLUDED_)
#define AFX_MAINFRM_H__BC470DA7_9756_4031_A0B9_BFB76ACC19A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMadelungDoc;
class PropertyPageSpaceGroup;
class SpaceGroup;
class AtomWindow;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	void SetMenus(UINT menu);
	void LoadSGDescription(PropertyPageSpaceGroup &sgPage);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
// true if progress bar in status bar	(=1 if with, 0 if without)
	int m_StatusBarWithProgressBar;
	CProgressCtrl m_wndProgressBar;
	bool m_ProgressBarCreated;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CDialogBar      m_wndDlgBar;
	CMadelungDoc* GetDoc();
	AtomWindow* m_pAW;
// TRUE if 1st function in the charge approx menu checked
	bool m_FitFunction_1;
	bool m_FitFunction_2;
	bool m_FitFunction_3;
	bool m_FitFunction_4;
	bool m_FitFunction_5;
	bool m_FitFunction_6;
	bool m_FitFunction_7;
	bool m_FitFunction_8;
	bool m_FitFunction_9;
// Generated message map functions
	protected:
	LRESULT OnOptJobDone(WPARAM wParam,LPARAM lParam);
	LRESULT OnCalcInProgress(WPARAM wParam,LPARAM lParam);
	LRESULT OnToggleStatusBar(WPARAM wParam,LPARAM lParam);
	LRESULT OnCreateProgressBar(WPARAM wParam,LPARAM lParam);
	LRESULT OnDestroyProgressBar(WPARAM wParam,LPARAM lParam);
	LRESULT OnModifyMainframeMenu(WPARAM wParam,LPARAM lParam);
	LRESULT OnPotentialsOK(WPARAM wParam,LPARAM lParam);
public:
	void ResetProgressBar();
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToolsSetupLattice();
	afx_msg void OnUpdateToolsPcdf1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsPcdf2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsPcdf3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsPcdf4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsPcdf5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsPcdf6(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsPcdf7(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsPcdf8(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsPcdf9(CCmdUI* pCmdUI);
	afx_msg void OnToolsPcdf1();
	afx_msg void OnToolsPcdf2();
	afx_msg void OnToolsPcdf3();
	afx_msg void OnToolsPcdf4();
	afx_msg void OnToolsPcdf5();
	afx_msg void OnToolsPcdf6();
	afx_msg void OnToolsPcdf7();
	afx_msg void OnToolsPcdf8();
	afx_msg void OnToolsPcdf9();
	afx_msg void OnPlotUnitcell();
	afx_msg void OnUpdatePlotUnitcell(CCmdUI* pCmdUI);
	afx_msg void OnViewWyckoff();
	afx_msg void OnUpdateViewWyckoff(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPlotAtom();
	afx_msg void OnDeleteAtom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__BC470DA7_9756_4031_A0B9_BFB76ACC19A4__INCLUDED_)
