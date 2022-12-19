#if !defined(AFX_EDITATOM_H__BF3FCB73_11A1_45DD_8B42_9C0781C7755F__INCLUDED_)
#define AFX_EDITATOM_H__BF3FCB73_11A1_45DD_8B42_9C0781C7755F__INCLUDED_

class AtomWindow;
class CMadelungDoc;

/////////////////////////////////////////////////////////////////////////////
// EditAtom dialog

class EditAtom : public CDialog
{
// Construction
public:
	EditAtom(CWnd* pParent = NULL);   // standard constructor
	virtual ~EditAtom();	// destructor

// Member variables
public:
	AtomWindow* m_pAW;	// atom coordinate display window

// Member functions
protected:
	void LoadWyckoffDescription(int pos);	// loads Wyckoff position desription in text
	void EnableDisableEditXYZ(char Wsymbol); // enable/disable edit X,Y,Z boxes
	CMadelungDoc* GetDoc();

public:
// Dialog Data
	//{{AFX_DATA(EditAtom)
	enum { IDD = IDD_DIALOG_EDIT_ATOM };
	double	m_x;
	double	m_y;
	double	m_z;
	double	m_charge;
	int		m_atomType;
	CString	m_WPosition;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditAtom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Generated message map functions
	//{{AFX_MSG(EditAtom)
	afx_msg void OnEditatomButtonOk();
	afx_msg void OnEditatomButtonCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnEditatomViewcoordinates();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeListEditatomWyck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITATOM_H__BF3FCB73_11A1_45DD_8B42_9C0781C7755F__INCLUDED_)
