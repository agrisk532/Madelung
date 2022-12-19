#if !defined(AFX_EDITMOLECULE_H__A78B649F_F886_408F_AABD_DE3560D01B03__INCLUDED_)
#define AFX_EDITMOLECULE_H__A78B649F_F886_408F_AABD_DE3560D01B03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditMolecule.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EditMolecule dialog

class EditMolecule : public CDialog
{
// Construction
public:
	EditMolecule(CWnd* pParent = NULL);   // standard constructor
	~EditMolecule();

// Dialog Data
	//{{AFX_DATA(EditMolecule)
	enum { IDD = IDD_EDITMOLECULE };
	CListCtrl	m_atomsList;
	double	m_alpha;
	double	m_beta;
	double	m_gamma;
	CString	m_name;
	double	m_X;
	double	m_Y;
	double	m_Z;
	//}}AFX_DATA

// icons for atoms
	CImageList* m_pImageListSmall;

// atoms in molecule
	vector<int>m_type;
	vector<double>m_x;
	vector<double>m_y;
	vector<double>m_z;
	vector<double>m_charge;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditMolecule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void MakeListHeaders();
	void FillAtomsList();
	void EnableEditButton(bool);
	void EnableRemoveButton(bool);
	void EnableRemoveAllButton(bool);
	void AddAtomToList();
	void RenumberAllItems();
	void Clear();

	// Generated message map functions
	//{{AFX_MSG(EditMolecule)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditmoleculeAdd();
	virtual void OnOK();
	afx_msg void OnEditmoleculeEdit();
	afx_msg void OnEditmoleculeRemoveatom();
	afx_msg void OnEditmoleculeRemoveall();
	afx_msg void OnDblclkEditmoleculeListcontrol(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int size() {return m_x.size();} // number of atoms in the molecule
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMOLECULE_H__A78B649F_F886_408F_AABD_DE3560D01B03__INCLUDED_)
