// Rule.h: interface for the Rule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RULE_H__BE7F318C_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_RULE_H__BE7F318C_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class Rule  
{
public:


	char Usage;			// beggining, middle, end or all
	char Type[7];		// Type of rule, ie multi...
	char Letters[100];	// The letters in the rule
	int LetterType;		
	char RuleDesc[100];  //The actual rule, as read in

	int  NLetters;	//Nummber of letters
	char Pron[4];	//The rules pronounciation
	int NPron;		//Number of Phonemes
	int Protected; //Is it a protected rule ?

	Rule();
	virtual ~Rule();

};


#endif // !defined(AFX_RULE_H__BE7F318C_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
