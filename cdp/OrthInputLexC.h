// OrthInputLexC.h: interface for the OrthInputLexC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORTHINPUTLEXC_H__BE7F3188_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_ORTHINPUTLEXC_H__BE7F3188_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "GlobalC.h"

class OrthInputLexC  
{
public:
	//float Activations[NUMBER_OF_WORDS];
	//float NetInput[NUMBER_OF_WORDS];
	
	int OrthConnects[NUMBER_OF_WORDS][5];
	int OrthNConnects[NUMBER_OF_WORDS];

	//float *Activations;
	float Activations[NUMBER_OF_WORDS];
	float NetInput[NUMBER_OF_WORDS];
	
	float LatInhibition;
	
	//Letter to Letter Inhibition//
	float Orth_Orth; 
	float exc_word_letter;
	float inh_word_letter;
	float exc_orth_phon;	
	float Decay;
	float Noise;

	//Parameter Values
	float LexM;		  // M Criterion
	float Total_Sum;  // Total_Sum

	float Total_Sum2;   // Total SUm change to 
	int	  Total_Sum_Change; // if after total_sum_change cycles 
	float Total_Sum_Act_Change; // acts is over total_sum_act_change

	int   M_Minus;		//if at M_time LexM > M_Act_Minus then Cycles -= M_Minus
	float M_Act_Minus;
	int   M_Time;

	
	//Current Values
	float VLexM;		  // M Criterion
	float VTotal_Sum;  // Total_Sum
	int VLexM_Spot;

	float exc_orth_sem;
	float inh_orth_sem;

	float OrthDecayPoint;
	int PointDecayOn;
	int OverPoint;

	void ResetOrth();
	void ResetLatInhibition();
	void Initialize_It(int NumW);

	int ORTHRED; 
	int ORTHGREEN; 
	int ORTHBLUE; 
	int ORTHWHITE; 
	int ORTHBLACK; 


	OrthInputLexC();
	virtual ~OrthInputLexC();

	int Num_Word;

private:

	

};


#endif // !defined(AFX_ORTHINPUTLEXC_H__BE7F3188_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
