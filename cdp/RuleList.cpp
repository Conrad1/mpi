// RuleList.cpp: implementation of the RuleList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RuleList.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

	


RuleList::RuleList()
{
	NMultiRules = 0;
	NTwoRules = 0;
	NSingleRules = 0;
	NOnetoMultiRules = 0;
	NOutputRules = 0;
	NCSRules= 0;
	
}
	
RuleList::~RuleList()
{
	free(MultiRules);
	free(TwoRules);
	free(SingleRules);
	free(OnetoMultiRules);
	free(OutputRules);
	free(CSRules);
	
}

int RuleList::LoadRules()
{ 
	FILE *Rules;

	char TempUsage;
	char STempUsage[50];
	char TempType[303];
	char TempLetters[303];
	char STempNLetters[50];
	char TempNLetters;
	char TempPron[300];
	char STempProtected[150];
	int TempProtected;

	
	int TotalRules = 0;
	

	//Stuff for breaking down the context
	int h,i,l,After ;
	int Spot;
	if( (Rules = fopen(RuleFileName,"r")) == 0) {
			return FALSE;
	}
	while ( (fscanf(Rules,"%s%s%s%s%s%s", STempUsage, TempType, TempLetters, STempNLetters, TempPron, STempProtected))  != EOF) {
		if( (strcmp(TempType,"multi")) == 0) NMultiRules++;
		else if( (strcmp(TempType,"two")) == 0) NTwoRules++;
		else if( (strcmp(TempType,"sing")) == 0) NSingleRules++;	
		else if( (strcmp(TempType,"mphon")) == 0) NOnetoMultiRules++;
		else if( (strcmp(TempType,"cs")) == 0) NCSRules++;
		else if( (strcmp(TempType,"out")) == 0) NOutputRules++;
		else return FALSE;
	}
	
	MultiRules = (ContextRule *) calloc(NMultiRules, sizeof(ContextRule));
	TwoRules = (ContextRule *) calloc(NTwoRules, sizeof(ContextRule));
	SingleRules = (ContextRule *) calloc(NSingleRules, sizeof(ContextRule));
	OnetoMultiRules = (ContextRule *) calloc(NOnetoMultiRules, sizeof(ContextRule));
	CSRules = (ContextRule *) calloc(NCSRules, sizeof(ContextRule));
	OutputRules = (ContextRule *) calloc(NOutputRules, sizeof(ContextRule));
	
	if(!MultiRules) return FALSE;
	if(!TwoRules) return FALSE;
	if(!SingleRules) return FALSE;
	if(!OnetoMultiRules) return FALSE;
	if(!CSRules) return FALSE;
	if(!OutputRules) return FALSE;




	rewind(Rules);

	 NMultiRules = NTwoRules = NSingleRules = NOnetoMultiRules = NCSRules = NOutputRules = 0;
		
	// Something won't let me scan chars properly, so thats why it's like this.
	while ( (fscanf(Rules,"%s%s%s%s%s%s", STempUsage, TempType, TempLetters, STempNLetters, TempPron, STempProtected))  != EOF) {
		TempUsage = STempUsage[0];
		TempNLetters = STempNLetters[0];
	
		if( (strcmp(TempType,"out")) != 0) {
			if(STempProtected[0] == 'p') TempProtected = 1;		
			else TempProtected = 0;	
		} else {
			if(STempProtected[0] == '1') {
				TempProtected = 1;
			} else if (STempProtected[0] == '2') {
				TempProtected = 2;
			} else {
				TempProtected = 0;
			}
		}

		if( (strcmp(TempType,"multi")) == 0) {

			After = FALSE;			
			Spot = 0;
			MultiRules[NMultiRules].Usage = TempUsage;
			strcpy(MultiRules[NMultiRules].Type, TempType);
			strcpy(MultiRules[NMultiRules].RuleDesc, TempLetters);
			strcpy(MultiRules[NMultiRules].Pron, TempPron);
			MultiRules[NMultiRules].NPron = strlen(TempPron);
			MultiRules[NMultiRules].Protected = TempProtected;

			for(i=0;i<strlen(TempLetters);i++) {						
				if(TempLetters[i] == '[') {
					MultiRules[NMultiRules].ContextSensitive[Spot] = TRUE;
					i++;
					while(TempLetters[i] != ']') { 
						if(TempLetters[i] == 'C') {
							for(l = 0; l < strlen(CONSONANTS); l++) {
								MultiRules[NMultiRules].Contexts[Spot][MultiRules[NMultiRules].NContextRules[Spot]] = CONSONANTS[l];
								MultiRules[NMultiRules].NContextRules[Spot]++;
							}
						} else if (TempLetters[i] == 'V') {
							for(l = 0; l < strlen(VOWELS); l++) {
								MultiRules[NMultiRules].Contexts[Spot][MultiRules[NMultiRules].NContextRules[Spot]] = VOWELS[l];
								MultiRules[NMultiRules].NContextRules[Spot]++;
							}
						} else {
								MultiRules[NMultiRules].Contexts[Spot][MultiRules[NMultiRules].NContextRules[Spot]] = TempLetters[i];
								MultiRules[NMultiRules].NContextRules[Spot]++;
						}
						i++;
					}
					if(After == FALSE) MultiRules[NMultiRules].Number_Before++;
					else MultiRules[NMultiRules].Number_After++;
				} else {
					After = TRUE;
					if(TempLetters[i] == '.') MultiRules[NMultiRules].ContextSensitive[Spot] = TRUE;
					else MultiRules[NMultiRules].ContextSensitive[Spot] = FALSE;							
					MultiRules[NMultiRules].Contexts[Spot][0] = TempLetters[i];
					MultiRules[NMultiRules].NContextRules[Spot]++;
				}
				Spot++;
			}
			MultiRules[NMultiRules].NLetters = Spot;
			NMultiRules++;			
			
		} else if( (strcmp(TempType,"two")) == 0) {
		
			TwoRules[NTwoRules].Usage = TempUsage;
			strcpy(TwoRules[NTwoRules].Type, TempType);
			strcpy(TwoRules[NTwoRules].Pron, TempPron);
			strcpy(TwoRules[NTwoRules].RuleDesc, TempLetters);
			TwoRules[NTwoRules].NPron = strlen(TempPron);
			TwoRules[NTwoRules].Protected = TempProtected;	

			for(i=0;i<strlen(TempLetters);i++) {
				if(TempLetters[i] == '.') {
					TwoRules[NTwoRules].ContextSensitive[i] = TRUE;
					TwoRules[NTwoRules].Contexts[i][0] = TempLetters[i];
				} else {
					TwoRules[NTwoRules].ContextSensitive[i] = FALSE;
					TwoRules[NTwoRules].Contexts[i][0] = TempLetters[i];
				}
				TwoRules[NTwoRules].NContextRules[i]++;
			}
			TwoRules[NTwoRules].NLetters = strlen(TempLetters);
			NTwoRules++;

		} else if( (strcmp(TempType,"sing")) == 0) {	
			strcpy(SingleRules[NSingleRules].RuleDesc, TempLetters);
			SingleRules[NSingleRules].Usage = TempUsage;
			strcpy(SingleRules[NSingleRules].Type, TempType);
			strcpy(SingleRules[NSingleRules].Pron, TempPron);
			SingleRules[NSingleRules].NPron = strlen(TempPron);
			SingleRules[NSingleRules].Protected = TempProtected;	

			for(i=0;i<strlen(TempLetters);i++) {
				SingleRules[NSingleRules].ContextSensitive[i] = FALSE;
				SingleRules[NSingleRules].Contexts[i][0] = TempLetters[i];
				SingleRules[NSingleRules].NContextRules[i] = 1;
			}
			SingleRules[NSingleRules].NLetters = strlen(TempLetters);
		
			NSingleRules++;
									
		} else if( (strcmp(TempType,"mphon")) == 0) {
			strcpy(OnetoMultiRules[NOnetoMultiRules].RuleDesc, TempLetters);
			OnetoMultiRules[NOnetoMultiRules].Usage = TempUsage;
			strcpy(OnetoMultiRules[NOnetoMultiRules].Type, TempType);
			strcpy(OnetoMultiRules[NOnetoMultiRules].Pron, TempPron);
			OnetoMultiRules[NOnetoMultiRules].NPron = strlen(TempPron);
			OnetoMultiRules[NOnetoMultiRules].Protected = TempProtected;	

			for(i=0;i<strlen(TempLetters);i++) {
				OnetoMultiRules[NOnetoMultiRules].ContextSensitive[i] = FALSE;
				OnetoMultiRules[NOnetoMultiRules].Contexts[i][0] = TempLetters[i];
				OnetoMultiRules[NOnetoMultiRules].NContextRules[i] = 1;
			}
			OnetoMultiRules[NOnetoMultiRules].NLetters = strlen(TempLetters);
		
			NOnetoMultiRules++;
				
		} else if( (strcmp(TempType,"out")) == 0) {
			/* strcpy(OutputRules[NOutputRules].RuleDesc, TempLetters);
			OutputRules[NOutputRules].Usage = TempUsage;
			strcpy(OutputRules[NOutputRules].Type, TempType);
			strcpy(OutputRules[NOutputRules].Pron, TempPron);
			OutputRules[NOutputRules].NPron = strlen(TempPron);
			OutputRules[NOutputRules].Protected = STempProtected[0];	
			//Hack added later - be carful !

			for(i=0;i<strlen(TempLetters);i++) {
				OutputRules[NOutputRules].ContextSensitive[i] = FALSE;
				OutputRules[NOutputRules].Contexts[i][0] = TempLetters[i];
				OutputRules[NOutputRules].NContextRules[i] = 1;
			}
			OutputRules[NOutputRules].NLetters = strlen(TempLetters);
		
			NOutputRules++;*/
			
			After = FALSE;
						
			Spot = 0;
			strcpy(OutputRules[NOutputRules].RuleDesc, TempLetters);
			OutputRules[NOutputRules].Usage = TempUsage;
			strcpy(OutputRules[NOutputRules].Type, TempType);
			strcpy(OutputRules[NOutputRules].Pron, TempPron);
			OutputRules[NOutputRules].NPron = strlen(TempPron);
			OutputRules[NOutputRules].Protected = TempProtected;

			for(i=0;i<strlen(TempLetters);i++) {						
				if(TempLetters[i] == '[') {
					OutputRules[NOutputRules].ContextSensitive[Spot] = TRUE;
					i++;
					while(TempLetters[i] != ']') { 
						if(TempLetters[i] == 'C') {
							for(l = 0; l < strlen(CONSONANTS); l++) {
								OutputRules[NOutputRules].Contexts[Spot][OutputRules[NOutputRules].NContextRules[Spot]] = CONSONANTS[l];
								OutputRules[NOutputRules].NContextRules[Spot]++;
							}
						} else if (TempLetters[i] == 'V') {
							for(l = 0; l < strlen(VOWELS); l++) {
								OutputRules[NOutputRules].Contexts[Spot][OutputRules[NOutputRules].NContextRules[Spot]] = VOWELS[l];
								OutputRules[NOutputRules].NContextRules[Spot]++;
							}
						} else {
								OutputRules[NOutputRules].Contexts[Spot][OutputRules[NOutputRules].NContextRules[Spot]] = TempLetters[i];
								OutputRules[NOutputRules].NContextRules[Spot]++;
						}
						i++;
					}
					if(After == FALSE) OutputRules[NOutputRules].Number_Before++;
				} else {
					After = TRUE;
					OutputRules[NOutputRules].ContextSensitive[Spot] = FALSE;		
					OutputRules[NOutputRules].Contexts[Spot][0] = TempLetters[i];
					OutputRules[NOutputRules].NContextRules[Spot]++;
				}
				Spot++;
			}
			OutputRules[NOutputRules].NLetters = Spot;

			i = strlen(TempLetters) - 1;
			OutputRules[NOutputRules].Number_After = 0;
			while(i > 0) {
				if(TempLetters[i] == ']') {
					OutputRules[NOutputRules].Number_After++;
					while(TempLetters[i] != '[') i--;
					i--;					
				} else break;
			} 
			
			NOutputRules++;			


		} else if( (strcmp(TempType,"cs")) == 0) {
			
			After = FALSE;
						
			Spot = 0;
			strcpy(CSRules[NCSRules].RuleDesc, TempLetters);
			CSRules[NCSRules].Usage = TempUsage;
			strcpy(CSRules[NCSRules].Type, TempType);
			strcpy(CSRules[NCSRules].Pron, TempPron);
			CSRules[NCSRules].NPron = strlen(TempPron);
			CSRules[NCSRules].Protected = TempProtected;

			for(i=0;i<strlen(TempLetters);i++) {						
				if(TempLetters[i] == '[') {
					CSRules[NCSRules].ContextSensitive[Spot] = TRUE;
					i++;
					while(TempLetters[i] != ']') { 
						if(TempLetters[i] == 'C') {
							for(l = 0; l < strlen(CONSONANTS); l++) {
								CSRules[NCSRules].Contexts[Spot][CSRules[NCSRules].NContextRules[Spot]] = CONSONANTS[l];
								CSRules[NCSRules].NContextRules[Spot]++;
							}
						} else if (TempLetters[i] == 'V') {
							for(l = 0; l < strlen(VOWELS); l++) {
								CSRules[NCSRules].Contexts[Spot][CSRules[NCSRules].NContextRules[Spot]] = VOWELS[l];
								CSRules[NCSRules].NContextRules[Spot]++;
							}
						} else {
								CSRules[NCSRules].Contexts[Spot][CSRules[NCSRules].NContextRules[Spot]] = TempLetters[i];
								CSRules[NCSRules].NContextRules[Spot]++;
						}
						i++;
					}
					if(After == FALSE) CSRules[NCSRules].Number_Before++;
				} else {
					After = TRUE;
					if(TempLetters[i] == '.') CSRules[NCSRules].ContextSensitive[Spot] = TRUE;
					else CSRules[NCSRules].ContextSensitive[Spot] = FALSE;		
					CSRules[NCSRules].Contexts[Spot][0] = TempLetters[i];
					CSRules[NCSRules].NContextRules[Spot]++;
				}
				Spot++;
			}
			CSRules[NCSRules].NLetters = Spot;

			i = strlen(TempLetters) - 1;
			CSRules[NCSRules].Number_After = 0;
			while(i > 0) {
				if(TempLetters[i] == ']') {
					CSRules[NCSRules].Number_After++;
					while(TempLetters[i] != '[') i--;
					i--;					
				} else break;
			} 
			
			NCSRules++;

		} else {
			// Something has gone wrong :(
			return FALSE;
		}
		TotalRules++;
	
	}
	fclose(Rules);
	return TRUE;
}


int RuleList::Match(char *str, ContextRule TheRule, int pos, int *Cycles) 
{
	int i, slen;
	int here =0;
	int Start = 0, SecFound, j;
	
	int Doubled;
	
	FILE *fp, *fp1;

	
	slen = strlen(str);
	
	if(pos - TheRule.Number_Before < 0) return FALSE;

	if(TheRule.NLetters + pos - TheRule.Number_Before > slen) return FALSE;
	
	Doubled = FALSE;
	
	for(i=0;i<TheRule.NLetters;i++) {
		// Ok, not allowed to match a.e and then two letter non CS rukes.
		if( (TheRule.ContextSensitive[i] == FALSE) && Cycles[i + pos - TheRule.Number_Before] != MARKER) return FALSE; 
		SecFound = FALSE;
		for(j=0;j<TheRule.NContextRules[i];j++) {
			if( str[i + pos - TheRule.Number_Before] == TheRule.Contexts[i][j] || TheRule.Contexts[i][j] == '.') {
				SecFound = TRUE;
				break;
			}
		}
		if(SecFound == FALSE) return FALSE;
	}
	return TRUE;
}	









