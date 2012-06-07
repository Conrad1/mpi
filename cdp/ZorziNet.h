// ZorziNet.h: interface for the ZorziNet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZORZINET_H__3475B0F0_4B65_11D6_81FA_00A0C9B00366__INCLUDED_)
#define AFX_ZORZINET_H__3475B0F0_4B65_11D6_81FA_00A0C9B00366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GlobalC.h"
#include <math.h>
#include <stdio.h>

#if ENGLISH
	#define NONSETS (10)
	#define NVOWELS	(50)
	#define NCODAS	(26)
	#define ATTENTIONWINDOWSIZE (5)

#define SCODS 21
#define DCODS 57


#elif GERMAN
	#define NONSETS (4)
	#define NVOWELS	(24)
	#define NCODAS	(28)
	#define ATTENTIONWINDOWSIZE (5)
#define SCODS 21
#define DCODS 57

#elif FRENCH
	#define NONSETS (7)
	#define NVOWELS	(62) //aou
	#define NCODAS	(28)  // dh, bh lh
	#define ATTENTIONWINDOWSIZE (5)

	#define SCODS 21
	#define DCODS 57

#elif ITALIAN
	#define NONSETS (5)
//	#define NVOWELS	(14)
	#define NVOWELS (8)
	//#define NCODAS	(20)
	#define NCODAS	(5)
	#define SCODS 21
	#define DCODS 57
	#define ATTENTIONWINDOWSIZE (3)
#endif

#define NGRAPHEMES (NONSETS + NVOWELS + NCODAS)

//#define LEGALCODAS 176
#define LEGALCODAS 309
//#define LEGALCODAS 1654
//#define LEGALCODAS 231

//#define SPLITNETSPOTS 3
//#define SPLITNETSPOTS (NGRAPHEMES + NLETTERS)
//#define NTRIGRAMS 516
#define NTRIGRAMS 578

#ifdef FRENCH
//Thhis is a hack. Coda vowels are stuck in the letter set after the letter set, and subtracted later
#define SPLITNETSPOTS ((NGRAPHEMES + NLETTERS + NGRAPHEMES + NLETTERS + (NLETTERS+NGRAPHEMES)))
#else
#define SPLITNETSPOTS ((NGRAPHEMES + NLETTERS + NGRAPHEMES + NLETTERS ))
#endif

#define SPLITMEMORY (3)
class ZorziNet : public GlobalC  
{
public:
	int NSEGMENTATIONS;
	
	void PrintWord();
	void PrintWordName(char *Name);

	char GraphemesList[600][6];
	char ChosenGrapheme[8];
	char TotalGraphemes;
	void findGraphemeLetters(int GrNumber);
	void LoadFreqsInfo();
	
		//Training function
	

	char AlignedInputs[500];
	int resplit;
	void RecreateNewWord();
	

	void TestAllFile(char *FileName);
	int difvowels;
	//wa.l.t.z.e.s.

	//////
	void SegmentWord(char *Word);
	void ParseSegments();
	

	void StickCodasBack();
	
	int CheckPhons(char *FilePhon);
	int CheckOrths(char *FileWord);

	void ResetAll();

	void CheckMidE();
	void Check3TempVowels();
	void Check4TempVowels();
	void GetRidOfDoubleVowels(); 
	void CopyInfoAcross();
	void MakeNewInput();
	void ParseCodasTesting();
	void AutoTrain();
	void DetermineNumberofSyllables();
	void CreateFreqInfo();
	void CreateDB();

	void ParseCodasTesting2();
	void ParseCodasTesting4();
	void ParseCodasTesting3();

	int ParseCodasTestingUsed;
	int GraphemeMemory[20];
	int NGraphemeMemory;
	char AttentionWindow[10];
	int ParseCodasUpTo;
	int ParseCodasLast;
	int ParseCodasFinalLetter;
	int ParseCodaOldNLets;
/////////////////////////////////////

