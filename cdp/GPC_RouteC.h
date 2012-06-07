// GPC_RouteC.h: interface for the GPC_RouteC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPC_ROUTEC_H__BE7F3194_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_GPC_ROUTEC_H__BE7F3194_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#include "RuleList.h"
#include "GlobalC.h"

class GPC_RouteC : public GlobalC  
{
public:

	float gpc_cyc_ph_ph;
	float gpc_act_offset; 
	float exc_gpc_phone;


	RuleList UsingRules;


	int   PhWord[MAXPHONE + 2];		 //The list of winning spots
	float PhAct[MAXPHONE + 2];		 //Activation for each phoneme
	float WdAct[MAXPOS + 2];		 //Initial word activation

	char Spoken[MAXPHONE + 2];	 //What the GPCRoute produced
	char FinalWord[MAXPHONE + 2]; //What should be named

	int NLetters; //Number of letters in word - for end rules
	int EOW;
	int   N2;
	
	char Results[40][200];  // Put what happened in here for future looking//Should really use a LL

	int  NResults;
	int Protected[MAXPOS +1];  // Is the phoneme protected ?


	void InputGpc(char *Word);
	int Load_GPC_Rules();
	void Pronounce(char *word);		//Works out the initial rules for the model
	void OutputWord();				// The final cleaned pronounciation
	GPC_RouteC();
	virtual ~GPC_RouteC();

private:
	int CycleCount[MAXPOS + 1]; // Counts which cycles a letter has been used
	int ToPronounce[MAXPOS + 1];
	void SubLetts(ContextRule TheRule, int Pos, int Cycle);
	char WordCopy[MAXPOS + 1];

	

};


#endif // !defined(AFX_GPC_ROUTEC_H__BE7F3194_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
