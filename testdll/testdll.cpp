// testdll.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "spg_names.h"
#include "string.h"
using namespace std;

int main(int argc, char* argv[])
{
	spg_names* sg = new spg_names;
	string str("C 2/m 2/c 2s1/m");
	int number = sg->sgdll_GetFull(str);
	printf("%s number in the class is %d\n",&str,number);
	return 0;
}
