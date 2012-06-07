// PhonemeFeatures.h: interface for the PhonemeFeatures class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONEMEFEATURES_H__D12A39E1_6674_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_PHONEMEFEATURES_H__D12A39E1_6674_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GlobalC.h"
#include "PhonFeature.h"

class PhonemeFeatures : public GlobalC 
{
public:

	PhonFeature PhonFeatureSet[MAXPOS];
	int TakeAwayPhonetic;
	float inh_pfeature_phoneme;
	float exc_pfeature_phoneme;
	float Noise;

	void resetfeatures();
	void ClampPhon(char *Word);

	PhonemeFeatures();
	virtual ~PhonemeFeatures();

};

#endif // !defined(AFX_PHONEMEFEATURES_H__D12A39E1_6674_11D1_B56F_0080C8586A61__INCLUDED_)
