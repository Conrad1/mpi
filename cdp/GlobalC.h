// GlobalC.h: interface for the GlobalC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALC_H__BE7F3182_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_GLOBALC_H__BE7F3182_3B34_11D1_B56F_0080C8586A61__INCLUDED_


#define ISMFC 0
#define TRUE 1
#define FALSE 0

#if ISMFC
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#endif


#define ENGLISH 1	
#define GERMAN 0
#define FRENCH 0
#define ITALIAN 0


#define ISMARCO 1
#define USEDERIV 1

#define USESPACE 0

#define Morphotacticalize (0)


#define ISRED 1
#define ISGREEN 2
#define ISBLUE 3
#define ISBREAD 4
#define ISCOOK 5
#define ISTOAST 6
#define ISWHITE 7
#define ISBLACK 8

//#define MAXSYLL 4

// Note that these two just need to be bigger than the database
// THere in here as using sttatic arrays is faster than malloced mem
// 
#define NUMBER_OF_WORDS 66000
#define NUMBER_OF_PRONS	66000

//#define MAXPOS (16)
//#define MAXPHONE (16)

#if ENGLISH
	#define MAXONSETS (3)
	#define MAXCODAS (4)
	#define MAXSYLLABLES (2) /* USE 2 for RUNNING */
	#define MAXSPOTSPERSYLL (MAXONSETS+MAXCODAS+1)
	#define MAXPOS (MAXSPOTSPERSYLL * MAXSYLLABLES)
	#define MAXPHONE (MAXSPOTSPERSYLL * MAXSYLLABLES)
	#define NSTRESS (2)
#elif ITALIAN
	#define MAXONSETS (3)
	#define MAXCODAS (1)
	#define MAXSYLLABLES (4) /*ONLY EVER TRAINED ON 3 SYLLABLE WORDS IF THIS IS FIXED CHANGE PARSE_CODAS_TESTING2 IN ZORZINET!!!*/
	#define MAXSPOTSPERSYLL (MAXONSETS+MAXCODAS+1)
	#define MAXPOS (MAXSPOTSPERSYLL * MAXSYLLABLES)
	#define MAXPHONE (MAXSPOTSPERSYLL * MAXSYLLABLES)
	#define NSTRESS (3)
#elif GERMAN
	#define MAXONSETS (3)
	#define MAXCODAS (4)
	#define MAXSYLLABLES (2)  /* USE 2 for RUNNING and 3 for GRAPHEME GENERATION */
	#define MAXSPOTSPERSYLL (MAXONSETS+MAXCODAS+1)
	#define MAXPOS (MAXSPOTSPERSYLL * MAXSYLLABLES)
	#define MAXPHONE (MAXSPOTSPERSYLL * MAXSYLLABLES)
	#define NSTRESS (2)
#elif FRENCH
	#define MAXONSETS (4)
	#define MAXCODAS (4)
	#define MAXSYLLABLES (3)
	#define MAXSPOTSPERSYLL (MAXONSETS+MAXCODAS+1)
	#define MAXPOS (MAXSPOTSPERSYLL * MAXSYLLABLES)
	#define MAXPHONE (MAXSPOTSPERSYLL * MAXSYLLABLES)
	#define NSTRESS (2)
#endif

#define WORDPAD '+'
#define NPHONEME_FEATURES 14
#define MARKER 999
#define MAX_ORTH_FREQ 1093547.0
#define MAX_PHON_FREQ 1093547.0


#if ENGLISH
	#define NLETTERS 27
	#define NUMBER_OF_FEATURES 14
	#define NUMBER_OF_PHONEMES 48 
#elif GERMAN
	#define NLETTERS 31
	#define NUMBER_OF_FEATURES 15
	#define NUMBER_OF_PHONEMES 47
#elif FRENCH
	#define NLETTERS 37
	#define NUMBER_OF_FEATURES 14
	#define NUMBER_OF_PHONEMES 39
#elif ITALIAN
	#define NLETTERS 32
	#define NUMBER_OF_FEATURES 15
	#define NUMBER_OF_PHONEMES 35
#endif

//#define NLETTERS 30
//#define NUMBER_OF_FEATURES 15
//#define NUMBER_OF_PHONEMES 43



class GlobalC  
{
public:
	int PhonemeBlock[NUMBER_OF_PHONEMES][NPHONEME_FEATURES];
	int LetterBlock[NLETTERS][NUMBER_OF_FEATURES]; // the block of features// Should also be moved to feature.cpp
	char Alphabet[NLETTERS+2];					   // The alphabet - Should be moved duplicated
	
	char PGCRuleName[40];
	char RuleFileName[40];
	char DataFileName[40];
	char VOWELS[50];
	char CONSONANTS[60];
	char VowelPhonemes[60];

	float word_log_divisor;
	float phon_log_divisor;

	char Phonemes[NUMBER_OF_PHONEMES + 2];
	int PhonemeVowelIndex(char Letter);

	int AlphabetIndex(char Letter);					// Move too. Returns the position of a the character
													// in the alphabet
	int CheckWord(char *Word);						// Move too. Checks to see if a word is ok
		
	float act_rate;									// Overall activation rate
	float bias_multiplier;							// Modifies the frequency in old DRC's
													// Currently chopped out = now uses fgain_tmp
	char Language_Hack(char Letter); //Used since the original DRC
									  // uses capitals, but the German one doesnt.
	char Language_Data_Hack(char Letter);  
	int ConsonantNum(char Letter);
	float Box_Muller_Polar(float noise);
	float VectLength(float *Vect, int length);
	int IsVowel(char Letter);
	int IsVowelPhoneme(char test);
	int PhonemeIndex(char Letter);

	int NoSpaces(char* Line);


	
	GlobalC();
	virtual ~GlobalC();

};


#endif // !defined(AFX_GLOBALC_H__BE7F3182_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
