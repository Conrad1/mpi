// GPC_RouteC.cpp: implementation of the GPC_RouteC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GPC_RouteC.h"
#include <stdio.h>
#include <string.h>
#include "../common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


GPC_RouteC::GPC_RouteC()
{
	gpc_cyc_ph_ph = 10.0;
	gpc_act_offset = 100;
	exc_gpc_phone = 0.08;

}

GPC_RouteC::~GPC_RouteC()
{

}

void GPC_RouteC::InputGpc(char *Word)
{
	int i;
	//Might be changed later. Just sitting here as a stub //
	Pronounce(Word);
}



void GPC_RouteC::Pronounce(char *word)
{
	int i, j, RLen = 0, Len = 0, WPos = 0, PPos = 0;
	int Cycle = 0, Places;
	int MaxCycle = MARKER; //Just a marker
	int Pronounce[MAXPOS];	
	int Found;
	int testcount = 0;
	float AveAct;
	char tempchar;
	int eow =0; // Number of +'s

	int N;
	FILE *fp;


	NResults = 0;
	for(i=0;i<MAXPOS;i++) {
		CycleCount[i] = MaxCycle;
		Protected[i] = FALSE;
	}

	Len = strlen(word);

	for(i=0;i<Len;i++) {
		if(word[i] == WORDPAD) {
			eow++;
			WordCopy[i] = WORDPAD;
		} else {
			WordCopy[i] = Language_Hack(word[i]);
		}
	}

	WordCopy[Len] = 0;

	for(i=0;i<MAXPHONE;i++) {
		Spoken[i] = 0;
		Protected[i] = FALSE;
		PhAct[i] = 0.0;
	}
	Spoken[i] = 0;
	
		
	Found = FALSE;

	// Test MultiRules
	for(i=0;i<UsingRules.NMultiRules;i++) {
		if(UsingRules.MultiRules[i].Usage == 'b' || UsingRules.MultiRules[i].Usage == 'A') {
			Found = UsingRules.Match(WordCopy, UsingRules.MultiRules[i], 0, CycleCount);
			if(Found == TRUE) {
				SubLetts(UsingRules.MultiRules[i], 0, Cycle);
				strcat(Spoken, UsingRules.MultiRules[i].Pron);
				for(j=0;j<UsingRules.MultiRules[i].NPron;j++) Protected[j] = UsingRules.MultiRules[i].Protected;
				RLen = UsingRules.MultiRules[i].NPron;
				Protected[PPos] = UsingRules.MultiRules[i].Protected;
				sprintf(Results[0],"M %d %s %s %s", i, Spoken, UsingRules.MultiRules[i].Pron, UsingRules.MultiRules[i].RuleDesc);
				NResults++;
				break;
			}
		}
	}


	// Test Context Sensitive Rules
	if(Found == FALSE) {
		for(i=0;i<UsingRules.NCSRules ;i++) {
			if(UsingRules.CSRules[i].Usage == 'b' || UsingRules.CSRules[i].Usage == 'A') {
				Found = UsingRules.Match(WordCopy, UsingRules.CSRules[i], 0, CycleCount);
				if(Found == TRUE) {
					SubLetts(UsingRules.CSRules[i], 0, Cycle);
					strcat(Spoken, UsingRules.CSRules[i].Pron);
					for(j=0;j<UsingRules.CSRules[i].NPron;j++) Protected[j] = UsingRules.CSRules[i].Protected;
					RLen = UsingRules.CSRules[i].NPron;
					Protected[PPos] = UsingRules.CSRules[i].Protected;
					sprintf(Results[0],"C %d %s %s %s", i, Spoken, UsingRules.CSRules[i].Pron, UsingRules.CSRules[i].RuleDesc);
					NResults++;
					break;
				}
			}
		}
	}



	//Two Letter Rules
	if(Found == FALSE) {
		for(i=0;i< UsingRules.NTwoRules ;i++) {
			if(UsingRules.TwoRules[i].Usage == 'b' || UsingRules.TwoRules[i].Usage == 'A') {
				Found = UsingRules.Match(WordCopy, UsingRules.TwoRules[i], 0, CycleCount);
				if(Found == TRUE) {
					SubLetts(UsingRules.TwoRules[i], 0, Cycle);
					strcat(Spoken, UsingRules.TwoRules[i].Pron);
					for(j=0;j<UsingRules.TwoRules[i].NPron;j++) Protected[j] = UsingRules.TwoRules[i].Protected;
					RLen = UsingRules.TwoRules[i].NPron;
					Protected[PPos] = UsingRules.TwoRules[i].Protected;
					sprintf(Results[0],"T %d %s %s %s", i, Spoken, UsingRules.TwoRules[i].Pron, UsingRules.TwoRules[i].RuleDesc);
					NResults++;
					break;
				}
			}
		}
	}

	// Single Letter Rules	
	if(Found == FALSE) {
		for(i=0;i<UsingRules.NSingleRules; i++) {
			if(UsingRules.SingleRules[i].Usage == 'b' || UsingRules.SingleRules[i].Usage == 'A') {
				Found = UsingRules.Match(WordCopy, UsingRules.SingleRules[i], 0, CycleCount);
				if(Found == TRUE) {
					SubLetts(UsingRules.SingleRules[i], 0, Cycle);
					strcat(Spoken, UsingRules.SingleRules[i].Pron);
					for(j=0;j<UsingRules.SingleRules[i].NPron;j++) Protected[j] = UsingRules.SingleRules[i].Protected;
					RLen = 1;
					Protected[PPos] = UsingRules.SingleRules[i].Protected;
					sprintf(Results[0],"S %d %s %s %s", i, Spoken, UsingRules.SingleRules[i].Pron, UsingRules.SingleRules[i].RuleDesc);
					NResults++;
					break;
				}
			}
		}
	}

	// Default - Just Ignore it
	if(Found == FALSE) {
			CycleCount[WPos] = Cycle;
			//CHANGE XX
			Spoken[PPos] = WordCopy[WPos] /*'@'*/;
			RLen = 1;
			Protected[PPos] = TRUE;
			sprintf(Results[NResults],"* 0 %s %c %c", Spoken, Spoken[PPos], Spoken[PPos]);
			NResults++;

	}
		
	N = 0; AveAct = 0;

	for(i = 0; i < Len;i++) {
		if(CycleCount[i] == Cycle) {
			AveAct += WdAct[i];
			N++;
		}
	}	
	for(i=0;i<RLen;i++) {
		PhAct[PPos + i] = AveAct / (float) N;
	}
	PPos += RLen;

	for(WPos = 0;WPos < Len;WPos++) {
		if(CycleCount[WPos] == MaxCycle) {
			break;
		}
	}
		
	// Pronounce the rest of the word using WPos to Index word from Left to Right
	while(WPos < Len) {
		Cycle++;
		testcount++;
		Found = FALSE;
		for(i=0;i<UsingRules.NMultiRules;i++) {
			if(UsingRules.Match(WordCopy, UsingRules.MultiRules[i], WPos, CycleCount) ) {
				if( (UsingRules.MultiRules[i].Usage == 'e') ) {
					if( (UsingRules.MultiRules[i].Number_After == 0) && (UsingRules.MultiRules[i].NLetters + WPos  - UsingRules.MultiRules[i].Number_Before == (Len - eow)) && EOW == TRUE) {
						Found = TRUE;	
					}
				} else if (UsingRules.MultiRules[i].Usage == 'A') {
					Found = TRUE;
				} else if (UsingRules.MultiRules[i].Usage == 'n') {			
					if( (UsingRules.MultiRules[i].NLetters + WPos - UsingRules.MultiRules[i].Number_Before - UsingRules.MultiRules[i].Number_After  < (Len - eow)) || (EOW == FALSE && (UsingRules.MultiRules[i].NLetters + WPos  - UsingRules.MultiRules[i].Number_Before  - UsingRules.MultiRules[i].Number_After  <= (Len - eow))) ) {
						Found = TRUE;	
					}
				}			
															
				if(Found == TRUE) {
					SubLetts(UsingRules.MultiRules[i], WPos, Cycle);
					strcat(Spoken, UsingRules.MultiRules[i].Pron);
					RLen = UsingRules.MultiRules[i].NPron;
					Protected[PPos] = UsingRules.MultiRules[i].Protected;
					sprintf(Results[NResults],"M %d %s %s %s", i,Spoken, UsingRules.MultiRules[i].Pron, UsingRules.MultiRules[i].RuleDesc);
					NResults++;
					break;
				}
			}	
		}

		if(Found == FALSE) {
			for(i=0;i<UsingRules.NCSRules;i++) {
				if(UsingRules.Match(WordCopy, UsingRules.CSRules[i], WPos, CycleCount) ) {
					if( (UsingRules.CSRules[i].Usage == 'e') ) {
						if( (UsingRules.CSRules[i].Number_After == 0) && (UsingRules.CSRules[i].NLetters + WPos  - UsingRules.CSRules[i].Number_Before == (Len - eow)) && EOW == TRUE) {
							Found = TRUE;	
						}
					} else if (UsingRules.CSRules[i].Usage == 'A') {
						Found = TRUE;
					} else if (UsingRules.CSRules[i].Usage == 'n') {			
						if( ( (UsingRules.CSRules[i].NLetters + WPos - UsingRules.CSRules[i].Number_Before - UsingRules.CSRules[i].Number_After) < (Len - eow) ) || ( (EOW == FALSE) && (UsingRules.CSRules[i].NLetters + WPos  - UsingRules.CSRules[i].Number_Before - UsingRules.CSRules[i].Number_After  <= (Len - eow))) ) {
							Found = TRUE;	
						}
					}			
				}
				if(Found == TRUE) {
					SubLetts(UsingRules.CSRules[i], WPos, Cycle);
					strcat(Spoken, UsingRules.CSRules[i].Pron);
					RLen = UsingRules.CSRules[i].NPron;
					Protected[PPos] = UsingRules.CSRules[i].Protected;
					sprintf(Results[NResults],"C %d %s %s %s", i, Spoken, UsingRules.CSRules[i].Pron, UsingRules.CSRules[i].RuleDesc);
					NResults++;
					break;
				}
			}
		}

	
		if(Found == FALSE) {
			for(i=0;i<UsingRules.NOnetoMultiRules; i++) {
				if( UsingRules.Match(WordCopy, UsingRules.OnetoMultiRules[i], WPos, CycleCount) ) {
					SubLetts(UsingRules.OnetoMultiRules[i], WPos, Cycle);
					strcat(Spoken, UsingRules.OnetoMultiRules[i].Pron);
					RLen =UsingRules.OnetoMultiRules[i].NPron;
					Found = TRUE;
					sprintf(Results[NResults],"1 %d %s %s %s", i, Spoken, UsingRules.OnetoMultiRules[i].Pron, UsingRules.OnetoMultiRules[i].RuleDesc);
					NResults++;
					break;
				}
			}
		}


		if(Found == FALSE) {
			for(i=0;i<UsingRules.NTwoRules; i++) {
				if(UsingRules.Match(WordCopy,UsingRules.TwoRules[i], WPos, CycleCount) ) {
					if(UsingRules.TwoRules[i].Usage == 'e') {
						if( (UsingRules.TwoRules[i].NLetters + WPos == (Len - eow)) && EOW == TRUE) {
								Found = TRUE;
						}
					} else if(UsingRules.TwoRules[i].Usage == 'n') {
						if( (UsingRules.TwoRules[i].NLetters + WPos < (Len - eow) )|| (EOW == FALSE && (UsingRules.TwoRules[i].NLetters + WPos  - UsingRules.TwoRules[i].Number_Before  - UsingRules.TwoRules[i].Number_After <= (Len - eow))) ) {
								Found = TRUE;
						}
					} else if(UsingRules.TwoRules[i].Usage == 'A') {
						Found = TRUE;
					} 
				}
				if(Found == TRUE) {
					SubLetts(UsingRules.TwoRules[i], WPos, Cycle);
					strcat(Spoken, UsingRules.TwoRules[i].Pron);
					RLen = UsingRules.TwoRules[i].NPron;
					Protected[PPos] = UsingRules.TwoRules[i].Protected;
			
					sprintf(Results[NResults],"T %d %s %s %s", i, Spoken, UsingRules.TwoRules[i].Pron, UsingRules.TwoRules[i].RuleDesc);
					NResults++;
			
					break;
				}
			}
		}


		if(Found == FALSE) {
			for(i=0;i<UsingRules.NSingleRules; i++) {
				if(UsingRules.Match(WordCopy, UsingRules.SingleRules[i], WPos, CycleCount) ) {
					if(UsingRules.SingleRules[i].Usage == 'e') {
						if(UsingRules.SingleRules[i].NLetters == (strlen(&WordCopy[WPos]) - eow)  && EOW == TRUE) {
							Found = TRUE;
						}
					} else if(UsingRules.SingleRules[i].Usage == 'n') {
						if(UsingRules.SingleRules[i].NLetters < (strlen(&WordCopy[WPos]) - eow) || (EOW == FALSE && (UsingRules.SingleRules[i].NLetters <= strlen(&WordCopy[WPos]) - eow) )) {
							Found = TRUE;
						}
					} else if(UsingRules.SingleRules[i].Usage == 'A') {
						Found = TRUE;
					} 
					if(Found == TRUE) {
						SubLetts(UsingRules.SingleRules[i], WPos, Cycle);
						strcat(Spoken, UsingRules.SingleRules[i].Pron);
						RLen = 1;
						Protected[PPos] = UsingRules.SingleRules[i].Protected;
						sprintf(Results[NResults],"S %d %s %s %s", i, Spoken, UsingRules.SingleRules[i].Pron, UsingRules.SingleRules[i].RuleDesc);
						NResults++;
						break;
					}
				}
			}
		}

		if(Found == FALSE) {
			CycleCount[WPos] = Cycle;
			//XXX Change!!!
			Spoken[PPos] = WordCopy[WPos]  /* '@'*/;
			Protected[PPos] = TRUE;
			RLen = 1;
			sprintf(Results[NResults],"* 0 %s %c %c", Spoken, Spoken[PPos], Spoken[PPos]);
			NResults++;
		}
			

		N = 0; AveAct = 0;
		for(i = 0; i < Len;i++) {
			if(CycleCount[i] == Cycle) {
				AveAct += WdAct[i];
				N++;
			}
		}	

		for(i=0; i<RLen; i++) {
			PhAct[PPos + i] = AveAct / (float) N;
		}

		PPos += RLen;

		for(WPos = 0; WPos < Len; WPos++) {
			if(CycleCount[WPos] == MaxCycle) {
				break;
			}
		}
	}
	Spoken[PPos] = 0;
    
//	fp=fopen("fuck.txt","a");
//	fprintf(fp,"%s \n",Spoken);
//	fclose(fp);

}			


