#if !defined(AFX_PROPERTYPAGEMOLECULESINUNITCELL_H__25791133_8414_482B_BAA5_82DFE3B545EA__INCLUDED_)
#define AFX_PROPERTYPAGEMOLECULESINUNITCELL_H__25791133_8414_482B_BAA5_82DFE3B545EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageMoleculesInUnitCell.h : header file
//
#include <vector>
#include <string>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// PropertyPageMoleculesInUnitCell dialog

class PropertyPageMoleculesInUnitCell : public CPropertyPage
{
	DECLARE_DYNCREATE(PropertyPageMoleculesInUnitCell)

// Construction
public:
	PropertyPageMoleculesInUnitCell();
	~PropertyPageMoleculesInUnitCell();

// for molecules
// used to store the atoms in molecules, later copied to g_pDoc m_molecules
	vector<Molecule*> m_ObjMolecules;

// Dialog Data
	//{{AFX_DATA(PropertyPageMoleculesInUnitCell)
	enum { IDD = IDD_PP_MOLECULESINUNITCELL };
	CListCtrl	m_moleculesList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropertyPageMoleculesInUnitCell)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PropertyPageMoleculesInUnitCell)
	virtual BOOL OnInitDialog();
	afx_msg void OnPpMoleculesinunitcellAdd();
	afx_msg void OnPpMoleculesinunitcellEdit();
	afx_msg void OnPpMoleculesinunitcellRemove();
	afx_msg void OnPpMoleculesinunitcellRemoveall();
	afx_msg void OnDblclkPpMoleculesinunitcellListcontrol(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void MakeListHeaders(void);
	void EnableEditButton(bool);
	void EnableRemoveButton(bool);
	void EnableRemoveAllButton(bool);
	void FillMoleculesList(void);
	void RenumberAllItems(void);
	void Clear();
	void AddMoleculeToList();
public:
	int size() {return m_ObjMolecules.size();}
	bool m_MoleculesAreReady; // used to enable/disable menu item
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEMOLECULESINUNITCELL_H__25791133_8414_482B_BAA5_82DFE3B545EA__INCLUDED_)
