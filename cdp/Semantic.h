// Semantic.h: interface for the Semantic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMANTIC_H__ACA26B60_3285_11D2_8073_00A0C9B00366__INCLUDED_)
#define AFX_SEMANTIC_H__ACA26B60_3285_11D2_8073_00A0C9B00366__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "WordDataC.h"
#include "GlobalC.h"

//Note this will have to become a big blob with the word data...
class Semantic 
{
public:

	Semantic();
	virtual ~Semantic();

	void InitializeSem(int ORTHRED,int ORTHGREEN,int ORTHBLUE,int ORTHWHITE,int ORTHBLACK,int PHONRED,int PHONGREEN,int PHONBLUE,int PHONWHITE,int PHONBLACK);

	
	void ResetFeatures();
	int SemStart;
	int CurrentSem;
	int Connects[10][5];
	int NConnects[10];
	float SemAct[10];
	float NetInput[10];
	int OrthConnects[10][5];
	int OrthNConnects[10];
	float exc_sem_phon;
	float inh_sem_phon;
	float exc_sem_orth;
	float inh_sem_orth;
	float SemDecay;

	void ResetLatInhibition();
	float LatInhibition;
	float inh_sem_sem;
	float exc_sem_semfeatures;

	int Sem_to_SemFeat[10][5]; 
	int NSem_to_SemFeat[5];
	float Sem_to_SemFeatWeight[10][5];
	
	
};

#endif // !defined(AFX_SEMANTIC_H__ACA26B60_3285_11D2_8073_00A0C9B00366__INCLUDED_)
