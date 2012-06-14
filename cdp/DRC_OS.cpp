// DRC_OS.cpp: implementation of the DRC_OS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DRC_OS.h"

#include <stdio.h>
#include <stdlib.h>
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

DRC_OS::DRC_OS()
{
	int i;

	IsPrime = 0;

	InterestSaveON = FALSE;
	QuickSaveOn = FALSE;
	sum_init = FALSE;
	sum_init2 = FALSE;

#if ISMFC

#else
	InterestSave[0] = 0;
#endif

	for(i=0;i<MAXPOS;i++) {
		MinShowLetterGraphs[i] = 0.01;
		MinShowPhonemeGraphs[i] = 0.01;
		MinShowGPCGraphs[i] = 0.01;
		MinShowPGCGraphs[i] = 0.01;
	
	}
	MinShowOrthLexGraph = 0.01;
	MinShowPhonLexGraph=0.01;
	MinShowTotal_Sum_Graph= 0.01;
	MinShowTotal_Sum_Graph2 =0.01;
	SaveType = 1;
	OnlyLexicons = 0;
}

DRC_OS::~DRC_OS()
{

	free(SavingLetters);
	free(SavingPhonemes);
	free(SavingGPC);
	free(SavingPGC);
	free(SavingOrth);
	free(SavingPhon);
	free(GraphLetterMap);
	free(GraphOrthMap);
	free(GraphPhonMap);
	free(GraphPhonemeMap);
	free(GraphGPCMap);
	free(GraphPGCMap);
}

int DRC_OS::NoSpaces(char* Line)
{
	int i, count = 0;

	for(i=0;i<strlen(Line);i++) {
		if(Line[i] == ' ') count++;
	}
	return count;
}

void DRC_OS::ClearAllNoRedraw() 
{
#if ISMFC
	int i,j;
	for(i=0;i<MAXPOS;i++) {
		LetterGraphs[i]->ClearGraph();
		PhonemeGraphs[i]->ClearGraph();
		GPCGraphs[i]->ClearGraph();
		PGCGraphs[i]->ClearGraph();
	}
	Total_Sum_Graph->ClearGraph();
	Total_Sum_Graph2->ClearGraph();
	OrthLexGraph->ClearGraph();
	PhonLexGraph->ClearGraph();

	
	sum_init = FALSE;

	for(i=0;i<50;i++) {
		NGraphLetterMap[i] = 1;  
		NGraphPhonemeMap[i] = 1;
		NGraphGPCMap[i] = 1;
		NGraphPGCMap[i] = 1;
	}
	NGraphOrthMap = 1;
	NGraphPhonMap = 1;
	
	for(i=0;i</*Num_Word*/8000;i++) GraphOrthMap[i] = 0;
	for(i=0;i</*Num_Phon*/8000;i++) GraphPhonMap[i] = 0;

	for(i=0; i <MAXPOS;i++) {
		for(j=0;j<NLETTERS;j++) GraphLetterMap[i][j] = 0;
		for(j=0;j<NLETTERS;j++) GraphPGCMap[i][j] = 0;
	}
		
	for(i=0; i <MAXPHONE;i ++) {
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			GraphGPCMap[i][j] = 0;
			GraphPhonemeMap[i][j] = 0;
		}
	}
#endif
}


void DRC_OS::ClearAll() 
{
#if ISMFC
	int i,j;
	for(i=0;i<MAXPOS;i++) {
		LetterGraphs[i]->ClearGraph();
		PhonemeGraphs[i]->ClearGraph();
		GPCGraphs[i]->ClearGraph();
		PGCGraphs[i]->ClearGraph();
		LetterGraphs[i]->RedrawWindow();
		PhonemeGraphs[i]->RedrawWindow();
		GPCGraphs[i]->RedrawWindow();
		PGCGraphs[i]->RedrawWindow();
	}
	Total_Sum_Graph->ClearGraph();
	Total_Sum_Graph2->ClearGraph();
	OrthLexGraph->ClearGraph();
	PhonLexGraph->ClearGraph();

	Total_Sum_Graph->RedrawWindow();
	OrthLexGraph->RedrawWindow();
	PhonLexGraph->RedrawWindow();
	
	sum_init = FALSE;

	for(i=0;i<50;i++) {
		NGraphLetterMap[i] = 1;  
		NGraphPhonemeMap[i] = 1;
		NGraphGPCMap[i] = 1;
		NGraphPGCMap[i] = 1;
	}
	NGraphOrthMap = 1;
	NGraphPhonMap = 1;
	
	for(i=0;i</*Num_Word*/ 8000;i++) GraphOrthMap[i] = 0;
	for(i=0;i</*Num_Phon*/ 8000;i++) GraphPhonMap[i] = 0;

	for(i=0; i <MAXPOS;i++) {
		for(j=0;j<NLETTERS;j++) GraphLetterMap[i][j] = 0;
		for(j=0;j<NLETTERS;j++) GraphPGCMap[i][j] = 0;
	}
		
	for(i=0; i <MAXPHONE;i ++) {
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			GraphGPCMap[i][j] = 0;
			GraphPhonemeMap[i][j] = 0;
		}
	}
#endif
}

