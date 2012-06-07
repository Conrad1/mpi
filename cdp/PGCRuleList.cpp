// PGCRuleList.cpp: implementation of the PGCRuleList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PGCRuleList.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PGCRuleList::PGCRuleList()
{
	NPGCRules = 0;		
}

PGCRuleList::~PGCRuleList()
{
	free(PGCRules);
}

int PGCRuleList::LoadRules()
{
	FILE *Rules;

	char TempUsage;
	char STempUsage[200];
	char TempType[200];
	char TempLetters[200];
	char STempNLetters[200];
	char TempNLetters;
	char TempPron[200];
	
	int TotalRules = 0, j;
		

	//Stuff for breaking down the context
	int h,i,l,After ;
	int Spot;
	if( (Rules = fopen(PGCRuleName,"r")) == 0) {
			return FALSE;
	}
	NPGCRules = 0;
	while ( (fscanf(Rules,"%s%s%s", TempPron, TempLetters, STempUsage))  != EOF) NPGCRules++;		
	
	PGCRules = (ContextRule *) calloc(NPGCRules + 2, sizeof(ContextRule));
	//The plus two is there since I'm getting occasiaonal
	//mem errors on spelling, which corrupt the programe.

	
	if(!PGCRules) exit(1);

	NPGCRules = 0;
	rewind(Rules);


	// Something won't let me scan chars properly, so thats why it's like this.
	while ( (fscanf(Rules,"%s%s%s", TempPron, TempLetters, STempUsage))  != EOF) {
		TempUsage = STempUsage[0];	
		PGCRules[NPGCRules].Usage = TempUsage;
		strcpy(PGCRules[NPGCRules].Letters, TempLetters);
		strcpy(PGCRules[NPGCRules].RuleDesc, TempPron);
		PGCRules[NPGCRules].NLetters = strlen(TempLetters);
		PGCRules[NPGCRules].NPron = 0;
		PGCRules[NPGCRules].Number_Before = 0;
		PGCRules[NPGCRules].Number_After = 0;
		Spot = 0;
		After = FALSE;
		for(i=0;i<strlen(TempPron);i++) {						
			if(TempPron[i] == '[') {
				PGCRules[NPGCRules].ContextSensitive[Spot] = TRUE;
				i++;
				while(TempPron[i] != ']') { 
						PGCRules[NPGCRules].Contexts[Spot][PGCRules[NPGCRules].NContextRules[Spot]] = TempPron[i];
						PGCRules[NPGCRules].NContextRules[Spot]++;
						i++;
				}
				if(After == FALSE) PGCRules[NPGCRules].Number_Before++;
				else PGCRules[NPGCRules].Number_After++;
			} else {
				PGCRules[NPGCRules].ContextSensitive[Spot] = FALSE;
				PGCRules[NPGCRules].Contexts[Spot][0] = TempPron[i];
				PGCRules[NPGCRules].NContextRules[Spot]++;
				After = TRUE;
			}
			Spot++;
		}
		PGCRules[NPGCRules].NPron = Spot ;
		
		NPGCRules++;		
	}

	fclose(Rules);

	return TRUE;
}

int PGCRuleList::Match(char *str, ContextRule TheRule, int pos, int *Cycles) 
{
	int i, slen;
	int here =0;
	int Start = 0, SecFound, j;

	slen = strlen(str);

	if(pos - TheRule.Number_Before < 0) return FALSE;
	if(TheRule.NPron + pos - TheRule.Number_Before > slen) return FALSE;
		

	for(i=0;i<TheRule.NPron;i++) {
		SecFound = FALSE;
		for(j=0;j<TheRule.NContextRules[i];j++) {
			if(TheRule.Contexts[i][j] == '.') {
				SecFound = TRUE;
				break;
			} else if( str[i + pos - TheRule.Number_Before] == TheRule.Contexts[i][j])  {
				SecFound = TRUE;
				break;
			}
		}
		if(SecFound == FALSE) return FALSE;
	} 
	return TRUE;
}	



