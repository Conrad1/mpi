// FeatureSetC.h: interface for the FeatureSetC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEATURESETC_H__BE7F3187_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_FEATURESETC_H__BE7F3187_3B34_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Feature.h"
#include "GlobalC.h"

class FeatureSetC : GlobalC
{
public:
	Feature FeatureSet[MAXPOS];

	float inh_feature_letter;
	float exc_feature_letter;
	int Noise;
	float FeatProbErr;
	float FeatActLevel;

	void resetfeatures();
	void ClampWord(char *Word);
	void ClampWords(char *Word, char *Word2, float amount1, float amount2);
	void SwitchFeature(int Which, int Where);
	void SwitchOnFeature(int Which, int Where);
	void SwitchBothFeature(int Which, int Where);
	//char Test2[10];
	void MessFeatures(float prob, int UpTo);

	FeatureSetC();
	virtual ~FeatureSetC();

};


#endif // !defined(AFX_FEATURESETC_H__BE7F3187_3B34_11D1_B56F_0080C8586A61__INCLUDED_)
