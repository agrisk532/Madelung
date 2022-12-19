///////////////////////////////////////////////////////////////////
//
//	this class contains description of all implemented space groups

#include "stdafx.h"
#include "spg_names.h"

///////////////////////////////////////////////////////////////////
// construction/destruction

spg_names::spg_names()
{
	sgNumber[0] = 0;		sgFullName[0] = "";					sgShortName[0] = "";		sgSchoenflies[0] = "";			sgHall[0] = "";
	sgNumber[1] = 1;		sgFullName[1] = "p1";				sgShortName[1] = "p1";		sgSchoenflies[1] = "c1(1)";		sgHall[1] = "";
	sgNumber[2] = 2;		sgFullName[2] = "p_1";				sgShortName[2] = "p_1";		sgSchoenflies[2] = "ci(1)";		sgHall[2] = "";
	sgNumber[3] = 3001;		sgFullName[3] = "p121";				sgShortName[3] = "p2:b";	sgSchoenflies[3] = "c2(1)";		sgHall[3] = "";
	sgNumber[4] = 3002;		sgFullName[4] = "p112";				sgShortName[4] = "p2:c";	sgSchoenflies[4] = "c2(1)";		sgHall[4] = "";
	sgNumber[5] = 4001;		sgFullName[5] = "p12s11";			sgShortName[5] = "p2s1:b";	sgSchoenflies[5] = "c2(2)";		sgHall[5] = "";
	sgNumber[6] = 4002;		sgFullName[6] = "p112s1";			sgShortName[6] = "p2s1:c";	sgSchoenflies[6] = "c2(2)";		sgHall[6] = "";
	sgNumber[7] = 5001;		sgFullName[7] = "c121";				sgShortName[7] = "c2:b";	sgSchoenflies[7] = "c2(3)";		sgHall[7] = "";
	sgNumber[8] = 5002;		sgFullName[8] = "a121";				sgShortName[8] = "c2:b";	sgSchoenflies[8] = "c2(3)";		sgHall[8] = "";
	sgNumber[9] = 5003;		sgFullName[9] = "i121";				sgShortName[9] = "c2:b";	sgSchoenflies[9] = "c2(3)";		sgHall[9] = "";
	sgNumber[10] = 5004;	sgFullName[10] = "a112";			sgShortName[10] = "c2:c";	sgSchoenflies[10] = "c2(3)";	sgHall[10] = "";
	sgNumber[11] = 5005;	sgFullName[11] = "b112";			sgShortName[11] = "c2:c";	sgSchoenflies[11] = "c2(3)";	sgHall[11] = "";
	sgNumber[12] = 5006;	sgFullName[12] = "i112";			sgShortName[12] = "c2:c";	sgSchoenflies[12] = "c2(3)";	sgHall[12] = "";
	sgNumber[13] = 6001;	sgFullName[13] = "p1m1";			sgShortName[13] = "pm:b";	sgSchoenflies[13] = "cs(1)";	sgHall[13] = "";
	sgNumber[14] = 6002;	sgFullName[14] = "p11m";			sgShortName[14] = "pm:c";	sgSchoenflies[14] = "cs(1)";	sgHall[14] = "";
	sgNumber[15] = 63;		sgFullName[15] = "c2/m2/c2s1/m";	sgShortName[15] = "cmcm";	sgSchoenflies[15] = "d2h(17)";	sgHall[15] = "";
	sgNumber[16] = 195;		sgFullName[16] = "p23";				sgShortName[16] = "p23";	sgSchoenflies[16] = "t(1)";		sgHall[16] = "";
	sgNumber[17] = 47;		sgFullName[17] = "p2/m2/m2/m";		sgShortName[17] = "pmmm";	sgSchoenflies[17] = "d2h(1)";	sgHall[17] = "";
	sgNumber[18] = 71;		sgFullName[18] = "i2/m2/m2/m";		sgShortName[18] = "immm";	sgSchoenflies[18] = "d2h(25)";	sgHall[18] = "";
	sgNumber[19] = 64;		sgFullName[19] = "c2/m2/c2s1/a";	sgShortName[19] = "cmca";	sgSchoenflies[19] = "d2h(18)";	sgHall[19] = "";
	sgNumber[20] = 10001;	sgFullName[20] = "p112/m";			sgShortName[20] = "p2/m";	sgSchoenflies[20] = "c2h(1)";	sgHall[20] = "";
	sgNumber[21] = 10002;	sgFullName[21] = "p12/m1";			sgShortName[21] = "p2/m";	sgSchoenflies[21] = "c2h(1)";	sgHall[21] = "";
}

spg_names::~spg_names()
{}

////////////////////////////////////////////////////////////////////////
//	returns the number of the sg (as implemented) - see the constructor
int spg_names::sgdll_GetFull(string txt)
{
	for(int i=0;i<implementedSpaceGroups;i++)	
	{
		if(!sgFullName[i].compare(txt))		// if FullName exists in the list
			return sgNumber[i];
	}
	return -1;		// not found
}

/////////////////////////////////////////////////////////////////////////
//	returns the number of the sg (as implemented) - see the constructor
int spg_names::sgdll_GetShort(string txt)
{
	for(int i=0;i<implementedSpaceGroups;i++)	
	{
		if(!sgShortName[i].compare(txt))		// if FullName exists in the list
			return sgNumber[i];
	}
	return -1;		// not found
}

/////////////////////////////////////////////////////////////////////////
//	returns the number of the sg (as implemented) - see the constructor
int spg_names::sgdll_GetSchoenflies(string txt)
{
	for(int i=0;i<implementedSpaceGroups;i++)	
	{
		if(!sgSchoenflies[i].compare(txt))		// if FullName exists in the list
			return sgNumber[i];
	}
	return -1;		// not found
}

/////////////////////////////////////////////////////////////////////////
//	returns the number of the sg (as implemented) - see the constructor
int spg_names::sgdll_GetNumber(string txt)
{
	for(int i=0;i<implementedSpaceGroups;i++)	
	{
		int value = atoi(txt.c_str());
		if (value>1000) return value/1000;
		else
			return value;
	}
	return -1;	// not found
}

/////////////////////////////////////////////////////////////////////////
//	returns the number of the sg (as implemented) - see the constructor
int spg_names::sgdll_GetHall(string txt)
{
	for(int i=0;i<implementedSpaceGroups;i++)	
	{
		if(!sgHall[i].compare(txt))		// if FullName exists in the list
			return sgNumber[i];
	}
	return -1;		// not found
}
