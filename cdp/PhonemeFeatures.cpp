// PhonemeFeatures.cpp: implementation of the PhonemeFeatures class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PhonemeFeatures.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PhonemeFeatures::PhonemeFeatures()
{
	inh_pfeature_phoneme = -0.15;
	exc_pfeature_phoneme = 0.005;
	TakeAwayPhonetic = 1000;
	Noise = 0.0;
}

PhonemeFeatures::~PhonemeFeatures()
{

}

void PhonemeFeatures::resetfeatures()
{
	int i;

	for(i=0;i<MAXPHONE;i++) {
		PhonFeatureSet[i].ResetFeature();
	}
}

void PhonemeFeatures::ClampPhon(char *Word)
{
	int i;

	for(i=0;i<MAXPHONE;i++) {
		PhonFeatureSet[i].SetFeature(Word[i]);
	}
}