int DRC_OS::Initialize_DRCOS(int NumW, int NumP)
{
	int i,j;

	Num_Word = NumW;
	Num_Phon = NumP;

	SavingLetters = (int **) calloc(MAXPOS, sizeof(int *));
	if(!SavingLetters) exit(1);
	SavingPGC = (int **) calloc(MAXPOS, sizeof(int *));
	if(!SavingPGC) exit(1);
	SavingPhonemes = (int **) calloc(MAXPHONE, sizeof(int *));
	if(!SavingPhonemes) exit(1);
	SavingGPC = (int **) calloc(MAXPHONE, sizeof(int *));
	if(!SavingGPC) exit(1);
	SavingOrth = (int *) calloc(/* Num_Word*/ 8000, sizeof(int));
	if(!SavingOrth) exit(1);
	SavingPhon = (int *) calloc(/*Num_Phon*/ 8000, sizeof(int));
	if(!SavingPhon) exit(1);

	for(i=0;i</*Num_Word*/ 8000;i++) SavingOrth[i] = 0;
	for(i=0;i</*Num_Phon*/ 8000;i++) SavingPhon[i] = 0;
	for(i=0; i <MAXPOS;i ++) {
	SavingLetters[i] = (int *) calloc(NLETTERS, sizeof(int) );
	SavingPGC[i] = (int *) calloc(NLETTERS, sizeof(int));
	if(!SavingLetters[i]) exit(1);
	if(!SavingPGC) exit(1);
	for(j=0;j<NLETTERS;j++) {
		SavingLetters[i][j] = 0;
		if(!SavingLetters[i]) exit(1);
		SavingPGC[i][j] = 0;
		if(!SavingPGC[i]) exit(1);	
		}
	}
		
	for(i=0; i <MAXPHONE;i ++) {
		SavingPhonemes[i] = (int *) calloc(NUMBER_OF_PHONEMES, sizeof(int) );
		SavingGPC[i] = (int *) calloc(NUMBER_OF_PHONEMES, sizeof(int) );
		if(!SavingPhonemes[i]) exit(1);
		if(!SavingGPC[i]) exit(1);
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			SavingGPC[i][j] = 0;
			SavingPhonemes[i][j] = 0;
			
		}
	}
	GraphLetterMap = (int **) calloc(MAXPOS, sizeof(int *));
	if(!GraphLetterMap) exit(1);
	GraphPGCMap = (int **) calloc(MAXPOS, sizeof(int *));
	if(!GraphPGCMap) exit(1);
	GraphPhonemeMap = (int **) calloc(MAXPHONE, sizeof(int *));
	if(!GraphPhonemeMap) exit(1);
	GraphGPCMap = (int **) calloc(MAXPHONE, sizeof(int *));
	if(!GraphGPCMap) exit(1);
	GraphOrthMap = (int *) calloc(/*Num_Word*/ 8000, sizeof(int));
	if(!GraphOrthMap) exit(1);
	GraphPhonMap = (int *) calloc(/* Num_Phon*/ 8000, sizeof(int));
	if(!GraphPhonMap) exit(1);

	for(i=0;i</*Num_Word*/ 8000; i++) GraphOrthMap[i] = 0;
	for(i=0;i</* Num_Phon*/ 8000; i++) GraphPhonMap[i] = 0;

	for(i=0; i <MAXPOS;i ++) {
		GraphLetterMap[i] = (int *) calloc(NLETTERS, sizeof(int) );
		if(!GraphLetterMap[i]) exit(1);
		for(j=0;j<NLETTERS;j++) GraphLetterMap[i][j] = 0;
		GraphPGCMap[i] = (int *) calloc(NLETTERS, sizeof(int) );
		if(!GraphPGCMap[i]) exit(1);
		for(j=0;j<NLETTERS;j++) GraphPGCMap[i][j] = 0;
	}
		
	for(i=0; i <MAXPHONE;i ++) {
		GraphPhonemeMap[i] = (int *) calloc(NUMBER_OF_PHONEMES, sizeof(int) );
		GraphGPCMap[i] = (int *) calloc(NUMBER_OF_PHONEMES, sizeof(int) );

		if(!GraphPhonemeMap[i]) exit(1);
		if(!GraphGPCMap[i]) exit(1);

		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			GraphGPCMap[i][j] = 0;
			GraphPhonemeMap[i][j] = 0;
		}
	}

	for(i=0;i<50;i++) {
		NGraphLetterMap[i] = 1;  
		NGraphPhonemeMap[i] = 1;
		NGraphGPCMap[i] = 1;
		NGraphPGCMap[i] = 1;
	}
	NGraphOrthMap = 1;
	NGraphPhonMap = 1;

//	InitSav();
	return 1;
}

void DRC_OS::QuickSave(char *Word, char *Word2, int Named, int Cycles)
{
	FILE *Fp;

	if(QuickSaveOn == TRUE) {
		if( (Fp = fopen(QuickSaveName, "a")) != 0) {
			fprintf(Fp, "%d %s %s %d\n",Cycles, Word, Word2, Named);
			fclose(Fp);
		}
	}

}

void DRC_OS::InitSav() 
{
	
	int i, j;

	for(i=0;i<MAXPOS;i++) {
		for(j=0;j<NLETTERS;j++) {
			SavingLetters[i][j] = FALSE;
			SavingPGC[i][j] = FALSE;
		}
	}

	for(i=0;i<MAXPHONE;i++) {
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			SavingPhonemes[i][j] = 0;
			SavingGPC[i][j] = 0;
		}
	}

	for(i=0;i</* Num_Word */ 8000;i++) {
		SavingOrth[i] = 0;
	}
	for(i=0;i</*Num_Phon*/ 8000;i++) {
		SavingPhon[i] = 0;
	}
}

