// Semantic.cpp: implementation of the Semantic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Semantic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Semantic::Semantic()
{
	CurrentSem = 8;
	exc_sem_phon = 0.0;
	inh_sem_phon = 0.0;
	exc_sem_orth = 0.0;
	inh_sem_orth = 0.0;

	SemDecay = 0.0;
	SemStart = 1000;
	inh_sem_sem = 0.0;
	exc_sem_semfeatures = 0.05;

	Sem_to_SemFeat[3][0] = 3;
	Sem_to_SemFeat[4][0] = 3;
	Sem_to_SemFeat[4][1] = 4;
	Sem_to_SemFeat[5][2] = 5;
	NSem_to_SemFeat[3] = 1;
	NSem_to_SemFeat[4] = 2;
	NSem_to_SemFeat[5] = 1;
	Sem_to_SemFeatWeight[3][0] = 1;
	Sem_to_SemFeatWeight[4][0] = 1;
	Sem_to_SemFeatWeight[4][1] = 1;
	Sem_to_SemFeatWeight[5][0] = 1;
}

void Semantic::ResetLatInhibition()
{
	LatInhibition = 0;
}

Semantic::~Semantic()
{

}

void Semantic::ResetFeatures()
{
	int i;

	for(i=0;i<CurrentSem;i++) SemAct[i] = 0;

//	SemAct[0] = 0;	//Red
//	SemAct[1] = 0;	//Green
//	SemAct[2] = 0;	//Blue
}


void Semantic::InitializeSem(int ORTHRED,int ORTHGREEN, int ORTHBLUE, int ORTHWHITE, int ORTHBLACK,int PHONRED,int PHONGREEN,int PHONBLUE,int PHONWHITE,int PHONBLACK)
{
	FILE *fp;

	/* stroop */

	//Red
	Connects[0][0] = PHONRED; 
	NConnects[0] = 1;
	
	//Green
	Connects[1][0] = PHONGREEN;
	NConnects[1] = 1;

	//Blue
	Connects[2][0] = PHONBLUE;
	NConnects[2] = 1;

	//White
	Connects[6][0] = PHONWHITE;
	NConnects[6] = 1;

	//Black
	Connects[7][0] = PHONBLACK;
	NConnects[7] = 1;



	//Bread
	Connects[3][0] = 487;
	Connects[4][0] = 1091;
	Connects[5][0] = 6385;
	NConnects[3] = 1;
	NConnects[4] = 1;
	NConnects[5] = 1;



	OrthConnects[0][0] = ORTHRED; 
	OrthNConnects[0] = 1;
	
	//Green
	OrthConnects[1][0] = ORTHGREEN;
	OrthNConnects[1] = 1;

	//Blue
	OrthConnects[2][0] = ORTHBLUE;
	OrthNConnects[2] = 1; 

	//White
	OrthConnects[3][0] = ORTHWHITE;
	OrthNConnects[3] = 1;

	//Black
	OrthConnects[4][0] = ORTHBLACK;
	OrthNConnects[4] = 1; 


}

