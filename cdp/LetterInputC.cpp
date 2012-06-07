// LetterInputC.cpp: implementation of the LetterInputC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LetterInputC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LetterInputC::LetterInputC()
{
	Letter_Letter = 0.0;
	if(MAXPOS == 9) exc_letter_word = 0.062;
	else	exc_letter_word = 0.07;

	inh_letter_word =  -.435;
	Decay = 0.0;
	Noise = 0.0;
	SpellMin = 0.43;	
	LetterOver = FALSE;
//	LetterOverPar = .21;
	LetterOverPar = .05; /*Words better*/
}

LetterInputC::~LetterInputC()
{

}

void LetterInputC::ResetLetters() 
{
	int i, j;
	
	for(i=0;i<MAXPOS;i++) {
		for(j=0;j<NLETTERS;j++) {
			Letters[i][j] = 0;
		}
	}


}

void LetterInputC::ResetLatInhibition()
{
	int i;
	
	for(i=0;i<MAXPOS;i++) {
		LatInhibition[i] = 0;
	}
}

int LetterInputC::SpellWord()
{
	int i, j, max_units[MAXPOS];
	int NullPos = 0, spot;
	float max;

	for(i=0;i<MAXPOS;i++) {
		// Get Maximum 		
		max = -1;
		for(j=0;j<NLETTERS;j++) {
			if(Letters[i][j] > max) {
				max = Letters[i][j];
				spot = j;
			}
		}
		if(max < SpellMin) {
			WordSoFar[i] = 0;
			return FALSE;
		} else if (Alphabet[spot] == WORDPAD) {
			WordSoFar[i] = 0;
			break;
		} else {
			WordSoFar[i] = Alphabet[spot];
		}
	}
	WordSoFar[i] = 0;
	
	return TRUE;
}