void DRC_OS::WordInf(int Cycle, char *ActualWord)
{
	FILE *Fp;


	if(SaveType == 1 && InterestSaveON == TRUE) {
		sprintf(LettersI,"%s.Lets.txt",InterestSave);
		sprintf(OrthI,"%s.Orth.txt",InterestSave);
		sprintf(PhonI,"%s.Phon.txt",InterestSave);
		sprintf(PhonemeI,"%s.Phoneme.txt",InterestSave);
		sprintf(TotalOrthI,"%s.TotalOrth.txt",InterestSave);
		sprintf(TotalPhonI,"%s.TotalPhon.txt",InterestSave);
		sprintf(TotalPhonemeI,"%s.TotalPhoneme.txt",InterestSave);
		sprintf(StressI,"%s.Stress.txt",InterestSave);


		
		if( (Fp = fopen(OrthI, "a")) != 0) {
			fprintf(Fp, "%d Word %s \n",Cycle, ActualWord);
			fclose(Fp);
		}
		if( (Fp = fopen(PhonI, "a")) != 0) {
			fprintf(Fp, "%d Word %s \n",Cycle, ActualWord);
			fclose(Fp);
		}
		if( (Fp = fopen(TotalOrthI, "a")) != 0) {
			fprintf(Fp, "%d Word %s \n",Cycle, ActualWord);
			fclose(Fp);
		}
		if( (Fp = fopen(TotalPhonI, "a")) != 0) {
			fprintf(Fp, "%d Word %s \n",Cycle, ActualWord);
			fclose(Fp);
		}
		if(OnlyLexicons == 0) {
			if( (Fp = fopen(LettersI, "a")) != 0) {
				fprintf(Fp, "%d Word %s \n",Cycle, ActualWord);
				fclose(Fp);
			}

			if( (Fp = fopen(PhonemeI, "a")) != 0 && DisPhonemes == FALSE) {
				fprintf(Fp, "%d Word %s \n",Cycle, ActualWord);
				fclose(Fp);
			}
		
			if( (Fp = fopen(TotalPhonemeI, "a")) != 0) {
				fprintf(Fp, "%d Word %s \n",Cycle, ActualWord);
				fclose(Fp);
			}
			if( (Fp = fopen(StressI, "a")) != 0) {
				fprintf(Fp, "%d Word %s \n",Cycle, ActualWord);
				fclose(Fp);
			}
		}
	}
}

void DRC_OS::LetterInf(int i, int j, int Cycle)
{
	char TheName[30];
	FILE *Fp;


	if(LetterSpots.Letters[i][j] > MinShowLetterGraphs[i]) {
#if ISMFC
		if(SaveGraphs == TRUE) {
			if(GraphLetterMap[i][j] == 0) {
				LetterGraphs[i]->AddCategory(NGraphLetterMap[i]);
				TheName[0] = GlobalStuff.Alphabet[j];
				TheName[1] = 0;
				LetterGraphs[i]->AddName(TheName, NGraphLetterMap[i]);
				GraphLetterMap[i][j] = NGraphLetterMap[i];
				NGraphLetterMap[i]++;
			} 
			LetterGraphs[i]->AddElement(LetterSpots.Letters[i][j], Cycle, GraphLetterMap[i][j]); 
			LetterGraphs[i]->SendMessage(WM_PAINT);
			
		}
#endif
		if(InterestSaveON == TRUE && DisLetters == FALSE && OnlyLexicons == 0) {
			if(SaveType == 0) {
				if( (Fp = fopen(InterestSave, "a")) != 0) {
					fprintf(Fp, "%d Letter %d %d %f %c \n",Cycle,i,j, LetterSpots.Letters[i][j], GlobalStuff.Alphabet[j]);
					fclose(Fp);
				} 
			} else { // ==1
				if( (Fp = fopen(LettersI, "a")) != 0) {
					fprintf(Fp, "%d Letter %f %d %d %c \n",Cycle,LetterSpots.Letters[i][j], i,j,  GlobalStuff.Alphabet[j]);
					fclose(Fp);
				} 
			}
		}
	}
	if(SpecSave != FALSE) {
		if(SavingLetters[i][j] == TRUE && OnlyLexicons == 0) {
			sprintf(TheName,"%s%dLT.%c",BaseSave,i,GlobalStuff.Alphabet[j]);
			if( (Fp = fopen(TheName,"a")) != 0) {
				fprintf(Fp,"%d %f\n", Cycle, LetterSpots.Letters[i][j]);
				fclose(Fp);
			}
		}
	}
}

void DRC_OS::OrthLexInf(int i, int Cycle)
{
	char TheName[30], TempName[200],QuickName[50];
	FILE *Fp;


#if ISMFC
	LPTSTR p; 
	p = InterestSave.GetBuffer( 200 );
	strcpy( TempName, p ); 
#else
	strcpy(TempName,InterestSave);
#endif
	/*if(IsPrime == 0) {
		sprintf(QuickName,"%sOL.txt",TempName);
	} else {
		sprintf(QuickName,"%sOLpr.txt",TempName);
	}*/

	if(OrthLex.Activations[i] > MinShowOrthLexGraph) {
#if ISMFC
		if(SaveGraphs == TRUE) {
			if(GraphOrthMap[i] == 0) {
				OrthLexGraph->AddCategory(NGraphOrthMap);
				OrthLexGraph->AddName(TheWords.Word[i], NGraphOrthMap);
				GraphOrthMap[i] = NGraphOrthMap;
				NGraphOrthMap++;
			} 			
			OrthLexGraph->AddElement(OrthLex.Activations[i], Cycle, GraphOrthMap[i]); 
			OrthLexGraph->SendMessage(WM_PAINT);	
		}
#endif
		if(InterestSaveON == TRUE) {
			if(SaveType == 0) {
				if( (Fp = fopen(InterestSave, "a")) != 0) {
					fprintf(Fp, "%d Orth %d %f %s \n",Cycle,i,OrthLex.Activations[i], TheWords.Word[i]);
					fclose(Fp);
				}
			} else {
				if( (Fp = fopen(OrthI, "a")) != 0) {
					fprintf(Fp, "%d Orth %f %d  %s \n",Cycle,OrthLex.Activations[i],i, TheWords.Word[i]);
					fclose(Fp);
				}
			}
		}
		
	}
	if(SpecSave != FALSE) {
		if(SavingOrth[i] == TRUE) {
			sprintf(TheName,"%sOR.%s",BaseSave,TheWords.Word[i]);
			if( (Fp = fopen(TheName,"a")) != 0) {
				fprintf(Fp,"%d %f\n", Cycle, OrthLex.Activations[i]);
				fclose(Fp);
			}	
		}
	}
}

