// PhonLexC.h: interface for the PhonLexC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONLEXC_H__BE7F318B_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_PHONLEXC_H__BE7F318B_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "GlobalC.h"
#include "WordDataC.h"

class PhonLexC : WordDataC
{
public:

	float Activations[NUMBER_OF_WORDS];
	float NetInput[NUMBER_OF_WORDS];

	float LatInhibition;


	float exc_stress;
	float inh_stress;

	float exc_phonU_phon;
	float inh_phonU_phon;
	float exc_phonU_orth;	
	float PhonUnit_PhonUnit;
	
	float Decay;
	float Noise;
	float MaxPhon;
	int OverPoint;

	float VTotal_Sum;  // Total_Sum

	void Initialize_It(int NumP);
	void ResetPhon();
	void ResetLatInhibition();

	PhonLexC();
	virtual ~PhonLexC();

	int PhonPointDecayOn ;
	float PhonDecayPoint;

	int PHONRED; 
	int PHONGREEN; 
	int PHONBLUE;  
	int PHONWHITE;
	int PHONBLACK;

 
	int PhConnects[NUMBER_OF_PRONS][5];
	int PhNConnects[NUMBER_OF_PRONS];
	float exc_phon_sem;

	int Num_Phon;
private:

};



#endif // !defined(AFX_PHONLEXC_H__BE7F318B_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
