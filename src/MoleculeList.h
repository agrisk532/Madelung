#if !defined(AFX_MOLECULELIST_H__E097120A_16FB_42C2_8654_56628106C0B3__INCLUDED_)
#define AFX_MOLECULELIST_H__E097120A_16FB_42C2_8654_56628106C0B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoleculeList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MoleculeList dialog

class MoleculeList : public CDialog
{
// Construction
public:
	MoleculeList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MoleculeList)
	enum { IDD = IDD_MOLECULELIST };
	CListCtrl	m_moleculesList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MoleculeList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	int m_selectedMolecule; // the number of the selected molecule

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MoleculeList)
	virtual BOOL OnInitDialog();
	afx_msg void OnMoleculesNew();
	afx_msg void OnMoleculesAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void MakeListHeaders();
	void EnableEditButton(bool flag);
	void EnableRemoveButton(bool flag);
	void EnableRemoveAllButton(bool flag);
	void FillMoleculesList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOLECULELIST_H__E097120A_16FB_42C2_8654_56628106C0B3__INCLUDED_)
