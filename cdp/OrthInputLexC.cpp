// OrthInputLexC.cpp: implementation of the OrthInputLexC class.
//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "OrthInputLexC.h"
#include <malloc.h>
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

OrthInputLexC::OrthInputLexC()
{
	int i;

	LexM = 0.5;		  // M Criterion
	Total_Sum = 0.8;  // Total_Sum
	Total_Sum2 = 5.0;   // Total SUm2
	Total_Sum_Change = 10;
	Total_Sum_Act_Change = 5.0;
	
	Orth_Orth = -0.06;
	exc_word_letter = 0.30;
	inh_word_letter = 0;
	exc_orth_phon = 0.2;

	M_Minus = 5;	//if at M_time LexM > M_Act_Minus then Cycles -= M_Minus
	M_Act_Minus = 5.0;
	M_Time = 10;

	OrthDecayPoint = 0.5;
	Decay = 0.0;
	Noise = 0.0;
	exc_orth_sem = 0.0;
	inh_orth_sem = 0.0;
}

OrthInputLexC::~OrthInputLexC()
{
	//free(Activations);
	//free(NetInput);
}

void OrthInputLexC::Initialize_It(int NumW) 
{
	int i;
	FILE *fp1;

	/*fp1 = fopen("lookit.txt","a");
	fprintf(fp1,"%d \n",NumW);
	fclose(fp1);*/

	Num_Word = NumW;

//	Activations = (float *) calloc(NumW + 2, sizeof(float));
	//NetInput = (float *) calloc(NumW + 2, sizeof(float));

//	if(!Activations) exit(1);
//	if(!NetInput) exit(1);

	for(i=0;i<Num_Word;i++) {
		OrthNConnects[i] = 0;
	}


/*	OrthConnects[ORTHRED][0] = 0;
	OrthNConnects[ORTHRED] = 1;	
	OrthConnects[ORTHGREEN][0] = 1;
	OrthNConnects[ORTHGREEN] = 1;
	OrthConnects[ORTHBLUE][0] = 2;
	OrthNConnects[ORTHBLUE] = 1;
	
	OrthConnects[ORTHWHITE][0] = 6;
	OrthNConnects[ORTHWHITE] = 1;	
	OrthConnects[ORTHBLACK][0] = 7;
	OrthNConnects[ORTHBLACK] = 1;*/
}


void OrthInputLexC::ResetOrth()
{
	int i;
	FILE *fp1;

	for(i=0;i<Num_Word;i++) {
		Activations[i] = 0;
	}
}

void OrthInputLexC::ResetLatInhibition()
{
	LatInhibition = 0;
}
