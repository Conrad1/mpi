#pragma once

#include "ZorziNet.h"

class GraphemeParsing: public ZorziNet
{
public:
	void FixEarlyExceptions();
	void FixEarlyExceptionsItalian();
	void FixEarlyExceptionsGerman();
	void FixEarlyExceptionsFrench();

	void CreateOrths();
	void AlignInputs();
	void AlignInputsItalian();
	void AlignInputsGerman();
	void AlignInputsFrench();
	void TrainSegNetwork(int cycles);
	void TestAllSegNetwork();

	void FixExceptions();
	void FixExceptionsFrench();
	void FixExceptionsItalian();

	void TrainSegmentations();
	void SaveSegmentations();
	void LoadSegmentations(FILE *fp, int blank);
	void CreateGraphemePattern(float freqmod);

	void AddGraphemeDB();
	void PrintGraphemeDB();

	int OnsetCounts[1000];
	int VowelCounts[1000];
	int CodaCounts[1000];

	GraphemeParsing(void);
	~GraphemeParsing(void);
};