void DRC_OS::OrthLexSpecSave(int SpecSaveNumber, int Cycle)
{
	FILE *Fp;
	//Should make this choosable whenever
	if(SpecSaveNumber != -1) {
		if( (Fp = fopen("OrthSpecSave","a")) != 0) {
			fprintf(Fp, "%d SpecOrth %s %f \n", Cycle, TheWords.Word[SpecSaveNumber], OrthLex.Activations[SpecSaveNumber]);
		}
		fclose(Fp);
	}
}


void DRC_OS::PhonLexSpecSave(int SpecSaveNumber, int Cycle)
{
	FILE *Fp;
	//Should make this choosable whenever
	if(SpecSaveNumber != -1) {
		if( (Fp = fopen("PhonSpecSave","a")) != 0) {
			fprintf(Fp, "%d SpecPhon %s %f \n", Cycle, TheWords.Phon[SpecSaveNumber], PhonLex.Activations[SpecSaveNumber]);
		}
		fclose(Fp);
	}
}

void DRC_OS::TotOrthSpecSave(int Cycle)
{
	FILE *Fp;

	if(SaveType == 0) {
		if( (Fp = fopen("TotalOrth", "a")) != 0) {
			fprintf(Fp, "%d TotalSum  %f  \n",Cycle,OrthLex.VTotal_Sum);
			fclose(Fp);
		}
	} else {
		if( (Fp = fopen(TotalOrthI, "a")) != 0) {
			fprintf(Fp, "%d TotalSum  %f  \n",Cycle,OrthLex.VTotal_Sum);
			fclose(Fp);
		}	
	}
}

void DRC_OS::SemInf(int i, int Cycle)
{
	FILE *Fp;

	if(SemSys.SemAct[i] > MinShowPhonLexGraph) {

		if( (Fp = fopen(InterestSave, "a")) != 0) {
			fprintf(Fp, "%d SemInf Col = %d %f \n",Cycle,i,SemSys.SemAct[i]);
			fclose(Fp);
		}
	}
}

void DRC_OS::SemFeatInf(int i, int Cycle)
{
	FILE *Fp;

	if(SemFeatures.FeatureAct[i] > MinShowPhonLexGraph) {

		if( (Fp = fopen(InterestSave, "a")) != 0) {
			fprintf(Fp, "%d SemFeatInf Col = %d %f \n",Cycle,i,SemFeatures.FeatureAct[i]);
			fclose(Fp);
		}
	}
}

void DRC_OS::TotPhonSpecSave(int Cycle)
{
	FILE *Fp;

	if(SaveType == 0) {
		if( (Fp = fopen("TotalPhon", "a")) != 0) {
			fprintf(Fp, "%d PhonSum  %f  \n",Cycle,OrthLex.VTotal_Sum);
			fclose(Fp);
		} 
	} else {
		if( (Fp = fopen(TotalPhonI, "a")) != 0) {
			fprintf(Fp, "%d PhonSum  %f  \n",Cycle,OrthLex.VTotal_Sum);
			fclose(Fp);
		} 
	}
}

void DRC_OS::OrthLexSumInf(int i, int Cycle)
{
	FILE *Fp;

	if(OrthLex.VTotal_Sum > MinShowTotal_Sum_Graph) {
#if ISMFC
		if(SaveGraphs == TRUE) {
			if(sum_init == FALSE) {
				Total_Sum_Graph->AddCategory(1);
				Total_Sum_Graph->AddName("Sum",1);
				sum_init = TRUE;
			}
			Total_Sum_Graph->AddElement(OrthLex.VTotal_Sum, Cycle, 1); 
			Total_Sum_Graph->SendMessage(WM_PAINT);	
		}
#endif
		if(InterestSaveON == TRUE) {
			if(SaveType == 0) {
				if( (Fp = fopen(InterestSave, "a")) != 0) {
					fprintf(Fp, "%d TotalSum  %f  \n",Cycle,OrthLex.VTotal_Sum);
					fclose(Fp);
				}
			} else {
				if( (Fp = fopen(TotalOrthI, "a")) != 0) {
					fprintf(Fp, "%d TotalSum  %f  \n",Cycle,OrthLex.VTotal_Sum);
					fclose(Fp);
				}
			}
		}
	}
}

