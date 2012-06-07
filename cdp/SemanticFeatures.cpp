// SemanticFeatures.cpp: implementation of the SemanticFeatures class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SemanticFeatures.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


SemanticFeatures::SemanticFeatures()
{

	NumFeatures = 7;

	exc_semfeat_sem = 0.05;
	
	SemFeat_to_Sem[0][0] = 0; //Red
	SemFeat_to_Sem[1][0] = 1; //Green
	SemFeat_to_Sem[2][0] = 2; //Blue
	SemFeat_to_Sem[5][0] = 6; //White
	SemFeat_to_Sem[6][0] = 7; //Black

	SemFeat_to_SemWeight[0][0] = 1;
	SemFeat_to_SemWeight[1][0] = 1;
	SemFeat_to_SemWeight[2][0] = 1;
	SemFeat_to_SemWeight[5][0] = 1;
	SemFeat_to_SemWeight[6][0] = 1;

	
	NSemFeat_to_SemWeight[0] = 1;
	NSemFeat_to_SemWeight[1] = 1;
	NSemFeat_to_SemWeight[2] = 1;
	NSemFeat_to_SemWeight[5] = 1;
	NSemFeat_to_SemWeight[6] = 1;

	NSemFeat_to_SemWeight[3] = 2;
	NSemFeat_to_SemWeight[4] = 2;



	SemFeat_to_Sem[3][0] = 3; //Bread
	SemFeat_to_Sem[3][1] = 5; //Toast
	SemFeat_to_Sem[4][0] = 4; //COok
	SemFeat_to_Sem[4][1] = 5; //Toast
	SemFeat_to_SemWeight[3][0] = 1;
	SemFeat_to_SemWeight[3][1] = /*sqrt(1.0)*/ .7;
	SemFeat_to_SemWeight[4][0] = 1;
	SemFeat_to_SemWeight[4][1] = /*sqrt(1.0)*/ .7;


	exc_out_sem = 0.5;


}


SemanticFeatures::~SemanticFeatures()
{

}

void SemanticFeatures::ResetFeatures()
{
	int i;

	for(i=0;i<NumFeatures;i++) {
		FeatureAct[i] = 0.0;
	}
}
