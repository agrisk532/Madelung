#if !defined(AFX_PROPERTYPAGEATOMSINUNITCELL_H__10E792DB_02F7_4A81_A8DE_88DC47C8BF92__INCLUDED_)
#define AFX_PROPERTYPAGEATOMSINUNITCELL_H__10E792DB_02F7_4A81_A8DE_88DC47C8BF92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageAtomsInUnitCell.h : header file
//

#include <vector>
using namespace std;

class CMadelungDoc;

/////////////////////////////////////////////////////////////////////////////
// PropertyPageAtomsInUnitCell dialog

class PropertyPageAtomsInUnitCell : public CPropertyPage
{
	DECLARE_DYNCREATE(PropertyPageAtomsInUnitCell)

// Construction
public:
	PropertyPageAtomsInUnitCell();
	~PropertyPageAtomsInUnitCell();

// Member variables
	vector<CString> m_WPosition;	// Wyckoff position
	vector<double> m_x;	// current/selected atoms coordinate
	vector<double> m_y;
	vector<double> m_z;
	vector<double> m_charge; // current/selected atom charge
	vector<int>	m_atomType; // current/selected atom type
// for insert/delete operations in vector
	vector<CString>::iterator m_iterator_Wyckoff;	
	vector<double>::iterator m_iterator_x; 
	vector<double>::iterator m_iterator_y;
	vector<double>::iterator m_iterator_z;
	vector<double>::iterator m_iterator_charge;
	vector<int>::iterator m_iterator_type;
	int m_NumberOfAtomsInUnitCell;	// Wyckoff positions (one atom per position)
	CImageList *m_pImageListSmall;
	bool m_AtomCoordinatesAreReady; // used to enable/disable menu item

// Member functions
protected:
	void AddAtomToList();
	void MakeListHeaders();
	void EnableAddButton(bool);
	void EnableEditButton(bool);
	void EnableRemoveButton(bool);
	void EnableRemoveAllButton(bool flag);
	void RenumberAllItems(); // used after user deletes an item from the listcontrol box
	void FillAtomsList(); // fills the listcontrol with items from the document
	CMadelungDoc* GetDoc();

// Dialog Data
public:
	//{{AFX_DATA(PropertyPageAtomsInUnitCell)
	enum { IDD = IDD_PP_ATOMSINUNITCELL };
	CListCtrl	m_atomsList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropertyPageAtomsInUnitCell)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
// clears all items from m_atomsList
	void Clear();
protected:

	// Generated message map functions
	//{{AFX_MSG(PropertyPageAtomsInUnitCell)
	afx_msg void OnPpAtomsinunitcellAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnPpEditatomsRemoveatom();
	afx_msg void OnPpAtomsinunitcellEdit();
	afx_msg void OnPpEditatomsRemoveall();
	afx_msg void OnDblclkPpAtomsinunitcellListcontrol(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEATOMSINUNITCELL_H__10E792DB_02F7_4A81_A8DE_88DC47C8BF92__INCLUDED_)