void DRC_OS::PhonLexSumInf(int i, int Cycle)
{
	FILE *Fp;

	if(PhonLex.VTotal_Sum > MinShowTotal_Sum_Graph2) {
#if ISMFC
		if(SaveGraphs == TRUE) {
			if(sum_init2 == FALSE) {
				Total_Sum_Graph2->AddCategory(1);
				Total_Sum_Graph2->AddName("Sum",1);
				sum_init2 = TRUE;
			}
			Total_Sum_Graph2->AddElement(PhonLex.VTotal_Sum, Cycle, 1); 
			Total_Sum_Graph2->SendMessage(WM_PAINT);	
		}
#endif
		if(InterestSaveON == TRUE) {
			if(SaveType == 0) {
				if( (Fp = fopen(InterestSave, "a")) != 0) {
					fprintf(Fp, "%d PhonSum  %f  \n",Cycle,PhonLex.VTotal_Sum);
					fclose(Fp);
				}
			} else {
				if( (Fp = fopen(TotalPhonI, "a")) != 0) {
					fprintf(Fp, "%d PhonSum  %f  \n",Cycle,PhonLex.VTotal_Sum);
					fclose(Fp);
				}
			}
		}
	}
}


void DRC_OS::PhonemeSumInf(int Cycle, float amount)
{
	FILE *Fp;

	if(amount > MinShowTotal_Sum_Graph2 && OnlyLexicons == 0) {
		if(SaveType == 0) {
	
		} else {
			if( (Fp = fopen(TotalPhonemeI, "a")) != 0) {
				fprintf(Fp, "%d PhonemeSum %f  \n",Cycle,amount);
				fclose(Fp);
			}
		}
	}
}


void DRC_OS::PhonLexInf(int i, int Cycle) 
{
	char TheName[30], TempName[200], QuickName[50];
	FILE *Fp;


	
#if ISMFC
	LPTSTR p; 
	p = InterestSave.GetBuffer( 200 );
	strcpy( TempName, p ); 
#else
	strcpy(TempName,InterestSave);
#endif
	
		

	if(PhonLex.Activations[i] > MinShowPhonLexGraph) {
#if ISMFC
		if(SaveGraphs == TRUE) {
			if(GraphPhonMap[i] == 0) {
				PhonLexGraph->AddCategory(NGraphPhonMap);
				PhonLexGraph->AddName(TheWords.Phon[i], NGraphPhonMap);
				GraphPhonMap[i] = NGraphPhonMap;
				NGraphPhonMap++;
			} 
			PhonLexGraph->AddElement(PhonLex.Activations[i], Cycle, GraphPhonMap[i]); 
			/* if(PhonLexGraph->GetActiveView() != NULL) */ PhonLexGraph->SendMessage(WM_PAINT);	
		}
#endif
		if(InterestSaveON == TRUE /*&& DisPhonLex == FALSE*/) {
			if(IsPrime == 0) {
				sprintf(QuickName,"%sPL.txt",TempName);
			} else {
				sprintf(QuickName,"%sPLpr.txt",TempName);
			}			
			//	if( (Fp = fopen(InterestSave, "a")) != 0) {
			if(SaveType == 0) {
				if( (Fp = fopen(QuickName, "a")) != 0) {			
					fprintf(Fp, "%d Phon %d %f %s \n",Cycle,i,PhonLex.Activations[i], TheWords.Phon[i]);			
					fclose(Fp);
				}
			} else {
				if( (Fp = fopen(PhonI, "a")) != 0) {			
					fprintf(Fp, "%d Phon %f %d %s \n",Cycle,PhonLex.Activations[i],i, TheWords.Phon[i]);			
					fclose(Fp);
				}
			}
		}
	}			
	if(SavingPhon[i] == TRUE ) {
		sprintf(TheName,"%sPR.%s",BaseSave,TheWords.Phon[i]);
		if( (Fp = fopen(TheName,"a")) != 0) {
			fprintf(Fp,"%d %f\n", Cycle, PhonLex.Activations[i]);
			fclose(Fp);
		}
	}
}

void DRC_OS::PhonemeInf(int i, int j, int Cycle)
{
	char TheName[30];
	FILE *Fp;

	if(PhonBuf.Buffer[i][j] > MinShowPhonemeGraphs[i]) {
#if ISMFC
		if(SaveGraphs == TRUE) {
			if(GraphPhonemeMap[i][j] == 0) {
				PhonemeGraphs[i]->AddCategory(NGraphPhonemeMap[i]);
				TheName[0] = PhonBuf.Phonemes[j];
				TheName[1] = 0;
				PhonemeGraphs[i]->AddName(TheName, NGraphPhonemeMap[i]);
				GraphPhonemeMap[i][j] = NGraphPhonemeMap[i];
				NGraphPhonemeMap[i]++;
			} 
			PhonemeGraphs[i]->AddElement(PhonBuf.Buffer[i][j], Cycle, GraphPhonemeMap[i][j]); 
			PhonemeGraphs[i]->SendMessage(WM_PAINT);	
		}
#endif
		if(InterestSaveON == TRUE && DisPhonemes == FALSE && OnlyLexicons == 0 ) {
			if(SaveType == 0) {
				if( (Fp = fopen(InterestSave, "a")) != 0) {
					fprintf(Fp, "%d Phoneme %d %d %f %c \n",Cycle,i,j,PhonBuf.Buffer[i][j], PhonBuf.Phonemes[j]);
					fclose(Fp);
				}
			} else {
				if(j==45 || j == 46) {
					if( (Fp = fopen(StressI, "a")) != 0) {			
						fprintf(Fp, "%d Phoneme %f %d %d  %c \n",Cycle,PhonBuf.Buffer[i][j],i,j, PhonBuf.Phonemes[j]);
						fclose(Fp);
					}
				} else {
					if( (Fp = fopen(PhonemeI, "a")) != 0) {			
						fprintf(Fp, "%d Phoneme %f %d %d %c \n",Cycle,PhonBuf.Buffer[i][j],i,j, PhonBuf.Phonemes[j]);
						fclose(Fp);
					}
				}
			}
		}
	}
	if(SpecSave != FALSE) {
		if(SavingPhonemes[i][j] == TRUE && OnlyLexicons == 0 ) {
			sprintf(TheName,"%s%dPH.%c",BaseSave,i,PhonBuf.Phonemes[j]);
			if( (Fp = fopen(TheName,"a")) != 0) {
				fprintf(Fp,"%d %f\n", Cycle, PhonBuf.Buffer[i][j]);
				fclose(Fp);
			}
		}
	}
}


