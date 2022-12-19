// TestReply.h: interface for the TestReply class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTREPLY_H__C3DBD4AD_4C5C_4C09_ABB4_85EBD6619948__INCLUDED_)
#define AFX_TESTREPLY_H__C3DBD4AD_4C5C_4C09_ABB4_85EBD6619948__INCLUDED_

#include "win32Header.h"

#include <string>
using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class UNITCELLDLL_API TestReply  
{
public:
	TestReply();
	virtual ~TestReply();

// Member Variables
	string m_Message;		// return message (hint to user what to do)
	bool m_ReturnValue;	// return value - true if a,b,c or alpha,beta,gamma validated
// Member Functions

};

#endif // !defined(AFX_TESTREPLY_H__C3DBD4AD_4C5C_4C09_ABB4_85EBD6619948__INCLUDED_)
