#if !defined(AFX_PROPERTYPAGESPACEGROUP_H__DF62525B_39C1_4FD9_9F3E_5D9DA8A121DE__INCLUDED_)
#define AFX_PROPERTYPAGESPACEGROUP_H__DF62525B_39C1_4FD9_9F3E_5D9DA8A121DE__INCLUDED_

#include <string>
using namespace std;
#include "resource.h"

class SpaceGroup;
class CMadelungDoc;

/////////////////////////////////////////////////////////////////////////////
// PropertyPageSpaceGroup dialog

class PropertyPageSpaceGroup : public CPropertyPage
{
	DECLARE_DYNCREATE(PropertyPageSpaceGroup)
// Construction
public:
	PropertyPageSpaceGroup();   // standard constructor
	~PropertyPageSpaceGroup();

// Dialog Data
	//{{AFX_DATA(PropertyPageSpaceGroup)
	enum { IDD = IDD_PP_SPACE_GROUP };
	int		m_sg;
	CString	m_sg_edit;
	double	m_a;
	double	m_alpha;
	double	m_b;
	double	m_beta;
	double	m_c;
	double	m_gamma;
	CString	m_pagenumber;
	CString	m_sgnumber;
	CString	m_shortValue;
	CString	m_longValue;
	CString	m_schValue;
	CString	m_hallValue;
	CString	m_pgValue;
	CString	m_crysystValue;
	CString	m_latticeSymbolValue;
	CString	m_addInfoValue;
	CString	m_distanceRestrictions;
	CString	m_angleRestrictions;
	//}}AFX_DATA
// Member variables
public:
	SpaceGroup* m_pSpaceGroup; 
// constructs/destructs the SG when user click on the 'LOAD' button
// Member functions
public:
	void CreateSpaceGroup(int SGNumber);	// creates the group from the number in tables
	int GetSGNumber();	// returns the number of the space group from the symbol
	bool DeleteSpaceGroup();
//	clears all items in the PropertyPageAtomsInUnitCell
//	used when loading new SpaceGroup
	void ClearPropertyPageAtomsInUnitCell();
// Clears the CMadelungView;
	void ClearView();
protected:
	string CStringToString(CString txt);
	CMadelungDoc* GetDoc();
	void LoadSGDescription();
	void EnableDisableEditBoxes();
// checks and converts string name to the number according to International tables
	int GetSgNumber(string sgName);
//	Removes blank characters and converts string to lowercase
	CString ConvertSGEditString();
//	checks whether specified SpaceGroup is already loaded
	bool IsSpaceGroupLoaded(CString str);
// displays a message box with warning and returns input focus to editbox
// when unitcell parameters are not correct
	void ShowMessageBox(string msg,int ID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PropertyPageSpaceGroup)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PropertyPageSpaceGroup)
	afx_msg void OnButtonSgsetupLoad();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioSpgroupsFullHm();
	afx_msg void OnRadioSpgroupsHall();
	afx_msg void OnRadioSpgroupsNumber();
	afx_msg void OnRadioSpgroupsSchoenflies();
	afx_msg void OnRadioSpgroupsShortHm();
	afx_msg void OnChangeEditA();
	afx_msg void OnChangeEditAlpha();
	afx_msg void OnKillfocusEditA();
	afx_msg void OnKillfocusEditB();
	afx_msg void OnKillfocusEditC();
	afx_msg void OnKillfocusEditAlpha();
	afx_msg void OnKillfocusEditBeta();
	afx_msg void OnKillfocusEditGamma();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGESPACEGROUP_H__DF62525B_39C1_4FD9_9F3E_5D9DA8A121DE__INCLUDED_)
