// PGCRuleList.h: interface for the PGCRuleList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PGCRULELIST_H__BE7F318F_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_PGCRULELIST_H__BE7F318F_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GlobalC.h"
#include "ContextRule.h"

class PGCRuleList : public GlobalC
{
public:

	char PGCRuleListName[50];

	PGCRuleList();
	virtual ~PGCRuleList();

	ContextRule *PGCRules;	//The PGC Rules
	int NPGCRules;				//The number of rules	
	int Match(char *str, ContextRule TheRule, int pos, int *Cycles); 
	int LoadRules();


};

#endif // !defined(AFX_PGCRULELIST_H__BE7F318F_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
