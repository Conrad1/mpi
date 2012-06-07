// PhonFeature.cpp: implementation of the PhonFeature class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PhonFeature.h"
#include "GlobalC.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PhonFeature::PhonFeature()
{

}

PhonFeature::~PhonFeature()
{

}

void PhonFeature::SetFeature(char Letter) 
{
	int i, spot;

	for(spot=0;spot<NUMBER_OF_PHONEMES;spot++) {
		if(Letter == Phonemes[spot] ) break;
	}

	if(spot != strlen(Phonemes) ) {
		for(i=0;i<NPHONEME_FEATURES;i++) {
			Block[i] = PhonemeBlock[spot][i];
			ABlock[i] = 1 - Block[i];
		}
	} else {
		for(i=0;i<NPHONEME_FEATURES;i++) {
			Block[i] = 0;
			ABlock[i] = 0;
		}	
	}
}

void PhonFeature::ResetFeature() 
{
	int i;
	for(i=0;i<NPHONEME_FEATURES;i++) {
		Block[i] = 0;
		ABlock[i] = 0;
	}
}


