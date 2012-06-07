// PGC_Route.h: interface for the PGC_Route class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PGC_ROUTE_H__BE7F3191_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_PGC_ROUTE_H__BE7F3191_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#include "GlobalC.h"
#include "PGCRuleList.h"

class PGC_Route  
{
public:
	PGC_Route();
	virtual ~PGC_Route();

	PGCRuleList UsingRules;

	float pgc_act_offset;
	float pgc_cyc_ph_ph; 
	float exc_pgc_phone;

	int CycleCount[MAXPHONE + 20];
	float PhAct[MAXPHONE + 10];
	float WdAct[MAXPOS + 10];
	char TheWord[MAXPOS + 20]; //Just in case 

	void InputPgc(char *Phon);
	int Load_PGC_Rules();
	void Spell(char *Pron);		//Works out the initial rules for the model

	char Results[50][100];  // Put what happened in here for future looking
	int Rules[30];		   // Save the rule number which was used
 	int  NResults;
	int EOW;

private:
	void ActLetters(ContextRule TheRule, float Act);
	void SubLetts(ContextRule TheRule, int Cycle);
	void AddLetts(ContextRule TheRule, int Cycle);

	int Pos;	//Current Position of phoneme being examined
	int WPos;	//Where to write the word next
	int OldWPos;	//Where to the letters start for activation
};


#endif // !defined(AFX_PGC_ROUTE_H__BE7F3191_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
