// PhonLexC.cpp: implementation of the PhonLexC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PhonLexC.h"
#include "../common.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PhonLexC::PhonLexC()
{
	int i;
	PhonUnit_PhonUnit = -0.07;

	exc_phonU_phon = 0.14;
	inh_phonU_phon = -0.02;
	exc_phonU_orth = 0.2;	
	exc_phon_sem = 0.05;

	exc_stress = 0.14;
	inh_stress = 0;
	
	Decay = 0;
	Noise = 0;

	PhonDecayPoint = 0.5;


}


PhonLexC::~PhonLexC()
{
	//free(Activations);
	//free(NetInput);

}


void PhonLexC::Initialize_It(int NumP)
{
	int i;

	Num_Phon = NumP;
	
//	Activations = (float *) calloc(NumP + 1, sizeof(float) );
	//NetInput = (float *) calloc(NumP + 1, sizeof(float) );
	//if(!Activations) exit(1);
	//if(!NetInput) exit(1);
	
	for(i=0;i<Num_Phon;i++) {
		PhNConnects[i] = 0;
	}


	//* Fix this some time */
		
	PhConnects[PHONRED][0] = 0;
	PhNConnects[PHONRED] = 1;	
	PhConnects[PHONGREEN][0] = 1;
	PhNConnects[PHONGREEN] = 1;
	PhConnects[PHONBLUE][0] = 2;
	PhNConnects[PHONBLUE] = 1;
	PhConnects[PHONWHITE][0] = 6;
	PhNConnects[PHONWHITE] = 1;
	PhConnects[PHONBLACK][0] = 7;
	PhNConnects[PHONBLACK] = 1;

}

void PhonLexC::ResetPhon()
{
	int i;

	for(i=0;i<Num_Phon;i++) Activations[i] = 0;
}

void PhonLexC::ResetLatInhibition()
{
	LatInhibition = 0;
}
