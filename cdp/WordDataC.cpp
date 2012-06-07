
// WordDataC.cpp: implementation of the WordDataC class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "WordDataC.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "../common.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


WordDataC::WordDataC()
{
	fgain_tmp = 0.05;
	
}

WordDataC::~WordDataC()
{

}

int WordDataC::LoadData()
{

	FILE *fp;


	//These two are for later

	char wd[25];
	char ph[25];
	char phSeg[25];
	float F, SF;


	FILE *fp1, *fp2;
	int i, j = 0, k, count = 0, len, len2;

	
	//exit(1);

	highestorth = -1000;
	highestphon = -1000;
	
	//exit(1);
	if( (fp = fopen(DataFileName,"r")) == 0) {
			return FALSE;
	}
	
////	if( (fp1 = fopen("trash.txt","w")) == 0) {
//			return FALSE;	
//	}
	

//	fp1 = fopen("trash4.txt","a");
//	fprintf(fp1,"here \n");
//	fclose(fp1);
//	exit(1);

	while (fscanf(fp, "%s%s%s%f%f", wd, ph, phSeg, &F, &SF) != EOF) {
		fp1 = fopen("trash5.txt","a");
		fprintf(fp1,"%s%s%s%f%f\n", wd, ph, phSeg, &F, &SF);
		fclose(fp1);

		len = strlen(wd);
		len2 = len;
		for (i=0; i<len; i++) wd[i] = Language_Data_Hack(wd[i]);
		for (; i<MAXPOS; i++) wd[i] = WORDPAD;
		wd[MAXPOS] = 0;
		strcpy(TempWord[count], wd);

		len = strlen(ph);
	    for (i=len; i<MAXPHONE; i++) ph[i] = WORDPAD;
	    ph[MAXPHONE] = 0;
		strcpy(TempPron[count], ph);
		if(F < 1.0) F = 1.0;
		if(SF < 1.0) SF = 1.0;

	    TempFreq[count] = F;
		TempSFreq[count] = SF; 
		
	/*	if(Morphotacticalize == 1) {
			exit(1);
			if(wd[len2-1] == 'd' && wd[len2 - 2] == 'e') {
			   if(ph[len-1] == 't') {
					ph[len - 1] = 'd';
			   }         
			} else if ( wd[len2 -1] == 's' && ph[len-1] == 'z') {
				if( !(strcmp(wd,"has") == 0 || strcmp(wd,"is") == 0 || strcmp(wd,"his") == 0) ) {
					ph[len-1] = 's';
				}
			}
		} */

		count++;

	


	}
//	for(i=0;i<count;i++) {
//		fprintf(fp1,"%d %s %s %f \n",i,TempWord[i],TempPron[i],TempFreq[i]);
//	}
//	fclose(fp1);
//	fprintf(fp1,"%d \n",count);
	fclose(fp);
	AllCount = count;

	Word_Count = 0;
	
	for(i=0;i<count;i++) {
		Taken[i] = 0;
		WTaken[i] = 0;
	}

	for(i=0;i<NUMBER_OF_WORDS;i++) Freq[i] = 0;
	for(i=0;i<NUMBER_OF_PRONS;i++) PhFreq[i] = 0;
	   
	//Squish Orth - Take Out Dups
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(WTaken[j] == 1) {
					if( (strcmp(TempWord[i], TempWord[j]) ) == 0) {
						break;
					}
				}
			}
		}
		if(j == count) {
			strcpy(Word[Word_Count], TempWord[i]);
			WTaken[i] = 1;
#if ENGLISH
			Freq[Word_Count] = TempFreq[i]; 
#else
			Freq[Word_Count] += TempFreq[i]; 
#endif
			if(Freq[Word_Count] > highestorth) {
				highestorth = Freq[Word_Count];
			}
			Word_Count++;
		} else {
			k = 0;
		//	fp2 = fopen("HHs","a");
		//	fprintf(fp2,"%s \n", TempWord[i]);
		//	fclose(fp2);
			while( strcmp(TempWord[i], Word[k]) != 0) k++; 
#if ENGLISH	
			Freq[k] = TempFreq[i];
#else
			Freq[Word_Count] += TempFreq[i]; 
#endif
			
			if(Freq[k] > highestorth) {
				highestorth = Freq[k];
			}

		}
	}


	//Squish Phon - Take Out Dups
	Pron_Count = 0;
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(Taken[j] == 1) {
					if( (strcmp( TempPron[i], TempPron[j]) ) == 0) {
						break;
					}
				}
				
			}
		}
		if(j == count) {
			strcpy(Phon[Pron_Count], TempPron[i]);
			Taken[i] = 1;
#if ENGLISH
			PhFreq[Pron_Count] = TempSFreq[i];
#else
			PhFreq[Pron_Count] += TempSFreq[i];
#endif
			
			if(PhFreq[Pron_Count] > highestphon) {
				highestphon = PhFreq[Pron_Count];
			}
			Pron_Count++;
		} else {
			k = 0;
			while(  strcmp(TempPron[i], Phon[k]) != 0) k++;
#if ENGLISH
			PhFreq[k] = TempSFreq[i];
#else			
			PhFreq[k] += TempSFreq[i];
#endif
			//	fprintf(fp1,"%s \n",TempPron[i]);
			if(PhFreq[k] > highestphon) {
				highestphon = PhFreq[k];
			}

		}
	}
	//Fix Freqs
   	for(i=0;i<Word_Count;i++) {
		OldFreq[i] = Freq[i];
		Freq[i] = ( (log10(Freq[i]) - log10(highestorth)) / log10(highestorth) ) ;
	}
	for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]) - log10(highestphon)) / log10(highestphon)) ;  
	word_log_divisor = log10(highestorth);
	phon_log_divisor = log10(highestphon);

	
	// Connect the new lexicons //
	for(i=0;i<Word_Count;i++) NWord_to_Phon[i] = 0;
	for(i=0;i<Pron_Count;i++) {
		NPhon_to_Word[i] = 0;
	}
	for(i=0;i<Word_Count;i++) {
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			if(Word[i][j] == WORDPAD) {
				Word_Length[i] = j ;
				break;
			}
		}
	}

	
	for(i=0;i<count;i++) {
		for(j=0;j<Pron_Count;j++) {
			if( (strcmp(TempPron[i], Phon[j])) == 0) break;
		}
		for(k=0;k<Word_Count;k++) {
			if( (strcmp(TempWord[i], Word[k])) == 0) break;
		}
		//if(j == Pron_Count) exit(1); //Something has gone wrong :(
		Word_to_Phon[k][ NWord_to_Phon[k] ] = j;
		NWord_to_Phon[k] += 1;
		Phon_to_Word[j][ NPhon_to_Word[j] ] = k;
		NPhon_to_Word[j] += 1;
	}
//	for(i=0;i<Word_Count;i++) {
//		fprintf(fp1,"%d %s\n",i,Word[i]);
//	}
//	fprintf(fp1,"%d %d\n",Word_Count,Pron_Count);
//	fclose(fp1);
	return TRUE; 
}

int WordDataC::WordPos(char *TheWord) 
{
	int i;
	for(i=0;i<Word_Count;i++) {
		if(strcmp(TheWord, Word[i]) == 0) return i;
	}
	return -1;
}

int WordDataC::PhonPos(char *TheWord) 
{
	int i;
	FILE *fp1;
//	fp1 = fopen("phns.txt","a");
//	fprintf(fp1,"%s \n",TheWord);
	for(i=0;i<Pron_Count;i++) {
	//	fprintf(fp1,"%s \n",Phon[i]);
		if(strcmp(TheWord, Phon[i]) == 0) return i;
	
	}
//	fclose(fp1);
	return -1;
}



int WordDataC::LoadOldData()
{
		return FALSE; //Ok not supporting this at the mo;
	//return TRUE;
}



