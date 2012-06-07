// Feature.cpp: implementation of the Feature class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Feature.h"

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


Feature::Feature()
{
		
}

Feature::~Feature()
{

}

void Feature::SetFeature(char Letter) 
{
	int i, spot;
	FILE *fp;

	for(spot=0;spot<NLETTERS;spot++) {
		if(Letter == Alphabet[spot] ) break;
	}
	/*fp = fopen("featsset.txt","a");
	fprintf(fp,"%d %c \n",spot,Letter);
	fclose(fp);*/

	if(spot != strlen(Alphabet) ) {
		for(i=0;i<NUMBER_OF_FEATURES;i++) {
			Block[i] = LetterBlock[spot][i];
			ABlock[i] = 1 - Block[i];
		}
	} else exit(1);
}


void Feature::ResetFeature() 
{
	int i;

	for(i=0;i<NUMBER_OF_FEATURES;i++) {
		Block[i] = 0;
		ABlock[i] = 0;
	}
}


void Feature::SwitchIt(int theFeature) 
{
	if(theFeature > NUMBER_OF_FEATURES) {
		ABlock[theFeature - NUMBER_OF_FEATURES] = 1 - ABlock[theFeature - NUMBER_OF_FEATURES]; 	
	} else {
		Block[theFeature] = 1 - Block[theFeature]; 	
	} 
}

void Feature::SwitchOverBoth(int theFeature) 
{
	ABlock[theFeature] = 1 - ABlock[theFeature];
	Block[theFeature] = 1 - Block[theFeature];
}

void Feature::SwitchOnIt(int theFeature)
{
	int i = theFeature;
	while(Block[i] == 0) {
		i++;
		if(i == NUMBER_OF_FEATURES) i = 0;
	}
	ABlock[i] = 1 - ABlock[i];
	Block[i] = 1 - Block[i];
}

void Feature::SwitchOn(int theFeature)
{
	ABlock[theFeature] = 0;
	Block[theFeature] = 1;
}

void Feature::SwitchOff(int theFeature)
{
	ABlock[theFeature] = 1;
	Block[theFeature] = 0;
}


void Feature::MessIt(float prob) 
{
//	int i;




//	for(i=0;i<NUMBER_OF_FEATURES;i++) {
///		if( ((float) rand() / RAND_MAX) < prob)  {
//			Block[i] = 1;
//			ABlock[i] = 0;
//		}
//	}
}