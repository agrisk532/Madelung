// StringArray.h: interface for the StringArray class.
//	this class is thread-safe
//	many work threads (one per atom) write to it
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGARRAY_H__802EB393_13D8_4A6E_A288_95F17BB6FA62__INCLUDED_)
#define AFX_STRINGARRAY_H__802EB393_13D8_4A6E_A288_95F17BB6FA62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "CMcl.h"
using namespace std;

class StringArray : public CObject  
{
public:
	CMclCritSec m_cs;	// the main thread initializes it
public:
	StringArray();
	virtual ~StringArray();
protected:
	vector<CString> m_pStrings;

public:
	void AddString(CString str);
	void Clear();
	CString GetString(int);
	int Size() {return m_pStrings.size();}
};

#endif // !defined(AFX_STRINGARRAY_H__802EB393_13D8_4A6E_A288_95F17BB6FA62__INCLUDED_)
