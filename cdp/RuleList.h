// RuleList.h: interface for the RuleList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULELIST_H__BE7F3192_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_RULELIST_H__BE7F3192_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "Rule.h"
#include "ContextRule.h"
#include "GlobalC.h"


class RuleList : public GlobalC 
{
public:

	char RuleListName[50];
	
	ContextRule *MultiRules;
	ContextRule *TwoRules;
	ContextRule *SingleRules;
	ContextRule *OnetoMultiRules;
	ContextRule *OutputRules;
	ContextRule *CSRules;
	
	int NMultiRules;
	int	NTwoRules;
	int NSingleRules;
	int NOnetoMultiRules;
	int NOutputRules;
	int NCSRules;

	
	
	char TestLoading[30]; //Testing stuff - remove later
	
	
	int LoadRules();

	// returns whether a rule matches string[0 + pos]
	// In is the letter string being matched, the rule to try, the position to start 
	// at, and what is taken.

	int Match(char *str, ContextRule TheRule, int pos, int *Cycle); 
	
	
	RuleList();
	virtual ~RuleList();

};


#endif // !defined(AFX_RULELIST_H__BE7F3192_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
