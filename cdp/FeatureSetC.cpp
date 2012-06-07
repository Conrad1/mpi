// FeatureSetC.cpp: implementation of the FeatureSetC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FeatureSetC.h"
#include "GlobalC.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


FeatureSetC::FeatureSetC()
{
	inh_feature_letter = -0.15;
	exc_feature_letter = 0.005;
	FeatActLevel = 0;
	Noise = 0.0;
}

FeatureSetC::~FeatureSetC()
{

}


void FeatureSetC::resetfeatures()
{
	int i;

	//fix this !
	for(i=0;i<MAXPOS;i++) {
		FeatureSet[i].ResetFeature();
	}
}

void FeatureSetC::ClampWord(char *Word)
{
	int i;

	for(i=0;i<MAXPOS;i++) {
		FeatureSet[i].SetFeature(Word[i]);
	}
}

void  FeatureSetC::ClampWords(char *Word, char *Word2, float amount1, float amount2)
{
	int i,j, spot, spot2;
	char Letter1, Letter2;
	FILE *fp;

	fp=fopen("inhere.txt","w");
	fprintf(fp,"%s %s %f %f \n",Word,Word2,amount1,amount2);
	/*for(i=0;i<NLETTERS;i++) {
		for(j=0;j<NUMBER_OF_FEATURES;j++) {
			fprintf(fp,"%d ",LetterBlock[i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);*/

	for(i=0;i<MAXPOS;i++) {
		for(spot=0;spot<NLETTERS;spot++) {
			if(Word[i] == Alphabet[spot]) break;
		}
		for(spot2=0;spot2<NLETTERS;spot2++) {
			if(Word2[i] == Alphabet[spot2]) break;
		}
		fp=fopen("inhere.txt","a");
		fprintf(fp,"%s %s %f %f %d %d \n",Word,Word2,amount1,amount2,spot,spot2);
		fclose(fp);
		if(spot != strlen(Alphabet) && spot2 != strlen(Alphabet) ) {
			for(j=0;j<NUMBER_OF_FEATURES;j++) {
				if(spot == spot2) {
					if(LetterBlock[spot][j] == 0) {
						//FeatureSet[i].ABlock[j] = LetterBlock[spot][j] * amount1;
						//FeatureSet[i].Block[j] = 1 - LetterBlock[spot][j] * amount1;
						FeatureSet[i].Block[j] = 1 - amount1;
					} else {
						FeatureSet[i].Block[j] = LetterBlock[spot][j] * amount1;
					//	FeatureSet[i].ABlock[j] = 1 - LetterBlock[spot][j] * amount1;	
					}
				} else {
					if (LetterBlock[spot2][j] == 1) {
						FeatureSet[i].Block[j] = LetterBlock[spot2][j] * amount2;
						/*fp=fopen("inhere.txt","a");
						fprintf(fp,"-- %d %d %d %f\n",spot2,j,LetterBlock[spot2][j],LetterBlock[spot2][j] * amount2);
						fclose(fp);*/
					//	FeatureSet[i].ABlock[j] = 1 - (LetterBlock[spot][j] * amount2);
					}
					if(LetterBlock[spot][j] == 1) {
						FeatureSet[i].Block[j] = LetterBlock[spot][j] * amount1;
					//	FeatureSet[i].ABlock[j] = 1 - (LetterBlock[spot][j] * amount1);
					}			
				}
			}
			for(j=0;j<NUMBER_OF_FEATURES;j++) {
				FeatureSet[i].ABlock[j] = 1-FeatureSet[i].Block[j];
			}
		} 
	}
	fp=fopen("inhere.txt","a");
				
	for(i=0;i<MAXPOS;i++) {
		for(j=0;j<NUMBER_OF_FEATURES;j++) {
				fprintf(fp,"%f ",FeatureSet[i].Block[j]);
		}
		fprintf(fp,"\n");
		for(j=0;j<NUMBER_OF_FEATURES;j++) {
				fprintf(fp,"%f ",FeatureSet[i].ABlock[j]);
		}
		fprintf(fp,"\n\n");
	}
				
	fclose(fp);
}

void FeatureSetC::SwitchFeature(int Which, int Where)
{
	FeatureSet[Which].SwitchIt(Where);
}

void FeatureSetC::SwitchOnFeature(int Which, int Where)
{
	FeatureSet[Which].SwitchOnIt(Where);
}

void FeatureSetC::SwitchBothFeature(int Which, int Where)
{
	FeatureSet[Which].SwitchOverBoth(Where);
}

void FeatureSetC::MessFeatures(float prob, int UpTo)
{
	int i,j;

//	srand( (unsigned)time( NULL ) );
	
	for(i=0;i<UpTo;i++)	{
		for(j=0;j<NUMBER_OF_FEATURES;j++) {
	/*		if( ((float) rand() / RAND_MAX) < prob)  {
				FeatureSet[i].SwitchOn(j);
			} else {
				FeatureSet[i].SwitchOff(j);
			}
		} */
			if( ((float) rand() / RAND_MAX) < prob)  {
				FeatureSet[i].SwitchOverBoth(j);
			}
		}
	}
}
