// PGC_Route.cpp: implementation of the PGC_Route class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PGC_Route.h"
#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PGC_Route::PGC_Route()
{
	pgc_act_offset = 10;
	pgc_cyc_ph_ph = 17;
	exc_pgc_phone = 0.055;
}

PGC_Route::~PGC_Route()
{

}


void PGC_Route::InputPgc(char *Phon)
{
	int i;
	//Might be changed later. Just sitting here as a stub //
	Spell(Phon);
}



void PGC_Route::Spell(char *Pron)
{
	int Found, Done;
	float AveAct;
	int Cycle = 0;
	int Len;

	int TempLen;
	int  i, N, j, eow = 0;
	
	FILE *fp;

	NResults = Pos = WPos = 0;

	Len = strlen(Pron);

	for(i=0;i<Len;i++) {
		if(Pron[i] == WORDPAD) {
			eow++;
		}
	}


	if(strlen(Pron) == 0) {
		TheWord[0] = 0;
		return;
	}
	if(strlen(Pron) > MAXPHONE) Pron[MAXPHONE] = 0;

	for(i=0;i<MAXPOS+9;i++) TheWord[i] = 0;
	for(i=0;i<MAXPOS+9;i++) WdAct[i] = 0;
	for(i=0;i<MAXPHONE + 9;i++) CycleCount[i] = MARKER;

	Found = FALSE;
	for(i=0;i<UsingRules.NPGCRules;i++) {
		if(UsingRules.PGCRules[i].Usage == 'b' || UsingRules.PGCRules[i].Usage == 'A') {
			Found = UsingRules.Match(Pron, UsingRules.PGCRules[i], 0, CycleCount);
			if(Found == TRUE) {
				AddLetts(UsingRules.PGCRules[i], Cycle);
				
				N = 0; AveAct = 0;
				for(j = 0; j < MAXPHONE;j++) {
					if(CycleCount[j] == Cycle) {
						AveAct += PhAct[j];
						N++;
					}
				}
				ActLetters(UsingRules.PGCRules[i], AveAct / (float) N);				
				sprintf(Results[NResults],"%s %s %s", TheWord, UsingRules.PGCRules[i].Letters, UsingRules.PGCRules[i].RuleDesc);
				Rules[NResults] = i; 
				NResults++;
				break;
			}			
		}
	}	

	if(Found == FALSE) {
		TheWord[0] = Pron[0];
		CycleCount[0] = 0;	
		WdAct[0] = PhAct[0];
		TheWord[1] = 0;
		WPos = 1;
		Pos = 1;
		sprintf(Results[NResults],"*%s %c %c", TheWord, Pron[0], Pron[0]);
		Rules[NResults] = -1;
		NResults++;
	}

	for(i=0;i<MAXPHONE + 19;i++) {
		if(CycleCount[i] == MARKER) {
			Pos = i;
			break;
		}
	}

	while(Pos < Len) {
		Cycle++;
		Found = FALSE;
		for(i=0;i<UsingRules.NPGCRules;i++) {
			if((UsingRules.PGCRules[i].Usage == 'e') && (EOW == TRUE) && (UsingRules.PGCRules[i].NPron + Pos - UsingRules.PGCRules[i].Number_Before == (Len - eow)) && (TheWord[WPos + 1] == 0) && (UsingRules.PGCRules[i].Number_After == 0) ) {
				Found = UsingRules.Match(Pron, UsingRules.PGCRules[i], Pos, CycleCount);
			} else if( (UsingRules.PGCRules[i].Usage == 'n') && ( ((EOW == FALSE && UsingRules.PGCRules[i].NPron + Pos - UsingRules.PGCRules[i].Number_Before - UsingRules.PGCRules[i].Number_After  <= (Len - eow) ) || (EOW == TRUE && UsingRules.PGCRules[i].NPron + Pos - UsingRules.PGCRules[i].Number_Before - UsingRules.PGCRules[i].Number_After < (Len - eow) )) || (TheWord[WPos + 1] != 0)) ) {
				Found = UsingRules.Match(Pron, UsingRules.PGCRules[i], Pos, CycleCount);
			} else if(UsingRules.PGCRules[i].Usage == 'A') {
				Found = UsingRules.Match(Pron, UsingRules.PGCRules[i], Pos, CycleCount);
			}
				
			if(Found == TRUE) {
				AddLetts(UsingRules.PGCRules[i], Cycle);
				N = 0; AveAct = 0;
				for(j = 0; j < MAXPHONE;j++) {
					if(CycleCount[j] == Cycle) {
						AveAct += PhAct[j];
						N++;
					}
				}
				ActLetters(UsingRules.PGCRules[i], AveAct / (float) N);
				sprintf(Results[NResults],"%s %s %s", TheWord, UsingRules.PGCRules[i].Letters, UsingRules.PGCRules[i].RuleDesc);
				Rules[NResults] = i; 
				NResults++;
				break;
			}
		}

		//OK cant be a phoneme
		if(Found == FALSE) {
			TempLen = strlen(TheWord);
			TheWord[WPos] = Pron[Pos];
			CycleCount[Pos] = Cycle;	
			WdAct[WPos] = PhAct[Pos];
			Pos++;

			//Find next spot

			for(j=0;j<TempLen;j++) {
				if(TheWord[j] == '.') {
					WPos = j;
					break;
				}
			}
			
			if(j == TempLen) {
				WPos = TempLen + 1;
				TheWord[WPos] = 0;
			}
			fp = fopen("Crap","a");
			fprintf(fp,"->%s\n",TheWord);
			fclose(fp);

			sprintf(Results[NResults],"*%s %c %c", TheWord, Pron[Pos - 1], Pron[Pos  - 1]);
			Rules[NResults] = -1; 	
			NResults++;
		}
		//Work out where to start next time
		for(j=0;j<MAXPHONE + 19; j++) {
			if(CycleCount[j] == MARKER) {
				Pos = j;
				break;
			}
		}
	}	
	for(i=0;i<strlen(TheWord);i++) {
		if(TheWord[i] == '`') {
			for(j=i;j<strlen(TheWord) - 1;j++) {
				TheWord[j] = TheWord[j + 1];
			}
			TheWord[j] = 0;
		}
	}
}


