// PhonemeOutBufC.h: interface for the PhonemeOutBufC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONEMEOUTBUFC_H__BE7F318A_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_PHONEMEOUTBUFC_H__BE7F318A_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "GlobalC.h"

#define MAXMEM 501

class PhonemeOutBufC : public GlobalC 
{
public:
	float Buffer[MAXPHONE * MAXSYLLABLES][NUMBER_OF_PHONEMES+1];		//Contains the activations oh the pm buffer
	float OldBuffer[MAXMEM][MAXPHONE * MAXSYLLABLES][NUMBER_OF_PHONEMES+1];
	float Derivatives[MAXPHONE * MAXSYLLABLES][NUMBER_OF_PHONEMES+1];
	float LatInhibition[MAXSYLLABLES * MAXPHONE+1];
	float StressLatInhibition;
	float StressPhon_Phon;
	float StressLev;

	float NetInput[MAXPHONE * MAXSYLLABLES][NUMBER_OF_PHONEMES+1] ;
	
	float Phon_Phon;
	float exc_phon_word;	
	float inh_phon_word;
		
	float Decay;
	float Noise;

	float PhoneMin;

	float AboveMin;
	float Deriv;

	int WordSaid;
	int WordLatency;
	
	char WordSoFar[(MAXPHONE+1) * MAXSYLLABLES];
	float TestHigh;

	void ResetBuf();			//Clear all the old values
	void ResetLatInhibition();
	void ClampPron(char *Pron); //Add a new phoneme set 
//	int PhonemeIndex(char Letter); //return position of phoneme X
	int CheckPhon(char *Phon);

	int PronWord();
	int StartChecking;

	int MaxStress;
	//int Cycle;
	
	float exc_sem_phon;

	PhonemeOutBufC();
	virtual ~PhonemeOutBufC();

	int NOrthSylls;
	float SecondVowelAct;
//	float themax, themaxderiv;

};

#endif // !defined(AFX_PHONEMEOUTBUFC_H__BE7F318A_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
