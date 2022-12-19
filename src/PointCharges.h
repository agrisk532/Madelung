// PointCharges.h: interface for the PointCharges class.
//	optimization routine accepts function f(x), with double x[].
// This class serves to pass other parameters into f()
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTCHARGES_H__E79F7361_D73A_4466_A7F3_E3C5A89B0779__INCLUDED_)
#define AFX_POINTCHARGES_H__E79F7361_D73A_4466_A7F3_E3C5A89B0779__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMadelungDoc;

class PointCharges  
{
private:
	CMadelungDoc* m_pDoc;
public:
	PointCharges(CMadelungDoc* pDoc) {m_pDoc = pDoc;}
	virtual ~PointCharges() {};
// f(x) is used in optimization routine
	double f(double x[]);

};

#endif // !defined(AFX_POINTCHARGES_H__E79F7361_D73A_4466_A7F3_E3C5A89B0779__INCLUDED_)