void PGC_Route::ActLetters(ContextRule TheRule, float Act)
{

	int Dot = FALSE;
	int DotSpot, i;

	for(i=0;i<TheRule.NLetters;i++) {
		if(TheRule.Letters[i] != '.') {
			WdAct[OldWPos + i] = Act;
		} else {
			WdAct[OldWPos + i] = 0;
		}
	}
}


void PGC_Route::AddLetts(ContextRule TheRule, int Cycle)
{
	int i;
	int right_count = 0;
	
	OldWPos = WPos;

		//ok were writing in a space eg. a.e
	if(TheWord[WPos] == '.') {
		//Move everything across INCLUDING THE End of word BLANK
		if(TheRule.NLetters > 1) {
			for(i = WPos + 1;i<strlen(TheWord);i++) right_count++;
			for(i = right_count + 1; i > 0; i--) {
				TheWord[WPos + i + (TheRule.NLetters - 1) ] = TheWord[WPos + i];
				WdAct[WPos + i + (TheRule.NLetters - 1) ] = WdAct[WPos + i];
			}
		}
		strncpy(TheWord + WPos, TheRule.Letters, TheRule.NLetters);
		for(i = Pos;i<Pos + TheRule.NPron;i++) {
//			if(TheRule.Letters[i - Pos ] != '.') {
			if(TheRule.Contexts[i - Pos][0] != '.') {
				if(TheRule.ContextSensitive[i - Pos] == FALSE) {
					CycleCount[i - TheRule.Number_Before] = Cycle;
				}			
			}
		}
	} else {
		strncpy(TheWord + WPos, TheRule.Letters, TheRule.NLetters);
		for(i = Pos;i<Pos + TheRule.NPron;i++) {
			if(TheRule.Contexts[i - Pos ][0] != '.') {
				if(TheRule.ContextSensitive[i - Pos] == FALSE) {
					CycleCount[i - TheRule.Number_Before] = Cycle;
				}
			}
		}
		WPos += TheRule.NLetters;
		TheWord[WPos] = 0;
	}			

	//Needed in case of embedded .'s ie a.e 
	for(i=0;i<strlen(TheWord);i++) {
		if(TheWord[i] == '.') {
			WPos = i;
			break;
		} 
	} 
	if(i == strlen(TheWord)) WPos = i;
}

//Get rid of this
void PGC_Route::SubLetts(ContextRule TheRule, int Cycle)
{
}