void DRC_OS::PGCInf(int Cycle)
{
	char TheName[30];
	FILE *Fp;
	int i,j, Spot;

	if(InterestSaveON == TRUE && DisPGC == FALSE && OnlyLexicons == 0) {
		if( (Fp = fopen(InterestSave, "a")) != 0) {
			fprintf(Fp, "%d Spell %s ",Cycle,PGCRoute.TheWord);
			for(i=0;i<strlen(PGCRoute.TheWord);i++) {
				fprintf(Fp, "%f ",PGCRoute.WdAct[i]);
			}
			fprintf(Fp, "\n");
			fclose(Fp);
		}
	}
	if(SaveGraphs == TRUE) {
#if ISMFC
		for(i=0;i<strlen(PGCRoute.TheWord);i++) {
			if(PGCRoute.WdAct[i] > MinShowPGCGraphs[i]) {			
				Spot = GlobalStuff.AlphabetIndex(PGCRoute.TheWord[i]);
				if(Spot != -1) {
					if(GraphPGCMap[i][Spot] == 0) {
						PGCGraphs[i]->AddCategory(NGraphPGCMap[i]);
						TheName[0] = GlobalStuff.Alphabet[Spot];
						TheName[1] = 0;
						PGCGraphs[i]->AddName(TheName, NGraphPGCMap[i]);
						GraphPGCMap[i][Spot] = NGraphPGCMap[i];
						NGraphPGCMap[i]++;
					} 
					PGCGraphs[i]->AddElement(PGCRoute.WdAct[i], Cycle, GraphPGCMap[i][Spot]); 
					PGCGraphs[i]->SendMessage(WM_PAINT);	
				}
			}
		}
#endif
	}


	if(SpecSave != FALSE && OnlyLexicons == 0) {
		for(i=0;i<MAXPOS;i++) {
			for(j=0;j<NLETTERS;j++) {
				if(SavingPGC[i][j] == TRUE ) {
					sprintf(TheName,"%s%dAS.%c",BaseSave,i,GlobalStuff.Alphabet[j]);
					if( (Fp = fopen(TheName,"a")) != 0) {
						if(GlobalStuff.Alphabet[j] == PGCRoute.TheWord[i] ) {
							fprintf(Fp,"%d %f\n", Cycle, PGCRoute.WdAct[i]);
						} else {
							fprintf(Fp, "%d 0\n",Cycle);
						}
						fclose(Fp);
					}	
				}
			}
		}
	}
}

void DRC_OS::GPCInf(int Cycle)
{
	char TheName[30];
	FILE *Fp;
	int i,j, Spot;
	 
	if(InterestSaveON == TRUE && DisGPC == FALSE && OnlyLexicons == 0) {
		if( (Fp = fopen(InterestSave, "a")) != 0) {
			fprintf(Fp, "%d Assemble %s ",Cycle,GPCRoute.FinalWord);
			for(i=0;i<strlen(GPCRoute.FinalWord);i++) {
				fprintf(Fp, "%f ",GPCRoute.PhAct[i]);
			}
			fprintf(Fp, "\n");
			fclose(Fp);
		}
	}
	
#if ISMFC
	if(SaveGraphs == TRUE && OnlyLexicons == 0) {
		for(i=0;i<strlen(GPCRoute.FinalWord);i++) {
			if(GPCRoute.PhAct[i] > MinShowGPCGraphs[i]) {			
				Spot = PhonBuf.PhonemeIndex(GPCRoute.FinalWord[i]);
				if(Spot != -1) {
					if(GraphGPCMap[i][Spot] == 0) {
						GPCGraphs[i]->AddCategory(NGraphGPCMap[i]);
						TheName[0] = PhonBuf.Phonemes[Spot];
						TheName[1] = 0;
						GPCGraphs[i]->AddName(TheName, NGraphGPCMap[i]);
						GraphGPCMap[i][Spot] = NGraphGPCMap[i];
						NGraphGPCMap[i]++;
					} 
					GPCGraphs[i]->AddElement(GPCRoute.PhAct[i], Cycle, GraphGPCMap[i][Spot]); 
					GPCGraphs[i]->SendMessage(WM_PAINT);	
				}
			}
		}
	}
#endif
	if(SpecSave != FALSE && OnlyLexicons == 0) {
		for(i=0;i<MAXPHONE;i++) {
			for(j=0;j<NUMBER_OF_PHONEMES;j++) {
				if(SavingGPC[i][j] == TRUE) {
					sprintf(TheName,"%s%dAS.%c",BaseSave,i,PhonBuf.Phonemes[j]);
					if( (Fp = fopen(TheName,"a")) != 0) {
						if(PhonBuf.Phonemes[j] == GPCRoute.FinalWord[i] ) {
							fprintf(Fp,"%d %f\n", Cycle, GPCRoute.PhAct[i]);
						} else {
							fprintf(Fp, "%d 0\n",Cycle);
						}
						fclose(Fp);
					}		
				}
			}
		}
	}
}

