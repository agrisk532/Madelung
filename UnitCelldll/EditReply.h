// EditReply.h: interface for the EditReply class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITREPLY_H__EE620831_9F85_49D8_AA77_2A7316BEB358__INCLUDED_)
#define AFX_EDITREPLY_H__EE620831_9F85_49D8_AA77_2A7316BEB358__INCLUDED_

#include "win32Header.h"
#include <string>
using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class UNITCELLDLL_API EditReply  
{
public:
	EditReply();
	virtual ~EditReply();

// Member variables
	string m_text1;	// text in the 1st edit box
	string m_text2;	// text in the 2nd edit box
	string m_text3;	// text in the 3rd edit box
};

#endif // !defined(AFX_EDITREPLY_H__EE620831_9F85_49D8_AA77_2A7316BEB358__INCLUDED_)