	int TheBreak;
	int NOrthSylls;

	int NewInput[10000];
	char OrthSegWord[500];
	int ToTrain;
	int IsE, IsED,IsE2, IsED2, foundit;

	///

	int TestSegment(char *pattern, int *memory, int maxmem);

	int NSEGINPUTS;

	float GlobalFreqMod;
	char GlobalWord[50];

	float SplitNetWeights[/*(MAXPOS) * (NLETTERS + NGRAPHEMES + 5)*/4000][800];
	float SplitNetSums[800];
	float SplitNetActs[800];
	float SplitNetAnswers[800];
	float SplitNetErrors[800];
	int SplitMaxPos;

	void SplitForwardTrigrams(char *thisword);
	
	int SplitESyllablePlace;
	float SplitESyllablePlaceVal;
	char WholeWord[100];


	int Trigrams[2000];
	char TrigramsSeq[2000][4];

	void SplitNetSaveWeights();
	void SplitNetLoadWeights(char *thename);
	char SplitLetterWord[30];
	void SplitNetForward(char *theword);
	int SplitNetMax;
	float SplitNetMaxVals;
	int SplitNetCodCount;
	
	char BigString[20][10];
	char SplitTemplates[2000][5];
	int UpToSplitTemplates;


	//Segmentation Net


	void TrainCycleFromAll2(char *FileWord, char *ParsedOrth, char *FilePhon, float Freq, int NumSylls, int TheStress);
	
	void FixDeadCodas();
	void FixDeadNodes();
	void SplitNetForwardSegmentation(char *theword);

	int SplitNetMemory[3500];	
#if FRENCH 
	float Weights[(MAXPOS) * (NLETTERS + NGRAPHEMES + NLETTERS+ NGRAPHEMES + 5)][(NUMBER_OF_PHONEMES + 5) * MAXPHONE];
	float SumWeights[(MAXPOS) * (NLETTERS + NGRAPHEMES + NLETTERS+NGRAPHEMES + 5) ];
	float SumPosWeights[ MAXPOS * (NLETTERS + NGRAPHEMES + NLETTERS+NGRAPHEMES + 5) ];
#else
	float Weights[(MAXPOS) * (NLETTERS + NGRAPHEMES + 5)][(NUMBER_OF_PHONEMES + 5) * MAXPHONE];
	float SumWeights[(MAXPOS) * (NLETTERS + NGRAPHEMES + 5) ];
	float SumPosWeights[ MAXPOS * (NLETTERS + NGRAPHEMES + 5) ];
#endif
	void LoadNet(FILE *fp, int blank);
	

	float Input[MAXPOS][NLETTERS + NGRAPHEMES ];
	float Output[MAXPHONE][NUMBER_OF_PHONEMES ];

	float ActFunc(float Sum); 
	float SumAct(int Spot, int Phoneme);
	float Temperature;

	char scods[200][8];
	char dcods[200][8];


//	float Buffer[1000];

	float ErrorBuffer[MAXPHONE][NUMBER_OF_PHONEMES ];

	
	void ForwardPass();
	void SetNet(char *Word);
	void MakeOrthographicInput(char *Word);
	void ResetInputs();
	void CutRime(char *Word);
	void CutOnset(char *Word);
	void GraphemeCalc(char *Word);
	void CutVowel();
	int IsVowelGrapheme(char *graph);
	
	void TrainCycle(char *Word, char *Phon, float FreqMod, int TheSpot);
	void TrainCycleFromAll(char *FileWord, char *ParsedOrth, char *FilePhon, float Freq, int NumSylls, int TheStress);
	

	void MakePhon(char *Phon);

	int CheckForOnsetGrapheme(char *Grapheme);
	int CheckForCodaGrapheme(char *Grapheme);

	void CountPhonSylls(char *Word);
	int	GlobalNPhonSylls;
	char GlobalPhon[50];

