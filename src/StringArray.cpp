// StringArray.cpp: implementation of the StringArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "madelung.h"
#include "StringArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//	critical section to protect data from thread concurrency

StringArray::StringArray()
{

}

StringArray::~StringArray()
{

}

//////////////////////////////////////////////////////////////////////////////
//	thread safe adding of string
//
void StringArray::AddString(CString str)
{
	m_cs.Enter();
	this->m_pStrings.push_back(str);
	m_cs.Leave();
}

///////////////////////////////////////////////////////////////////////////////
//	thread safe clearing of vector
//
void StringArray::Clear()
{
	m_cs.Enter();
	this->m_pStrings.clear();
	m_cs.Leave();
}

///////////////////////////////////////////////////////////////////////////////
//
CString StringArray::GetString(int i)
{
	if(m_pStrings.size() < i)
	{
		TRACE0("StringArray out of range");
		return "";
	}

	return this->m_pStrings[i];
}
