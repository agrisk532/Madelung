#if !defined(AFX_PROPERTYPAGESETUPLATTICE_H__9D290085_8EF3_4897_971D_3048A828262D__INCLUDED_)
#define AFX_PROPERTYPAGESETUPLATTICE_H__9D290085_8EF3_4897_971D_3048A828262D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageSetupLattice.h : header file
//

#include "EnumTypes.h"
#include <afx.h>

class UnitCell;
class SimpleCubic;
class BodyCenteredCubic;
class FaceCenteredCubic;
class SimpleTetragonal;
class CenteredTetragonal;
class SimpleOrthorombic;
class BaseCenteredOrthorombic;
class BodyCenteredOrthorombic;
class FaceCenteredOrthorombic;
class SimpleMonoclinic;
class CenteredMonoclinic;
class Triclinic;
class Trigonal;
class Hexagonal;

/////////////////////////////////////////////////////////////////////////////
// PropertyPageSetupLattice dialog

class PropertyPageSetupLattice : public CPropertyPage
{
	DECLARE_DYNCREATE(PropertyPageSetupLattice)

// Construction
public:
	PropertyPageSetupLattice();
	~PropertyPageSetupLattice();

// Dialog Data
	//{{AFX_DATA(PropertyPageSetupLattice)
	enum { IDD = IDD_PP_CRYSTAL_LATTICE };
	double	m_a;
	double	m_b;
	double	m_c;
	double	m_alpha;
	double	m_beta;
	double	m_gamma;
	int		m_crystalType;
	//}}AFX_DATA
// pointers to the objects created in the Document object
	SimpleCubic *m_s_cubic;
	BodyCenteredCubic *m_bc_cubic;
	FaceCenteredCubic *m_fc_cubic;
	SimpleTetragonal *m_s_tetragonal;
	CenteredTetragonal *m_c_tetragonal;
	SimpleOrthorombic *m_s_orthorombic;
	BaseCenteredOrthorombic *m_basec_orthorombic;
	BodyCenteredOrthorombic *m_bodyc_orthorombic;
	FaceCenteredOrthorombic *m_facec_orthorombic;
	SimpleMonoclinic *m_s_monoclinic;
	CenteredMonoclinic *m_c_monoclinic;
	Triclinic *m_triclinic;
	Trigonal *m_trigonal;
	Hexagonal *m_hexagonal;

// used to enable/disable menu items
	bool m_LatticeDataIsOK;
	void SetHintBox(CString);
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropertyPageSetupLattice)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
		void EnableDisableEditBoxes(UnitCell *crystalType);
		void SetEditBoxes(double,double,double,double,double,double);
		void EnableEditABox(bool);
		void EnableEditBBox(bool);
		void EnableEditCBox(bool);
		void EnableEditAlphaBox(bool);
		void EnableEditBetaBox(bool);
		void EnableEditGammaBox(bool);
		void SetEditABox(CString);
		void SetEditBBox(CString);
		void SetEditCBox(CString);
		void SetEditAlphaBox(CString);
		void SetEditBetaBox(CString);
		void SetEditGammaBox(CString);


	// Generated message map functions
	//{{AFX_MSG(PropertyPageSetupLattice)
	afx_msg void OnClsTypeCubic();
	afx_msg void OnClsTypeBcc();
	afx_msg void OnClsTypeFcc();
	afx_msg void OnClsTypeTetragonal();
	afx_msg void OnClsTypeCenteredTetragonal();
	afx_msg void OnClsTypeOrthorombic();
	afx_msg void OnClsTypeBcOrthorombic();
	afx_msg void OnClsTypeBodycenteredOrthorombic();
	afx_msg void OnClsTypeFcOrthorombic();
	afx_msg void OnClsTypeMonoclinic();
	afx_msg void OnClsTypeCenteredMonoclinic();
	afx_msg void OnClsTypeTriclinic();
	afx_msg void OnClsTypeTrigonal();
	afx_msg void OnClsTypeHaxagonal();
	afx_msg void OnChangeEditA();
	afx_msg void OnChangeEditAlpha();
	afx_msg void OnKillfocusEditB();
	afx_msg void OnKillfocusEditC();
	afx_msg void OnKillfocusEditAlpha();
	afx_msg void OnKillfocusEditBeta();
	afx_msg void OnKillfocusEditGamma();
	afx_msg void OnKillfocusEditA();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGESETUPLATTICE_H__9D290085_8EF3_4897_971D_3048A828262D__INCLUDED_)
