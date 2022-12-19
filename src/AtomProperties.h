// AtomProperties.h: interface for the AtomProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATOMPROPERTIES_H__22AD7B3C_4C5F_44FC_9193_663E522842A0__INCLUDED_)
#define AFX_ATOMPROPERTIES_H__22AD7B3C_4C5F_44FC_9193_663E522842A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AtomProperties : public CObject  
{
public:
	AtomProperties();
	virtual ~AtomProperties();


	double m_Radius[77];	// contains atom radii (in Angstroms)
	double m_Charge[77];	// contains common oxidation state
	double m_Color[77][3];	// contains rgb color
	CString m_Type[77];

};

#endif // !defined(AFX_ATOMPROPERTIES_H__22AD7B3C_4C5F_44FC_9193_663E522842A0__INCLUDED_)
