// LetterInputC.h: interface for the LetterInputC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LETTERINPUTC_H__BE7F3185_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_LETTERINPUTC_H__BE7F3185_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GlobalC.h"

class LetterInputC : public GlobalC 
{
public:

	// Holds the activations of the letters for each position

	int LetterOver;
	float LetterOverPar;

	float Letters[MAXPOS][NLETTERS];	//The block of letters
	float NetInput[MAXPOS][NLETTERS];	

	float LatInhibition[MAXPOS];

	float Letter_Letter;
	float exc_letter_word;
	float inh_letter_word;
	float Decay;
	float Noise;

	float SpellMin;

	void ResetLetters();
	void ResetLatInhibition();
	int SpellWord();
	char WordSoFar[MAXPOS + 1];

	LetterInputC();
	virtual ~LetterInputC();

};



#endif // !defined(AFX_LETTERINPUTC_H__BE7F3185_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
