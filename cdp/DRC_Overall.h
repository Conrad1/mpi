// DRC_Overall.h: interface for the DRC_Overall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRC_OVERALL_H__BE7F3190_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_DRC_OVERALL_H__BE7F3190_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if MSDEV

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#endif

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include "GlobalC.h"
#include "DRC_OS.h"
#include "The_DRC_Parts.h"

class DRC_Overall : public DRC_OS, GlobalC
{
public:
	//For running a word
	int TestWord(char *theword, int spot, int runcycles, int Fast);		
	
	int ClearGraph;

//	DRC_OS A_DRC_OS;
	int LetterOff, OrthOff, PhonOff, PhonemeOff, GPCOff, PGCOff;
	int SpecSaveNumber, SpecSaveNumberPhon;
	int SpecTotOrthSave, SpecTotPhonSave;
	int SwitchBoth;
	
	int MarcoDelay, MarcoNormal, MarcoGrapheme;

	
	void ClearRun();
	int RunCycle();
	int RunCycleSpell();
	void FixPron(); //changes the phonbuf word into FinalWord, in the GPCRoute
	
	void AddWord(char *Word);
	void AddWords(char *Word, char *Word2, float amount1, float amount2);
	void AddPron(char pron[]);
	void TrainNetwork(int Cycles);


	int Initialize_It(int OldFreq); 
	int Initialize_Rules();
	int DecayEveryCycle;
	void DecayCycle();

	int Cycle;



	int Num_Word;	//Number of Words
	int Num_Phon;	//Number of Prons
	int TheColor; //For Stroop

	char TestLoading[30];

	DRC_Overall();
	virtual ~DRC_Overall();

	int InitialWordLength;
	int AddNoise;

	int UsingZorzi;

	int Overall_Count;

	//int temporder[32000];
	//int temporderphon[32000];
	//float summedfreq[32000];
	int  CyclesTrained;

	//int temp[32000];
	//int Atemporder[32000];
	//int Atemporderphon[32000];

	int randomized;

	int OldN;
	int JustChanged;
	float OldLastLetterSpotMax[30], LastLetterSpotMax[30];
	int LastLetterSpot[30], testcycle1, OldLastLetterSpot[30];

	int resetcyc;

	int alldone;


	// Lexical Decision Mechanisms
	inline float ActivationFunc(float actinput, float Initial_Act);
private:
	int sum_init; // Initialized the total sum graph ?
	
	float WdAct[MAXPOS];	// Carries the word activation for the GPC route
	float MaxSpots[MAXPOS];
	char LetterStr[MAXPOS + 1];
	char PhonemeStr[MAXPOS + 1];
	char ActualWord[MAXPOS + 1];
	char lowerword[40];

	void UpdatePgcRoute();
	void PGCPtO();
	void SetLatInhibition();	//Sets up the lateral inhibition
	void FtOtLetter();			// Letter Level Update - features and orthlex
	void PhUtPhoneme();			// phoneme level updat - phonlex
	void LtPhtOrthography();	// orthlex update - letters and phonlex
	void PhtOtPhonLex();		//	phonlex updat - phonemes and orthlex
	void GPCOtP();				//Add activation produced by the GPC route	
	void UpdateGpcRoute();
	void UpdateSemantic();
	void UpdateActivation();


	inline float ConstrainAct(float Act);
};


#endif // !defined(AFX_DRC_OVERALL_H__BE7F3190_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
