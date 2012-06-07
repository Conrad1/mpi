// ContextRule.h: interface for the ContextRule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTEXTRULE_H__BE7F318D_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_CONTEXTRULE_H__BE7F318D_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "Rule.h"

class ContextRule : public Rule  
{
public:

	int Number_Before;	//Number of Context positions before the letters
	int Number_After;
	char Contexts[6][30];  //the last part of the array is for if multi letter rules are going to be used
	int NContextRules[8];
	int ContextSensitive[8];

	// Used for stats keeping on some of the database functions
	int used; 
	int rights;
	int wrongs;
	
	//more stats for generating rule freq's, hits and misses
	char UptoPron;
	char phons[10][3];
	float SumPhonFreqs[10];
	
	ContextRule();
	virtual ~ContextRule();

};


#endif // !defined(AFX_CONTEXTRULE_H__BE7F318D_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
