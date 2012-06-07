// SemanticFeatures.h: interface for the SemanticFeatures class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMANTICFEATURES_H__EA068371_6D5F_11D2_80E3_00A0C9B00366__INCLUDED_)
#define AFX_SEMANTICFEATURES_H__EA068371_6D5F_11D2_80E3_00A0C9B00366__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class SemanticFeatures  
{
public:

	int NumFeatures;
//	float FeatureActivations[50];

	int SemFeat_to_Sem[50][10];
	float SemFeat_to_SemWeight[50][10];
	float FeatureAct[50];
	int NSemFeat_to_SemWeight[50];
	float exc_semfeat_sem;
	float exc_out_sem;
	float NetInput[50];

	void ResetFeatures();
	SemanticFeatures();
	virtual ~SemanticFeatures();

};

#endif // !defined(AFX_SEMANTICFEATURES_H__EA068371_6D5F_11D2_80E3_00A0C9B00366__INCLUDED_)