void DRC_OS::PrintInfo(char *FileName, int upto)
{
	FILE *fp;
	int i;

	fp = fopen(FileName,"w");
	for(i=0;i<upto;i++) {
		fprintf(fp,"%d %d %s \n", WordRes[i],StressRes[i],PhonRes[i]);
	}
	fclose(fp);
}


void DRC_OS::LoadParametersName(char *FileName)
{
	FILE *fp,*fp2;
	char first[50], crap[100],Line[200];

	//FileDialog FileStuff(TRUE, NULL, NULL);
	//FileStuff.m_ofn.lpstrTitle = "Enter Parameter Load File";
	//if(FileStuff.DoModal() != IDOK) return;

	fp = fopen(FileName,"r");
	if(fp == 0) return;
	//if( (fp = fopen(FileStuff.GetPathName(),"r") ) != 0) {
		fgets(Line,200,fp);
		sscanf(Line,"%s",first);
		//fscanf(fp,"%s",first); 
		if(strcmp(first,"parameters") !=0 ) {
		//MessageBox("Not a parameter File", "Loaded", MB_ICONEXCLAMATION | MB_OK);
		//	fclose(fp);
			return;
		}
		fgets(Line,200,fp);

		if(NoSpaces(Line) > 0) sscanf(Line,"%f%s", &Features.exc_feature_letter, crap);
		else sscanf(Line,"%f", &Features.exc_feature_letter);

		fgets(Line,200,fp); /*fgetc(fp);*/
		if(NoSpaces(Line)> 0) sscanf(Line,"%f%s",&Features.inh_feature_letter, crap);
		else sscanf(Line,"%f",&Features.inh_feature_letter);
		
		fgets(Line,200,fp); 
		if(NoSpaces(Line) > 0) sscanf(Line,"%d%s",&Features.Noise, crap);
		else sscanf(Line,"%d",&Features.Noise);
		
		fgets(Line,200,fp); 
		if(NoSpaces(Line) > 0) sscanf(Line,"%f%s", &OrthLex.Orth_Orth, crap); 
		else sscanf(Line,"%f", &OrthLex.Orth_Orth); 	
		
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0) sscanf(Line,"%f%s", &OrthLex.exc_word_letter, crap);
		else sscanf(Line,"%f", &OrthLex.exc_word_letter);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0) sscanf(Line,"%f%s", &OrthLex.inh_word_letter, crap);
		else sscanf(Line,"%f", &OrthLex.inh_word_letter);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &OrthLex.exc_orth_phon, crap);	
		else sscanf(Line,"%f", &OrthLex.exc_orth_phon);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &OrthLex.Decay, crap);
		else sscanf(Line,"%f", &OrthLex.Decay);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &OrthLex.Noise, crap);
		else sscanf(Line,"%f", &OrthLex.Noise);
		fgets(Line,200,fp);

		if(NoSpaces(Line) > 0) sscanf(Line,"%f%s", &OrthLex.LexM, crap);
		else sscanf(Line,"%f", &OrthLex.LexM);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0) sscanf(Line,"%f%s", &OrthLex.Total_Sum, crap);
		else sscanf(Line,"%f", &OrthLex.Total_Sum, crap);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &OrthLex.Total_Sum2, crap);
		else sscanf(Line,"%f", &OrthLex.Total_Sum2);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%d%s", &OrthLex.Total_Sum_Change, crap);
		else sscanf(Line,"%d", &OrthLex.Total_Sum_Change);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &OrthLex.Total_Sum_Act_Change, crap);
		else sscanf(Line,"%f", &OrthLex.Total_Sum_Act_Change);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%d%s", &OrthLex.M_Minus, crap);
		else sscanf(Line,"%d", &OrthLex.M_Minus);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &OrthLex.M_Act_Minus, crap);
		else sscanf(Line,"%f", &OrthLex.M_Act_Minus);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%d%s", &OrthLex.M_Time, crap);
		else sscanf(Line,"%d", &OrthLex.M_Time);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &LetterSpots.Letter_Letter, crap);
		else sscanf(Line,"%f", &LetterSpots.Letter_Letter);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &LetterSpots.exc_letter_word, crap);
		else sscanf(Line,"%f", &LetterSpots.exc_letter_word);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &LetterSpots.inh_letter_word, crap);
		else sscanf(Line,"%f", &LetterSpots.inh_letter_word);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &LetterSpots.Decay, crap);
		else sscanf(Line,"%f", &LetterSpots.Decay);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &LetterSpots.Noise, crap);		
		else sscanf(Line,"%f", &LetterSpots.Noise);	
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonLex.PhonUnit_PhonUnit, crap);
		else sscanf(Line,"%f", &PhonLex.PhonUnit_PhonUnit);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonLex.exc_phonU_phon, crap);
		else sscanf(Line,"%f", &PhonLex.exc_phonU_phon);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonLex.inh_phonU_phon, crap);
		else sscanf(Line,"%f", &PhonLex.inh_phonU_phon);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonLex.exc_phonU_orth, crap);	
		else sscanf(Line,"%f", &PhonLex.exc_phonU_orth);	
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonLex.Decay, crap);
		else sscanf(Line,"%f", &PhonLex.Decay);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonLex.Noise, crap);	
		else sscanf(Line,"%f", &PhonLex.Noise);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonBuf.Phon_Phon, crap);
		else sscanf(Line,"%f", &PhonBuf.Phon_Phon);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonBuf.exc_phon_word, crap);	
		else sscanf(Line,"%f", &PhonBuf.exc_phon_word);	
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonBuf.inh_phon_word, crap);
		else sscanf(Line,"%f", &PhonBuf.inh_phon_word);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonBuf.Decay, crap);
		else sscanf(Line,"%f", &PhonBuf.Decay);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonBuf.Noise, crap);			
		else sscanf(Line,"%f", &PhonBuf.Noise);	
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &GPCRoute.exc_gpc_phone, crap);
		else sscanf(Line,"%f", &GPCRoute.exc_gpc_phone);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &GPCRoute.gpc_cyc_ph_ph, crap);
		else sscanf(Line,"%f", &GPCRoute.gpc_cyc_ph_ph);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &GPCRoute.gpc_act_offset, crap);		
		else sscanf(Line,"%f", &GPCRoute.gpc_act_offset);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &GlobalStuff.act_rate, crap);
		else sscanf(Line,"%f", &GlobalStuff.act_rate);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &TheWords.fgain_tmp, crap);
		else sscanf(Line,"%f", &TheWords.fgain_tmp);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonBuf.PhoneMin, crap);
		else sscanf(Line,"%f", &PhonBuf.PhoneMin);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PGCRoute.exc_pgc_phone, crap);
		else sscanf(Line,"%f", &PGCRoute.exc_pgc_phone);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PGCRoute.pgc_cyc_ph_ph, crap);
		else sscanf(Line,"%f", &PGCRoute.pgc_cyc_ph_ph);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PGCRoute.pgc_act_offset, crap); 
		else sscanf(Line,"%f", &PGCRoute.pgc_act_offset);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &LetterSpots.SpellMin, crap);	
		else sscanf(Line,"%f", &LetterSpots.SpellMin);	
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonFeatures.inh_pfeature_phoneme, crap);
 		else sscanf(Line,"%f", &PhonFeatures.inh_pfeature_phoneme);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&PhonFeatures.exc_pfeature_phoneme, crap);
		else sscanf(Line,"%f",&PhonFeatures.exc_pfeature_phoneme);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%d%s",&ClampingCycles,crap);
		else sscanf(Line,"%d",&ClampingCycles);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&SemFeatures.exc_semfeat_sem, crap);
		else sscanf(Line,"%f",&SemFeatures.exc_semfeat_sem);
		fgets(Line,200,fp);
		
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&SemSys.exc_sem_phon, crap);
 		else sscanf(Line,"%f",&SemSys.exc_sem_phon);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&PhonLex.exc_phon_sem, crap);
		else sscanf(Line,"%f",&PhonLex.exc_phon_sem);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%d%s",&SemSys.SemStart, crap);
		else sscanf(Line,"%d",&SemSys.SemStart);
		fgets(Line,200,fp); 
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&SemSys.SemDecay, crap);
		else sscanf(Line,"%f",&SemSys.SemDecay);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&OrthLex.OrthDecayPoint, crap);
		else sscanf(Line,"%f",&OrthLex.OrthDecayPoint);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&OrthLex.exc_orth_sem, crap);
		else sscanf(Line,"%f",&OrthLex.exc_orth_sem);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&OrthLex.inh_orth_sem, crap);
		else sscanf(Line,"%f",&OrthLex.inh_orth_sem);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&SemSys.exc_sem_orth, crap);
		else sscanf(Line,"%f",&SemSys.exc_sem_orth);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&SemSys.inh_sem_orth, crap);
		else sscanf(Line,"%f",&SemSys.inh_sem_orth);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&SemSys.inh_sem_sem, crap);
		else sscanf(Line,"%f",&SemSys.inh_sem_sem);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&SemSys.exc_sem_semfeatures, crap);
		else sscanf(Line,"%f",&SemSys.exc_sem_semfeatures);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&SemFeatures.exc_out_sem, crap);
		else sscanf(Line,"%f",&SemFeatures.exc_out_sem);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&PhonLex.PhonDecayPoint, crap);
		else sscanf(Line,"%f",&PhonLex.PhonDecayPoint);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%d%s",&PhonFeatures.TakeAwayPhonetic, crap);
		else sscanf(Line,"%d",&PhonFeatures.TakeAwayPhonetic);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&Features.FeatProbErr, crap);
		else sscanf(Line,"%f",&Features.FeatProbErr);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&Features.FeatActLevel, crap);
		else sscanf(Line,"%f",&Features.FeatActLevel);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%d%s",&CyclesBeforeNaming, crap);
		else sscanf(Line,"%d",&CyclesBeforeNaming);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonBuf.AboveMin, crap);
		else sscanf(Line,"%f", &PhonBuf.AboveMin);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%d%s", &DerivCycles, crap);
		else sscanf(Line,"%d", &DerivCycles);
		fgets(Line,200,fp);
			
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s", &PhonBuf.Deriv, crap);
		else sscanf(Line,"%f", &PhonBuf.Deriv);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&PhonLex.exc_stress, crap);
		else sscanf(Line,"%f",&PhonLex.exc_stress);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&PhonLex.inh_stress, crap);
		else sscanf(Line,"%f",&PhonLex.inh_stress);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&PhonBuf.StressPhon_Phon, crap);
		else sscanf(Line,"%f",&PhonBuf.StressPhon_Phon);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&PhonBuf.StressLev, crap);
		else sscanf(Line,"%f",&PhonBuf.StressLev);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&ZNet.exc_ZNet_phone, crap);
		else sscanf(Line,"%f",&ZNet.exc_ZNet_phone);
		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&LetterSpots.LetterOverPar, crap);
		else sscanf(Line,"%f",&LetterSpots.LetterOverPar);

		fgets(Line,200,fp);
		if(NoSpaces(Line) > 0)sscanf(Line,"%f%s",&ZNet.DeadWeightLevel, crap);
		else sscanf(Line,"%f",&ZNet.DeadWeightLevel);					
		fclose(fp);
}