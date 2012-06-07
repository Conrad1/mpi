// PhonemeOutBufC.cpp: implementation of the PhonemeOutBufC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PhonemeOutBufC.h"
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

PhonemeOutBufC::PhonemeOutBufC()
{
	Phon_Phon = -0.15;
	exc_phon_word = 0.04;	
	inh_phon_word = -0.16;
	StressPhon_Phon = -0.02;
	StressLev = .7;
	SecondVowelAct = .3;

	Decay = 0.0;
	Noise = 0.0;

	PhoneMin = 0.68;
	WordLatency = 0;
	WordSaid = FALSE;
	AboveMin = 0.05;	
	Deriv = .0025;
}


PhonemeOutBufC::~PhonemeOutBufC()
{

}

void PhonemeOutBufC::ResetBuf()
{
	int i, j;

	WordSaid = 0;	
	for(i=0;i<MAXPHONE;i++) {
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			Buffer[i][j] = 0;
		}
	}
}

void PhonemeOutBufC::ResetLatInhibition()
{
	int i;
	
	StressLatInhibition = 0;
	for(i=0;i<MAXPHONE;i++) {
		LatInhibition[i] = 0;
	}
}

void PhonemeOutBufC::ClampPron(char *Pron)
{
	int i, j;
	
	for(i=0;i<MAXPHONE;i++) {
		j = PhonemeIndex(Pron[i]);
		if(j != -1) Buffer[i][j] = 1;
	}
}

/*int PhonemeOutBufC::PhonemeIndex(char Letter)
{
	int i = 0;

	while(i < NUMBER_OF_PHONEMES) {
		if(Phonemes[i] == Letter) {
			return i;
		}
		i++;
	}
	
	return -1;
}*/