#if FRENCH

void GPC_RouteC::OutputWord() 
{
	// Needs to have the stuff set from	Pronounce
	// Puts the pronounciation in FinalWord

	int Found, h,i, j, k;
	int in1, in2;
	int Ends = 0; //How many Word-pads ?
	int SpokenLen;
	int FinalWordSpot = 0;
	int Spot = 0;
	int TempProtected;
	char bit[50];

	FILE *crud;
	

//	EOW = TRUE;
	for(i=0;i<MAXPOS;i++) {
		CycleCount[i] = MARKER;
	}

	SpokenLen = strlen(Spoken);
	
	for(i=0;i<SpokenLen;i++) {
		if(Spoken[i] == '.') {
			strcpy(bit, Spoken + 1 + i);
			strcpy(Spoken + i, bit);
			Spoken[SpokenLen - 1] = 0;
			SpokenLen--;
		}
	}

	for(i=0;i<SpokenLen;i++) {
		if(Spoken[i] == WORDPAD) Ends++;
	}

	FinalWord[0] = 0;

	while(Spot < strlen(Spoken) ) {
	//	crud = fopen("crud","a");
	//	fprintf(crud,"%d \n",Spot);
	//	fclose(crud);
		Found = FALSE;
		for(i=0;i<UsingRules.NOutputRules && Found == FALSE; i++) {
			//fprintf(crud,"crud 11 %d %d  \n",SpokenLen, Spot);	 
			if(UsingRules.OutputRules[i].Number_Before == UsingRules.OutputRules[i].NLetters) {
				if( (SpokenLen - Ends - Spot) == 2) {
					in1 = FALSE; in2 = FALSE;
					for(j=0;j<UsingRules.OutputRules[i].NContextRules[0];j++)  {
						if(Protected[Spot] == TRUE) break;
						if(Spoken[Spot] == UsingRules.OutputRules[i].Contexts[0][j]) {
							in1 = TRUE;
							break;
						}
					}
					if(in1 == TRUE) {
						for(j=0;j<UsingRules.OutputRules[i].NContextRules[1];j++)  {
							if(Protected[Spot + 1] == TRUE) break;
							if(Spoken[Spot+1] == UsingRules.OutputRules[i].Contexts[1][j]) {
								in2 = TRUE;
								break;
							}
						}
					} 
					if(in1 == TRUE && in2 == TRUE) {
						bit[0] = Spoken[Spot];
						bit[1] = Spoken[Spot+1];
						bit[2] = '^';
						bit[3] = 0;
						strcat(FinalWord,bit);
						sprintf(Results[NResults],"O %d %s %s ", i, FinalWord, UsingRules.OutputRules[i].Pron);
						Spot += 2;
						FinalWordSpot += 3;
						NResults++;			
						Found = TRUE;
					}
				}
			} else {
				if( (UsingRules.OutputRules[i].Usage == 'e')  && (SpokenLen - Ends - Spot  - UsingRules.OutputRules[i].NLetters == 0)  && (EOW == TRUE) ) {
				//	fprintf(crud,"crud 19 %d %s \n",Spot, Spoken);
					if(UsingRules.Match(Spoken, UsingRules.OutputRules[i], Spot , CycleCount) ) {
				//		fprintf(crud,"crud 20 %d %s \n",Spot, Spoken);
				
						TempProtected = FALSE;
						if(UsingRules.OutputRules[i].Protected == '1') {
								if(Protected[Spot + 1] == TRUE) TempProtected = TRUE;
						} else if (UsingRules.OutputRules[i].Protected == '2') {
								if(Protected[Spot] == TRUE) TempProtected = TRUE;
						} else {
							//for(j=0;j<UsingRules.OutputRules[i].NLetters;j++) {
							//	if(Protected[Spot + j] == TRUE) TempProtected = TRUE;
							//}
						}
			
//					fprintf(crud,"crud 2\n");
					//for(j=0;j<UsingRules.OutputRules[i].NLetters;j++) {
				//		if(Protected[Spot + j] == TRUE) TempProtected = TRUE;
				//	}
						if(TempProtected == TRUE) {
							FinalWord[FinalWordSpot] = Spoken[Spot]; 
							Spot++;
							FinalWordSpot++;
						} else {
							strcat(FinalWord + UsingRules.OutputRules[i].Number_Before, UsingRules.OutputRules[i].Pron);	
							Spot += UsingRules.OutputRules[i].NLetters  - UsingRules.OutputRules[i].Number_Before /* Just added! */ - UsingRules.OutputRules[i].Number_After;
							if(UsingRules.OutputRules[i].NLetters - UsingRules.OutputRules[i].Number_Before  == 0) Spot++;
							FinalWordSpot += UsingRules.OutputRules[i].NPron;
							sprintf(Results[NResults],"O %d %s %s %s", i, FinalWord, UsingRules.OutputRules[i].Pron, UsingRules.OutputRules[i].RuleDesc);
							NResults++;
						}	
						Found = TRUE;
					}
				} else if( (UsingRules.OutputRules[i].Usage == 'n') && ( (SpokenLen - Ends - Spot + UsingRules.OutputRules[i].NLetters > 0) ||  (EOW == TRUE && SpokenLen - Ends - Spot + UsingRules.OutputRules[i].NLetters >= 0)) ) {
	//			if( (strncmp(UsingRules.OutputRules[i].Letters, Spoken + Spot, UsingRules.OutputRules[i].NLetters) == 0) ) {
					if(UsingRules.Match(Spoken, UsingRules.OutputRules[i], Spot, CycleCount) ) {

						TempProtected = FALSE;
						if(UsingRules.OutputRules[i].Protected == '1') {
								if(Protected[Spot + 1] == TRUE) TempProtected = TRUE;
						} else if (UsingRules.OutputRules[i].Protected == '2') {
								if(Protected[Spot] == TRUE) TempProtected = TRUE;
						} else {
					//	for(j=0;j<UsingRules.OutputRules[i].NLetters;j++) {
					//		if(Protected[Spot + j] == TRUE) TempProtected = TRUE;
					//	}
						}
						if(TempProtected == FALSE) {
							strcat(FinalWord, UsingRules.OutputRules[i].Pron);
							FinalWordSpot += UsingRules.OutputRules[i].NPron;
							FinalWord[FinalWordSpot] = 0;
							Spot += UsingRules.OutputRules[i].NLetters - UsingRules.OutputRules[i].Number_Before /* Just added! */ - UsingRules.OutputRules[i].Number_After;
							sprintf(Results[NResults],"O %d %s %s %s", i, FinalWord, UsingRules.OutputRules[i].Pron, UsingRules.OutputRules[i].RuleDesc);
							NResults++;
						} else {
							FinalWord[FinalWordSpot] = Spoken[Spot]; 
							Spot++;
							FinalWordSpot++;
						}
						Found = TRUE;
					}
				}
			}
		}

		if(Found == FALSE) {
			// Leave that phoneme alone XXX Changed
			FinalWord[FinalWordSpot] =Spoken[Spot] ;
			FinalWordSpot++;
			FinalWord[FinalWordSpot] = 0;
			sprintf(Results[NResults],"X O %d %s ", i, FinalWord);
			NResults++;

			Spot++;
		}
	}

	h = strlen(FinalWord);
	for(i=0;i<h;i++) {
		j = PhonemeVowelIndex(FinalWord[i]);
		if(j!= -1) k++;

		if(FinalWord[i] == '.')k++;
		if(FinalWord[i] == '.' && k > 1) {
			strcpy(bit, FinalWord + 1 + i);
			strcpy(FinalWord + i, bit);
			Spoken[h - 1] = 0;
			h--;
		
			FinalWord[h] = 0;
			sprintf(Results[NResults],"X O %d %s ", i, FinalWord);
			NResults++;
			Spot++;
		} else if (FinalWord[i] == '.') {
			FinalWord[i] = '^';
		}
		FinalWordSpot = h;
	}
	sprintf(Results[NResults],"XX FO %d %s ", i, FinalWord);
	NResults++;
	
	FinalWord[FinalWordSpot] = 0; 
}


