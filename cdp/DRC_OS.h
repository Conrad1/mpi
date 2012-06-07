// DRC_OS.h: interface for the DRC_OS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRC_OS_H__045A4601_414C_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_DRC_OS_H__045A4601_414C_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000

#pragma once

#endif // _MSC_VER >= 1000



#include "The_DRC_Parts.h"

//Adds files handling graphs and so on


class DRC_OS : public The_DRC_Parts 
{
public:

	//The_DRC_Parts TheParts;

	int SaveGraphs;
	int SpecSave, SpecOrthSave;

	int Num_Word, Num_Phon;

	int DisLetters, DisOrthLex, DisPhonLex, DisPhonemes, DisGPC, DisPGC, DisTotalSum; 
	int DisableClamping;

	void ClearAll();
	void ClearAllNoRedraw();
	//This is a bit of a hack - contains the base name for saving
	char BaseSave[50];

	int InterestSaveON;   //Using interesting stuff save ?
	int QuickSaveOn ;

	int OnlyLexicons;

#if ISMFC
	CString InterestSave; //Filename for interesting stuff save
	CString QuickSaveName;
#else
	char InterestSave[200]; //Filename for interesting stuff save
	char QuickSaveName[200];
#endif


	void InitSav(); 
	void QuickSave(char *Word, char *Word2, int Named, int Cycles);
	int **SavingLetters;
	int *SavingOrth;
	int *SavingPhon;
	int **SavingPhonemes;
	int **SavingGPC;
	int **SavingPGC;
	
	//Graph stuff
#if ISMFC
	ErrWinF *LetterGraphs[MAXPOS];
	ErrWinF *OrthLexGraph;
	ErrWinF *PhonLexGraph;
	ErrWinF *PhonemeGraphs[MAXPOS];
	ErrWinF *GPCGraphs[MAXPOS];
	ErrWinF *PGCGraphs[MAXPOS];
	ErrWinF *Total_Sum_Graph;
	ErrWinF *Total_Sum_Graph2;
#endif

	float MinShowLetterGraphs[MAXPOS];
	float MinShowOrthLexGraph;
	float MinShowPhonLexGraph;
	float MinShowPhonemeGraphs[MAXPOS];
	float MinShowGPCGraphs[MAXPOS];
	float MinShowPGCGraphs[MAXPOS];
	float MinShowTotal_Sum_Graph;
	float MinShowTotal_Sum_Graph2;

	

	//Converts category X into Category Y on the Graphs
	int **GraphLetterMap;
	int *GraphOrthMap;
	int *GraphPhonMap;
	int **GraphPhonemeMap;
	int **GraphGPCMap;
	int **GraphPGCMap;
	//Up to category N  - start at 1

	int NGraphLetterMap[50];  
	int NGraphOrthMap;
	int NGraphPhonMap;
	int NGraphPhonemeMap[50];
	int NGraphGPCMap[50];
	int NGraphPGCMap[50];	

	void WordInf(int Cycle, char *ActualWord);
	void SemInf(int i, int Cycle);
	void SemFeatInf(int i, int Cycle);
	void LetterInf(int i, int j, int Cycle);	//These functions save info & update the graphs.
	void OrthLexInf(int i, int Cycle);
	void PhonLexInf(int i, int Cycle);
	void PhonemeInf(int i, int j, int Cycle);
	void GPCInf(int Cycle);
	void PGCInf(int Cycle);
	void OrthLexSumInf(int i, int Cycle);
	void OrthLexSpecSave(int SpecSaveNumber, int Cycle);
	int  Initialize_DRCOS(int NumW, int NumP);
	void PhonLexSumInf(int i, int Cycle);
	void PhonLexSpecSave(int SpecSaveNumber, int Cycle);
	void TotOrthSpecSave(int Cycle);
	void TotPhonSpecSave(int Cycle);

	void PhonemeSumInf(int Cycle, float amount);
	void LoadParametersName(char *FileName);
	int NoSpaces(char* Line);

	int IsPrime;
	int SaveType;

	//QuickSaveNames
	char LettersI[100];
	char OrthI[100];
	char PhonI[100];
	char PhonemeI[100];
	char TotalOrthI[100];
	char TotalPhonI[100];
	char StressI[100];
	char TotalPhonemeI[100];


	//Info saver

	int WordRes[10000];
	int StressRes[10000];
	char PhonRes[10000][20];
	char OrthRes[10000][20];

	void PrintInfo(char *FileName, int upto);

	
	int ClampingCycles;
	int CyclesBeforeNaming;
	int DerivCycles;


	DRC_OS();
	virtual ~DRC_OS();
private:
	int sum_init;   //Orthography
	int sum_init2; //Phonology

};

#endif // !defined(AFX_DRC_OS_H__045A4601_414C_11D1_B56F_0080C8586A61__INCLUDED_)