	char FinalPhon[100];
	char GuessAgain(char *grapheme, int StartLooking);

	char OnsetGraphemes[100][10];
	char VowelGraphemes[200][10];
	char CodaGraphemes[100][10];
 
	char OrthOnset[20] ;
    char OrthRime[20];
	char PhonOnset[20];
	char PhonRime[20];
	void MakeRule(char *Word, char *Phon);
	void TrainRule();

	void CutPhonRime(char *Word);
	void CutPhonOnset(char *Word);


	char vowel[20];
	char restofword[20];
    
	int NOnsets;
	int NVowels;
	int NCodas;
	int NGraphemes;

	float Threshold;

	void loadVowels();
	void loadOnsets();
	void loadCodas();
	void loadSCodas();
	void loadDCodas();
	void MakePattern(char *Word, char *Phon, int Spot);
	void loadLegitCodas();
	void MakePhonStr(char *Pattern);
	void FindBestPlace();
	
	///
	void AddGraphemePhonemeInf();

	int GraphemesInf[10][200];
	int PhonemesInf[10][200];
	float GraphemeFreqsInf[10][200];
	int nGraphemesUpTo[10];
	char GraphemesInflets[10][200][6];
	char PhonemesCInf[10][200];



	int FindLetter(char Letter);

	int findOnsetnum(char *graph);
	int findCodanum(char *graph);
	int findVowelnum(char *graph);
	int findGrapheme(char *graph);

	int CountMultiGraphemes();
	int CheckPhonsItalian(char *Phon);

	int NMARCOLETTERS;
	int NMARCOLETTERS2;
	int MAXORTHBODY;
	int MAXORTHONSET;

	char LegitCodas[2000][10];


	char Path[100];
	char Final2[8000] ;
	int Final[8000];

	int OldLen;
	int NeedForward;

	int VowelWithCoda;
	int AttachE;
	int UseSingleContext;

	float TotalError;
	float LearningRate;


	char OrthString[100];
	void CreateOrthString();

	void SaveNet();
//	float SumAct(int Phoneme);

	
	//int NewInputs[2000][MAXPOS+1][NLETTERS + NGRAPHEMES+1];
	int NewOutputs[3500][500];

	char phontempcodas[20][8];
	int nphontempcodas;

	char tempcodas[8][12][8];
	int ntempcodas[8];
	int tempcodasnum[8][12];
	char tempvowels[8][12];
	int ntempvowels;
	int tempvowelsnum[8];
	char temponsets[8][12][8];
	int ntemponsets[8];
	int temponsetsnum[8][12];

	char tempcodas2[8][12][8];
	int ntempcodas2[8];
	int tempcodasnum2[8][12];
	char tempvowels2[8][8];
	int ntempvowels2;
	int tempvowelsnum2[8];
	char temponsets2[8][12][8];
	int ntemponsets2[8];
	int temponsetsnum2[8][12];

	
	char phononsets[8][12];
	char phonvowels[8];
	char phoncodas[8][12];


	int nphonvowels;
	int nphoncodas[8];
	int nphononsets[8];

	int phoncodsbeforespot;

	int UpToSyll;
	char TheWord[50];
	char TempPhon[20];

	char ThisWord[100];



	int InTraining;


	int TheTrainingCycle;
	int ThisStress;
	int ThisFreq;

	float StressVal1, StressVal2, StressVal3;
	char PhonString[200];
	char ThisPhon[100];
	
	int ThisNumSylls;
	int GotThrough;
	int GlobalVowels;
	int HitDeadNode;
	int rejected;


	float DeadWeightLevel;

	FILE *Phonologies;

	float exc_ZNet_phone; //excitatino

	ZorziNet();
	virtual ~ZorziNet();


};

#endif // !defined(AFX_ZORZINET_H__3475B0F0_4B65_11D6_81FA_00A0C9B00366__INCLUDED_)
