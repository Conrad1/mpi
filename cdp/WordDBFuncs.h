// WordDBFuncs.h: interface for the WordDBFuncs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORDDBFUNCS_H__89B8F761_40D4_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_WORDDBFUNCS_H__89B8F761_40D4_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WordDataC.h"


class WordDBFuncs : public WordDataC
{
public:
	char LoadDirectory[1000];
	
	void SaveBodies();
	void SaveCCVC();
	void SaveHomophones();			//Need to stick interface up
	void SaveSplitHomophones();		//Need to stick interface up

	int ReLoadData(char *NeighWord);
	int Neighbours(char *The_Word);
	int HFNNeighbours(char *The_Word, float WordFreq);
	int PhonNeighbours(char *NeighWord); 
	int PhonNeighboursFromPhon(char *NeighWord); 
	int BodNeighbours(char *The_Word);
	int countit(char *line);
	void FunnyNeigh(char *The_Word);
	int NPhonBods(char *The_Word);
	float Consistency(char *The_Word, char *The_Phon) ;
	float FBConsistency(char *The_Word, char *The_Phon) ;


	int ReLoadDataSlowPrime(char *NeighWord, char *AssPron, char *PrimeWord, char *AssPronPrime);
	int ReLoadDataSlow2(char *NeighWord, char *AssPron);
	int ReLoadDataSlow3(char *NeighWord, char *AssPron);

	void CalcAllBods();
	void GetPhonBod(char *The_Word);
	void GetBod(char *The_Word);
	int CountStartPron(char *The_Word);
	void SplitIt();
	void SplitVowels();
	void SplitVowels2();
	void ConsistentBodies();
	void RimeOnsetLength(); 
	int ReLoadDataSlow(char *NeighWord, char *AssPron);
	void OnePlusNeigh();
	void ChopAllPhons();
	int MarcoLoadData();

	
	void CalcVowelContingency(char *TheWord, char *The_Pron);
	void CalcContingency(char *The_Word, char *The_Pron);
	void CalcCodaContingency(char *The_Word, char *The_Pron);
	void SaveContingencyHomophones();
	void Onsets(); 
	void CalcSecondSyllConsis();
	int levenshtein( char *a, char *b );
	float levenshteintop20( char *TheWord, int OorP);

	void AddWordOrth(char *theword, char *thephon, int thestress, float freq, float phonfreq);



	int TLCs(char *The_Word);
	void RimeBods();


	int OnsetSpecific;
	int OnsetTotalCount;
	float AvgContingency;
	float MaxContingency;
	float AvgCodaContingency;
	float MaxCodaContingency;

	

	//Initial Consonant
	char WDBPhonemes[30];
	char WDBLetters[30][20][6];
	int LettersCount[30][20];
	int UpToPhonemes;
	int UpToLetters[30];


	char VowelPhonemes[30];
	char VowelPhonemeLetters[30][22][6];
	int VowelPhonemeLettersCount[30];
	int NVowelPhonemeLettersCount[30][22];

	int VowelCount[20];
	int UpToVowel;

	char CodaPhonemes[30];
	char CodaPhonemeLetters[30][22][6];
	int CodaPhonemeLettersCount[30];
	int NCodaPhonemeLettersCount[30][22];

	int CodaCount[20];
	int UpToCoda;


	WordDBFuncs();

	virtual ~WordDBFuncs();

	char LastBod[10];
	int LastBodLength;
	char LastOnset[10];
	int LastOnsetLength;
	char LastPhonBod[10];
	int LastPhonBodLength;
	char LastPhonOnset[10];
	 int LastPhonOnsetLength;

	 char Results[200][100];
	int NResults;

	int SameConsis;
	int DifConsis;
	float SameConsisFreq , DifConsisFreq ;

	 float neighbourfreqs;

	float NewTempFreq[NUMBER_OF_WORDS];
	float NewTempSFreq[NUMBER_OF_WORDS];
//	char NewTempSegPron[NUMBER_OF_WORDS][MAXPHONE + 1];
	char NewTempPron[NUMBER_OF_WORDS][MAXPOS + 1];
	char NewTempWord[NUMBER_OF_WORDS][MAXPHONE + 1]; 
	int NewStressPos[NUMBER_OF_WORDS];
	int thisword[NUMBER_OF_WORDS];

	float TempTokenConsis;
	int OverFreq;
	int FoundWords;

	char TheFoundWord[50];


	int DoneArray[NUMBER_OF_WORDS];

	//int compare(const void * a, const void * b);



	struct Lev {
		int LOD;
		int freq;
		char Word[30];
	};
	
	struct Lev LevValues[31000];
	float freq20;

private:
	
};

#endif // !defined(AFX_WORDDBFUNCS_H__89B8F761_40D4_11D1_B56F_0080C8586A61__INCLUDED_)
