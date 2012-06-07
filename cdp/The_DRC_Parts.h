// The_DRC_Parts.h: interface for the The_DRC_Parts class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THE_DRC_PARTS_H__045A4602_414C_11D1_B56F_0080C8586A61__INCLUDED_)
#define AFX_THE_DRC_PARTS_H__045A4602_414C_11D1_B56F_0080C8586A61__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "FeatureSetC.h"
#include "LetterInputC.h"
#include "OrthInputLexC.h"
#include "PhonLexC.h"
#include "PhonemeOutBufC.h"
#include "GlobalC.h"
#include "RuleList.h"
#include "GPC_RouteC.h"
#include "PGC_Route.h"
#include "WordDBFuncs.h"
#include "PhonemeFeatures.h"
#include "Semantic.h"
#include "SemanticFeatures.h"
#include "ZorziNet.h"
#include "GraphemeParsing.h"

class The_DRC_Parts  
{
public:
	The_DRC_Parts();
	virtual ~The_DRC_Parts();

	FeatureSetC		Features;
	LetterInputC	LetterSpots;
	OrthInputLexC	OrthLex;
	PhonLexC		PhonLex;
	PhonemeOutBufC	PhonBuf;
	GlobalC			GlobalStuff;
	WordDBFuncs		TheWords; 
	GPC_RouteC		GPCRoute;
	PGC_Route		PGCRoute;
	PhonemeFeatures PhonFeatures;
	Semantic		SemSys;
	SemanticFeatures SemFeatures;
	GraphemeParsing		ZNet;
};


#endif // !defined(AFX_THE_DRC_PARTS_H__045A4602_414C_11D1_B56F_0080C8586A61__INCLUDED_)
