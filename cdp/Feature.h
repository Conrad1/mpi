// Feature.h: interface for the Feature class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEATURE_H__BE7F3183_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_FEATURE_H__BE7F3183_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GlobalC.h"


class Feature : public GlobalC
{
public:
	//int Letter_Features[NLETTERS][NUMBER_OF_FEATURES];
	
	// This logic is taken from the original. It's probably worth updating it
	// so it just stores a count of the number of letters activated.
	int  Block[NUMBER_OF_FEATURES];
	int  ABlock[NUMBER_OF_FEATURES]; 

	char TestFeature;

	void SetFeature(char Letter);
	void ResetFeature(); 
	void SwitchIt(int theFeature); 
	void SwitchOnIt(int theFeature); 
	void SwitchOverBoth(int theFeature);
	
	void MessIt(float prob); 
	void SwitchOn(int theFeature);
	void SwitchOff(int theFeature);

		Feature();
	virtual ~Feature();

};



#endif // !defined(AFX_FEATURE_H__BE7F3183_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