int PhonemeOutBufC::PronWord()
{
	int i, j, max_units[MAXPHONE];
	int NullPos = 0, spot, minspot;
	int UpTo = 0;
	float max, min;
	float MaxDeriv,MaxDerivSpot,MaxStressVal;
	char TempWord;
	char NAssOnsets = 0;
	char TheVowelUnentered;

	float temp1, temp2;
	FILE *fp1;

	int USESUMPOS = 0,UpToSyll;
	int UseDerivative = USEDERIV;
	int VowelSyll = 0;
	float SumPos;
	int Pos[100];

	


	WordSoFar[0] = 0;

	if(WordSaid) return FALSE;

#if ISMARCO
	if(UseDerivative == 0) {  /* Not currently used */
		exit(1);
		return TRUE;
	} else {
		/* HERE *////////////////////////////////////////////////////
		MaxStress = -1;
		MaxStressVal = -1;
		for(i=(NUMBER_OF_PHONEMES-NSTRESS);i<NUMBER_OF_PHONEMES;i++) {
		/*	fp1 = fopen("teststressss.txt","a");
			fprintf(fp1,"%d %f %f \n",i,Buffer[MAXPHONE-1][i],StressLev);
			fclose(fp1);*/

			if(Buffer[MAXPHONE-1][i]>MaxStressVal) {
				MaxStressVal = Buffer[MAXPHONE-1][i];
				MaxStress = i-(NUMBER_OF_PHONEMES-NSTRESS);
			}
		}
		if(ITALIAN == 1) {
			if(MaxStress == 2) MaxStress = 1;
			else if(MaxStress == 1) MaxStress = 2;
		}
		if(MaxStressVal < StressLev) return FALSE;
		
		for(i=0;i<100;i++) Pos[i] = 0;

		UpToSyll = 0;	
		for(i=0;i<MAXPHONE;i++) {
			if(i % MAXSPOTSPERSYLL == 0) UpToSyll++;
	
			//LAST THREE SPOTS IN NETWORK DETERMINE STRESS 
			max = -1;
			min = 10000;
			MaxDeriv = -10000;
			spot = 0;
			SumPos = 0;
			for(j=0;j<NUMBER_OF_PHONEMES;j++) {
				if( !((i==MAXPHONE-1) && j > (NUMBER_OF_PHONEMES-(NSTRESS+1) ) ) ) {		
					SumPos += Buffer[i][j];
					if(Buffer[i][j] > max) {
						max = Buffer[i][j];
						spot = j;
					}
					if(Derivatives[i][j] > MaxDeriv) MaxDeriv = Derivatives[i][j];
				}
			}
			if(USESUMPOS == 0) {
				if(MaxDeriv > Deriv && max < PhoneMin) {
					WordSoFar[UpTo] = 0;
					return FALSE;
				} else if (max > PhoneMin) {
					/*fp1 = fopen("test33.txt","a");
					fprintf(fp1,"-->%d %d %d\n",Pos[i-MAXONSETS],i,i-MAXONSETS);
					fclose(fp1);*/
					Pos[i] = 1;
				}
			}
			if(USESUMPOS == 1) {
				if(MaxDeriv > Deriv && SumPos < PhoneMin) {
					WordSoFar[UpTo] = 0;
					return FALSE;
				}
			}
			/*fp1=fopen("test111.txt","a");
			fprintf(fp1,"%d %d %d %f %f\n",i, i%(MAXONSETS+1+MAXCODAS),MAXONSETS,max,SecondVowelAct);
			fclose(fp1);*/
			if( ((i%(MAXONSETS+1+MAXCODAS)) == (MAXONSETS)) && (i != (MAXONSETS)) ) { //Only use a second syll if it is there.
				if(USESUMPOS == 0) {
					if(max > PhoneMin) StartChecking = TRUE;								
					if( (Pos[i-MAXONSETS] == 1) || max > PhoneMin) {		//Use first part of or for Onset		
						/*fp1 = fopen("test33.txt","a");
						fprintf(fp1,"%d %d %d\n",Pos[i-MAXONSETS],i,i-MAXONSETS);
						fclose(fp1);*/
						
						//if(max > SecondVowelAct) { //Use this for second vowel criterion//
						WordSoFar[UpTo] = Phonemes[spot];
						UpTo++;
						VowelSyll = 0;
					} else {
						VowelSyll = UpToSyll;
						TheVowelUnentered = Phonemes[spot];
					}
				}
				if(USESUMPOS == 1) {
					if(SumPos > PhoneMin) StartChecking = TRUE;
					if(SumPos > SecondVowelAct) {
						WordSoFar[UpTo] = Phonemes[spot];
						UpTo++;
					}
				}

			} else if(i!=(MAXONSETS)) { //All others except vowel		
				if(USESUMPOS == 0) {				
					if(max > PhoneMin) {	
						StartChecking = TRUE;
						WordSoFar[UpTo] = Phonemes[spot];
						UpTo++;
					//	fp1 = fopen("testttt.txt","a");
					//	fprintf(fp1,"%d %d %d \n",i, VowelSyll, i/MAXSPOTSPERSYLL);
					//	fclose(fp1);
						if(i % MAXSPOTSPERSYLL > MAXONSETS) { 
							if(VowelSyll != 0 && (VowelSyll-1 == (int) ( i /  MAXSPOTSPERSYLL)) ) {
								WordSoFar[UpTo] = Phonemes[spot];
								WordSoFar[UpTo-1] = TheVowelUnentered;
								UpTo++;
								VowelSyll = 0;
							}
						} else if(i % MAXSPOTSPERSYLL < MAXONSETS) {
							if(VowelSyll != 0 && (VowelSyll-1 == (int) ( i /  MAXSPOTSPERSYLL)+1) ) {
								WordSoFar[UpTo] = Phonemes[spot];
								WordSoFar[UpTo-1] = TheVowelUnentered;
								UpTo++;
								VowelSyll = 0;
							}
						}

					} 
				}
				if(USESUMPOS == 1) {				
					if(SumPos > PhoneMin) {	
						StartChecking = TRUE;
						WordSoFar[UpTo] = Phonemes[spot];
						UpTo++;
					} 
				}
	
			} else {//First Vowel
				if(USESUMPOS == 0) {
					if(max > PhoneMin) StartChecking = TRUE;
				}
				if(USESUMPOS == 1) {
					if(max > PhoneMin) StartChecking = TRUE;
				}			
				WordSoFar[UpTo] = Phonemes[spot];
				UpTo++;

				/*fp1 = fopen("test333.txt","a");
				fprintf(fp1,"3 %d %d %d %d %f %s\n",i,MAXONSETS+1,i != (MAXONSETS+1), StartChecking,max,WordSoFar);
				fclose(fp1);*/
				//exit(1);
			}
		}

		WordSoFar[UpTo] = 0;
		WordSaid = 1;
			
			//	fp1 = fopen("test11111.txt","a");
			//	fprintf(fp1,"%d %s \n",UpTo, WordSoFar);
			//	fclose(fp1);

		if(StartChecking == FALSE) return FALSE;

		if(UpTo == 0) return FALSE;
		return TRUE;
	}

#else
	return TRUE;
#endif
}

int PhonemeOutBufC::CheckPhon(char *Phon)
{
	int test, i;

	for(i=0;i<MAXPOS;i++) {
		test = PhonemeIndex(Phon[i]); 
		if(test > strlen(Phonemes) || test < 0 && Phon[i] != WORDPAD) return FALSE;
	}
	return TRUE;
}
