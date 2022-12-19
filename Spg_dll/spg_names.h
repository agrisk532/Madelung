///////////////////////////////////////////////////////////////////////
//
// header file for the spg_names class
//
///////////////////////////////////////////////////////////////////////

#if !defined __spg_names_h
#define __spg_names_h

#include "win32HeaderSG.h"	//	dll export/import stuff
#include <string>
using namespace std;

////////////////////////////////////////////////////////////////////////
// spacegroup names
////////////////////////////////////////////////////////////////////////

const int implementedSpaceGroups = 22;

class SPG_DLL_API spg_names
{
public:
	int sgNumber[implementedSpaceGroups];
	string sgFullName[implementedSpaceGroups];
	string sgShortName[implementedSpaceGroups];
	string sgSchoenflies[implementedSpaceGroups];
	string sgHall[implementedSpaceGroups];
public:
	spg_names();
	virtual ~spg_names();
	int sgdll_GetFull(string txt);	// returns the number of the group from the Full HM symbol
	int sgdll_GetShort(string txt);	// returns the number from the short symbol
	int sgdll_GetNumber(string txt);// returns the number of the space group
	int sgdll_GetSchoenflies(string txt);	// returns the number of the group
	int sgdll_GetHall(string txt);	// returns the number of the group

};

#endif
