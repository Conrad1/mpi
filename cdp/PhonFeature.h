// PhonFeature.h: interface for the PhonFeature class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONFEATURE_H__D12A39E2_6674_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_PHONFEATURE_H__D12A39E2_6674_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GlobalC.h"

class PhonFeature : public GlobalC 
{
public:
	int Block[NPHONEME_FEATURES];
	int ABlock[NPHONEME_FEATURES]; 


	char TestFeature;

	void SetFeature(char Letter) ;

	void ResetFeature();
	
	PhonFeature();
	virtual ~PhonFeature();

};

#endif // !defined(AFX_PHONFEATURE_H__D12A39E2_6674_11D1_B56F_0080C8586A61__INCLUDED_)
