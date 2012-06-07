// WordDataC.h: interface for the WordDataC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORDDATAC_H__BE7F318E_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_WORDDATAC_H__BE7F318E_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GlobalC.h"
//#include <stdio.h>
#include <math.h>

class WordDataC : public GlobalC 
{
public:
	//Fix this later

	// Note - Just runs over Frequency at the moment
	// Need to work out Spelling Sound Consistency Freq's

	float highestorth ;
	float highestphon ;
	
	char WordFileName[40];

	float OldFreq[NUMBER_OF_WORDS];

	int Word_Length[NUMBER_OF_WORDS];
	float Freq[NUMBER_OF_WORDS];				
	float PhFreq[NUMBER_OF_PRONS];
	char Phon[NUMBER_OF_PRONS][MAXPHONE + 2];
	char Word[NUMBER_OF_WORDS][MAXPOS + 1];
	char PhonSeg[NUMBER_OF_PRONS][MAXPHONE + 1];


	int Word_to_Phon[NUMBER_OF_WORDS][6];
	int NWord_to_Phon[NUMBER_OF_WORDS];
	int Phon_to_Word[NUMBER_OF_PRONS][6];
	int NPhon_to_Word[NUMBER_OF_PRONS];
	

	char OnsetParts[NUMBER_OF_WORDS][9][7][5];
	char OnsetPartsPhon[NUMBER_OF_WORDS][9][5];
	int  OnsetNumber[NUMBER_OF_WORDS][9];

	char CodaParts[NUMBER_OF_WORDS][9][7][5];
	char CodaPartsPhon[NUMBER_OF_WORDS][9][5];
	int  CodaNumber[NUMBER_OF_WORDS][9];
	int  HasCoda[NUMBER_OF_WORDS];
	
	char TheVowel[NUMBER_OF_WORDS][3][9];
	char TheVowelLetters[NUMBER_OF_WORDS][3][9][6];
	
	char TheCoda[NUMBER_OF_WORDS][3][9];
	char TheCodaLetters[NUMBER_OF_WORDS][9][6];

	char TheOnset[NUMBER_OF_WORDS][3][9];

	char AntiBody[NUMBER_OF_WORDS][8];
	int AntiBodyN[NUMBER_OF_WORDS];
	int StressPos[NUMBER_OF_WORDS];


	float fgain_tmp;

	int Word_Count;
	int Pron_Count;

	int count; 

	int LoadData();
	int LoadOldData(); //Load Incorrect Freq's (eg. drcl)
	int WordPos(char *TheWord); 
	int PhonPos(char *TheWord); 

	WordDataC();
	virtual ~WordDataC();

/*	float TempFreq[32000];
	float TempSFreq[32000];
	char TempPron[32000][MAXPOS + 1];
	char TempSegPron[32000][MAXPHONE + 3];
	int Taken[32000];
	int WTaken[32000];
	char TempWord[32000][MAXPHONE + 2];
	int NumberSylls[32000];*/

	float TempFreq[NUMBER_OF_WORDS];
	float TempSFreq[NUMBER_OF_WORDS];
	char TempPron[NUMBER_OF_WORDS][MAXPOS + 1];
	char TempSegPron[NUMBER_OF_WORDS][MAXPHONE + 3];
	int Taken[NUMBER_OF_WORDS];
	int WTaken[NUMBER_OF_WORDS];
	char TempWord[NUMBER_OF_WORDS][MAXPHONE + 2];
	int NumberSylls[NUMBER_OF_WORDS];


	int AllCount;

};


#endif // !defined(AFX_WORDDATAC_H__BE7F318E_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