#else 

void GPC_RouteC::OutputWord()
{
	// Needs to have the stuff set from	Pronounce
	// Puts the pronounciation in FinalWord

	int Found, i, j;
	int Ends = 0; //How many Word-pads ?
	int SpokenLen;
	int FinalWordSpot = 0;
	int Spot = 0;
	int TempProtected;
	int Adds = 0;
	int Subs = 0;
	char endbit[50];
	FILE *fp1;

//	fp1 = fopen("tryit11.txt","a");
//	for(i=0;i<8;i++) {
//		fprintf(fp1,"%d ",Protected[i]);
//	}
//	fprintf(fp1," %s %d %d \n",Spoken, FALSE, TRUE);
//	fclose(fp1);
	
	
	for(i=0;i<MAXPOS;i++) {
		CycleCount[i] = MARKER;
	}

	SpokenLen = strlen(Spoken);

	for(i=0;i<SpokenLen;i++) {
		if(Spoken[i] == WORDPAD) Ends++;
	}

	FinalWord[0] = 0;


	while(Spot < strlen(Spoken)  ) {
		Found = FALSE;
		for(i=0;i<UsingRules.NOutputRules && Found == FALSE; i++) {
			if( (UsingRules.OutputRules[i].Usage == 'e')  && (SpokenLen - Ends - Spot - UsingRules.OutputRules[i].NLetters == 0) && (EOW == TRUE) ) {
			//	exit(1);
		
				if(UsingRules.Match(Spoken, UsingRules.OutputRules[i], Spot, CycleCount) ) {
					TempProtected = FALSE;
					if(UsingRules.OutputRules[i].Protected == 1) {
							if(Protected[Spot + 1] == TRUE) TempProtected = TRUE;
					} else if (UsingRules.OutputRules[i].Protected == 2) {
							if(Protected[Spot] == TRUE) TempProtected = TRUE;
					} else {
						for(j=0;j<UsingRules.OutputRules[i].NLetters;j++) {
							if(Protected[Spot + j] == TRUE) TempProtected = TRUE;
						}
					}
				
					if(TempProtected == TRUE) {
						FinalWord[FinalWordSpot] = Spoken[Spot]; 
						Spot++;
						FinalWordSpot++;
					} else {

						strcat(FinalWord, UsingRules.OutputRules[i].Pron);
						strcpy(endbit,Spoken + Spot + UsingRules.OutputRules[i].NLetters); 				
						
						for(j=Spot;j< (Spot + UsingRules.OutputRules[i].NPron);j++) {
							Protected[j] = FALSE;
							Spoken[j] = UsingRules.OutputRules[i].Pron[j-Spot];
						}
						Spot++;
						strcpy(Spoken + j, endbit);
					
						//Spot += UsingRules.OutputRules[i].NLetters;
						//FinalWordSpot += UsingRules.OutputRules[i].NPron;
						FinalWordSpot++;
						sprintf(Results[NResults],"O %d %d %s %s %s %s %d",i, UsingRules.OutputRules[i].NLetters, Spoken, FinalWord, UsingRules.OutputRules[i].Pron, UsingRules.OutputRules[i].RuleDesc,UsingRules.OutputRules[i].Protected);
						NResults++;
					//	Adds += (UsingRules.OutputRules[i].NPron - UsingRules.OutputRules[i].NLetters);
					}	
					Found = TRUE;
				}
			} else if( (UsingRules.OutputRules[i].Usage == 'n') && ( (SpokenLen - Ends - Spot + UsingRules.OutputRules[i].NLetters > 0) ||  (EOW == TRUE && SpokenLen - Ends - Spot + UsingRules.OutputRules[i].NLetters >= 0)) ) {
	//			if( (strncmp(UsingRules.OutputRules[i].Letters, Spoken + Spot, UsingRules.OutputRules[i].NLetters) == 0) ) {
					if(UsingRules.Match(Spoken, UsingRules.OutputRules[i], Spot, CycleCount) ) {

					TempProtected = FALSE;
					if(UsingRules.OutputRules[i].Protected == 1) {
							if(Protected[Spot + 1] == TRUE) TempProtected = TRUE;
					} else if (UsingRules.OutputRules[i].Protected == 2) {
							if(Protected[Spot] == TRUE) TempProtected = TRUE;
					} else {
						for(j=0;j<UsingRules.OutputRules[i].NLetters;j++) {
							if(Protected[Spot + j] == TRUE) TempProtected = TRUE;
						}
					}
					if(TempProtected == FALSE) {
						strcat(FinalWord, UsingRules.OutputRules[i].Pron);
						//FinalWordSpot += UsingRules.OutputRules[i].NPron;
						FinalWordSpot++;
						FinalWord[FinalWordSpot] = 0;
						strcpy(endbit,Spoken+Spot + UsingRules.OutputRules[i].NLetters); 		
					
						
						for(j=Spot;j< (Spot + UsingRules.OutputRules[i].NPron);j++) {
							Protected[j] = FALSE;						
							Spoken[j] = UsingRules.OutputRules[i].Pron[j-Spot];
						}
						Spot++;
						//strcpy(Spoken + Spot + (UsingRules.OutputRules[i].NPron - UsingRules.OutputRules[i].NLetters) + 1, endbit);					
						strcpy(Spoken + j, endbit);					
						sprintf(Results[NResults],"O %d %d %s %s %s %s %d",i, UsingRules.OutputRules[i].NLetters, Spoken, FinalWord, UsingRules.OutputRules[i].Pron, UsingRules.OutputRules[i].RuleDesc,UsingRules.OutputRules[i].Protected);
						//	Spot += UsingRules.OutputRules[i].NLetters;
			
						NResults++;
					//	Adds += (UsingRules.OutputRules[i].NPron - UsingRules.OutputRules[i].NLetters);

					} else {
						FinalWord[FinalWordSpot] = Spoken[Spot]; 
						Spot++;
						FinalWordSpot++;
					}
					Found = TRUE;
				}
			} 

		}

		if(Found == FALSE) {
			// Leave that phoneme alone
			FinalWord[FinalWordSpot] = Spoken[Spot];
			FinalWordSpot++;
			FinalWord[FinalWordSpot] = 0;
			sprintf(Results[NResults],"X O %d %s", i, FinalWord, UsingRules.OutputRules[i].Pron);
			NResults++;
			Spot++;
		}
	}

	FinalWord[FinalWordSpot] = 0;
//	fp1 = fopen("tryit.txt","a");
//	fprintf(fp1,"%s \n",FinalWord);
//	fclose(fp1);
	
}
#endif



void GPC_RouteC::SubLetts(ContextRule TheRule, int Pos, int Cycle)
{
	int i = 0;
	
	for(i=0;i<TheRule.NLetters;i++) {
		if(TheRule.ContextSensitive[i] == FALSE)  {
			CycleCount[Pos + i - TheRule.Number_Before] = Cycle;
		}
	}
}
