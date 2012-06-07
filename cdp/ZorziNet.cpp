// ZorziNet.cpp: implementation of the ZorziNet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h" 
#include "ZorziNet.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../common.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ZorziNet::ZorziNet()
{

	FILE *fp;
	int i,j;

	for(i=0;i<10;i++) nGraphemesUpTo[i] = 0;
	 DeadWeightLevel = 15.0;
	////fp = fopen("Crap.txt","a");
	Temperature = 3.0;
	strcpy(Path,"./cdp/");
	//strcpy(Path,"./");

	NOnsets = NONSETS;
    NVowels = NVOWELS;
    NCodas = NCODAS;
	NGraphemes = NGRAPHEMES;

	NMARCOLETTERS = NLETTERS + NOnsets + NVowels + NCodas ;
	NMARCOLETTERS2 = NMARCOLETTERS * 2;
	MAXORTHBODY = 5;
	MAXORTHONSET = 3;

	NSEGMENTATIONS = 0;

	Threshold = 0.05;

	OldLen = -1;
	
	loadOnsets();
	loadVowels();
	loadCodas();
	AttachE = 1;
	UseSingleContext = 0;
	InTraining = 0;

	LearningRate = 0.05;
	ntempvowels = 0;

	UpToSplitTemplates = 0;

	//SplitESyllablePlaceVal = 0.52;
	SplitESyllablePlaceVal = 0.50;

//	LearningRate = 0.0;
	exc_ZNet_phone = .072;

//	fprintf(fp,"here \n");
//	fclose(fp);
	//loadLegitCodas();


	if(FRENCH==1) {
		NMARCOLETTERS = (NLETTERS + NGRAPHEMES) ;
		NMARCOLETTERS2 = NMARCOLETTERS * 3;
	}

	for(i=0;i<2000;i++) SplitNetMemory[i] = 0;

	for(i=0;i<NOnsets;i++) strcpy(GraphemesList[i],OnsetGraphemes[i]);
	for(j=0;j<NVowels;j++) {
		strcpy(GraphemesList[i],VowelGraphemes[j]);
		i++;
	}
	for(j=0;j<NCodas;j++) {
		strcpy(GraphemesList[i],CodaGraphemes[j]);
		i++;
	}
	for(j=0;j<strlen(Alphabet);j++) {
		if(ENGLISH ==1) GraphemesList[i][0] = tolower(Alphabet[j]);
		else GraphemesList[i][0] = Alphabet[j];
		GraphemesList[i][1] = 0;
		i++;
	}

	/*if(FRENCH==1) { //add extra duplicate//just letters
		for(j=0;j<strlen(Alphabet);j++) {
			GraphemesList[i][0] = Alphabet[j];
			GraphemesList[i][1] = 0;
			i++;
		}
	}*/

	//duplicate 
	for(j=0;j<NOnsets;j++) {
		strcpy(GraphemesList[i],OnsetGraphemes[j]);
		i++;
	}
	for(j=0;j<NVowels;j++) {
		strcpy(GraphemesList[i],VowelGraphemes[j]);
		i++;
	}
	for(j=0;j<NCodas;j++) {
		strcpy(GraphemesList[i],CodaGraphemes[j]);
		i++;
	}
	for(j=0;j<strlen(Alphabet);j++) {

		if(ENGLISH ==1) GraphemesList[i][0] = tolower(Alphabet[j]);
		else  GraphemesList[i][0] = Alphabet[j];
		GraphemesList[i][1] = 0;
		i++;
	}
	if(FRENCH==1) {
		for(j=0;j<NOnsets;j++) {
			strcpy(GraphemesList[i],OnsetGraphemes[j]);
			i++;
		}
		for(j=0;j<NVowels;j++) {
			strcpy(GraphemesList[i],VowelGraphemes[j]);
			i++;
		}
		for(j=0;j<NCodas;j++) {
			strcpy(GraphemesList[i],CodaGraphemes[j]);
			i++;
		}
		for(j=0;j<strlen(Alphabet);j++) {
			GraphemesList[i][0] = Alphabet[j];
			GraphemesList[i][1] = 0;
			i++;
		}
	}



	NSEGINPUTS = /*522*/ ((NMARCOLETTERS2 * SPLITMEMORY )) + (NLETTERS * ATTENTIONWINDOWSIZE);
	fp = fopen("graphslist.txt","w");
	for(j=0;j<i;j++) {
		fprintf(fp,"%d %s \n",j, GraphemesList[j]);
	}
	fprintf(fp,"%d %d %d \n",NOnsets,NVowels,NCodas);
	fclose(fp);

}

ZorziNet::~ZorziNet()
{

}

void ZorziNet::findGraphemeLetters(int GrNumber)
{
	strcpy(ChosenGrapheme, GraphemesList[GrNumber]);
}	


void ZorziNet::loadLegitCodas() {
    FILE *in;
    int i;
	char AllPath[200];

	strcpy(AllPath,"./");



//	printf("hre \n");
//	strcat(AllPath,"Codaslist.txt");
	strcat(AllPath,"EnglishCods2.txt");
//	strcat(AllPath,"AllBods2.txt");
//	strcat(AllPath,"CutDownCods231.txt");	

	if( (in = fopen(AllPath,"r")) != 0) {
		for(i=0;i<LEGALCODAS;i++) {
			fscanf(in,"%s",LegitCodas[i]);
		}
		fclose(in);
	} else {
		exit(1);
	}
 }

void ZorziNet:: loadOnsets() {
    FILE *in, *fp2;
    int i;
	char AllPath[200];

	strcpy(AllPath,Path);
	if(ENGLISH == 1) {
		strcat(AllPath,"./EnglishRes/EnglishOnsets.txt");
	} else if (GERMAN == 1) {
		strcat(AllPath,"./GermanRes/GermanOnsets.txt");
	} else if (FRENCH == 1) {
		strcat(AllPath,"./FrenchRes/FrenchOnsets.txt");
	} else {
		strcat(AllPath,"./ItalianRes/ItalianOnsets.txt");
	}

	if( (in = fopen(AllPath,"r")) != 0) {
		for(i=0;i<NOnsets;i++) {
			fscanf(in,"%s",OnsetGraphemes[i]);
		}
		fclose(in);
	} else {
		fp2 = fopen("Failedload.txt","w");
		fprintf(fp2,"failed on \n");
		fclose(fp2);
		exit(1);
	}
 }

void ZorziNet::loadVowels() {
    FILE *in, *out, *fp2;
    int i;
	char AllPath[200];

	strcpy(AllPath,Path);

	if(ENGLISH == 1) {
		strcat(AllPath,"./EnglishRes/EnglishVowels.txt");
	} else if (GERMAN == 1) {
		strcat(AllPath,"./GermanRes/GermanVowels.txt");
	} else if (FRENCH == 1) {
		strcat(AllPath,"./FrenchRes/FrenchVowels.txt");
	} else {
		strcat(AllPath,"./ItalianRes/ItalianVowels.txt");
	}
	if( (in = fopen(AllPath,"r")) != 0) {
		for(i=0;i<NVowels;i++) {
			fscanf(in,"%s",VowelGraphemes[i]);
		}
		fclose(in);
	} else {
		fp2 = fopen("Failedload.txt","w");
		fprintf(fp2,"failed vowels \n");
		fclose(fp2);
		exit(1);
	}
 }

int ZorziNet::IsVowelGrapheme(char *graph)
{
	int i;
	for(i=0;i<NVowels;i++) {
		if( (strcmp(graph,VowelGraphemes[i])) == 0) {
			return 1;
		}
	}
	return -1;
}

int ZorziNet::findGrapheme(char *graph)
{
	int i,j;
	FILE *fp;

/*	fp = fopen("graphslist2.txt","w");
	for(j=0;j<NGRAPHEMES+NLETTERS+NLETTERS;j++) {
		fprintf(fp,"[%d %s] \n",j, GraphemesList[j]);
		if(j<NOnsets) {
			fprintf(fp,"%s \n",OnsetGraphemes[j]);
		}
	}
	fclose(fp);
	fp=fopen("test23345.txt","a");
	fprintf(fp,"%d %d %d %s \n",NOnsets, NVowels,NCodas,graph);
	fclose(fp);*/

	for(i=0;i<NOnsets;i++) {
		if( (strcmp(graph, OnsetGraphemes[i])) == 0) {
			return i;
		}
	}
	for(i=0;i<NVowels;i++) {
		if( (strcmp(graph, VowelGraphemes[i])) == 0) {
			return (i + NOnsets);
		}
	}
	for(i=0;i<NCodas;i++) {
		if( (strcmp(graph, CodaGraphemes[i])) == 0) {
			return (i + NOnsets + NVowels);
		}
	}
	return -1;

}


int ZorziNet::findOnsetnum(char *graph)
{
	int i;
	FILE *fp1;

	for(i=0;i<NOnsets;i++) {
		if( (strcmp(graph, OnsetGraphemes[i])) == 0) {
			return i;
		}
	}
	return -1;
}


int ZorziNet::findCodanum(char *graph)
{
	int i;
	FILE *fp;

	for(i=0;i<NCodas;i++) {
		//fp = fopen("testst.txt","a");
		//fprintf(fp,"%d %s %s \n",i,graph,CodaGraphemes[i]);
		//fclose(fp);
		if( (strcmp(graph, CodaGraphemes[i])) == 0) {
			return i;
		}
	}
	/*fp = fopen("testst222.txt","a");
	fprintf(fp,"%d %s  \n",i,graph);
	fclose(fp);*/

	return -1;
}

int ZorziNet::findVowelnum(char *graph)
{
	int i;
	for(i=0;i<NVowels;i++) {
		if( (strcmp(graph, VowelGraphemes[i])) == 0) {
			return i;
		}
	}
	return -1;
}



void ZorziNet::loadCodas() {
    FILE *in, *fp2;
    int i;
	char AllPath[200];

	strcpy(AllPath,Path);
	if(ENGLISH == 1) {
		strcat(AllPath,"./EnglishRes/EnglishCodas.txt");
	} else if (GERMAN == 1) {
		strcat(AllPath,"./GermanRes/GermanCodas.txt");
	} else if (FRENCH == 1) {
		strcat(AllPath,"./FrenchRes/FrenchCodas.txt");
	} else {
		strcat(AllPath,"./ItalianRes/ItalianCodas.txt");
	}
	if( (in = fopen(AllPath,"r")) != 0) {
		for(i=0;i<NCodas;i++) {
			fscanf(in,"%s",CodaGraphemes[i]);
		}
		fclose(in);
	} else {
		fp2 = fopen("Failedload.txt","w");
		fprintf(fp2,"failed cod \n");
		fclose(fp2);
		exit(1);
	}
 }



void ZorziNet::loadSCodas() {
    FILE *in;
    int i;
	char AllPath[200];

	strcpy(AllPath,Path);
	if(ENGLISH == 1) {
		strcat(AllPath,"EngSCods.txt");
	} else if (GERMAN == 1) {
		strcat(AllPath,"GermanCodas.txt");
	} else {
		strcat(AllPath,"FrenchCodas.txt");
	}
	if( (in = fopen(AllPath,"r")) != 0) {
		for(i=0;i<SCODS;i++) {
			fscanf(in,"%s",scods[i]);
		}
		fclose(in);
	} else {
		exit(1);
	}
 }


void ZorziNet::loadDCodas() {
    FILE *in;
    int i;
	char AllPath[200];

	strcpy(AllPath,Path);
	if(ENGLISH == 1) {
		strcat(AllPath,"EngDCods.txt");
	} else if (GERMAN == 1) {
		strcat(AllPath,"GermanCodas.txt");
	} else {
		strcat(AllPath,"FrenchCodas.txt");
	}
	if( (in = fopen(AllPath,"r")) != 0) {
		for(i=0;i<DCODS;i++) {
			fscanf(in,"%s",dcods[i]);
		}
		fclose(in);
	} else {
		exit(1);
	}
 }


//	for(i=0;i<(NMARCOLETTERS) * MAXPOS;i++) {
//		for(j=0;j<(NUMBER_OF_PHONEMES) * MAXPHONE ;j++) {
//			fprintf(fp,"%f ",Weights[i][j]);
//		}
//	}
//   fclose(fp);

void ZorziNet::LoadNet(FILE *fp, int blank)
{
	int i,j, count = 0;

//	float Weights[MAXPOS * (NLETTERS - 1)][(NPHONEMES - 1) * MAXPHONE];
	FILE *fp2;
	//fp2 = fopen("newnet.txt","a");

	if(blank == 0) {	
		for(i=0;i<(NMARCOLETTERS) * MAXPOS;i++) {
			if((i % NMARCOLETTERS) == 0) count++;
			SumWeights[i] = 0;
			for(j=0;j<(NUMBER_OF_PHONEMES) * MAXPHONE ;j++) {
				fscanf(fp,"%f",&Weights[i][j]);
			//	fprintf(fp2,"%d %d %s %c %f\n",i, j, GraphemesList[i], Phonemes[j%NUMBER_OF_PHONEMES],Weights[i][j]);
				SumWeights[i] += fabs(Weights[i][j]);
				if(Weights[i][j]>0) SumPosWeights[i] += Weights[i][j];
			}
		//	fp2 = fopen("sumsww.txt","a");
		//	fprintf(fp2,"%d %d %d %s %f \n",count,i, i%NMARCOLETTERS,GraphemesList[i%NMARCOLETTERS],SumWeights[i]);
		//	fclose(fp2);
		}
	} else {

		for(i=0;i<(NMARCOLETTERS) * MAXPOS;i++) {
			SumWeights[i] = 0;
			for(j=0;j<(NUMBER_OF_PHONEMES) * MAXPHONE ;j++) {
				Weights[i][j] = 0.0;
			}
		}
	}

//
//	}

	/*fp2 = fopen("testload.txt","w");
	for(i=0;i<(NMARCOLETTERS) * MAXPOS;i++) {
		for(j=0;j<(NUMBER_OF_PHONEMES) * MAXPHONE;j++) {
			fprintf(fp2,"%f ",Weights[i][j]);
		}
	}
	fclose(fp2);*/
}

void ZorziNet::SetNet(char *Word)
{
		FILE *fp2;

		//MakeOrthographicInput(Word);
	ParseCodasTesting2();
		//ParseCodasTesting4();
}


float ZorziNet::ActFunc(float  Sum)
{	
	return(1.0/((1.0+ (exp( (-1.0 * (Sum - 1.0))  * Temperature))))); 	
}

float ZorziNet::SumAct(int Spot, int Phoneme)
{
	int i,j;
	FILE *fp1;
	

	float sum = 0;
  	for(i=0;i<MAXPOS;i++) {
		for(j=0;j<(NMARCOLETTERS);j++) {
			if(Input[i][j] > .01) { //Speed
				sum += Weights[(i * (NMARCOLETTERS)) + j][ (Spot * (NUMBER_OF_PHONEMES )) +  Phoneme] * Input[i][j];
			/*	fp1 = fopen("teststst.txt","a");
				fprintf(fp1,"%d %d %d %d %f \n",i,j,Spot,Phoneme,sum);
				fclose(fp1)*/;
			}
		}
	}


//0-2,3,4-7,8-10,11,12-15.
 /*	for(i=0;i<MAXPOS;i++) {
		for(j=0;j<(NMARCOLETTERS);j++) {
			if(Input[i][j] > .01) { //Speed
				if( (Spot == 15 && Phoneme > NUMBER_OF_PHONEMES - 3) ) {
					sum += Weights[(i * (NMARCOLETTERS)) + j][ (Spot * (NUMBER_OF_PHONEMES )) +  Phoneme] * Input[i][j];			
				} else if(Spot < 3) { //Onsets//Only vowels
					if(i < 4) {
						sum += Weights[(i * (NMARCOLETTERS)) + j][ (Spot * (NUMBER_OF_PHONEMES )) +  Phoneme] * Input[i][j];
					} 
				} else if (Spot == 3) { //Vowel//All SYllable plus other vowel
					if(i < 8 || i == 11) {
						sum += Weights[(i * (NMARCOLETTERS)) + j][ (Spot * (NUMBER_OF_PHONEMES )) +  Phoneme] * Input[i][j];
					} 
				} else if (Spot > 3 && Spot < 8) { //Codas can go into onset 1.extrems.
					if(i >= 3 && i < 11) {//codas can affect each other.
						sum += Weights[(i * (NMARCOLETTERS)) + j][ (Spot * (NUMBER_OF_PHONEMES )) +  Phoneme] * Input[i][j];
					} 
				}  else if (Spot >= 8 && Spot < 11) { //onsets. what about split k.s. NOTE ?
					if(i >= 8) { //any place a vowel effects an onset?
						sum += Weights[(i * (NMARCOLETTERS)) + j][ (Spot * (NUMBER_OF_PHONEMES )) +  Phoneme] * Input[i][j];
					} 
				} else if (Spot == 11) {//vowel..
					if(i >= 8 || i == 3) {
						sum += Weights[(i * (NMARCOLETTERS)) + j][ (Spot * (NUMBER_OF_PHONEMES )) +  Phoneme] * Input[i][j];
					} 
				} else if (Spot >= 12 && Spot <= 15) {  //codas
					if(i >= 9 ) {// need le.add 11?
						sum += Weights[(i * (NMARCOLETTERS)) + j][ (Spot * (NUMBER_OF_PHONEMES )) +  Phoneme] * Input[i][j];
					} 
				} else {
					fp1 = fopen("funnyerr.txt","a");
					fprintf(fp1,"%d %d %d\n",i,j, Spot);
					fclose(fp1);
				}

			}
		}
	}*/



	return sum;
}


void ZorziNet::CutPhonOnset(char *Word) 
{
    int i = 0;
    int NVowels = 0;
	

    PhonOnset[0] = 0;

    while(IsVowelPhoneme(Word[i]) == false) {
          PhonOnset[i] = Word[i];
          i++;
    }
	PhonOnset[i] = 0;
}

void ZorziNet::CutPhonRime(char *Word)
{
	int i;

	i = strlen(PhonOnset);
	strcpy(PhonRime, Word + i);
}

void ZorziNet::CountPhonSylls(char *Word)
{
	int i, Ss = 0;
	
	
	for(i=0;i<strlen(Word);i++) {
		if(Word[i] == '.') Ss++;
	}
	GlobalNPhonSylls = Ss;
}

void ZorziNet::MakePattern(char *Word, char *Phon, int Spot)
{

	char NewPhon[30], NewWord[30];
	int i,j,Upto;
	FILE *fp, *crap;

	ThisStress = Spot;
	strcpy(ThisPhon,Phon);

//	crap = fopen("ttttt.txt","a");
//	fprintf(crap,"%s ",Word);
//	fprintf(crap,"PHon <%s> \n",Phon);
//	fclose(crap);

	//CountPhonSylls(Phon);

	NewPhon[0] = 0; NewWord[0] = 0;

	OldLen = -1;
	

	for(i=0;i<MAXPOS;i++) {
		if(Phon[i] == '+') NewPhon[i] = 0;
		else NewPhon[i] = Phon[i];
		if(ENGLISH == 1) {
			if(Word[i] == '+') NewWord[i] = 0;
			else NewWord[i] = tolower(Word[i]);
		} else {
			if(Word[i] == '+') NewWord[i] = 0;
			else NewWord[i] = (Word[i]);	
		}
	}

	MakePhonStr(NewPhon);
//	MakeOrthographicInput(NewWord);


	////Final[Upto] = 0; Upto++;

	//STRESS////////////

//	if(GlobalNPhonSylls == 2) {
		if(Spot == 1) {
			NewOutputs[MAXPHONE -1][(NUMBER_OF_PHONEMES-2)] = 1;
			NewOutputs[MAXPHONE -1][(NUMBER_OF_PHONEMES-1)] = 0;
		} else {
			NewOutputs[MAXPHONE -1][(NUMBER_OF_PHONEMES-2)] = 0;
			NewOutputs[MAXPHONE -1][(NUMBER_OF_PHONEMES-1)] = 1;
		}
//	} else {
//			NewOutputs[MAXPHONE -1][(NUMBER_OF_PHONEMES-2)] = 0;
//			NewOutputs[MAXPHONE -1][(NUMBER_OF_PHONEMES-1)] = 0;
//	}

	Upto = 0;
}





void ZorziNet::MakePhon(char *Phon)
{
	int h,i,j,pos, nsylls, nvowelpass, passeddot = 0;
	int Upto = 0;
	int UptoSyll = 0;
	int Start = 0;
	int TempPhonPlace;
	FILE *fp;
	//Temp
	//int TheNOnsets = 3, TheNCodas = 5;

	for(i=0;i<MAXSYLLABLES+2;i++) {
		nphononsets[i] = 0;
		nphoncodas[i] = 0;
	}
	nphonvowels = 0;
	phoncodsbeforespot = 0;

	h = 0;
	for(i=0;i<strlen(Phon);i++) {
		if(Phon[i] != '.') {
			//fp = fopen("phon1111.txt","a");fprintf(fp,"%c %d\n",Phon[i],i); fclose(fp);
			if(Start == 0) {//onset
				 if((IsVowelPhoneme(Phon[i])) == true) {
					Start = 1;
					phonvowels[nphonvowels] = Phon[i];
					nphonvowels++;
				} else {
					//fp = fopen("phon1112.txt","a");fprintf(fp,"%c %d %d\n",Phon[i],IsVowelPhoneme(Phon[i]),h); fclose(fp);	
					phononsets[h][nphononsets[h]] = Phon[i];
					nphononsets[h]++;
				}
			} else if (Start == 1) {
				if((IsVowelPhoneme(Phon[i])) == true) {
					phonvowels[nphonvowels] = Phon[i];
					nphonvowels++;
				} else {
					Start++;
					phoncodas[h][nphoncodas[h]] = Phon[i];
					nphoncodas[h]++;
				}
			} else { //Start == 2)
				if((IsVowelPhoneme(Phon[i])) == true) { /*can't see why it should get here */
					fp = fopen("whyhere.txt","a");fprintf(fp,"%c %d\n",Phon[i],IsVowelPhoneme(Phon[i])); fclose(fp);
					phonvowels[nphonvowels] = Phon[i];
					nphonvowels++;
				} else {
					phoncodas[h][nphoncodas[h]] = Phon[i];
					nphoncodas[h]++;
				}
			}
		} else {
			Start = 0;
			h++;
		}
	}
	
	strcpy(TheWord, Phon);
	
	for(h=0;h<MAXSYLLABLES;h++) {
		i = 0;
		for(;i<nphononsets[h];i++) {
			PhonString[i + (h * MAXSPOTSPERSYLL)] = phononsets[h][i];
		}
		for(;i<MAXONSETS;i++) PhonString[i + (h * MAXSPOTSPERSYLL)] = '*'; 

		if(h<nphonvowels) {
			PhonString[i + (h*MAXSPOTSPERSYLL)] = phonvowels[h];
		} else PhonString[i + (h*MAXSPOTSPERSYLL)] = '*';
		i++;

		for(; i< nphoncodas[h] + (MAXONSETS + 1) ;i++) {
			PhonString[i + (h * MAXSPOTSPERSYLL)] = phoncodas[h][i-(MAXONSETS + 1)];
		}
		for(;i<MAXSPOTSPERSYLL;i++) PhonString[i + (h * MAXSPOTSPERSYLL)] = '*';
	}
	PhonString[i + (h * MAXSPOTSPERSYLL)] = 0;

	Upto = 0;
	for(i=0;i<strlen(PhonString);i++) {
		pos = PhonemeIndex(PhonOnset[i]);
		for(j=0;j<pos;j++) {
			Final[Upto] = 0;
			Upto++;
		}
		Final[Upto] = 1;
		for(;j<NUMBER_OF_PHONEMES;j++) {
			Final[Upto] = 0;
		}
	}

/*	fp = fopen("try333444.txt","a");
	fprintf(fp,"%s %s %d %d %d %d %d %d \n",GlobalWord,PhonString,nphonvowels, nphononsets[0],nphoncodas[0],nphononsets[1],nphoncodas[1],nphononsets[2]);
	fclose(fp);*/
}

void ZorziNet::TrainCycleFromAll2(char *FileWord, char *ParsedOrth, char *FilePhon, float Freq, int NumSylls, int TheStress)
{
	FILE *fp, *fp1;
	int h,i,j,k, UpTo, thenum, NewInput[8000];
	char bit[30];

	strcpy(GlobalWord, FileWord);

	for(i=0;i<MAXSYLLABLES;i++) {
		ntempcodas[i] = 0; 
		ntemponsets[i] = 0 ;
		ntempcodas2[i] = 0; 
		ntemponsets2[i] =0; 
	}
	ntempvowels = 0;

	/*fp1 = fopen("testphons.txt","a");
	fprintf(fp1,"%s %d %d %d \n",FilePhon, NOnsets,NVowels,NCodas);
	fclose(fp1);*/

	MakePhonStr(FilePhon);
	//Add Stress
	for(i=0;i<NSTRESS;i++) NewOutputs[MAXPHONE -1][(NUMBER_OF_PHONEMES-i-1)] = 0;	
	NewOutputs[MAXPHONE -1][(NUMBER_OF_PHONEMES-TheStress)] = 1; //3, 2, 1

	for(i=0;i<MAXSYLLABLES;i++) {
		nphoncodas[i] = 0;
		nphononsets[i] = 0;
	}
	nphonvowels = 0;

	for(i=0;i<MAXPOS * NMARCOLETTERS;i++) NewInput[i] = 0;
	j=0;

	for(i=0;i<strlen(ParsedOrth);i++) {
		if(ParsedOrth[i] == '+') j++;
	}

/*	if(j!=MAXSYLLABLES * 3) {
		fp = fopen("testok.txt","a");
		fprintf(fp,"%s %s \n",FileWord,ParsedOrth);
		fclose(fp);
		return;
	}*/
	/*fp = fopen("testst1111.txt","a");
	fprintf(fp,"%s ",FileWord);
	for(k=0;k<NCodas;k++) {
		fprintf(fp,"%d %s ", k,CodaGraphemes[k]);
	}
	fprintf(fp,"\n");
	fclose(fp);*/
	i = 0;
	for(h=0;h<MAXSYLLABLES;h++) {
			j = 0;
			//bit[j] = 0; bit[j+1] = 0; bit[j+2] = 0; bit[j+3] = 0;
			for(;;) { //First onset	
				if(j==0 && ParsedOrth[i] == '+') break;
				bit[j] = ParsedOrth[i]; j++;
				if( (ParsedOrth[i] == '.' || ParsedOrth[i] == '+') && j >= 1) {
					bit[j-1] = 0;
				
					if( (strlen(bit)) > 1) thenum = findGrapheme(bit);
					else thenum = FindLetter(bit[0])+ NGRAPHEMES ;
					
					//fp = fopen("bitt.txt","a");	fprintf(fp,"->%s %s [%s] \n",FileWord,ParsedOrth, bit);	fclose(fp);
		
					if(thenum == -1) {
						fp = fopen("ErrorBad.txt","a");	fprintf(fp,"->%s %s [%s] %d\n",FileWord,ParsedOrth, bit, findCodanum(bit));	fclose(fp);
						for(k=0;k<NCodas;k++) {
							fp = fopen("testst.txt","a");
							fprintf(fp,"%d %s %s \n",k,bit,CodaGraphemes[k]);
							fclose(fp);
							if( (strcmp(bit, CodaGraphemes[k])) == 0) {
								fprintf(fp,"matched %d\n",k);
							}
							fclose(fp);	
							exit(1);
						}
					}
					//fp = fopen("Etestok11.txt","a");	fprintf(fp,"->%s %s [%s] %d %s \n",FileWord,ParsedOrth, bit, thenum, GraphemesList[thenum]);	fclose(fp);
				
					strcpy(temponsets[h][ntemponsets[h]], bit);
					temponsetsnum[h][ntemponsets[h]] = thenum; 
					ntemponsets[h]++;
							
					j= 0;
					if(ParsedOrth[i] == '+' ) {
						i++;
						break;
					}
				}	
				i++;			
			}
			if(ParsedOrth[i] == '+' ) i++;

		//Vowel
		j=0;
		for(;ParsedOrth[i] != '+' && ParsedOrth[i] != '.';i++) {
			bit[j] = ParsedOrth[i];
			j++;
		}

		bit[j] = 0;
		if(strlen(bit) > 0) {
			ntempvowels++; //First vowel
		//	fp = fopen("bitt.txt","a");	fprintf(fp,"->%s %s [%s] \n",FileWord,ParsedOrth, bit);	fclose(fp);
			
			if(strlen(bit) == 1) {
				if(bit[0] == '*') {		
					thenum = -1;
				/*	fp = fopen("Errtestok22.txt","a");
					fprintf(fp,"o = %s %s [%s] %d\n",FileWord,ParsedOrth, bit, thenum);
					fclose(fp);*/
				} else {
					thenum = NGRAPHEMES + FindLetter(bit[0]);
				}
			} else {
				thenum = findGrapheme(bit) ;

				if(thenum == -1) {
					fp = fopen("Errtestok22.txt","a");
					fprintf(fp,"o = %s %s [%s] %d\n",FileWord,ParsedOrth, bit, thenum);
					fclose(fp);
				}
			}
			strcpy(tempvowels[h],bit);
			tempvowelsnum[h] = thenum;
		}
		/*fp = fopen("testok33.txt","a");
		fprintf(fp,"v = %s %s [%s] %d\n",FileWord,ParsedOrth, bit, thenum );
		fclose(fp);*/

		if(ParsedOrth[i] == '.') i++;
		if(ParsedOrth[i] == '+') i++;

	
		if(ParsedOrth[i] == '+') {
			ntempcodas[h] = 0;
		} else {
			j = 0;
			for(;;) { //First codas
				if(j==0 && ParsedOrth[i] == '+') break;	
				bit[j] = ParsedOrth[i]; j++;

				if((ParsedOrth[i] == '.' || ParsedOrth[i] == '+') && j >= 1) {
					bit[j-1] = 0;
				
				//	fp = fopen("bitt.txt","a");	fprintf(fp,"->%s %s [%s] \n",FileWord,ParsedOrth, bit);	fclose(fp);
			
					if( (strlen(bit)) > 1) {
						thenum = findGrapheme(bit);
						if(thenum == -1) {
							fp = fopen("Errtestok55.txt","a");	fprintf(fp,"->%s %s [%s] \n",FileWord,ParsedOrth, bit);	fclose(fp);
							break;		
						}
						/*thenum = findOnsetnum(bit);
						if(thenum == -1) {
							thenum = findCodanum(bit);	
							if(thenum == -1) {
								thenum = findVowelnum(bit);//need to get ue
								if(thenum == -1) {
									fp = fopen("Errtestok55.txt","a");	fprintf(fp,"->%s %s [%s] \n",FileWord,ParsedOrth, bit);	fclose(fp);
									break;
								} else thenum += NONSETS;
							} thenum += NONSETS+NVOWELS;
						} */
					} else thenum = FindLetter(bit[0])+ NONSETS+NVOWELS+NCODAS ;
					

					strcpy(tempcodas[h][ntempcodas[h]], bit);
					tempcodasnum[h][ntempcodas[h]] = thenum; 
					ntempcodas[h]++;
							
					j = 0;
					if(ParsedOrth[i] == '+' ) {
						i++;
						break;
					}
				} 
				i++;
			}
		}	
		i++;
	}

	/*fp = fopen("testst1112.txt","a");
	fprintf(fp,"%s ",FileWord);
	for(k=0;k<NCodas;k++) {
		fprintf(fp,"%d %s ", k,CodaGraphemes[k]);
	}
	fprintf(fp,"\n");
	fclose(fp);*/


	
	/*fp = fopen("WordInfo33.txt","a");
	fprintf(fp,"c = %s %s %f\n ",FileWord,ParsedOrth, Freq);
	fclose(fp);
	PrintWord();*/
	
	UpTo = 0;
	for(h=0;h<MAXSYLLABLES;h++) {
		for(i=0;i<ntemponsets[h];i++) {
			for(j=0;j<temponsetsnum[h][i];j++) {
				NewInput[UpTo] = 0; UpTo++;
			}
			NewInput[UpTo] = 1; UpTo++; j++;
			for(;j<NMARCOLETTERS;j++) {
				NewInput[UpTo] = 0; UpTo++;
			}
		}
		for(;i<MAXONSETS;i++) {
			for(j=0;j<NMARCOLETTERS;j++) {
				NewInput[UpTo] = 0; UpTo++; 
			}
		}
		if(h<ntempvowels) {
			if(tempvowelsnum[h] == -1) { /*CHANGE THIS FOR ADDING NULL VOWELS*/
				NewInput[UpTo] = 0; UpTo++;
				for(i=1;i<NMARCOLETTERS;i++) {
					NewInput[UpTo] = 0; 
					UpTo++;
				}
			} else {
				for(i=0;i<tempvowelsnum[h];i++) { //Vowel
					NewInput[UpTo] = 0; UpTo++;
				}
				NewInput[UpTo] = 1; UpTo++; i++;
				for(;i<NMARCOLETTERS;i++) {
					NewInput[UpTo] = 0; UpTo++;			
				}
			}
		} else {
			for(j=0;j<NMARCOLETTERS;j++) {
				NewInput[UpTo] = 0; UpTo++;
			}
		}
			
		for(i=0;i<ntempcodas[h];i++) { //CODAS				
			for(j=0;j<tempcodasnum[h][i];j++) {
				NewInput[UpTo] = 0; UpTo++;
			}
			NewInput[UpTo] = 1; UpTo++;j++;
			for(;j<NMARCOLETTERS;j++) {
				NewInput[UpTo] = 0; UpTo++;
			}
		}	
		for(;i<MAXCODAS;i++) {
			for(j=0;j<NMARCOLETTERS;j++) {
				NewInput[UpTo] = 0; UpTo++; 
			}
		}
	}
	UpTo = 0;

	/*fp = fopen("testst1113.txt","a");
	fprintf(fp,"%s ",FileWord);
	for(k=0;k<NCodas;k++) {
		fprintf(fp,"%d %s ", k,CodaGraphemes[k]);
	}
	fprintf(fp,"\n");
	fclose(fp);*/


//	fp = fopen("examineit.txt","a");
//	fprintf(fp,"%s %s\n",FileWord,ParsedOrth);
	for(i=0;i<MAXPOS;i++) {
		for(j=0;j<NMARCOLETTERS;j++) {
			Input[i][j] = NewInput[UpTo++];
//			fprintf(fp,"%1.0f",Input[i][j]);
		}
//		fprintf(fp,"\n");
	}
//	fclose(fp);


	/*fp=fopen("phons1.txt","a");
	fprintf(fp,"%s %s %d\n",FileWord,ParsedOrth,TheStress);
	for(i=0;i<MAXPOS;i++) {
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			fprintf(fp,"%d",NewOutputs[i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);*/
/*	fp = fopen("testst1111.txt","a");
	fprintf(fp,"%s ",FileWord);
	for(k=0;k<NCodas;k++) {
		fprintf(fp,"%d %s ", k,CodaGraphemes[k]);
	}
	fprintf(fp,"\n");
	fclose(fp);*/

	//PrintWord();

	if(ENGLISH == 1 && ntempvowels == 2) {
		if(ntempcodas[0] == 2) {
			if( (strcmp(tempcodas[0][1],"e")) == 0) {
				//fp = fopen("enottrain.txt","a");
				//fprintf(fp,"%s %s %s %s\n",FileWord, FilePhon, tempcodas[0][1],tempcodas[0][2]);
				//fclose(fp);
			}
		} else if (ntempcodas[0] > 2) {
			if( (strcmp(tempcodas[0][1],"e")) == 0 || (strcmp(tempcodas[0][2],"e")) == 0)  {
				//fp = fopen("enottrain.txt","a");
				//fprintf(fp,"%s %s %s %s\n",FileWord, FilePhon, tempcodas[0][1],tempcodas[0][2]);
				//fclose(fp);
			} 
		} else TrainCycle(FileWord, FilePhon, Freq, 0);
	} else TrainCycle(FileWord, FilePhon, Freq, 0);
}

void ZorziNet::TrainCycleFromAll(char *FileWord, char *ParsedOrth, char *FilePhon, float Freq, int NumSylls, int TheStress)
{
	
}



void ZorziNet::TrainCycle(char *Word, char *Phon, float FreqMod, int TheSpot)
{
	int i,j,k,l, allzero;
	float Value;
	float Delta;
	int counter = 0, Spot, Phoneme; 
	char NewWord[30];
	char NewPhon[30];

	FILE *fp;

	GlobalFreqMod = FreqMod;

	//TotalError = 0;
	
/*	fp = fopen("4444 shit.txt","a");
	fprintf(fp,"here %s %s %f\n", Word, Phon, FreqMod);
	fclose(fp);*/

	for(i=0;i<MAXPHONE;i++) {
		if(Phon[i] == '+') NewPhon[i] = 0;
		else NewPhon[i] = Phon[i];

		/*if(ENGLISH == 1) {
			if(Word[i] == '+') NewWord[i] = 0;
			else NewWord[i] = tolower(Word[i]);
		} else {
			if(Word[i] == '+') NewWord[i] = 0;
			else NewWord[i] = (Word[i]);
		}*/
	}

	OldLen = -1;
	NeedForward = TRUE;

	
	ForwardPass();

	
//	fp = fopen("shit111711.txt","a");
	for(i=0;i<MAXPHONE;i++) {
	//	fprintf(fp,"%d  ",i);
      for(j=0;j<(NUMBER_OF_PHONEMES);j++) {
			if(Output[i][j] < Threshold) Output[i][j] = 0;
	//		fprintf(fp,"%1.1f ",Output[i][j]);
	  }
	//  fprintf(fp," \n");
	}
//	fclose(fp);

//	MakePhon(Phon);
	
	
	counter = 0;
	for(i=0;i<MAXPHONE;i++) {
    	for(j=0;j<(NUMBER_OF_PHONEMES);j++) {
			ErrorBuffer[i][j] = 0;
			
			Value = (float) /*Final[counter]*/ /*NewOutputs[TheSpot][counter]*/ NewOutputs[i][j];
		    ErrorBuffer[i][j] = Value - Output[i][j];	
			TotalError += fabs(ErrorBuffer[i][j]);		
			counter++;
      }
	
    }
	//fclose(fp);
//	exit(1);


//	fp = fopen("shit.txt","a");
//	fprintf(fp,"here 4 %s %s\n",Word, Phon);
//	fclose(fp);


/*		fp = fopen("3shit.txt","a");
	fprintf(fp,"here %s %s %f\n", Word, Phon, FreqMod);
	fclose(fp);*/


	allzero = 0;
	int sspot = NONSETS+NCODAS+NVOWELS+FindLetter('k'), sphoneme = PhonemeIndex('k');
	FILE *fp2,*fp1;


	 for(Spot = 0;Spot<MAXPHONE;Spot++) {
		 for(Phoneme = 0;Phoneme<(NUMBER_OF_PHONEMES);Phoneme++) {
			 for(i=0;i<MAXPOS;i++) {
				for(j=0;j< (NMARCOLETTERS);j++) {
					if(Input[i][j] > .99) { //For Speed.
						/*if(Spot == 8 && Phoneme == sphoneme) {
							fp2 = fopen("Weightstest.txt","a");
							fprintf(fp2,"%d %d %f \n", i,j, SumWeights[i]);
							fclose(fp2);
						}*/

						Delta = FreqMod * LearningRate * ErrorBuffer[Spot][Phoneme] * Input[i][j];				
						if(Delta > 1) exit(1);
						Weights[(i * (NMARCOLETTERS)) + j][ (Spot * (NUMBER_OF_PHONEMES )) +  Phoneme] += Delta;
			
					} else {
						Delta = 0;
					}
				}
			}
		 }
	 }


		for(i=0;i<(NMARCOLETTERS) * MAXPOS;i++) {
			SumWeights[i] = 0;
			for(j=0;j<(NUMBER_OF_PHONEMES) * MAXPHONE ;j++) {
				SumWeights[i] += fabs(Weights[i][j]);		
			}
		//	fp2 = fopen("Weightstest.txt","a");
		//	fprintf(fp2,"%d %d %f \n", i,j, SumWeights[i]);
		//	fclose(fp2);
		}


	/* for(i=0;i<MAXPOS;i++) {
		 for(j=0;j< (NMARCOLETTERS);j++) {
			SumWeights[i * NMARCOLETTERS + j] = 0;
			for(k=0;k<MAXPHONE;k++) {
				for(l=0;l<NUMBER_OF_PHONEMES;l++) {
					SumWeights[i * NMARCOLETTERS + j] += fabs (Weights[i * NMARCOLETTERS + j][k * NUMBER_OF_PHONEMES + l]);
				}
			}
		 }
	 }*/

//	fp = fopen("shittt.txt","a");
//	fprintf(fp,"here 5 %s \n",Word);
//	fclose(fp);
}

void ZorziNet::ForwardPass()
{
	FILE *testit,*fp1,*fp2;

	float max;
	int maxspot;
	char netstring[200];
	int count;

	int i,j,k,l;

	for(i=0;i<MAXPHONE;i++) {
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			/*if(SumAct(i,j) > .1) {
				fp1 = fopen("hmmmmmm2.txt","a");
				fprintf(fp1,"%d %d %f %f %f\n",i,j,Output[i][j], Threshold, ActFunc(SumAct(i,j)));
				fclose(fp1);
			}*/
			Output[i][j] = ActFunc(SumAct(i,j));
			if(Output[i][j] < Threshold) Output[i][j] = 0;
			else {
				/*p1 = fopen("hmmmmmm1.txt","a");
				fprintf(fp1,"%d %d %f %f\n",i,j,Output[i][j], Threshold);
				fclose(fp1);*/
			}
		}
	}
	//	fclose(fp2);
	for(i=0;i<MAXPHONE;i++) {
		max = -100;
		maxspot = -100;
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			if(max < Output[i][j]) {
				max = Output[i][j];
				maxspot = j;
			}
		}
		if(max > 0.05) {
			netstring[i] = Phonemes[maxspot];
		} else netstring[i] = '*';
		netstring[i] = 0;
	}

	StressVal1 = Output[MAXPHONE -1][(NUMBER_OF_PHONEMES-3)];
	StressVal2 = Output[MAXPHONE -1][(NUMBER_OF_PHONEMES-2)];
	StressVal3 = Output[MAXPHONE -1][(NUMBER_OF_PHONEMES-1)];

	
//	fp1 = fopen("teststress.txt","a");
//	fprintf(fp1,"%s %f %f %f \n",SplitLetterWord, StressVal1, StressVal2, StressVal3);
//	fclose(fp1);
	
	//	fclose(testit);
}

void ZorziNet::CutRime(char *Word)
{
	char TempRime[100];
	char TempOnset[100];
	char TempVowel[100]; 
    int i, startspot;


	if(strlen(Word) > 0) {
		TempRime[0] = 0;
		TempOnset[0] = 0;
		TempVowel[0] = 0;

		//strcpy(TempOnset, OrthRime);
 
		i = strlen(OrthOnset);
	//	startspot = strlen(TempOnset);
		strcpy(TempRime,Word + i);
		strcpy(OrthRime,TempRime);
	//	return(TempRime);
	} else { 
		OrthRime[0] = 0;

	//	return(TempRime);
	}
}

void ZorziNet::CutVowel() {
//ONLY USE AFTER CUT RIME. SPECIAL FUNCTION !!! keeps continguis *.e rules!
	int i,j;
	char bit[100];
//	FILE *crap;


	VowelWithCoda = 0;

	for(i=0;i<strlen(OrthRime);i++) {
		if( (IsVowel(OrthRime[i])) == -1) break;
		else vowel[i] = OrthRime[i];
	}
	j = i;
	if(strlen(OrthRime) > i) {
		if(IsVowel(OrthRime[i-2]) == -1) {
			if(OrthRime[i+1] == 'e' && (OrthRime[i-1] == 'a' || OrthRime[i-1] == 'i' ||OrthRime[i-1] == 'o' || OrthRime[i-1] == 'u' ) ){
				for(;i<j+2;i++) {
						vowel[i] = OrthRime[i];
				}
				VowelWithCoda = 1;
				vowel[i] = 0;
			} else {
				vowel[i] = 0;
			}
		}
	}
	restofword[0] = 0;


	for(j=0;j<NVowels;j++) {
		if(strlen(VowelGraphemes[j]) <= strlen(OrthRime) ) {
			strncpy(bit,OrthRime, strlen(VowelGraphemes[j]));
			bit[strlen(VowelGraphemes[j])] = 0;
			if( (strcmp(VowelGraphemes[j],bit)) == 0) { //Matched//
			   break;
			}
		}
	}
	if(j!=NVowels) {
		strcpy(vowel, VowelGraphemes[j]);
		VowelWithCoda = 0;
	}
	
	
	if( strlen(vowel) != strlen(OrthRime) ) {
		strcpy(restofword, OrthRime + strlen(vowel));
	}
	for(i=0;i<strlen(restofword);i++) {
		if(restofword[i] == '+') {
			restofword[i] = 0;
			break;
		}
	}

//	crap = fopen("temp.txt","a");
//	fprintf(crap,"**  %s ** /n \n",vowel);
//	fclose(crap);
//	exit(1);

}



void ZorziNet::CutOnset(char *Word)
{
	char Onset[100];
	int i = 0;
	char bit[10];
    
	FILE *fuck;

	Onset[0] = 0;
	
	
	if( (strlen(Word)) > 0) {
		
		if(Word[0] == 'y') {
		  Onset[0] = 'y';
		  i++;
		}

		 while(IsVowel(Word[i]) == -1 || Word[i] == '*' ) {
			bit[0] = Word[i];
			bit[1] = 0;
			strcat(Onset, bit);
			i++;
			Onset[i] = 0;
			if(i == MAXORTHONSET || i > strlen(Word) ) break;
		 }

		 if(i != 0) {
			 if(Onset[i-1] == 'q') {
				 bit[0] = Word[i];
				 bit[1] = 0;
				 strcat(Onset,bit);
				 i++;
			}
		 	Onset[i] = 0;
		 }

	} else {
		Onset[0] = 0;
	}
	strcpy(OrthOnset,Onset);
//	fuck = fopen("fuck.txt","a");
//	fprintf(fuck,"$$ %s $$ \n",OrthOnset);
//	fclose(fuck);
//	return Onset;
}


char ZorziNet::GuessAgain(char *grapheme, int StartLooking) {

	return '*';
}

int ZorziNet::CheckForOnsetGrapheme(char *Grapheme)
{

	int i,j;
	char bit[100];


	for(j=0;j<NOnsets;j++) {
		if( (strlen(OnsetGraphemes[j])) <= strlen(Grapheme) ) {
			strncpy(bit,Grapheme,strlen(OnsetGraphemes[j]));
			bit[strlen(OnsetGraphemes[j])] = 0;
			if( (strcmp(OnsetGraphemes[j],bit) ) == 0 ) { //Matched//
				return j; 
			}
		}
	}
	return j;
}

int ZorziNet::CheckForCodaGrapheme(char *Grapheme)
{

	int i,j;
	char bit[100];


	for(j=0;j<NCodas;j++) {
		if( (strlen(CodaGraphemes[j])) <= strlen(Grapheme) ) {
			strncpy(bit,Grapheme,strlen(CodaGraphemes[j]));
			bit[strlen(CodaGraphemes[j])] = 0;
			if( (strcmp(CodaGraphemes[j],bit) ) == 0) { //Matched//
				return j; 
			}
		}
	}
	return j;

}

void ZorziNet::GraphemeCalc(char *Word) 
{

	char TempString[2000] ;
    char LittleBit[200] ;
	char bit[200];
	
	char NewWord[200];

    int i,j,k,l,pos, GrLets = 0, upto = 0, extra = 0;
    int RimeCounter;
	float maxval,maxval2;
	int maxpos, maxslot, maxstart, maxpos2;
	int nextspot;
	int chunk[1000];


	FILE *crap;

	NeedForward = TRUE;
	//Threshold = 0;

	for(i=0;i<strlen(Word);i++) {
		if(ENGLISH ==1) NewWord[i] = tolower(Word[i]);
		else NewWord[i] = Word[i];
	}
	NewWord[i] = 0;

	CutOnset(NewWord);
	CutRime(NewWord);
	CutVowel();

	for(i=0;i<strlen(OrthOnset) && i < MAXORTHONSET;) {

    		for(j=0;j<1000;j++) Final2[j] = 0;	
			for(l=0;l<upto;l++) {
				for(j=0;j<NMARCOLETTERS;j++) {
					  strcat(Final2,"0");
				}
			}

			j = CheckForOnsetGrapheme(OrthOnset + i);
			k = CheckForCodaGrapheme(OrthOnset + i);
		//	j = NOnsets; k= -1;
			if(j == NOnsets && k != NCodas) { //CODA grapheme in ONSET POSITION
					FinalPhon[upto] = GuessAgain(OrthOnset + i,4);
					i+=strlen(CodaGraphemes[k]);
					upto++;
					FinalPhon[upto] = 0;
					//crap = fopen("testit3.txt","a");
				//	fprintf(crap,"%s \n",FinalPhon);
				//	fclose(crap);
					//exit(1);
			} else {
				if(j != NOnsets) { //Must be a grapheme//
					for(k=0;k<j;k++) strcat(Final2,"0");
					strcat(Final2,"1");
					for(k=j+1;k<NOnsets;k++) strcat(Final2,"0");
					for(k=0;k<NMARCOLETTERS - NOnsets;k++) strcat(Final2,"0");
					i += strlen(OnsetGraphemes[j]);
					strcpy(bit,OnsetGraphemes[j]);
					GrLets += strlen(OnsetGraphemes[j]) - 1;
				} else {
					for(k=0;k<NGraphemes;k++) strcat(Final2,"0"); //Blanks for the Onsests
					if(ENGLISH == 1) {
						pos = AlphabetIndex(toupper(OrthOnset[i]));
					} else {
						pos = AlphabetIndex(OrthOnset[i]);
					}
					bit[0] = OrthOnset[i];
					bit[1] = 0;
					//strcat(TempString,bit);

					for(k=0; k < NMARCOLETTERS - NGraphemes;k++) {
						if(k == pos) {
						   strcat(Final2,"1");
						} else {
						   strcat(Final2,"0");
						}
					}
					i++;
				}
				
				for(j=upto;j<MAXPOS;j++) {
					for(k=0;k<NMARCOLETTERS;k++) {
						strcat(Final2,"0");
					}
				}
				for(j=0;j<MAXPOS;j++) {
					for(k=0;k<NMARCOLETTERS;k++) {
						if(Final2[j * (NMARCOLETTERS) + k] == '0') Input[j][k] = 0;
						else Input[j][k] = 1;
					}
				}
		
				ZorziNet::ForwardPass();


				for(l=0;l<3;l++) {
					maxval = -1; maxpos = -1;

					for(k=0;k<NUMBER_OF_PHONEMES;k++) {
						if(Output[l][k] > maxval) {
							maxval = Output[l][k];
							maxpos = k;
							maxslot = l;
						}
					}
					if(maxval < .05 && extra == l) {
						FinalPhon[upto] = GuessAgain(bit,0);
						upto++;
						FinalPhon[upto] =  0;
					} else if (extra == l) {
						FinalPhon[upto] = Phonemes[maxpos];
						upto++;
						FinalPhon[upto] =  0;
					} else {
					//	exit(1);
						if(maxval > 0.995) {
							FinalPhon[upto] = Phonemes[maxpos];
							upto++;
							FinalPhon[upto] =  0;
						}
					}				
				}
			}	
			extra++;
	}


	i = 0;
    GrLets = 0;


	for(j=0;j<1000;j++) Final2[j] = 0;

	for(j=0;j<3;j++) {
		for(k=0;k<NMARCOLETTERS;k++) {
			strcat(Final2,"0");
		}
	}

	for(j=0;j<NVowels;j++) {
		if(strlen(VowelGraphemes[j]) <= strlen(vowel) ) {
			strncpy(bit,vowel, strlen(VowelGraphemes[j]));
			bit[strlen(VowelGraphemes[j])] = 0;
			if( (strcmp(VowelGraphemes[j],bit)) == 0) { //Matched//
			   break;
			}
		}
	}

	if(j != NVowels) { //Must be a grapheme//
		for(k=0;k<NOnsets;k++) strcat(Final2,"0");
		for(k=0;k<j;k++) strcat(Final2,"0"); 
		strcat(Final2,"1");
		for(k=j+1;k<NVowels;k++) strcat(Final2,"0");
		for(k=0;k<NMARCOLETTERS - NOnsets - NVowels;k++) strcat(Final2,"0");
		i += strlen(VowelGraphemes[j]);
		GrLets += strlen(VowelGraphemes[j]) - 1;
	} else {
	 	for(k=0;k<NGraphemes;k++) strcat(Final2,"0"); //Blanks for the graphemes
		if(ENGLISH == 1) {
			pos = AlphabetIndex(toupper(vowel[i]));
		} else {
			pos = AlphabetIndex(vowel[i]);
		}
		bit[0] = vowel[i];
		bit[1] = 0;
	//	strcat(TempString,bit);
		for(k=0; k < NMARCOLETTERS - NGraphemes;k++) {
		    if(k == pos) {
		        strcat(Final2,"1");
			} else {
				strcat(Final2,"0");
			}
		}
		i++;
	}
		
	
	for(;i < strlen(vowel) && i <= MAXORTHBODY;) {
	    for(j=0;j<NCodas;j++) {
			if(i + strlen(CodaGraphemes[j]) <= strlen(vowel) ) {
				strncpy(bit,vowel + i, strlen(CodaGraphemes[j]) );
				bit[strlen(CodaGraphemes[j])] = 0;	
				if( (strcmp(CodaGraphemes[j],bit)) == 0) { //Matched//
					break;
				}
			}
		}
		if(j != NCodas) { //Must be a grapheme//
			//strcat(TempString, CodaGraphemes[j]);
			//strcat(TempString,"*");
			for(k=0;k<NOnsets + NVowels;k++) strcat(Final2,"0");
			for(k=0;k<j;k++) strcat(Final2,"0");
			strcat(Final2,"1"); 
			for(k=j+1;k<NCodas;k++) strcat(Final2,"0");
			for(k=0;k<NMARCOLETTERS - NGraphemes;k++) strcat(Final2,"0");
			i += strlen(CodaGraphemes[j]);
			GrLets += strlen(CodaGraphemes[j]) - 1;
		 
		} else {
			for(k=0;k<NGraphemes;k++) strcat(Final2,"0"); //Blanks for the graphemes
		
			if(ENGLISH == 1) {
				pos = AlphabetIndex(toupper(vowel[i]));
			} else {
				pos = AlphabetIndex(vowel[i]);
			}
			bit[0] = vowel[i];
			bit[1] = 0;
	      //  strcat(TempString,bit);
			for(k=0; k < NMARCOLETTERS - NGraphemes;k++) {
				if(k == pos) strcat(Final2,"1");
				else strcat(Final2,"0");
			}
			i++;
		}
	} 

	for(;i<MAXORTHBODY + GrLets;i++) {
		for(j=0;j<NMARCOLETTERS;j++) {
			strcat(Final2,"0");
		}
	}
	for(j=0;j<MAXPOS;j++) {
		for(k=0;k<NMARCOLETTERS;k++) {
			if(Final2[j * (NMARCOLETTERS) + k] == '0') Input[j][k] = 0;
			else Input[j][k] = 1;
		}
	}
	ZorziNet::ForwardPass();

	maxval = -1; maxpos = -1;

	for(k=0;k<NUMBER_OF_PHONEMES;k++) {
		if(Output[3][k] > maxval) {
			maxval = Output[3][k];
			maxpos = k;
			
		}
	}
	if(maxval > .05) {
		FinalPhon[upto] = Phonemes[maxpos];
	} else {
		FinalPhon[upto] = '*';
	}
	upto++;
	maxslot = 3;

	FinalPhon[upto] =  0;

	if(VowelWithCoda == 1) {
		maxval = -1; maxpos = -1;
		for(k=0;k<NUMBER_OF_PHONEMES;k++) {
			if(Output[4][k] > maxval) {	
				maxval = Output[4][k];
				maxpos = k;
			}
		}
		if(maxval > .05) {
			FinalPhon[upto] = Phonemes[maxpos];
		} else {
			FinalPhon[upto] = '*';
		}
		upto++;
		FinalPhon[upto] =  0;
		maxslot = 4;

	}
	maxstart = maxslot + 1;

	for(i=0;i < strlen(restofword); ) {
		for(j=0;j<1000;j++) Final2[j] = 0;
		
		for(l=0;l<maxslot+1;l++) {
			for(j=0;j<NMARCOLETTERS;j++) {
				 strcat(Final2,"0");
			}
		}

	    for(j=0;j<NCodas;j++) {
			if(i + strlen(CodaGraphemes[j]) <= strlen(restofword) ) {
				strncpy(bit,restofword + i, strlen(CodaGraphemes[j]) );
				bit[strlen(CodaGraphemes[j])] = 0;	
				if( (strcmp(CodaGraphemes[j],bit)) == 0) { //Matched//
					break;
				}
			}
		}
		if(j != NCodas) { //Must be a grapheme//
			//strcat(TempString, CodaGraphemes[j]);
			//strcat(TempString,"*");
			for(k=0;k<NOnsets + NVowels;k++) strcat(Final2,"0");
			for(k=0;k<j;k++) strcat(Final2,"0");
			strcat(Final2,"1"); 
			for(k=j+1;k<NCodas;k++) strcat(Final2,"0");
			for(k=0;k<NMARCOLETTERS - NGraphemes;k++) strcat(Final2,"0");
			i += strlen(CodaGraphemes[j]);
			GrLets += strlen(CodaGraphemes[j]) - 1;
			strcpy(bit,CodaGraphemes[j]);

		} else {
			for(k=0;k<NGraphemes;k++) strcat(Final2,"0"); //Blanks for the graphemes
								
			if(ENGLISH == 1) {
				pos = AlphabetIndex(toupper(restofword[i]));
			} else {
				pos = AlphabetIndex(restofword[i]);
			}
			bit[0] = restofword[i];
			bit[1] = 0;
	      //  strcat(TempString,bit);
			for(k=0; k < NMARCOLETTERS - NGraphemes;k++) {
				if(k == pos) strcat(Final2,"1");
				else strcat(Final2,"0");
			}
			i++;
			if( (strncmp(restofword+i,"ue",2)) == 0) {
				i += 2;
			}
		}
	
		if(restofword[i] == restofword[i-1]) i++;

		if(restofword[i] == 'e') {
			if(AttachE == 1) {
				for(k=0;k<NGraphemes;k++) strcat(Final2,"0"); //Blanks for the graphemes
				pos = AlphabetIndex('E');
				bit[0] = restofword[i];
				bit[1] = 0;
				for(k=0; k < NMARCOLETTERS - NGraphemes;k++) {
					if(k == pos) strcat(Final2,"1");
					else strcat(Final2,"0");
				}
				k=strlen(bit);
				bit[k] = 'e';
				bit[k+1] = 0;
			}
			i++;
		}
			
		for(k = maxslot+2;k<MAXPHONE;k++) {
			for(j=0;j<NMARCOLETTERS;j++) {
				strcat(Final2,"0");
			}
		}
	
		for(j=0;j<MAXPOS;j++) {
			for(k=0;k<NMARCOLETTERS;k++) {
				if(Final2[j * (NMARCOLETTERS) + k] == '0') Input[j][k] = 0;
				else Input[j][k] = 1;
			}
		}	

		if(maxstart != 0) {
			for(k=0;k<NMARCOLETTERS;k++) {
				if(UseSingleContext == 1) {
					Input[maxstart-1][k] = chunk[k];
				}
			}
		}
		for(j=0;j<NMARCOLETTERS;j++) {
			chunk[j] = Input[maxstart][j];
		}
	

		ZorziNet::ForwardPass();

		maxval = -1; maxpos = -1;

		//for(k=0;k<NUMBER_OF_PHONEMES;k++) {
		//	if(Output[maxslot+1][k] > maxval) {
		//		maxval = Output[maxslot+1][k];
		//		maxpos = k;			
		//	}
		//}
		//maxslot++;


	//	if(maxval > .05) {
	//		FinalPhon[upto] = Phonemes[maxpos];
	//	} else {
	//		FinalPhon[upto] = GuessAgain(bit,0);
	//	}
	//	upto++;
	//	FinalPhon[upto] =  0;

	
		maxval = -1; maxpos = -1; 
		for(l=maxstart;l<MAXPOS;l++) {
			maxval = -1; maxpos = -1; //maxslot =  -1;
			for(k=0;k<NUMBER_OF_PHONEMES;k++) {
				if(Output[l][k] > maxval) {
					maxval = Output[l][k];
					maxpos = k;
				//		maxslot = l;
				}
			}
			if(maxval < .05 && maxstart == l) {
				if(bit[0] == 'x') {
				//HACK HERE!!!!!
					FinalPhon[upto] = 'k';
					FinalPhon[upto+1] = 's';
					FinalPhon[upto+2] = 0;
					upto += 2;
				} else {
					FinalPhon[upto] = GuessAgain(bit,0);
					upto++;
					FinalPhon[upto] =  0;
				}
			} else if (maxstart == l) {
				FinalPhon[upto] = Phonemes[maxpos];
				upto++;
				FinalPhon[upto] =  0;
			} else {
				if(maxval > .995) {
					FinalPhon[upto] = Phonemes[maxpos];
					upto++;
					FinalPhon[upto] =  0;
				}
			}	
		}
	
		maxstart++;
		maxslot++;
		extra++;
	} 


//	crap = fopen("test2.txt","a");
//	fprintf(crap,"** %s %c %d %s $%s$ %s <%s> %d %d %f ** /n \n",restofword,vowel[i - 1], pos,NewWord, OrthOnset, vowel,FinalPhon, maxpos, maxslot,maxval);
//	exit(1);
	
}

int ZorziNet::FindLetter(char Letter)
{

	int i;
	char templetter;
	FILE *fp;

	if(ENGLISH == 1) templetter = toupper(Letter);
	else templetter = Letter;

	for(i=0;i<NLETTERS;i++) if(templetter == Alphabet[i]) return i;
	
	return -1;


	//if(Letter >= 'A' && Letter <= 'Z') return (Letter /*- 65*/ - 'A');//A-Z
//	else if (Letter >= 'a' && Letter <= 'z') return (Letter /*- 97*/ - 'a'); //a-z 
//	else return(-1);
}


int ZorziNet::CheckPhons(char *FilePhon)
{
	int i, thelet;
	FILE *fp1;

	/*fp1 = fopen("tessssst.txt","a");
	fprintf(fp1,"%s \n",FilePhon);
	fclose(fp1);*/

	for(i=0;i<strlen(FilePhon);i++) {
		if(FilePhon[i] != '*') {
			thelet = PhonemeIndex(FilePhon[i]);
			if(thelet == -1) {
				//	fp1 = fopen("tessssst.txt","a");
				//fprintf(fp1,"%s %d %c\n",FilePhon,i,FilePhon[i] );
				//fclose(fp1);
				return -1;
			}
		}
	}
	return 0;
}

int ZorziNet::CheckOrths(char *FileWord)
{
	int i, thelet;
	FILE *fp1;
	/*fp1 = fopen("stufffff.txt","a");
	fprintf(fp1,"%s %d \n",FileWord, strlen(FileWord));
	fclose(fp1);*/

	for(i=0;i<strlen(FileWord);i++) {
		/*fp1 = fopen("stufffff.txt","a");
		fprintf(fp1,"%s %d %d %c\n",FileWord, strlen(FileWord),i,toupper(FileWord[i]));
		fclose(fp1);*/

		if(ENGLISH == 1) {
			thelet = FindLetter(toupper(FileWord[i]));
		} else {
			thelet = FindLetter(FileWord[i]);
		}
		if(thelet == -1) return -1; 
	}
	return 0;

}


void ZorziNet::CreateFreqInfo()
{
	FILE *fp, *fp2;
	char FileWord[50], FilePhon[50], newWord[50], crapphon[50];
	int count = 0,i=0,j, ok1, ok2, tempsylls, wordnum,tempstress;
	float thisfreq, thisfreq2;
	
	ntempcodas[0] = 0;
#if ENGLISH
	fp = fopen("./NewLex4.3.2.txt","r");
	while((fscanf(fp,"%s%s%d%f%f%s%d",FileWord, FilePhon, &tempstress, &thisfreq, &thisfreq2, crapphon, &wordnum)) != EOF) {	
#elif ITALIAN
	fp = fopen("./ItalianRunningDB2.txt","r");
	while((fscanf(fp,"%s%s%d%f%f%s%d",FileWord, FilePhon, &tempstress, &thisfreq, &thisfreq2, crapphon, &wordnum)) != EOF) {	
#elif GERMAN
	fp = fopen("./GermanRunningDB2.txt","r");
	while((fscanf(fp,"%s%s%d%f%f%s%d",FileWord, FilePhon, &tempstress, &thisfreq, &thisfreq2, crapphon, &wordnum)) != EOF) {	
#elif FRENCH
	fp = fopen("./FrenchRunningDB2.txt","r");
	while((fscanf(fp,"%s%s%d%f%f%s%d",FileWord, FilePhon, &tempstress, &thisfreq, &thisfreq2, crapphon, &wordnum)) != EOF) {	
#endif
	/*	fp2 = fopen("test112.txt","a");
		fprintf(fp2,"%d %s  \n",i,FileWord);
		fclose(fp2);*/
		ThisFreq = thisfreq;		
		strcpy(GlobalWord,FileWord);

		for(i=0;i<strlen(FileWord);i++) {
			newWord[i] = tolower(FileWord[i]);
		}
		newWord[i] = 0;	

		
	/*	fp2 = fopen("test112.txt","a");
		fprintf(fp2,"%d %s %s %d \n",i,FileWord, GlobalWord,strlen(FileWord));
		fclose(fp2);*/

		strcpy(FileWord,newWord);
		strcpy(GlobalPhon,FilePhon);


		for(i=0;i<4;i++) {
			ntempcodas[i] = 0; 
			ntempvowels = 0;
			ntemponsets[i] = 0 ;
			ntempcodas2[i] = 0; 
			ntemponsets2[i] =0; 
		}
		count++;

	/*	fp2 = fopen("test112.txt","a");
		fprintf(fp2,"%d \n",i);
		fclose(fp2);*/
		MakePhon(FileWord);

		ok1 = CheckPhons(FilePhon);
		ok2 = CheckOrths(FileWord);

		if( ok1 == -1 || ok2 == -1) {
		//	fp2 = fopen("shitty.txt","a");
		//	fprintf(fp2,"[%s] [%s] %d %d \n",FileWord, FilePhon, ok1, ok2);
		//	fclose(fp2);
		} else {
			difvowels = 0;
			MakePhonStr(FilePhon);
			GlobalNPhonSylls = nphonvowels;
			
			SegmentWord(FileWord); //turn it into consonants and graphemes
			ParseSegments(); //parse the consonants and graphemes
			
			//CreateOrthString();
			//AlignInputs();	
			
			//should be unaligned
			nphonvowels -= 2;
			/*fp2 = fopen("test111.txt","a");
			fprintf(fp2,"%s %s %d %d %d %d %d %d\n",GlobalWord,FilePhon, nphonvowels,ntempvowels, ntempcodas[0],ntemponsets[1],nphoncodas[0],nphononsets[1]);
			fclose(fp2);*/

			if(/*TheTrainingCycle == 0 &&*/ nphonvowels == 2 && (ntempcodas[0] + ntemponsets[1] == nphoncodas[0]+nphononsets[1]) ) AddGraphemePhonemeInf();  	
		}

	
	/*	fp2 = fopen("test112.txt","a");
		fprintf(fp2,"---%d %s %s %d \n",i,FileWord, GlobalWord,strlen(FileWord));
		fclose(fp2);*/
	}
	fclose(fp);

/*	fp =fopen("here2.txt","w");
	fprintf(fp,"%s\n",FileWord);
	fclose(fp);*/

	if(ENGLISH ==1) fp=fopen("graphinfo.txt","w");
	else fp=fopen("graphinfoItalian.txt","w");
	for(i=0;i<10;i++) {
		fprintf(fp,"%d %d  \n",i, nGraphemesUpTo[i]);
		for(j=0;j<nGraphemesUpTo[i];j++) {
			fprintf(fp," %d %s %f %d %c \n",GraphemesInf[i][j], GraphemesInflets[i][j], GraphemeFreqsInf[i][j], PhonemesInf[i][j], PhonemesCInf[i][j]); 
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}



int ZorziNet::CheckPhonsItalian(char *Phon)
{
	int i=0,j,k, count = 0, phonlen;
	FILE *fp;

/*	fp = fopen("lookit.txt","a");
	fprintf(fp,"%s \n",Phon);
	fclose(fp);*/
	
	phonlen = strlen(Phon);
	
	while(i<phonlen) {
		/*fp = fopen("lookit.txt","a");
		fprintf(fp,"%c %d\n",Phon[i], IsVowelPhoneme(Phon[i]));
		fclose(fp);*/

		if( (IsVowelPhoneme(Phon[i])) == true) count++;
		if(count == 2) return -1;
		if(Phon[i] == '.') count = 0;
		i++;
	}

	return 1;
}

void ZorziNet::CreateDB()
{
FILE *fp, *fp2;
	char FileWord[50], FilePhon[50], newWord[50],crapphon[50];
	int count = 0,i, j, ok1, ok2, ok3, tempsylls, tempstress,wordnum,thisnosylls = 0;
	float thisfreq, thisfreq2;
	
//	fp = fopen("./NewLex4.3.2.txt","r");

	if(ENGLISH == 1) fp = fopen("./EnglishRes/EveryThing.txt","r");
	else if(ITALIAN ==1) fp = fopen("./ItalianRes/ItalianWords2.txt","r");
	else if(GERMAN == 1) fp = fopen("./GermanRes/NewGerDB3.txt","r");
	else if(FRENCH==1) fp = fopen("./FrenchRes/FrenchWords.txt","r");

	if(fp == 0) exit(1);

	fp2 = fopen("DBRes.txt","w");

#if ENGLISH
	while((fscanf(fp,"%s%s%d%f%d",FileWord, FilePhon, &tempstress, &thisfreq, &thisnosylls)) != EOF) {	
#elif ITALIAN
	while((fscanf(fp,"%s%s%d%d%f",FileWord, FilePhon, &thisnosylls, &tempstress, &thisfreq)) != EOF) {	
#elif GERMAN
	while((fscanf(fp,"%s%s%d%f%f%d",FileWord, FilePhon, &tempstress, &thisfreq, &thisfreq2)) != EOF) {		
#elif FRENCH
	while((fscanf(fp,"%s%s%d%f%f%d",FileWord, FilePhon, &tempstress, &thisfreq, &thisfreq2)) != EOF) {		
#endif

		if(thisnosylls <= 2) {
			MakePhon(FilePhon);
			fprintf(fp2,"%s %s %d %5.0f %5.0f %s \n",FileWord, PhonString, tempstress, thisfreq, thisfreq2, FilePhon);
		}	
	}
	fclose(fp2);
}




void ZorziNet::TestAllFile(char *FileName)
{
//this is for graphemes..tries to learn 3 grapheme template with in one.

	int a,h,i,j,k,l=0, spot = 0, codcount, tempspot, tempupto, start_i, used_v = -1, used_c2 = 0, tempmaxplace;
	float inputs2[2000],sum, Delta, tempmax, maxorth,minorth, minavg,totalerror;
	FILE *fp1,*fp2,*fp3;
	char NewString[100], ChosenGrapheme2[8];
	char TempGlobalWord[100], bit1[30], bit2[30];
	char NewName[1000];
	int hasE, ok2;

	int  BigNumbers[50];
	int  UptoBig = 0;
	int  HasE = 0;
	int Espot;
	int g;
	char TemplateBit[15];

	char ScanWord[60],ScanInput[2000], ScanBit[50], thegraph[10], ScanWord2[60];
	int ScanCrap, ScanAnswer, ScanNumber, ScanSyll;
	float freqmod;
	char tempname[50];

	NewName[0] = 0;
	if(ENGLISH == 1) {
		strcat(NewName,"./EnglishRes/");
		strcat(NewName,FileName);
		fp1 = fopen(NewName,"r");
	} else if(ITALIAN == 1) {
		strcat(NewName,"./ItalianRes/");
		strcat(NewName,FileName);
		fp1 = fopen(NewName,"r");
	} else {
		strcat(NewName,"./GermanRes/");
		strcat(NewName,FileName);
		fp1 = fopen(NewName,"r");
	}

	
	
	fp2 = fopen("info.txt","a");
	fprintf(fp2,"%d %s \n",fp1,NewName);
	fclose(fp2);

	if(fp1==0) exit(1);

	totalerror = 0;


	//for(a=0;a<NSEGMENTATIONS;a++) {
	while( (fscanf(fp1,"%s",ScanWord)) != EOF) {

		ok2 = CheckOrths(ScanWord);
		if(ok2==-1) {
			fp3 = fopen("test12.3.txt","a");
			fprintf(fp3,"-1 %s\n",ScanWord);
			fclose(fp3);
		} else {
			for(i=0;i<strlen(ScanWord);i++) ScanWord2[i] = tolower(ScanWord[i]);
			ScanWord2[i] = 0;
			ResetAll();
			strcpy(SplitLetterWord, ScanWord2);
			ParseCodasFinalLetter = 1;
			ParseCodasTesting3();
		}
	}
	fclose(fp1);
}





void ZorziNet::PrintWordName(char *Name)
{
	FILE *fp;
	int i,j,k;
	
//if(ntempvowels == 2) {
	fp = fopen(Name,"a");
	//fprintf(fp,"%s %d %d  ",GlobalWord,ntempvowels, nphonvowels);
	if(ntempvowels == 1) fprintf(fp,"%s %d %d  ",GlobalWord,ntempvowels, nphonvowels);
	else fprintf(fp,"%s %d %d %d %d ",GlobalWord,ntempvowels, nphonvowels, ntempcodas[0],ntemponsets[1]);

	for(i=0;i<ntempvowels;i++) {
		fprintf(fp,"%d ",ntemponsets[i]);
		for(j=0;j<ntemponsets[i];j++) {
			fprintf(fp,"[o%d,%s %d] ",i,GraphemesList[temponsetsnum[i][j]], temponsetsnum[i][j]);
		}
		if(tempvowelsnum[i] == -1) {
			fprintf(fp,"[v%d,* %d] ",i,tempvowelsnum[i]);	
		}else {
			fprintf(fp,"[v%d,%s %d] ",i,GraphemesList[tempvowelsnum[i]], tempvowelsnum[i]);	
		}	
		fprintf(fp,"%d ",ntempcodas[i]);
		for(j=0;j<ntempcodas[i];j++) {
			fprintf(fp,"[c%d,%s %d] ",i,GraphemesList[tempcodasnum[i][j]], tempcodasnum[i][j]);
		}
	}
	fprintf(fp,"\n");
	fclose(fp);
//}
}


void ZorziNet::PrintWord()
{
	FILE *fp, *fp2;
	int i,j,k,max;
	//n1 = SumWeights[(NMARCOLETTERS * MAXSPOTSPERSYLL) + (i * NMARCOLETTERS) + (temponsetsnum[1][i])]; //coda 1
		
if(1==2){
	fp = fopen("WordInfo.txt","a");
	fprintf(fp,"%s %d %d  ",GlobalWord,ntempvowels, nphonvowels);
	if(ntempvowels>MAXSYLLABLES) max = ntempvowels;
	else max = MAXSYLLABLES;
	
	
	for(i=0;i<max;i++) {
		for(j=0;j<ntemponsets[i];j++) {
			if(i>=MAXSYLLABLES) {
				fp2=fopen("OutOnsets.txt","a");
				fprintf(fp2,"OB %s %d %d  %d %s %d 0 \n",GlobalWord,ntempvowels, nphonvowels,i,GraphemesList[temponsetsnum[i][j]], temponsetsnum[i][j]);					
				fclose(fp2);
			} //else {
				fprintf(fp,"[o%d,%s %d %3.3f] ",i,GraphemesList[temponsetsnum[i][j]], temponsetsnum[i][j],SumWeights[ i*(NMARCOLETTERS * MAXSPOTSPERSYLL) + (j * NMARCOLETTERS) + (temponsetsnum[i][j])] );
				if(SumWeights[ i*(NMARCOLETTERS * MAXSPOTSPERSYLL) + (j * NMARCOLETTERS) + (temponsetsnum[i][j])] < DeadWeightLevel) {
					fp2=fopen("DeadOnsets.txt","a");
					fprintf(fp2,"SY %s %d %d  %d %s %d %3.3f \n",GlobalWord,ntempvowels, nphonvowels,i,GraphemesList[temponsetsnum[i][j]], temponsetsnum[i][j],SumWeights[ i*(NMARCOLETTERS * MAXSPOTSPERSYLL) + (j * NMARCOLETTERS) + (temponsetsnum[i][j])] );					
					fclose(fp2);
				}
			//}
		}
		if(i<ntempvowels) {
			if(i>=MAXSYLLABLES) {
				fp2=fopen("OutVowels.txt","a");
				fprintf(fp2,"OB %s %d %d  %d %s %d \n",GlobalWord,ntempvowels, nphonvowels,i,GraphemesList[tempvowelsnum[i]], tempvowelsnum[i]);					
				fclose(fp2);
			} //else {
				if(tempvowelsnum[i] == -1) {
					fprintf(fp,"[v%d,* %d] ",i,tempvowelsnum[i]);	
				}else {
					fprintf(fp,"[v%d,%s %d %3.3f] ",i,GraphemesList[tempvowelsnum[i]], tempvowelsnum[i],SumWeights[ i*(NMARCOLETTERS * MAXSPOTSPERSYLL) + ((MAXONSETS) * NMARCOLETTERS) + (tempvowelsnum[i])] );	
					if(SumWeights[ i*(NMARCOLETTERS * MAXSPOTSPERSYLL) + ((MAXONSETS) * NMARCOLETTERS) + (tempvowelsnum[i])]   < DeadWeightLevel) {
						fp2=fopen("DeadVowels.txt","a");
						fprintf(fp2,"SY %s %d %d  %d %s %d %3.3f \n",GlobalWord,ntempvowels, nphonvowels,i,GraphemesList[tempvowelsnum[i]], tempvowelsnum[i],SumWeights[ i*(NMARCOLETTERS * MAXSPOTSPERSYLL) + ((MAXONSETS) * NMARCOLETTERS) + (tempvowelsnum[i])]  );					
						fclose(fp2);
					}
				}
			//}	
		}
		if(i>=MAXSYLLABLES) {
			fp2=fopen("OutCodas.txt","a");
			fprintf(fp2,"OB %s %d %d  %d %s %d \n",GlobalWord,ntempvowels,nphonvowels, i,GraphemesList[tempcodasnum[i][j]], tempcodasnum[i][j]);
			fclose(fp2);
		} //else {
			for(j=0;j<ntempcodas[i];j++) {
				fprintf(fp,"[c%d,%s %d %3.3f] ",i,GraphemesList[tempcodasnum[i][j]], tempcodasnum[i][j],SumWeights[ i*(NMARCOLETTERS * MAXSPOTSPERSYLL) + ((MAXONSETS + 1 + j) * NMARCOLETTERS) + (+ tempcodasnum[i][j])] );
				if(SumWeights[ i*(NMARCOLETTERS * MAXSPOTSPERSYLL) + ((MAXONSETS + 1 + j) * NMARCOLETTERS) + (+ tempcodasnum[i][j])] < DeadWeightLevel) {
					fp2=fopen("DeadCodas.txt","a");
					fprintf(fp2,"SY %s %d %d  %d %s %d %3.3f \n",GlobalWord,ntempvowels,nphonvowels, i,GraphemesList[tempcodasnum[i][j]], tempcodasnum[i][j],SumWeights[ i*(NMARCOLETTERS * MAXSPOTSPERSYLL) + ((MAXONSETS + 1 + j) * NMARCOLETTERS) + (+ tempcodasnum[i][j])] );
					fclose(fp2);
				}
			}
		//}
	}
	fprintf(fp,"\n");

	/*for(i=0;i<ntempvowels;i++) {
		fprintf(fp,"%d ",ntemponsets[i]);
		for(j=0;j<ntemponsets[i];j++) {
			fprintf(fp,"[o%d,%s %d] ",i,GraphemesList[temponsetsnum[i][j]], temponsetsnum[i][j]);
		}
		if(tempvowelsnum[i] == -1) {
			fprintf(fp,"[v%d,* %d] ",i,tempvowelsnum[i]);	
		}else {
			fprintf(fp,"[v%d,%s %d] ",i,GraphemesList[tempvowelsnum[i]], tempvowelsnum[i]);	
		}	
		fprintf(fp,"%d ",ntempcodas[i]);
		for(j=0;j<ntempcodas[i];j++) {
			fprintf(fp,"[c%d,%s %d] ",i,GraphemesList[tempcodasnum[i][j]], tempcodasnum[i][j]);
		}
	}
	fprintf(fp,"\n");*/




	fclose(fp);
}
}

void ZorziNet::RecreateNewWord()
{
	int i,j,k;

	AlignedInputs[0] = 0;

	for(i=0;i<ntempvowels;i++) {
		for(j=0;j<ntemponsets[i];j++) {
			strcat(AlignedInputs,temponsets[i][j]);
			strcat(AlignedInputs,".");
		}
		strcat(AlignedInputs,"+");
		strcat(AlignedInputs,tempvowels[i]);
		strcat(AlignedInputs,"+");
		for(j=0;j<ntempcodas[i];j++) {
			strcat(AlignedInputs,tempcodas[i][j]);
			strcat(AlignedInputs,".");
		}
		strcat(AlignedInputs,"+");
	}
	for(;i<MAXSYLLABLES;i++) {
		strcat(AlignedInputs,"+");
		strcat(AlignedInputs,"+");
		strcat(AlignedInputs,"+");
	}
}


void ZorziNet::StickCodasBack()
{
	int i,j;

	for(i=0;i<ntempvowels - 1;i++) {
		for(j=0;j<ntemponsets[i+1];j++) {
			strcpy(tempcodas[i][ntempcodas[i]],temponsets[i+1][j]);
			tempcodasnum[i][ntempcodas[i]] = temponsetsnum[i+1][j];
			ntempcodas[i]++;
		}
		ntemponsets[i+1] = 0;
	}
}


int ZorziNet::CountMultiGraphemes()
{
	int i;
	int count = 0;

	for(i=0;i<ntempvowels;i++) {
		if(strlen(tempvowels[i]) > 1) count++;
	}
	return(count);

}


void ZorziNet::SegmentWord(char *Word)
{
	char Onset[10];
	char OrthOnset[10];
	char bit[10];

	int i,j,k, donecoda;
	char vowel[10];
	char thevowels[10][7];
	int VowelNo = 0;
	char newword[100];
	char TempCod[50];

	FILE *fp;

/*	fp = fopen("wordinss.txt","a");
	fprintf(fp,"%s \n", Word);
	fclose(fp);*/

	OrthSegWord[0] = 0;
	newword[0] = 0;

	i=0;
	NOrthSylls = 0;
	while(i<strlen(Word) ) {		
		Onset[0] = 0;
		bit[0] = 0;
		if(i != strlen(Word)) {
			while( (IsVowel(Word[i]) == -1 && i < strlen(Word)) || (i==0 && Word[0] == 'y' /*&& (ENGLISH == 1 || GERMAN ==1)*/ )) {
				bit[0] = Word[i];
				bit[1] = 0;
				if(Word[i] == 'q' && Word[i+1] == 'u' ) {
					bit[1] = 'u';
					bit[2] = 0;
					i++;
				}
				if(FRENCH ==1) {
					if(Word[i] == 'g' && Word[i+1] == 'u' && IsVowel(Word[i+2]) != -1 ) {
						bit[1] = 'u';
						bit[2] = 0;
						i++;
					}
				}
				strcat(Onset, bit);
				i++;
				Onset[i] = 0;
				if(/*i == MAXORTHONSET || */ i > strlen(Word) ) break;
			 }

			strcpy(OrthOnset,Onset);
			//OrthOnset[i] = 0;
			strcat(newword,OrthOnset);
		}
		strcat(newword,".");
		
			
		/*	fp = fopen("wordinss111.txt","a");
			fprintf(fp,"* %s %s\n", Word,newword);
			fclose(fp);*/

		if(i!=strlen(Word) ) {
#if FRENCH
			for(j=0;j<NVOWELS-2;j++) {
#else
			for(j=0;j<NVOWELS;j++) {
#endif
				if( (strncmp(Word + i, VowelGraphemes[j], strlen(VowelGraphemes[j]))) ==0 ) {
					break;
				}
			}
#if FRENCH
			if(j==NVOWELS-2) {
#else
			if(j==NVOWELS) {
#endif
				vowel[0] = Word[i];
				vowel[1] = 0;
				strcpy(thevowels[VowelNo], vowel); 
				strcat(newword,vowel);
				strcat(newword,".");
				i++;
			} else {
				strcpy(vowel,VowelGraphemes[j]);
				strcpy(thevowels[VowelNo],vowel);
				i+= strlen(VowelGraphemes[j]);
				strcat(newword,VowelGraphemes[j]);
				strcat(newword,".");
			}
			VowelNo++;

			donecoda = 0;
			k = strlen(newword);
			while(i != strlen(Word) && (IsVowel(Word[i])) == -1) {
				newword[k] = Word[i];
				i++;
				k++;
				if(i>0){
					if(Word[i-1] == 'q' && Word[i] == 'u' ) {
						newword[k] = 'u';
						i++;
						k++;
					}
					if(FRENCH==1) {
						if(Word[i-1] == 'g' && Word[i] == 'u' && IsVowel(Word[i+1]) != -1  ) {
							newword[k] = 'u';
							i++;
							k++;
						}
					}
				}
			}
			newword[k] = '.';
			k++;
			newword[k] = 0;

			strcpy(OrthSegWord, newword);
/*			fp = fopen("wordinss111.txt","a");
			fprintf(fp,"%s %s\n", Word,OrthSegWord);
			fclose(fp);*/

		}
		NOrthSylls++;
	}
	j = strlen(newword);
	if(newword[j-1] == '.') newword[j-1] = 0;

	/*strcpy(OrthSegWord, newword);
	fp = fopen("wordinss111.txt","a");
	fprintf(fp,"%s %s\n", Word,OrthSegWord);
	fclose(fp);*/
}




void ZorziNet::ParseSegments()
{
	int UpTo, ItemCount,i,j,k, LetterSpot, SyllPos;
	FILE *fp1;

	TheBreak = 0;
	SyllPos = 0;//Onset
	UpTo = 0;
	ItemCount = 0;
	resplit = 0;

	UpToSyll = 0;

	ntempvowels = 0;
	for(i=0;i<6;i++) {
			ntempcodas[i] = 0;
			ntemponsets[i] = 0 ;
			ntempcodas2[i] = 0; 
			ntemponsets2[i] =0; 
	}


	/*fp1 =fopen("fuck.txt","a");
	fprintf(fp1,"%s \n",OrthSegWord);
	fclose(fp1);*/

	i= 0;
	while (i < strlen(OrthSegWord)) {		
/*		fp1 =fopen("fuck.txt","a");
		fprintf(fp1,"%s %d \n",OrthSegWord,SyllPos);
		fclose(fp1);*/

		if(OrthSegWord[i] == '.') {
			if(SyllPos == 0) {
				ItemCount = 0;
				SyllPos = 1;
			} else if (SyllPos == 1) { //Vowel				
				SyllPos = 2; 
				ItemCount = 0;
			} else { //CODA
				ItemCount = 0;
				SyllPos = 0;
				UpToSyll++;
			}
			i++;
		} else {
			/*fp1 =fopen("fuck.txt","a");
			fprintf(fp1,"--->%s %d \n",OrthSegWord,SyllPos);
			fclose(fp1);*/


			if(SyllPos == 0) {
			
			/*	if( (strncmp(OrthSegWord + i, "sch",3)) == 0 && (1 == 2)) {
					strcpy(temponsets[UpToSyll][ntemponsets[UpToSyll]],"s");
					temponsetsnum[UpToSyll][ntemponsets[UpToSyll]] = NONSETS+NVOWELS+NCODAS+FindLetter('s');
					ntemponsets[UpToSyll]++;
					strcpy(temponsets[UpToSyll][ntemponsets[UpToSyll]],"ch");
					temponsetsnum[UpToSyll][ntemponsets[UpToSyll]] = findGrapheme("ch");
					ntemponsets[UpToSyll]++;
					i+= 3;
				} else {*/
					for(j=0;j<NONSETS;j++) {
						if( (strncmp(OnsetGraphemes[j], OrthSegWord + i, strlen(OnsetGraphemes[j])))==0) {
							break;
						}
					}
					if(j==NONSETS) { //must be a letter
						if(ItemCount < 3) {
							LetterSpot = FindLetter(OrthSegWord[i]);
							temponsets[UpToSyll][ntemponsets[UpToSyll]][0] = OrthSegWord[i];
							temponsets[UpToSyll][ntemponsets[UpToSyll]][1] = 0;
							temponsetsnum[UpToSyll][ntemponsets[UpToSyll]] = NONSETS+NVOWELS+NCODAS+ LetterSpot; 
							ntemponsets[UpToSyll]++;
						}
						i++;
					} else {
						if(ItemCount < 3) {
							strcpy(temponsets[UpToSyll][ntemponsets[UpToSyll]], OnsetGraphemes[j]);
							temponsetsnum[UpToSyll][ntemponsets[UpToSyll]] = j; 
							ntemponsets[UpToSyll]++;	
						}
						i+= strlen(OnsetGraphemes[j]);
					}

				
				//}
				ItemCount++;
			} else if (SyllPos == 1) { //Vowel	
#if FRENCH
				for(j=0;j<NVOWELS-2;j++) {
#else
				for(j=0;j<NVOWELS;j++) {
#endif
					if( (strncmp(VowelGraphemes[j], OrthSegWord + i, strlen(VowelGraphemes[j])))==0){
						//	fp1 =fopen("fuck.txt","a");
						//	fprintf(fp1,"--->%s %d \n",VowelGraphemes[j],j);
						//	fclose(fp1);
							break;
					}
				}	
			
#if FRENCH
				if(j==NVOWELS-2) {
#else
				if(j==NVOWELS) {
#endif
					LetterSpot = FindLetter(OrthSegWord[i]);
					tempvowels[ntempvowels][0] = OrthSegWord[i];
					tempvowels[ntempvowels][1] = 0;
					tempvowelsnum[ntempvowels] = NONSETS+NVOWELS+NCODAS+ LetterSpot; 
					ntempvowels++;			
					i++;

					//fp1 =fopen("fuck.txt","a");
					//fprintf(fp1,"-++-->%s %s \n",tempvowels[ntempvowels-1]);
					//fclose(fp1);

				} else {		
					strcpy(tempvowels[UpToSyll], VowelGraphemes[j]);
					tempvowelsnum[ntempvowels] = NONSETS + j; 
					ntempvowels++;	
					i+= strlen(VowelGraphemes[j]);
				}					
				ItemCount = 0;
			} else {
				//CODAS //Need to work out what should be placed in onset/coda pos// Look for coda graphemes first, otherwise look for onsets

				
			/*	if( (strncmp(OrthSegWord + i, "sch",3)) == 0) {

					strcpy(tempcodas[UpToSyll][ntempcodas[UpToSyll]],"s");
					temponsetsnum[UpToSyll][ntempcodas[UpToSyll]] = NONSETS+NVOWELS+NCODAS+FindLetter('s');
					ntempcodas[UpToSyll]++;
					strcpy(tempcodas[UpToSyll][ntempcodas[UpToSyll]],"ch");
					tempcodasnum[UpToSyll][ntempcodas[UpToSyll]] = findGrapheme("ch");
					ntempcodas[UpToSyll]++;
					i+= 3;

				} else {	*/
					for(k=0;k<NONSETS;k++) {
						if( (strncmp(OnsetGraphemes[k], OrthSegWord + i, strlen(OnsetGraphemes[k]))) == 0){
							break;
						}
					}

					if(k==NONSETS) {
						for(j=0;j<NCODAS;j++) {

							if( (strncmp(CodaGraphemes[j], OrthSegWord + i, strlen(CodaGraphemes[j]))) == 0){
							/*	fp1 =fopen("fuck.txt","a");
								fprintf(fp1,"222-CG-->%s %s %d %d %d %d\n",OrthSegWord+i,CodaGraphemes[j],j, SyllPos,NONSETS,NCODAS);
								fclose(fp1);*/
										
								break;
							}
						}	
					}

	
					if(j == NCODAS && k == NONSETS) {  //CAN USE ONSET GRAPHEMES TOO
						/*fp1 =fopen("fuck.txt","a");
						fprintf(fp1,"shit0000---cod--->%s %d %d %d %d %c\n",OrthSegWord,SyllPos,ItemCount,k,j,OrthSegWord[i]);
						fclose(fp1);*/
						
						if(ItemCount < 7) {  //Should be up to 7. 
							LetterSpot = FindLetter(OrthSegWord[i]);			
							tempcodas[UpToSyll][ntempcodas[UpToSyll]][0] = OrthSegWord[i];
							tempcodas[UpToSyll][ntempcodas[UpToSyll]][1] = 0;
							tempcodasnum[UpToSyll][ntempcodas[UpToSyll]] = NONSETS+NVOWELS+NCODAS + LetterSpot; 
							ntempcodas[UpToSyll]++;
							
						}
						i++;
					} else {		
						if(k != NONSETS) { // Must be onsetgrapheme
			
							if(ItemCount < 7) {
								strcpy(tempcodas[UpToSyll][ntempcodas[UpToSyll]], OnsetGraphemes[k]);
								tempcodasnum[UpToSyll][ntempcodas[UpToSyll]] = k; 
								ntempcodas[UpToSyll]++;
							}
							i+= strlen(OnsetGraphemes[k]);
							
						} else {	
							if(ItemCount < 7) { //Must be coda graopheme
								strcpy(tempcodas[UpToSyll][ntempcodas[UpToSyll]], CodaGraphemes[j]);
								tempcodasnum[UpToSyll][ntempcodas[UpToSyll]] = /*NONSETS + NVOWELS + j*/findGrapheme(CodaGraphemes[j]); 
								ntempcodas[UpToSyll]++;					
							}
							i+= strlen(CodaGraphemes[j]);
						}
					}
			//	}
				ItemCount++;
			}	
			TheBreak = 0;
		}
	}

	//Exceptions.
	resplit = 0;
	//too many vowels.
	if (1==2 && (ntempvowels) == (GlobalNPhonSylls - 1) ) {
		for(i=0;i<ntempvowels;i++) {
			if(1 == 2 && ((strcmp(tempvowels[i],"ui")) == 0) || ((strcmp(tempvowels[i],"iu")) == 0) ||((strcmp(tempvowels[i],"io")) == 0) || ((strcmp(tempvowels[i],"ia")) == 0)|| ((strcmp(tempvowels[i],"iA")) == 0) || ((strcmp(tempvowels[i],"ua")) == 0)|| ((strcmp(tempvowels[i],"ao")) == 0) || ((strcmp(tempvowels[i],"uo")) == 0) || ((strcmp(tempvowels[i],"ai")) == 0) || ((strcmp(tempvowels[i],"ei")) == 0)|| ((strcmp(tempvowels[i],"ue")) == 0) || ((strcmp(tempvowels[i],"iE")) == 0) || ((strcmp(tempvowels[i],"iO")) == 0)|| ((strcmp(tempvowels[i],"ii")) == 0)) {
				fp1 = fopen("shittttt.txt","a");
				fprintf(fp1,"%d %d %s\n",ntempvowels, GlobalNPhonSylls - 1, OrthSegWord);
				fclose(fp1);
				
				for(j=ntempvowels;j>(i+1);j--) {
					strcpy(tempvowels[j], tempvowels[j-1]);
					tempvowelsnum[j] = tempvowelsnum[j-1];
				}
				tempvowels[i+1][0] = tempvowels[i][1];
				tempvowels[i+1][1] = 0;
				tempvowels[i][0] = tempvowels[i][0];
				tempvowels[i][1] = 0;

				tempvowelsnum[i] = (NGRAPHEMES) +  FindLetter(tempvowels[i][0]);
				tempvowelsnum[i+1] = (NGRAPHEMES) + FindLetter(tempvowels[i+1][0]);
						
				for(j=ntempvowels;j>(i);j--) {
					for(k=0;k<ntempcodas[j-1];k++) {
						strcpy(tempcodas[j][k],tempcodas[j-1][k]);
						tempcodasnum[j][k] = tempcodasnum[j-1][k];
						ntempcodas[j] = ntempcodas[j-1];
					}
				}
				ntempcodas[i] = 0;
				ntempvowels++;
				resplit = 1;
				break;
			}
		}
	}

	//fp1 =fopen("fuckout.txt","a");
		//	fprintf(fp1,"--->%s %d \n",OrthSegWord,SyllPos);
		//	fclose(fp1);
	PrintWord();
}




void ZorziNet::CheckMidE()
{

///////////MIDDLE E GRAPHEME SPLIT
			//char theletter; 
			//TA[1]P EW[2] OR[3] M.//STONEWALL = 4 //Graphemes incorrectly split across double vowels
			//Need to fix this so it doesn't delete middle 'e' vowels now!!!
			
			if(ntempvowels >= 3 && ntempcodas[1] == 0 && (strcmp(tempvowels[1], "e") == 0) ) {	
				/*fp1 = fopen("SplitEDiscarded2.txt","a");
				fprintf(fp1,"%s \n",Word);
				fclose(fp1);*/
				//return;
				ToTrain = 2;
			}
}

void ZorziNet::Check3TempVowels()
{
	int LetterSpot;

	if( ((strcmp(tempvowels[2],"e")) == 0) && ntempcodas[2] == 0) {
		/*	fp1 = fopen("threeE.txt","a");
			fprintf(fp1,"%s \n",Word);
			fclose(fp1);*/
			LetterSpot = FindLetter('e');			
			tempcodas[1][ntempcodas[1]][0] = 'e' ;
			tempcodas[1][ntempcodas[1]][1] = 0;
			tempcodasnum[1][ntempcodas[1]] = NONSETS+NVOWELS+NCODAS + LetterSpot; 
			ntempcodas[1]++;
			tempvowels[2][0] = 0;
			IsE2 = 1; 
			NOrthSylls--;
			ntempvowels--;
		} else if ( (strcmp(tempvowels[2],"e") == 0) && ntempcodas[2] == 1 && ( (tempcodas[2][0][0] == 's' || tempcodas[2][0][0] == 'd')) && strlen(tempcodas[2][0]) == 1 ) {
			LetterSpot = FindLetter('e');					
			tempcodas[1][ntempcodas[1]][0] = 'e'; 
			tempcodas[1][ntempcodas[1]][1] = 0;
			tempcodasnum[1][ntempcodas[1]] = NONSETS+NVOWELS+NCODAS + LetterSpot; 
			ntempcodas[1]++;

			if(tempcodas[2][0][0] == 'd') LetterSpot = FindLetter('d');				
			else if(tempcodas[2][0][0] == 's') LetterSpot = FindLetter('s');	
			strcpy(tempcodas[1][ntempcodas[1]],tempcodas[2][0]);
			tempcodasnum[1][ntempcodas[1]] = NONSETS+NVOWELS+NCODAS + LetterSpot; 
			ntempcodas[1]++;
			IsED2 = 1;IsE2 = 1;
			NOrthSylls--;
			ntempvowels--;
		}
}

void ZorziNet::Check4TempVowels()
{
	int LetterSpot;

		if( ((strcmp(tempvowels[3],"e")) == 0) && ntempcodas[3] == 0) {
		/*	fp1 = fopen("threeE.txt","a");
			fprintf(fp1,"%s \n",Word);
			fclose(fp1);*/
			LetterSpot = FindLetter('e');			
			tempcodas[2][ntempcodas[2]][0] = 'e' ;
			tempcodas[2][ntempcodas[2]][1] = 0;
			tempcodasnum[2][ntempcodas[1]] = NONSETS+NVOWELS+NCODAS + LetterSpot; 
			ntempcodas[2]++;
			tempvowels[3][0] = 0;
			IsE2 = 1; 
			NOrthSylls--;
			ntempvowels--;
		} else if ( (strcmp(tempvowels[3],"e") == 0) && ntempcodas[3] == 1 && ( (tempcodas[3][0][0] == 's' || tempcodas[3][0][0] == 'd')) && strlen(tempcodas[3][0]) == 1 ) {
			LetterSpot = FindLetter('e');					
			tempcodas[2][ntempcodas[2]][0] = 'e'; 
			tempcodas[2][ntempcodas[2]][1] = 0;
			tempcodasnum[2][ntempcodas[2]] = NONSETS+NVOWELS+NCODAS + LetterSpot; 
			ntempcodas[2]++;

			if(tempcodas[3][0][0] == 'd') LetterSpot = FindLetter('d');				
			else if(tempcodas[3][0][0] == 's') LetterSpot = FindLetter('s');	
			strcpy(tempcodas[2][ntempcodas[2]],tempcodas[3][0]);
			tempcodasnum[2][ntempcodas[2]] = NONSETS+NVOWELS+NCODAS + LetterSpot; 
			ntempcodas[2]++;
			IsED2 = 1;IsE2 = 1;
			NOrthSylls--;
			ntempvowels--;
		}
}

void ZorziNet::GetRidOfDoubleVowels()
{
	int a,j;
	FILE *fp1;

	a = strlen(tempvowels[0]);
	if(a > 1) {
		if(ConsonantNum(tempvowels[0][a-1]) != -1 /* && tempvowels[0][a-1] != 'w' */ && tempvowels[0][a-1] != 'y') {			
			fp1 = fopen("ConsonantsSplit.txt","a");
			fprintf(fp1,"%s \n",GlobalWord);
			fclose(fp1);
						
			ntemponsets[1] = 1;
			temponsets[1][0][0] = tempvowels[0][a-1];
			temponsets[1][0][1] = 0;
			temponsetsnum[1][0] = NONSETS+NVOWELS+NCODAS + FindLetter(temponsets[1][0][0]);
			ntempcodas[0] = 0;
					
			tempvowels[0][a-1] = 0;
			j = findVowelnum(tempvowels[0]);  
			if(j != -1) tempvowelsnum[0] = NONSETS + j;
			else tempvowelsnum[0] = NONSETS+NVOWELS+NCODAS + FindLetter(tempvowels[0][0]);						
			//NOrthSylls--;			
		}
	}
}

void ZorziNet::CopyInfoAcross()
{
	int h,i;
	FILE *fp;
	int max;

	if(ntempvowels > MAXSYLLABLES) max = ntempvowels;
	else max = MAXSYLLABLES;

	for(h=0;h<max;h++) {
		ntemponsets2[h] = ntemponsets[h];
		ntempcodas2[h] = ntempcodas[h];
		if(ntemponsets[h] >MAXONSETS) {
			fp = fopen("Overmaxonsets.txt","a");
			fprintf(fp,"%s %d \n",GlobalWord, max);
			fclose(fp);
		}
		for(i=0;i<ntemponsets[h] && i < MAXONSETS;i++) {
			strcpy(temponsets2[h][i], temponsets[h][i]);
			temponsetsnum2[h][i] = temponsetsnum[h][i];
		}
		strcpy(tempvowels2[h],tempvowels[h]);
		tempvowelsnum2[h] = tempvowelsnum[h];

		if(ntempcodas[h] >MAXCODAS) {
			fp = fopen("Overmaxcodas.txt","a");
			fprintf(fp,"%s %d \n",GlobalWord,max);
			fclose(fp);
		}
		for(i=0;i<ntempcodas[h] && i < MAXCODAS;i++) {
			strcpy(tempcodas2[h][i], tempcodas[h][i]);
			tempcodasnum2[h][i] = tempcodasnum[h][i];
		}
	}
	ntempvowels2 = ntempvowels;		
}

void ZorziNet::MakeNewInput()
{
	int	h,i,j,k,UpTo = 0;
	FILE *fp;

		for(h=0;h<MAXSYLLABLES;h++) {
			for(i=0;i<ntemponsets[h] && i < MAXONSETS;i++) {
				for(j=0;j<temponsetsnum[h][i];j++) {
					NewInput[UpTo] = 0; UpTo++;
				}
				NewInput[UpTo] = 1; UpTo++; j++;
				for(;j<NMARCOLETTERS;j++) {
					NewInput[UpTo] = 0; UpTo++;
				}
			}
			for(;i<MAXONSETS;i++) {
				for(j=0;j<NMARCOLETTERS;j++) {
					NewInput[UpTo] = 0; UpTo++; 
				}
			}

			if(h<ntempvowels) {
				if(tempvowelsnum[h] == NMARCOLETTERS || tempvowelsnum[h] == -1) {  //** NULL VOWELS
					for(i=0;i<NMARCOLETTERS;i++) {
						NewInput[UpTo] = 0; UpTo++;
					}
				} else {
					for(i=0;i<tempvowelsnum[h];i++) { //Vowel
						NewInput[UpTo] = 0; UpTo++;
					}
					NewInput[UpTo] = 1; UpTo++; i++;
					for(;i<NMARCOLETTERS;i++) {
						NewInput[UpTo] = 0; UpTo++;			
					}
				}
			} else {
				for(j=0;j<NMARCOLETTERS;j++) {
					NewInput[UpTo] = 0; UpTo++;
				}
			}
				
			for(i=0;i<ntempcodas[h] && i < MAXCODAS;i++) { //CODAS				
				for(j=0;j<tempcodasnum[h][i];j++) {
					NewInput[UpTo] = 0; UpTo++;
				}
				NewInput[UpTo] = 1; UpTo++;j++;
				for(;j<NMARCOLETTERS;j++) {
					NewInput[UpTo] = 0; UpTo++;
				}
			}	
			for(;i<MAXCODAS;i++) {
				for(j=0;j<NMARCOLETTERS;j++) {
					NewInput[UpTo] = 0; UpTo++; 
				}
			}
		}

		UpTo = 0;
/*		fp = fopen("lookylooky.txt","a");
		fprintf(fp,"%s %d\n",GlobalWord, ntempvowels);
		for(i=0;i<MAXSYLLABLES;i++) {
			fprintf(fp,"%d %d \n",ntemponsets[i],ntempcodas[i]);
			for(j=0;j<MAXSPOTSPERSYLL;j++) {
				fprintf(fp,"%d %d ",i,j);
				for(k=0;k<NMARCOLETTERS;k++) {
					fprintf(fp,"%d",NewInput[UpTo]);
					UpTo++;
				}
				fprintf(fp,"\n");
			}
		}
		fclose(fp);*/
}

int ZorziNet::TestSegment(char *pattern, int *memory, int maxmem)
{
	float inputs2[3000],sum, Delta, tempmax, maxorth,minorth, minavg,totalerror;
	FILE *fp1,*fp2;

	int spot,h,i,j,a,tempmaxplace;
	
	spot = 0;

	//if(maxmem == 0) inputs2[(NMARCOLETTERS) * 3 - 1] = 1;

	if(maxmem > 3) maxmem = 3;
	for(i=0;i<maxmem;i++) {
		for(j=0;j<NMARCOLETTERS2;j++) {
			if(memory[i] == j) inputs2[spot] = 1;
			else inputs2[spot] = 0;
			spot++;
		}
	}
	for(;i<3/*maxmem*/;i++) { //* THREE IS FOR MEMORY
		for(j=0;j<NMARCOLETTERS2;j++) {
			inputs2[spot] = 0;
			spot++;
		}
	}
	for(i=0;i<ATTENTIONWINDOWSIZE && i < strlen(pattern);i++) {
		a = FindLetter(pattern[i]);
		for(j=0;j<NLETTERS;j++) {
			if(a==j) inputs2[spot] = 1;
			else inputs2[spot] = 0;
			spot++;
		}
	}
	for(;i<ATTENTIONWINDOWSIZE;i++) {
		for(j=0;j<NLETTERS;j++) {
			inputs2[spot] = 0;
			spot++;
		}
	}
	
	SplitMaxPos = -1;
	tempmax = -1;
	for(h=0;h<SPLITNETSPOTS;h++) {
		SplitNetSums[h] = 0;
		for(i=0;i< NSEGINPUTS;i++) {
			if(inputs2[i] > .01) { //Speed
				SplitNetSums[h] += SplitNetWeights[i][h] * inputs2[i];			
			}
		}
		SplitNetActs[h] = ActFunc(SplitNetSums[h]);
		if(SplitNetActs[h] > tempmax) {
			SplitMaxPos = h;
			tempmax = SplitNetActs[h];
		}
		if(SplitNetActs[h] < .05) SplitNetActs[h] = 0;
	}	

	tempmax = -1;
	for(h=0;h<SPLITNETSPOTS;h++) {
		if(SplitNetActs[h] > tempmax) { /* must chosen only graphemes that match possible letters */
			if( (strncmp(pattern,GraphemesList[h],strlen(GraphemesList[h]))) == 0) {
				tempmax = SplitNetActs[h];
				tempmaxplace = h;
			}
		}
	}
	/*fp1 =fopen("res11.txt","a");
	fprintf(fp1,"%s %f %d %s\n",pattern, tempmax,tempmaxplace,GraphemesList[tempmaxplace]);
	for(i=0;i<maxmem;i++) fprintf(fp1,"%d %s  \n",memory[i],GraphemesList[memory[i]]);
	fprintf(fp1,"%d \n",NMARCOLETTERS2-1);

	spot = 0;
	for(i=0;i<3;i++) {
		for(j=0;j<NMARCOLETTERS2;j++) {
			fprintf(fp1,"%1.0f",inputs2[spot++]);
		}	
		fprintf(fp1,"\n");
	}	
	for(i=0;i<ATTENTIONWINDOWSIZE;i++) {
		for(j=0;j<NLETTERS;j++) {
			fprintf(fp1,"%1.0f",inputs2[spot++]);
		}
	}
	fprintf(fp1,"\n");
	fclose(fp1);*/

	return(tempmaxplace);
}

void ZorziNet::FixDeadNodes()
{
	int i,j,k,l;
	FILE *fp;

	
	if((AttentionWindow[0] == '+' || ntemponsets[ntempvowels] > 0) ) {
		//if(AttentionWindow[0] == '+') {		
		
			for(j=0;j<ntempcodas[(ntempvowels-1)];j++) {
				if(SumWeights[ (ntempvowels-1)*(NMARCOLETTERS * MAXSPOTSPERSYLL) + ((MAXONSETS + 1 + j) * NMARCOLETTERS) + ( tempcodasnum[(ntempvowels-1)][j])] < DeadWeightLevel) {
					break;
				}
			}
			if(j != ntempcodas[(ntempvowels-1)]) {
			
				for(k=0;k<ntempcodas[(ntempvowels-1)]-1;k++) {
					if( (strcmp(tempcodas[(ntempvowels-1)][k],"e") == 0)) {	
						ntemponsets[ntempvowels] = 0;
						ntemponsets[ntempvowels+1] = 0;
						for(l=0;l<k;l++) {
							strcpy(temponsets[(ntempvowels)][l],tempcodas[(ntempvowels-1)][l]);
							temponsetsnum[(ntempvowels)][l] = tempcodasnum[(ntempvowels-1)][l];
							ntemponsets[(ntempvowels)]++;
						}
						strcpy(tempvowels[(ntempvowels)],"e");
						tempvowelsnum[(ntempvowels)] = NGRAPHEMES + FindLetter('e');						
						ntempcodas[(ntempvowels)] = 0;
			
						fp = fopen("deadnodeEcorr1.txt","a");
						fprintf(fp,"%s %d %d %d %d %s\n",SplitLetterWord,k,l,ntempcodas[ntempvowels-1],ntempcodas[(ntempvowels-1)],tempcodas[(ntempvowels-1)][l+1]);
						fclose(fp);
						l++;
						for(;l<ntempcodas[(ntempvowels-1)];l++) {
							strcpy(tempcodas[(ntempvowels)][l-(k+1)], tempcodas[(ntempvowels-1)][l]);
							tempcodasnum[(ntempvowels)][l-(k+1)] = tempcodasnum[(ntempvowels-1)][l];
							ntempcodas[(ntempvowels)]++;
						}
						ntempvowels++;
						ntempcodas[(ntempvowels-2)] = 0;
						PrintWord();
						fp = fopen("deadnodeEcorr.txt","a");
						fprintf(fp,"%s %d %d %d\n",SplitLetterWord,k,l,ntempcodas[ntempvowels-1]);
						fclose(fp);
						break;
					}
				}
			}
		//}
	}
	




	//DANGLING CONSONANTS
	if(AttentionWindow[0] == '+') {
		if(ntemponsets[ntempvowels]>0) {
			for(i=0;i<ntemponsets[ntempvowels];i++) {
				strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]+i],temponsets[ntempvowels][i]);
				tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]+i] = temponsetsnum[ntempvowels][i];
				ntempcodas[ntempvowels-1]++;
				//PrintWord();
				fp = fopen("DanglingCons.txt","a");
				fprintf(fp,"%s \n",SplitLetterWord);
				fclose(fp);
			}
		}
		ntemponsets[ntempvowels] = 0;
	}
	PrintWord();
	if(ntempvowels == 3 && ntemponsets[2] == 0 && strcmp(tempvowels[2],"e") == 0) {
		strcpy(tempcodas[1][ntempcodas[1]],"e");
		ntempcodas[1]++;
		tempcodasnum[1][ntempcodas[1]-1] = NGRAPHEMES + FindLetter('e');
	}	
}


void ZorziNet::FixDeadCodas()
{
	float n1, s1, s2, s3;
	int i,j,k;
	FILE *fp;

	//for(i=0;i<(NMARCOLETTERS) * MAXPOS;i++) {
		
	i=0;
	if(ntemponsets[1] > 0) {
		do {
			n1 = SumWeights[(NMARCOLETTERS * MAXSPOTSPERSYLL) + (i * NMARCOLETTERS) + (temponsetsnum[1][i])]; //coda 1
			
			if(n1 < DeadWeightLevel) {
			//	s1 = SumWeights[(NMARCOLETTERS * ntempcodas[0]) + (temponsetsnum[1][i-1])];
				//check back if first spot, otherwise check onsets
				if(i==0) s1 = SumWeights[ ( (MAXONSETS+1) * NMARCOLETTERS) + (NMARCOLETTERS * ntempcodas[0]) + (temponsetsnum[1][i])];
				else s1 = SumWeights[(NMARCOLETTERS * MAXSPOTSPERSYLL) + ( (i-1) * NMARCOLETTERS) *  + (temponsetsnum[1][i-1])];


				fp = fopen("deads.txt","a");
				fprintf(fp,"%s %s %d %f %f\n",GlobalWord,GraphemesList[temponsetsnum[1][i]],i,n1,s1);
				fclose(fp);
		
				if(n1<s1) {
					strcpy(tempcodas[0][ntempcodas[0]],temponsets[1][0]);
					tempcodasnum[0][ntempcodas[0]] = temponsetsnum[1][0];
					ntempcodas[0]++;

					for(j=0;j<ntemponsets[1];j++) {
						strcpy(temponsets[1][j],temponsets[1][j+1]);
						temponsetsnum[1][j] = temponsetsnum[1][j+1];
					}
					ntemponsets[1]--;
					fp = fopen("deads.txt","a");
					fprintf(fp,"Fixed %s %s %d %f %f \n",GlobalWord,GraphemesList[temponsetsnum[1][i]],i,n1, s1);
					fclose(fp);
					i--;
				}	
			}
			i++;
		} while	(i<ntemponsets[1]);		
	}

	/*fp = fopen("deads2.txt","a");
	fprintf(fp,"in \n");
	fclose(fp);*/


	//	for(i=0;i<(NMARCOLETTERS) * MAXPOS;i++) {


	//n1 = SumWeights[(NMARCOLETTERS * (MAXONSETS+1)) + tempcodasnum2[h][i]]; //coda 1
	//s1 = SumWeights[(NMARCOLETTERS * (MAXSPOTSPERSYLL)) + tempcodasnum2[h][i]];
	//s2 = SumWeights[(NMARCOLETTERS * (MAXSPOTSPERSYLL + 1)) + tempcodasnum2[h][i+1]];
	//s3 = SumWeights[(NMARCOLETTERS * (MAXSPOTSPERSYLL + 2)) + tempcodasnum2[h][i+2]];
}


// THIS IS THE MAIN ROUTINE FOR PARSING THE CODAS. IT DOESN'T TRY AND ASSIGN THINGS UNTIL THE WINDOW IS FULL
void ZorziNet::ParseCodasTesting2()
{
	FILE *fp1;
	int h,i,j,k,len,len2,UpTo;
	int graphemeno;
	char thisgraph[10];
	float tempmax;
	int tempmaxplace,allowcodas;
	int LETTER_E = NONSETS+NVOWELS+NCODAS + FindLetter('E') + NMARCOLETTERS;


	//MakeOrthographicInput(SplitLetterWord);
	//ParseCodasTesting();
	//return;


	len = strlen(SplitLetterWord);

	/*fp1 = fopen("splitlets11.1.txt","a");
	fprintf(fp1,"%s %s %d %d \n",SplitLetterWord,AttentionWindow, len,ParseCodasFinalLetter);
	fclose(fp1);*/


	if(ParseCodasFinalLetter == 1) {
		//add last letter
		len2 = strlen(AttentionWindow);
		if(len2 != ATTENTIONWINDOWSIZE) {
			AttentionWindow[len2] = SplitLetterWord[len-1];
			AttentionWindow[len2+1] = 0;
		}
		for(i=strlen(AttentionWindow);i<ATTENTIONWINDOWSIZE;i++) {
			AttentionWindow[i] = '+';
		}
		AttentionWindow[ATTENTIONWINDOWSIZE] = 0;
		len = ATTENTIONWINDOWSIZE;
	}

	/*fp1 = fopen("splitlets.txt","a");
	fprintf(fp1,"%s %s %d \n",SplitLetterWord,AttentionWindow, len);
	fclose(fp1);*/


	if(AttentionWindow[0] != '+') {
		if(len < (ATTENTIONWINDOWSIZE)) { //Fill up Attentionwindow. Shuold always be filled right now.
			ParseCodasTestingUsed = 0;
			//NGraphemeMemory = 0;
			if(len > 0)	{
				for(i=0;i<strlen(SplitLetterWord);i++) {
					AttentionWindow[i] = SplitLetterWord[i];
				}
			}
//			AttentionWindow[len - 1] = SplitLetterWord[len -1];
			AttentionWindow[len] = 0;
/*			fp1 = fopen("test12.txt","a");
			fprintf(fp1,"%s %s %d \n",SplitLetterWord,AttentionWindow, len);
			fclose(fp1);*/
		} else { //okay filled
			i = strlen(AttentionWindow); 
			if(ParseCodasFinalLetter != 1) { //Can't add more letters if not there. 
				AttentionWindow[i] = SplitLetterWord[len - 1];
				AttentionWindow[i+1] = 0;
			} 		
			if(strlen(AttentionWindow) == ATTENTIONWINDOWSIZE) { //okay window is full		
				for(;;) {				
					if(NGraphemeMemory == 0) {//need to initialize blanks.
						GraphemeMemory[0] = (NMARCOLETTERS2 - 1);
					} 
					NGraphemeMemory++;

					graphemeno = TestSegment(AttentionWindow,GraphemeMemory,NGraphemeMemory);
					strcpy(thisgraph,GraphemesList[graphemeno]);

					if(NGraphemeMemory > 2) j = 2;
					else j = NGraphemeMemory;
					for(;j>0;j--) GraphemeMemory[j] = GraphemeMemory[j-1];
					GraphemeMemory[0] = graphemeno;	

					//force -e into second syll.
					//if(graphemeno == LETTER_E && ENGLISH ==1) graphemeno = NONSETS+NVOWELS+NCODAS+FindLetter('e');

					/*fp1 = fopen("test11.txt","a");
					fprintf(fp1,"%s %d %d %s %d %d\n",AttentionWindow,NGraphemeMemory, graphemeno, GraphemesList[graphemeno], ParseCodasLast,NMARCOLETTERS);
					tempmax = -1;
					 for(h=0;h<SPLITNETSPOTS;h++) {
						if(SplitNetActs[h] > tempmax) {
							tempmax = SplitNetActs[h];
							tempmaxplace = h;
						}
					//	fprintf(fp1,"%d %s %2.2f \n",i,GraphemesList[h],SplitNetActs[h]);
					}
					fprintf(fp1, "\n %d %s\n",tempmaxplace,GraphemesList[tempmaxplace] );	
					fclose(fp1);*/

					for(j=0;j<ATTENTIONWINDOWSIZE - strlen(GraphemesList[graphemeno]);j++) { //change window
						AttentionWindow[j] = AttentionWindow[j+strlen(GraphemesList[graphemeno])];
					}
					AttentionWindow[j] = 0;
					if(graphemeno >= NMARCOLETTERS) { //must be onset grapheme or LETTER_E includes single letters
						if( (graphemeno == LETTER_E && ENGLISH ==1)) {
							//|| \(FRENCH == 1 && (((IsVowel(thisgraph[0]) != -1 && strlen(thisgraph) == 1))|| (IsVowelGrapheme(thisgraph) != -1))) ) { 
							strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
							tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = LETTER_E-NMARCOLETTERS;
							ntempcodas[ntempvowels-1]++;
						} else {	
							//fp1 = fopen("test12.txt","a");
							//fprintf(fp1,"1 %s %s %d %s\n",SplitLetterWord,AttentionWindow, len, thisgraph);
							//fclose(fp1);
							if(ntempvowels == MAXSYLLABLES) {
								//don't go across 3 onset spots. change to coda
								fp1 = fopen("onset.3.txt","a");
								fprintf(fp1,"%s %s %d %s\n",SplitLetterWord,AttentionWindow, len, thisgraph);
								fclose(fp1);
								if(FRENCH == 1 && graphemeno >= NMARCOLETTERS * 2) {
									graphemeno -= NMARCOLETTERS * 2;
								} else {
									graphemeno -= NMARCOLETTERS;
								}
								strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
								tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = graphemeno;
								ntempcodas[ntempvowels-1]++;			
							} else {
								strcpy(temponsets[ntempvowels ][ntemponsets[ntempvowels ]],GraphemesList[graphemeno]);
								if(FRENCH == 1 && graphemeno >= NMARCOLETTERS * 2) {
									//tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = graphemeno - NMARCOLETTERS * 2;				
									//ntempcodas[ntempvowels-1]++;
									temponsetsnum[ntempvowels][ntemponsets[ntempvowels]] = graphemeno - NMARCOLETTERS * 2;	
									ntemponsets[ntempvowels]++;
									//ntempcodas[ntempvowels-1]++;

									fp1 = fopen("FRSpect.txt","a");	fprintf(fp1,"%d %d %s %s %d %s\n",NGRAPHEMES + FindLetter('e'),graphemeno - NMARCOLETTERS * 2, SplitLetterWord,AttentionWindow, len, thisgraph);	fclose(fp1);
								//	ParseCodasLast =1; /*stay in c	odas */				
								} else {
									temponsetsnum[ntempvowels][ntemponsets[ntempvowels]] = graphemeno - NMARCOLETTERS;				
									ntemponsets[ntempvowels]++;
									ParseCodasLast = 0; /*okay, can't go to codas until there is a vowel.*/		
								}
							}
						}
					} else if ( graphemeno < (NONSETS + NVOWELS) && graphemeno >= NONSETS ) { //okay multi lettervowels graphemes
						/*if(ntempvowels == 0) {
							strcpy(tempvowels[0],"X");
							tempvowelsnum[0] = NMARCOLETTERS-1; 
						}*/

						/*if(ntempvowels == MAXSYLLABLES) {
							fp1 = fopen("vowel.3.multi.txt","a");
							fprintf(fp1,"%s %s %d %s\n",SplitLetterWord,AttentionWindow, len, thisgraph);
							fclose(fp1);
							if(FRENCH == 1) {
								strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
								tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = graphemeno;
								ntempcodas[ntempvowels-1]++;
							}
						} else {*/
							strcpy(tempvowels[ntempvowels],GraphemesList[graphemeno]);
							tempvowelsnum[ntempvowels] = graphemeno;
							ntempvowels++;
							ParseCodasLast = 1;
						//}
					} else if (graphemeno < (NONSETS + NVOWELS + NCODAS)){	//codas//rotg.eap
						if(ParseCodasLast == 0) {
							//no non-vowels
						/*	strcpy(temponsets[ntempvowels][ntemponsets[ntempvowels]],GraphemesList[graphemeno]);
							temponsetsnum[ntempvowels][ntemponsets[ntempvowels]] = graphemeno;
							ntemponsets[ntempvowels]++;*/
							
							
							//This allows non-vowel sylls
							ParseCodasLast = 1;
							strcpy(tempcodas[ntempvowels][ntempcodas[ntempvowels]],GraphemesList[graphemeno]);
							tempcodasnum[ntempvowels][ntempcodas[ntempvowels]] = graphemeno;
							ntempcodas[ntempvowels]++;
							tempvowelsnum[ntempvowels] = /*NMARCOLETTERS*/ -1 ;
							ntempvowels++; /* okay no vowel coda */
						} else {
							strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
							tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = graphemeno;
							ntempcodas[ntempvowels-1]++;
						}
					} else if (graphemeno < NMARCOLETTERS) { //single CODA letters 
						//if(FRENCH == 1 && graphemeno > NMARCOLETTERS-NLETTERS) { //onset vowels				
						//} else {

						//should get here..!
#if ENGLISH					
							if(IsVowel(tolower(thisgraph[0])) != -1) { //is a vowel
#elif FRENCH 
							if(IsVowel(thisgraph[0]) != -1 &&  graphemeno < (NMARCOLETTERS) ) { //is a vowel
#else
							if(IsVowel(thisgraph[0]) != -1) { 
#endif 
								if(ENGLISH == 1) {
									if(ntempvowels!=MAXSYLLABLES || strcmp(GraphemesList[graphemeno],"e") != 0) {
										strcpy(tempvowels[ntempvowels],GraphemesList[graphemeno]);
										tempvowelsnum[ntempvowels] = graphemeno;
										ntempvowels++;
									} else {
										strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
										tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = LETTER_E-NMARCOLETTERS;
										ntempcodas[ntempvowels-1]++;
										fp1 = fopen("test12.efix.txt","a");
										fprintf(fp1,"3 %s %s %d %s tvn = %d\n",SplitLetterWord,AttentionWindow, len, thisgraph,ntempvowels);
										fclose(fp1);
									}
								} else {
									strcpy(tempvowels[ntempvowels],GraphemesList[graphemeno]);
									tempvowelsnum[ntempvowels] = graphemeno;
									ntempvowels++;
								}				

								ParseCodasLast = 1;
							} else { // must be a coda// can't be letter onset.
									//fp1 = fopen("test12.txt","a");
								//	fprintf(fp1,"2 %s %s %d %s\n",SplitLetterWord,AttentionWindow, len, thisgraph);
								//	fclose(fp1);
									//need to recategorize vowels back into the correct spot
								//if(FRENCH == 1 && IsVowel(thisgraph[0]) != -1) {
								//	if(graphemeno > NMARCOLETTERS * 2) graphemeno -= NMARCOLETTERS * 2;
								//	else graphemeno -= /*NLETTERS*/NMARCOLETTERS;
							//	}
								if(ParseCodasLast == 0) {
									/*strcpy(temponsets[ntempvowels][ntemponsets[ntempvowels]],GraphemesList[graphemeno]);
									temponsetsnum[ntempvowels][ntemponsets[ntempvowels]] = graphemeno;
									ntemponsets[ntempvowels]++;*/
							
									//allow no vowel sylls.
									ParseCodasLast = 1;
									strcpy(tempcodas[ntempvowels][ntempcodas[ntempvowels]],GraphemesList[graphemeno]);
									tempcodasnum[ntempvowels][ntempcodas[ntempvowels]] = graphemeno;
									ntempcodas[ntempvowels]++;
									tempvowelsnum[ntempvowels] = /*NMARCOLETTERS*/ -1;
									ntempvowels++; /* okay no vowel coda */
									//	if(ntempvowels == 0) ParseCodasLast = 1;
								} else {
									strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
									tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = graphemeno;
									ntempcodas[ntempvowels-1]++;
								}
							}
						//}	
					}			
					PrintWord();

					if(/*AttentionWindow[ATTENTIONWINDOWSIZE-1] == '+'*/ ParseCodasFinalLetter == 1) {
						if(AttentionWindow[0] == '+') break; //ok do all if at the end of a string.
						for(j=strlen(AttentionWindow);j<ATTENTIONWINDOWSIZE;j++) AttentionWindow[j] = '+';
						AttentionWindow[j] = 0;
					} else if (AttentionWindow[ATTENTIONWINDOWSIZE -1] != '+') {
						break;
					}
				} 
			}
			if(FRENCH ==1) {
				if(ntemponsets[ntempvowels] > 0 && AttentionWindow[0] == '+') {
					for(j=0;j<ntemponsets[ntempvowels];j++) {
						strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]+j],temponsets[ntempvowels][j]);
						tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]+j] = temponsetsnum[ntempvowels][j];
					}
					ntempcodas[ntempvowels-1]+=j;
					ntemponsets[ntempvowels] = 0;
					fp1 = fopen("TOFix.txt","a");	fprintf(fp1,"%s %s %d %s\n",SplitLetterWord,AttentionWindow, len, thisgraph);	fclose(fp1);			
				}
			}
			if(ENGLISH == 1 || GERMAN == 1) FixDeadCodas();
			if(ENGLISH == 1) FixDeadNodes();
			PrintWord();
			MakeNewInput(); 
		}
	

		UpTo = 0;
		for(i=0;i<MAXPOS;i++) {
			for(j=0;j<NMARCOLETTERS;j++) {
				Input[i][j] = NewInput[UpTo++];
			}
		}
	}
}


void ZorziNet::ParseCodasTesting4()
{
	FILE *fp1;
	int h,i,j,k,len,len2,UpTo;
	int graphemeno;
	char thisgraph[10];
	float tempmax;
	int tempmaxplace,allowcodas;
	int LETTER_E = NONSETS+NVOWELS+NCODAS + FindLetter('E') + NMARCOLETTERS;
	int movemem = 0;
	int first;

	int OldParseCodasLast = ParseCodasLast;
	int OldNGraphemeMemory = NGraphemeMemory;
	int OldGraphemeMemory[10];
	char OldAttentionWindow[10];

	for(j=0;j<3;j++) OldGraphemeMemory[j] = GraphemeMemory[j];
	strcpy(OldAttentionWindow,AttentionWindow);
	
	/*if(NGraphemeMemory > 2) j = 2; 
	else j = NGraphemeMemory;
	for(;j>0;j--) GraphemeMemory[j] = GraphemeMemory[j-1];
	GraphemeMemory[0] = graphemeno;	*/

	//MakeOrthographicInput(SplitLetterWord);
	//ParseCodasTesting();
	//return;
	CopyInfoAcross();

	len = strlen(SplitLetterWord);

	/*fp1 = fopen("splitlets11.1.txt","a");
	fprintf(fp1,"%s %s %d %d \n",SplitLetterWord,AttentionWindow, len,ParseCodasFinalLetter);
	fclose(fp1);*/

	if(ParseCodasFinalLetter == 1) {
		//add last letter
		len2 = strlen(AttentionWindow);
		if(len2 != ATTENTIONWINDOWSIZE) {
			AttentionWindow[len2] = SplitLetterWord[len-1];
			AttentionWindow[len2+1] = 0;
		}
		for(i=strlen(AttentionWindow);i<ATTENTIONWINDOWSIZE;i++) {
			AttentionWindow[i] = '+';
		}
		AttentionWindow[ATTENTIONWINDOWSIZE] = 0;
		len = ATTENTIONWINDOWSIZE;
		strcpy(OldAttentionWindow,AttentionWindow);
	}

	/*fp1 = fopen("splitlets.txt","a");
	fprintf(fp1,"%s %s %d \n",SplitLetterWord,AttentionWindow, len);
	fclose(fp1);
	*/
	first = 0;
	if(AttentionWindow[0] != '+') {
			i = strlen(AttentionWindow); 
			if(ParseCodasFinalLetter != 1) { //Can't add more letters if not there. 
				AttentionWindow[i] = SplitLetterWord[len - 1];
				AttentionWindow[i+1] = 0;
				strcpy(OldAttentionWindow,AttentionWindow);
			} 		
			//if(strlen(AttentionWindow) == ATTENTIONWINDOWSIZE) { //okay window is full		
		
				for(;;) {				
					if(NGraphemeMemory == 0) {//need to initialize blanks.
						GraphemeMemory[0] = (NMARCOLETTERS2 - 1);
					} 
					NGraphemeMemory++;

					//fp1 = fopen("test13.txt","a");fprintf(fp1,"%s %s %d \n",SplitLetterWord,AttentionWindow, len);	fclose(fp1);

					graphemeno = TestSegment(AttentionWindow,GraphemeMemory,NGraphemeMemory);
					strcpy(thisgraph,GraphemesList[graphemeno]);
					
					//fp1 = fopen("test13.txt","a");fprintf(fp1,"%d %s %s %s %d %d %d %d %d \n",graphemeno,SplitLetterWord,AttentionWindow, GraphemesList[graphemeno], len,ntempvowels, MAXSYLLABLES,ParseCodasLast,NMARCOLETTERS);	fclose(fp1);

					if( (strlen(AttentionWindow) == ATTENTIONWINDOWSIZE) && (first == 0) ) {
					//	fp1 = fopen("test14.txt","a");fprintf(fp1,"in=%d %s %s %s %d %d %d %d %d \n",graphemeno,SplitLetterWord,AttentionWindow, GraphemesList[graphemeno], len,ntempvowels, MAXSYLLABLES,ParseCodasLast,NMARCOLETTERS);	fclose(fp1);
						movemem = 1; //*move graphemes back but only if window is full.
						if(OldNGraphemeMemory > 2) j = 2; 
						else j = OldNGraphemeMemory;
						for(;j>0;j--) OldGraphemeMemory[j] = OldGraphemeMemory[j-1];
						OldGraphemeMemory[0] = graphemeno;	
						for(j=0;j<ATTENTIONWINDOWSIZE - strlen(GraphemesList[graphemeno]);j++) { //change window
							OldAttentionWindow[j] = OldAttentionWindow[j+strlen(GraphemesList[graphemeno])];
						}
						OldAttentionWindow[j] = 0;
					}
				
					if(NGraphemeMemory > 2) j = 2; 
					else j = NGraphemeMemory;
					for(;j>0;j--) GraphemeMemory[j] = GraphemeMemory[j-1];
					GraphemeMemory[0] = graphemeno;	

					for(j=0;j<ATTENTIONWINDOWSIZE - strlen(GraphemesList[graphemeno]);j++) { //change window
						AttentionWindow[j] = AttentionWindow[j+strlen(GraphemesList[graphemeno])];
					}
					AttentionWindow[j] = 0;
					//if(movemem ==1)	fp1 = fopen("test14.txt","a");fprintf(fp1,"in2=%d %s %s %s %d %d %d %d %d \n",graphemeno,SplitLetterWord,AttentionWindow, GraphemesList[graphemeno], len,ntempvowels, MAXSYLLABLES,ParseCodasLast,NMARCOLETTERS);	fclose(fp1);
					
					if(graphemeno >= NMARCOLETTERS) { //must be onset grapheme or LETTER_E includes single letters
						if(graphemeno == LETTER_E && ENGLISH ==1) { 
							if( (first == 0) && (movemem == 1) ) {
								strcpy(tempcodas2[ntempvowels2-1][ntempcodas2[ntempvowels2-1]],GraphemesList[graphemeno]);
								tempcodasnum2[ntempvowels2-1][ntempcodas2[ntempvowels2-1]] = LETTER_E-NMARCOLETTERS;
								ntempcodas2[ntempvowels2-1]++;
							}
							strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
							tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = LETTER_E-NMARCOLETTERS;
							ntempcodas[ntempvowels-1]++;
						} else {	
							if(ntempvowels == MAXSYLLABLES /*just add to syllas that don't eist 1 == 2*/) {
								//don't go across 3 onset spots. change to coda
									fp1 = fopen("onset.3.txt","a");
									fprintf(fp1,"%s %s %d \n",SplitLetterWord,AttentionWindow, len);
									fclose(fp1);
///////////////////////////////////////////////ADD THIS IF CODAS ARE TOO BE ADDED IN SYLLABLES THAT DON'T EXIST
								graphemeno -= NMARCOLETTERS;
								if( (first == 0) && (movemem == 1)) {
									strcpy(tempcodas2[ntempvowels2-1][ntempcodas2[ntempvowels2-1]],GraphemesList[graphemeno]);
									tempcodasnum2[ntempvowels2-1][ntempcodas2[ntempvowels2-1]] = graphemeno;
									ntempcodas2[ntempvowels2-1]++;
								}			
								strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
								tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = graphemeno;
								ntempcodas[ntempvowels-1]++;			
////////////////////////////////////////////////////////////////////////////////////////////////////////////							
							} else {
								//fp1 = fopen("test15.txt","a");fprintf(fp1,"%d %d--%d %s %s %s %d %d %d %d %d \n",first, movemem, graphemeno,SplitLetterWord,AttentionWindow, GraphemesList[graphemeno], len,ntempvowels, MAXSYLLABLES,ParseCodasLast,NMARCOLETTERS);	fclose(fp1);
								if( (first == 0) && (movemem == 1)) {
									strcpy(temponsets2[ntempvowels2 ][ntemponsets2[ntempvowels2]],GraphemesList[graphemeno]);
									temponsetsnum2[ntempvowels2][ntemponsets2[ntempvowels2]] = graphemeno - NMARCOLETTERS;
									ntemponsets2[ntempvowels2]++;
									OldParseCodasLast = 0;
								}
								strcpy(temponsets[ntempvowels][ntemponsets[ntempvowels]],GraphemesList[graphemeno]);
								temponsetsnum[ntempvowels][ntemponsets[ntempvowels]] = graphemeno - NMARCOLETTERS;
								ntemponsets[ntempvowels]++;
								//fp1 = fopen("test13.txt","a");fprintf(fp1,"--%d %d %s %s %s %d %d %d %d %d \n",ntemponsets[0],graphemeno,SplitLetterWord,AttentionWindow, GraphemesList[graphemeno], len,ntempvowels, MAXSYLLABLES,ParseCodasLast,NMARCOLETTERS);	fclose(fp1);
								ParseCodasLast = 0; /*okay, can't go to codas until there is a vowel.*/
								//PrintWord();
							}
						}
					} else if ( graphemeno < (NONSETS + NVOWELS) && graphemeno >= NONSETS ) { //okay multi lettervowels graphemes vowels
						/*if(ntempvowels == 0) {
							strcpy(tempvowels[0],"X");
							tempvowelsnum[0] = NMARCOLETTERS-1; 
						}*/
						if( (first == 0) && (movemem == 1) ) {
							strcpy(tempvowels2[ntempvowels2],GraphemesList[graphemeno]);
							tempvowelsnum2[ntempvowels2] = graphemeno;
							ntempvowels2++;
							OldParseCodasLast = 1;
						}	
						strcpy(tempvowels[ntempvowels],GraphemesList[graphemeno]);
						tempvowelsnum[ntempvowels] = graphemeno;
						ntempvowels++;
						ParseCodasLast = 1;
					} else if (graphemeno < (NONSETS + NVOWELS + NCODAS)){	//codas//rotg.eap
					
						//fp1 = fopen("test15.txt","a");fprintf(fp1,"cods=%d %d--%d %s %s %s %d %d %d %d %d \n",first, movemem, graphemeno,SplitLetterWord,AttentionWindow, GraphemesList[graphemeno], len,ntempvowels, MAXSYLLABLES,ParseCodasLast,NMARCOLETTERS);	fclose(fp1);	
						if(ParseCodasLast == 0) {
							//fp1 = fopen("test15.txt","a");fprintf(fp1,"cods2=%d %d--%d %s %s %s %d %d %d %d %d \n",first, movemem, graphemeno,SplitLetterWord,AttentionWindow, GraphemesList[graphemeno], len,ntempvowels, MAXSYLLABLES,ParseCodasLast,NMARCOLETTERS);	fclose(fp1);
							//no non-vowels
						/*	strcpy(temponsets[ntempvowels][ntemponsets[ntempvowels]],GraphemesList[graphemeno]);
							temponsetsnum[ntempvowels][ntemponsets[ntempvowels]] = graphemeno;
							ntemponsets[ntempvowels]++;*/			
							//This allows non-vowel sylls
							ParseCodasLast = 1;

							if( (first == 0) && (movemem == 1) ) {
								strcpy(tempcodas2[ntempvowels2][ntempcodas2[ntempvowels2]],GraphemesList[graphemeno]);
								tempcodasnum2[ntempvowels2][ntempcodas2[ntempvowels2]] = graphemeno;
								ntempcodas2[ntempvowels2]++;
								tempvowelsnum2[ntempvowels2] = /*NMARCOLETTERS*/ -1 ;
								ntempvowels2++; /* okay no vowel coda */
								OldParseCodasLast = 1;
							}
							strcpy(tempcodas[ntempvowels][ntempcodas[ntempvowels]],GraphemesList[graphemeno]);
							tempcodasnum[ntempvowels][ntempcodas[ntempvowels]] = graphemeno;
							ntempcodas[ntempvowels]++;
							tempvowelsnum[ntempvowels] = /*NMARCOLETTERS*/ -1 ;
							ntempvowels++; /* okay no vowel coda */
						} else {
						//	fp1 = fopen("test15.txt","a");fprintf(fp1,"cods3=%d %d--%d %s %s %s %d %d %d %d %d \n",first, movemem, graphemeno,SplitLetterWord,AttentionWindow, GraphemesList[graphemeno], len,ntempvowels, MAXSYLLABLES,ParseCodasLast,NMARCOLETTERS);	fclose(fp1);
							if( (first == 0) && (movemem == 1) ) {
								strcpy(tempcodas2[ntempvowels2-1][ntempcodas2[ntempvowels2-1]],GraphemesList[graphemeno]);
								tempcodasnum2[ntempvowels2-1][ntempcodas2[ntempvowels2-1]] = graphemeno;
								ntempcodas2[ntempvowels2-1]++;
							}
							strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
							tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = graphemeno;
							ntempcodas[ntempvowels-1]++;
						}
					} else if (graphemeno < NMARCOLETTERS) { //single CODA letters 		
						//should get here..!
#if ENGLISH					
						if(IsVowel(tolower(thisgraph[0])) != -1) { //is a vowel //*
#else
						if(IsVowel(thisgraph[0]) != -1) { //is a vowel
#endif 
							if( (first == 0) && (movemem == 1) ) {
								strcpy(tempvowels2[ntempvowels2],GraphemesList[graphemeno]);
								tempvowelsnum2[ntempvowels2] = graphemeno;
								ntempvowels2++;
								OldParseCodasLast = 1;
							}
							strcpy(tempvowels[ntempvowels],GraphemesList[graphemeno]);
							tempvowelsnum[ntempvowels] = graphemeno;
							ntempvowels++;
							ParseCodasLast = 1;
						} else { // must be a coda// can't be letter onset.
							if(ParseCodasLast == 0) {
								/*strcpy(temponsets[ntempvowels][ntemponsets[ntempvowels]],GraphemesList[graphemeno]);
								temponsetsnum[ntempvowels][ntemponsets[ntempvowels]] = graphemeno;
								ntemponsets[ntempvowels]++;*/		
								//allow no vowel sylls.
								if( (first == 0) && (movemem == 1) ) {
									strcpy(tempcodas2[ntempvowels2][ntempcodas2[ntempvowels2]],GraphemesList[graphemeno]);
									tempcodasnum2[ntempvowels2][ntempcodas2[ntempvowels2]] = graphemeno;
									
									ntempcodas2[ntempvowels2]++;
									tempvowelsnum2[ntempvowels2] = /*NMARCOLETTERS*/ -1;
									ntempvowels2++; /* okay no vowel coda */
									OldParseCodasLast = 1;
								}

								ParseCodasLast = 1;
								strcpy(tempcodas[ntempvowels][ntempcodas[ntempvowels]],GraphemesList[graphemeno]);
								tempcodasnum[ntempvowels][ntempcodas[ntempvowels]] = graphemeno;
								ntempcodas[ntempvowels]++;
								tempvowelsnum[ntempvowels] = /*NMARCOLETTERS*/ -1;
								ntempvowels++; /* okay no vowel coda */
								//	if(ntempvowels == 0) ParseCodasLast = 1;
							} else {
								if( (first == 0) && (movemem == 1) ) {
									strcpy(tempcodas2[ntempvowels2-1][ntempcodas2[ntempvowels2-1]],GraphemesList[graphemeno]);
									tempcodasnum2[ntempvowels2-1][ntempcodas2[ntempvowels2-1]] = graphemeno;
									ntempcodas2[ntempvowels2-1]++;		
								}
								strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
								tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = graphemeno;
								ntempcodas[ntempvowels-1]++;	
							}
						}		
					//	PrintWord();
					}			
					first = 1;

					if(strlen(AttentionWindow) == 0) break;
					//if(AttentionWindow[0] == '+') break;		
					if(ParseCodasFinalLetter == 1) {
						if(AttentionWindow[0] == '+') break; //ok do all if at the end of a string.
						for(j=strlen(AttentionWindow);j<ATTENTIONWINDOWSIZE;j++) AttentionWindow[j] = '+';
						AttentionWindow[j] = 0;
						strcpy(OldAttentionWindow,AttentionWindow);
						first = 0;
					} 		
				} 
		/*	}*/
			if(ENGLISH == 1 || GERMAN == 1) FixDeadCodas();
			if(ENGLISH == 1) FixDeadNodes();
			PrintWord();
			MakeNewInput(); 
		//}//
	

		UpTo = 0;
		for(i=0;i<MAXPOS;i++) {
			for(j=0;j<NMARCOLETTERS;j++) {
				Input[i][j] = NewInput[UpTo++];
			}
		}
	}

	//PrintWord();
	//if(movemem != 1) {
		//fp1 = fopen("test14.txt","a");fprintf(fp1,"%s %s %d \n",SplitLetterWord,AttentionWindow, len);	fclose(fp1);
		for(h=0;h<MAXSYLLABLES;h++) {
			ntemponsets[h] = ntemponsets2[h];
			ntempcodas[h] = ntempcodas2[h];
			for(i=0;i<ntemponsets2[h];i++) {
				strcpy(temponsets[h][i], temponsets2[h][i]);
				temponsetsnum[h][i] = temponsetsnum2[h][i];
			}
			strcpy(tempvowels[h],tempvowels2[h]);
			tempvowelsnum[h] = tempvowelsnum2[h];
			for(i=0;i<ntempcodas2[h];i++) {
				strcpy(tempcodas[h][i], tempcodas2[h][i]);
				tempcodasnum[h][i] = tempcodasnum2[h][i];
			}
		}
		ntempvowels = ntempvowels2;	
	//}
	ParseCodasLast = OldParseCodasLast;
	NGraphemeMemory = OldNGraphemeMemory;
	for(j=0;j<3;j++) GraphemeMemory[j] = OldGraphemeMemory[j];
	strcpy(AttentionWindow,OldAttentionWindow);
}

//* WARNING USED TO CHECK RUNNING ALL STIM ONLY!!!!
void ZorziNet::ParseCodasTesting3()
{
	FILE *fp1;
	int h,i,j,k,len,len2,UpTo;
	int graphemeno;
	char thisgraph[10];
	float tempmax;
	int tempmaxplace,allowcodas;
	int LETTER_E = NONSETS+NVOWELS+NCODAS + FindLetter('E') + NMARCOLETTERS;
	int UpToSplitWord = 0;

	//MakeOrthographicInput(SplitLetterWord);
	//ParseCodasTesting();
	//return;


	len = strlen(SplitLetterWord);

	/*fp1 = fopen("splitlets11.1.txt","a");
	fprintf(fp1,"%s %s %d %d \n",SplitLetterWord,AttentionWindow, len,ParseCodasFinalLetter);
	fclose(fp1);*/

	////
	strcpy(AttentionWindow, SplitLetterWord);
	/////

	if(ParseCodasFinalLetter == 1) {
		//add last letter
		len2 = strlen(AttentionWindow);
		if(len2 < ATTENTIONWINDOWSIZE) {
			UpToSplitWord = len2;
		} else {
			UpToSplitWord = ATTENTIONWINDOWSIZE;
		}
		for(i=strlen(AttentionWindow);i<ATTENTIONWINDOWSIZE;i++) {
			AttentionWindow[i] = '+';
		}
		AttentionWindow[ATTENTIONWINDOWSIZE] = 0;
		len = ATTENTIONWINDOWSIZE;
	}
	if(AttentionWindow[ATTENTIONWINDOWSIZE-1] != '+') ParseCodasFinalLetter = 0;

	/*fp1 = fopen("splitlets.txt","a");
	fprintf(fp1,"%s %s %d \n",SplitLetterWord,AttentionWindow, len);
	fclose(fp1);*/


	if(AttentionWindow[0] != '+') {
		if(len < (ATTENTIONWINDOWSIZE)) { //Fill up Attentionwindow. Shuold always be filled right now.
		} else { //okay filled
			if(strlen(AttentionWindow) == ATTENTIONWINDOWSIZE) { //okay window is full		
				for(;;) {	
					/*fp1 = fopen("test12.1.txt","a");
					fprintf(fp1,"%s %s %d \n",SplitLetterWord,AttentionWindow, len);
					fclose(fp1);*/

					if(NGraphemeMemory == 0) {//need to initialize blanks.
						GraphemeMemory[0] = (NMARCOLETTERS2 - 1);
					} 
					NGraphemeMemory++;

					graphemeno = TestSegment(AttentionWindow,GraphemeMemory,NGraphemeMemory);
					strcpy(thisgraph,GraphemesList[graphemeno]);

					if(NGraphemeMemory > 2) j = 2;
					else j = NGraphemeMemory;
					for(;j>0;j--) GraphemeMemory[j] = GraphemeMemory[j-1];
					GraphemeMemory[0] = graphemeno;	

					for(j=0;j<ATTENTIONWINDOWSIZE - strlen(GraphemesList[graphemeno]);j++) { //change window
						AttentionWindow[j] = AttentionWindow[j+strlen(GraphemesList[graphemeno])];
					}
					AttentionWindow[j] = 0;

					len=strlen(AttentionWindow);
					/*fp1 = fopen("test12.1.txt","a");
					fprintf(fp1,"[%d] %s %s %d \n",UpToSplitWord, SplitLetterWord,AttentionWindow, len);
					fclose(fp1);*/

					for(j=len;j<ATTENTIONWINDOWSIZE;j++) {
						if(UpToSplitWord < strlen(SplitLetterWord)) {
							AttentionWindow[j] = SplitLetterWord[UpToSplitWord];
							UpToSplitWord++;
						} else {
							AttentionWindow[j] = '+';
						}
					}

					/*fp1 = fopen("test12.1.txt","a");
					fprintf(fp1,"%s %s %d \n",SplitLetterWord,AttentionWindow, len);
					fclose(fp1);*/

					//REFUKK ATTENTION WINDOW

					if(graphemeno >= NMARCOLETTERS) { //must be onset grapheme or LETTER_E includes single letters
						if(graphemeno == LETTER_E && ENGLISH ==1) { 
							strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
							tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = LETTER_E-NMARCOLETTERS;
							ntempcodas[ntempvowels-1]++;
						} else {	
							if(/*ntempvowels == MAXSYLLABLES*/ 1==2) {
								//don't go across 3 onset spots. change to coda
									fp1 = fopen("onset.3.txt","a");
									fprintf(fp1,"%s %s %d \n",SplitLetterWord,AttentionWindow, len);
									fclose(fp1);
								graphemeno -= NMARCOLETTERS;
								strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
								tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = graphemeno;
								ntempcodas[ntempvowels-1]++;			
							} else {
								strcpy(temponsets[ntempvowels ][ntemponsets[ntempvowels ]],GraphemesList[graphemeno]);
								temponsetsnum[ntempvowels ][ntemponsets[ntempvowels]] = graphemeno - NMARCOLETTERS;
								ntemponsets[ntempvowels]++;
								ParseCodasLast = 0; /*okay, can't go to codas until there is a vowel.*/
							}
						}
					} else if ( graphemeno < (NONSETS + NVOWELS) && graphemeno >= NONSETS ) { //okay multi lettervowels graphemes
						/*if(ntempvowels == 0) {
							strcpy(tempvowels[0],"X");
							tempvowelsnum[0] = NMARCOLETTERS-1; 
						}*/
						strcpy(tempvowels[ntempvowels],GraphemesList[graphemeno]);
						tempvowelsnum[ntempvowels] = graphemeno;
						ntempvowels++;
						ParseCodasLast = 1;
					} else if (graphemeno < (NONSETS + NVOWELS + NCODAS)){	//codas//rotg.eap
						if(ParseCodasLast == 0) {
							//no non-vowels
						/*	strcpy(temponsets[ntempvowels][ntemponsets[ntempvowels]],GraphemesList[graphemeno]);
							temponsetsnum[ntempvowels][ntemponsets[ntempvowels]] = graphemeno;
							ntemponsets[ntempvowels]++;*/
										
							//This allows non-vowel sylls
							ParseCodasLast = 1;
							strcpy(tempcodas[ntempvowels][ntempcodas[ntempvowels]],GraphemesList[graphemeno]);
							tempcodasnum[ntempvowels][ntempcodas[ntempvowels]] = graphemeno;
							ntempcodas[ntempvowels]++;
							tempvowelsnum[ntempvowels] = /*NMARCOLETTERS*/ -1 ;
							ntempvowels++; /* okay no vowel coda */
						} else {
							strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
							tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = graphemeno;
							ntempcodas[ntempvowels-1]++;
						}
					} else if (graphemeno < NMARCOLETTERS) { //single CODA letters 		
						//should get here..!
#if ENGLISH					
						if(IsVowel(tolower(thisgraph[0])) != -1) { //is a vowel
#else
						if(IsVowel(thisgraph[0]) != -1) { //is a vowel
#endif 
							strcpy(tempvowels[ntempvowels],GraphemesList[graphemeno]);
							tempvowelsnum[ntempvowels] = graphemeno;
							ntempvowels++;
							ParseCodasLast = 1;
						} else { // must be a coda// can't be letter onset.
							if(ParseCodasLast == 0) {
								/*strcpy(temponsets[ntempvowels][ntemponsets[ntempvowels]],GraphemesList[graphemeno]);
								temponsetsnum[ntempvowels][ntemponsets[ntempvowels]] = graphemeno;
								ntemponsets[ntempvowels]++;*/
							
								//allow no vowel sylls.
								ParseCodasLast = 1;
								strcpy(tempcodas[ntempvowels][ntempcodas[ntempvowels]],GraphemesList[graphemeno]);
								tempcodasnum[ntempvowels][ntempcodas[ntempvowels]] = graphemeno;
								ntempcodas[ntempvowels]++;
								tempvowelsnum[ntempvowels] = /*NMARCOLETTERS*/ -1;
								ntempvowels++; /* okay no vowel coda */
								//	if(ntempvowels == 0) ParseCodasLast = 1;
							} else {
								strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],GraphemesList[graphemeno]);
								tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = graphemeno;
								ntempcodas[ntempvowels-1]++;	
							}
						}	
						
					}			
					//PrintWord();
					if(ntempvowels > 4) {
						fp1=fopen("over4.txt","a");
						fprintf(fp1,"%s \n",SplitLetterWord);
						fclose(fp1);
						ntempvowels = 4; //************ERROR TOO MANY SYLLABLES BLOCKS AT 4
						break;
					}
					if(AttentionWindow[ATTENTIONWINDOWSIZE-1] == '+') {
						if(AttentionWindow[0] == '+') break; //ok do all if at the end of a string.
						for(j=strlen(AttentionWindow);j<ATTENTIONWINDOWSIZE;j++) AttentionWindow[j] = '+';
						AttentionWindow[j] = 0;
					} //else if (AttentionWindow[ATTENTIONWINDOWSIZE -1] != '+') {
						//break;
					//}
					
				} 
			}

			//FixDeadCodas();
			//PrintWord();
			MakeNewInput(); 
		}
		UpTo = 0;
		for(i=0;i<MAXPOS;i++) {
			for(j=0;j<NMARCOLETTERS;j++) {
				Input[i][j] = NewInput[UpTo++];
			}
		}
	}
	fp1 = fopen("test12.3.txt","a");
	//fprintf(fp1,"%d \n",ntempvowels);
	fprintf(fp1,"%s ",SplitLetterWord);
	for(i=0;i<ntempvowels;i++) {
		for(j=0;j<ntemponsets[i];j++) {
			fprintf(fp1,"%s",temponsets[i][j]);
		}
		fprintf(fp1,"%s",tempvowels[i]);
		for(j=0;j<ntempcodas[i];j++) {
			fprintf(fp1,"%s",tempcodas[i][j]);
		}
		fprintf(fp1,".");
	}
	fprintf(fp1," ");
	for(i=0;i<ntempvowels-1;i++) {
		if(ntempcodas[i] > 0 && ntemponsets[j] == 0) {
			fprintf(fp1," 1\n");
			break;
		}
	}
	if(i==ntempvowels-1) {
		fprintf(fp1," 0\n");
	}
	fclose(fp1);
}




//make more efficient by not repeating.
void ZorziNet::ParseCodasTesting()
{
	FILE *fp1;
	float s1,s2,s3,n1;
	int DeadNodeLevel = 7,h,i,j,k;
	ToTrain = 0;
	HitDeadNode = 0;


	//Add first onsets. Can only be onsets.
	for(i=0;i<ntemponsets[0];i++) {
		strcpy(temponsets[0][i], temponsets2[0][i]);
		temponsetsnum[0][i] = temponsetsnum2[0][i];
		if(SumWeights[(NMARCOLETTERS * (i)) + temponsetsnum[0][i]] < DeadNodeLevel) {					
			HitDeadNode = 1;
			fp1=fopen("TestItDeadOnset.txt","a");
			fprintf(fp1,"%s %d %f \n",GlobalWord, i, SumWeights[(NMARCOLETTERS * (i)) + temponsetsnum[0][i]]);
			fclose(fp1);
		}
	}
	strcpy(tempvowels[0], tempvowels2[0]);
	tempvowelsnum[0] = tempvowelsnum2[0];
		

	if(SumWeights[(NMARCOLETTERS * MAXONSETS) + tempvowelsnum[0]] < DeadNodeLevel) {
		HitDeadNode = 2;
		fp1=fopen("TestItDeadVowel.txt","a");
		fprintf(fp1,"%s %d %f \n",GlobalWord, i, SumWeights[(NMARCOLETTERS * (3)) + tempvowelsnum[0]]);
		fclose(fp1);
	}

	for(h=0;h<MAXSYLLABLES && h < ntempvowels;h++) {
		if(h< (ntempvowels-1)) {
			for(i=0;i<ntempcodas[h];i++) {
				n1 = SumWeights[(NMARCOLETTERS * (MAXONSETS+1)) + tempcodasnum2[h][i]]; //coda 1
				s1 = SumWeights[(NMARCOLETTERS * (MAXSPOTSPERSYLL)) + tempcodasnum2[h][i]];
				s2 = SumWeights[(NMARCOLETTERS * (MAXSPOTSPERSYLL + 1)) + tempcodasnum2[h][i+1]];
				s3 = SumWeights[(NMARCOLETTERS * (MAXSPOTSPERSYLL + 2)) + tempcodasnum2[h][i+2]];
				
			//ALL this s1 > n1 should be deleted as it is worthless since if s1>then it won't shift back anyway.
				if(ntempcodas[h] - i == 2) { // works. Don't fuck with this.
					if((s1 > DeadNodeLevel || (s1 > n1)) && (s2 > DeadNodeLevel)) break ;
				} else if (ntempcodas[h] - i >= 3) {
					if( (s1 > DeadNodeLevel || (s1 > n1)) && s2 > DeadNodeLevel && s3 >DeadNodeLevel) break; 	
				} else {  //mt equal to 1.
					if((s1 > DeadNodeLevel  || (s1 > n1))) break;
				}
			}
		//	ntempcodas[h] = ntempcodas2[h] - i;
		//	ntemponsets[h+1] = i;

			ntemponsets[h+1] = ntempcodas2[h] - i;
			ntempcodas[h] = i;

		//	fp1=fopen("TestIt3.txt","a");
		//	fprintf(fp1,"%f %f %f %f%d %d %d %d %d %d\n",n1,s1,s2,s3,h,ntempvowels, ntempcodas2[h], i,ntempcodas[h],ntemponsets[h+1]);		
		//	fclose(fp1);

			for(i=0;i<ntempcodas[h];i++) {	
				strcpy(tempcodas[h][i], tempcodas2[h][i]);
				tempcodasnum[h][i] = tempcodasnum2[h][i];
				j++;
			}

		//	fp1=fopen("TestIt3.txt","a");
		//	fprintf(fp1,"%d %d %d %d %d %d\n",h,ntempvowels, ntempcodas2[h], i,ntempcodas[h],ntemponsets[h+1]);		
		//	fclose(fp1);
		
			j=0;
			for(;i<ntemponsets[h+1]+ntempcodas[h];i++) {	
				strcpy(temponsets[h+1][j], tempcodas2[h][i]);
				temponsetsnum[h+1][j] = tempcodasnum2[h][i];
				j++;
			}
		//	PrintWord();
		} else {				//add the rest.
			strcpy(tempvowels[h], tempvowels2[h]);
			tempvowelsnum[h] = tempvowelsnum2[h];
		//	if(SumWeights[((NMARCOLETTERS) * (MAXSPOTSPERSYLL) * h) + (MAXONSETS * NMARCOLETTERS) + tempvowelsnum2[h]] < DeadNodeLevel) {
				HitDeadNode = 5;
				fp1=fopen("TestItDeadVowel2.txt","a");
				fprintf(fp1,"%s %d %f \n",GlobalWord, i,SumWeights[((NMARCOLETTERS) * (MAXSPOTSPERSYLL) * h) + (MAXONSETS * NMARCOLETTERS) + tempvowelsnum2[h]]);
				fclose(fp1);
		//	}
			for(i=0;i<ntempcodas2[h];i++) {			
				strcpy(tempcodas[h][i], tempcodas2[h][i]);		
				tempcodasnum[h][i] = tempcodasnum2[h][i];
				ntempcodas[h] = ntempcodas2[1];
				if(SumWeights[(NMARCOLETTERS * MAXSPOTSPERSYLL*h) + ((MAXONSETS + 1) * NMARCOLETTERS) + tempcodasnum2[h][i]] < DeadNodeLevel) {
					HitDeadNode = 6;
					fp1=fopen("TestItDeadConsonant.txt","a");
					fprintf(fp1,"%s %d %f \n",GlobalWord, i, SumWeights[(NMARCOLETTERS) * (i + 12) + tempcodasnum2[1][i]]);
					fclose(fp1);
					
			/*		n1 = SumWeights[(NMARCOLETTERS * (i+(MAXONSETS + 1))) + tempcodasnum2[0][i]];
					s1 = SumWeights[(NMARCOLETTERS * MAXSPOTSPERSYLL) + tempcodasnum2[0][i]];
					
					if(n1 < DeadNodeLevel && n1 > s1) {
						HitDeadNode = 3;
						fp1 = fopen("TestItDeadCoda1.txt","a");
						fprintf(fp1,"%s %f %d %s %d %d\n",GlobalWord, SumWeights[(NMARCOLETTERS * (i+4)) + tempcodasnum2[0][i]], i, tempcodas2[0][i], tempcodasnum2[0][i], tempcodasnum[0][i]);
						fclose(fp1);
						break;
					}
					strcpy(tempcodas[0][i], tempcodas2[0][i]);
					tempcodasnum[0][i] = tempcodasnum2[0][i];*/ //Need to plonk the onset over 					
				}
			}
		}
	}


		//	fp1=fopen("TestIt4.txt","a");
		//	fprintf(fp1,"%s ntc0=%d nto1=%d ntc2=%d %d i=%d\n",GlobalWord, ntempcodas[0],ntemponsets[1],ntempcodas2[0], ntempvowels, i);		
		//	fclose(fp1);
		
			/////////////////////////////////////////////////////////////////////////

		//	ToTrain = 0
//	PrintWord();
}



void ZorziNet::AutoTrain()
{
	int i,j,k,l,a,thephoneme;
	FILE *fp1;

	foundit = 0;


			/*	fp1 = fopen("Test4.1.1.txt","a");
			fprintf(fp1,"%s %s \n",Word,ThisPhon);
			fclose(fp1);*/
			if(TheTrainingCycle == 1) FindBestPlace(); //HERE

			ntemponsets[0] = ntemponsets2[0];
			for(i=0;i<ntemponsets2[0];i++) {
				strcpy(temponsets[0][i], temponsets2[0][i]);
				temponsetsnum[0][i] = temponsetsnum2[0][i];
			}
			if(	ntemponsets2[1] == 1) { //Split Vowel case
				strcpy(temponsets[1][0],temponsets2[1][0]);
				temponsetsnum[1][0] = temponsetsnum2[1][0];				
				ntemponsets[1] = 1;
				ntempcodas[0] = 0;
				foundit = 1;

			/*	if(ntempcodas2[0] == 1) { //split grapheme case
					strcpy(tempcodas[0][0],tempcodas2[0][0]);
					tempcodasnum[0][0] = tempcodasnum2[0][0];				
					ntemponsets[1] = 1;
					ntempcodas[0] = 1;	
				}*/

				fp1 = fopen("SplitVowels.txt","a");	
				fprintf(fp1,"%s %s %s %d %d \n",ThisWord, tempvowels[0], temponsets[1][0], nphoncodas[0],nphononsets[1]);
				fclose(fp1);
				/*fprintf(fp1,"%d %d [%s] %s ntv=%d ntc0=%d ntc1=%d nto0=%d nto1=%d %d %d %d %d %d j=%d\n", TheTrainingCycle, i,Word,OrthSegWord, ntempvowels, ntempcodas[0],ntempcodas[1], ntemponsets[0], ntemponsets[1],  TheBreak, nphononsets[0],nphoncodas[0], nphononsets[1], nphoncodas[1],j);
				fprintf(fp1,"v1=%s ", tempvowels[0]);	for(k=0;k<ntempcodas[0];k++) {	fprintf(fp1,"c0.%d=%s ",k,tempcodas[0][k]);	}for(k=0;k<ntemponsets[1];k++) {	fprintf(fp1,"t1.%d=%s ",k,temponsets[1][k]);} fprintf(fp1,"v=%s ",tempvowels[1]);	for(k=0;k<ntempcodas[1];k++) {fprintf(fp1,"c1.%d=%s ",k,tempcodas[1][k]);}	fprintf(fp1,"v2=%s ",tempvowels[2]);for(k=0;k<ntempcodas[2];k++) {fprintf(fp1,"tp3=%s ",tempcodas[2][k]);}	fprintf(fp1,"\n"); fclose(fp1); */
			} else if(((ntempcodas[0] + ntemponsets[1] - IsE) == (nphoncodas[0] + nphononsets[1])) || nphonvowels == 1 && nphonvowels == ntempvowels) {
				//wheezes//wheezed
				fp1 = fopen("Normal.txt","a");
				fprintf(fp1,"%d %s ntc = 0 %d\n",TheTrainingCycle,ThisWord, ntempcodas[0]);
				fclose(fp1);
				
				//same same//
				if(ntempvowels == 1) {
					for(i=0;i<ntempcodas[0];i++) {
						strcpy(tempcodas[0][i],tempcodas2[0][i]);
						tempcodasnum[0][i] = tempcodasnum2[0][i];					
					}
				} else {
					ntempcodas[0] = nphoncodas[0] + IsE;
					for(i=0;i<ntempcodas[0];i++) {
						strcpy(tempcodas[0][i],tempcodas2[0][i]);
						tempcodasnum[0][i] = tempcodasnum2[0][i];			
					}
					ntemponsets[1] = ntempcodas2[0] - ntempcodas[0];
					for(i=0;i<ntemponsets[1];i++) {
						strcpy(temponsets[1][i], tempcodas2[0][i+ntempcodas[0]]);
						temponsetsnum[1][i] = tempcodasnum2[0][i+ntempcodas[0]];
					}
				}

				// For double vowels//SHOULD NOT EVER GET HERE
		
				foundit = 1;
			} else {
				//different
				foundit = 0;
				if(nphonvowels != ntempvowels) {
					fp1 = fopen("difvowels2.txt","a");
					fprintf(fp1,"%s \n",ThisWord);
					fclose(fp1);
					return;
				} else if( (ntempcodas[0] - IsE) > nphoncodas[0] + nphononsets[1]) {
					fp1 = fopen("greaterthan.txt","a");
					fprintf(fp1,"%s \n",ThisWord);
					fclose(fp1);
				} else {
					fp1 = fopen("lessthan.txt","a");
					fprintf(fp1,"%s \n",ThisWord);
					fclose(fp1);
				}


				if(nphonvowels == 2) {
					if(ntempcodas2[0] != 0 ) { //Try and find the middle.
						a = 1;	
						thephoneme = PhonemeIndex(phononsets[1][0]);
						for(j=ntempcodas2[0] - 1; j >= 0;j--) { //Orthographic places.					
							if(Weights[(NMARCOLETTERS * 8) + tempcodasnum2[0][j]][(8 * NUMBER_OF_PHONEMES) + thephoneme] > 0.7) {																	
								foundit = 1;
							/*	fp1 = fopen("tc6.2.txt","a");
								fprintf(fp1,"%d %s %d \n", TheTrainingCycle, Word, nphonvowels);
								fclose(fp1);*/
								break;
							}
						}
						if(j==-1) {
							fp1 = fopen("didnotfind.txt","a");
							fprintf(fp1,"%s \n",ThisWord);
							fclose(fp1);
						}		
					} else { //These are crappy shit.
						foundit = 1;
						fp1 = fopen("shitty11.txt","a");
						fprintf(fp1,"%d %s %d \n", TheTrainingCycle, ThisWord, nphonvowels);
						fclose(fp1);
						j=0;
						return;
					}
				
					if(foundit == 1) {
						for(k=0;k<j;k++) {
							strcpy(tempcodas[0][k],tempcodas2[0][k]);
							tempcodasnum[0][k] = tempcodasnum2[0][k/*+ntempcodas[0]*/];
						}
						for(;k<ntempcodas2[0];k++) {
							strcpy(temponsets[1][k-j],tempcodas2[0][k]);
							temponsetsnum[1][k-j] = tempcodasnum2[0][k];
						}
						ntempcodas[0] = j;
						ntemponsets[1] = k-j;
					//	fp1 = fopen("founds11.txt","a");	fprintf(fp1," %d %d [%s] %s ntv=%d ntc0=%d ntc1=%d nto0=%d nto1=%d %d %d %d %d %d j=%d\n",  TheTrainingCycle, i,Word,OrthSegWord, ntempvowels, ntempcodas[0],ntempcodas[1], ntemponsets[0], ntemponsets[1],  TheBreak, nphononsets[0],nphoncodas[0], nphononsets[1], nphoncodas[1],j);
					//	fprintf(fp1,"v1=%s ", tempvowels[0]);	for(k=0;k<ntempcodas[0];k++) {	fprintf(fp1,"c0.%d=%s ",k,tempcodas[0][k]);	}for(k=0;k<ntemponsets[1];k++) {	fprintf(fp1,"t1.%d=%s ",k,temponsets[1][k]);} fprintf(fp1,"v=%s ",tempvowels[1]);	for(k=0;k<ntempcodas[1];k++) {fprintf(fp1,"c1.%d=%s ",k,tempcodas[1][k]);}	fprintf(fp1,"v2=%s ",tempvowels[2]);for(k=0;k<ntempcodas[2];k++) {fprintf(fp1,"tp3=%s ",tempcodas[2][k]);}	fprintf(fp1,"\n"); fclose(fp1);
				
					} else if (ntempvowels != nphonvowels) {
						fp1 = fopen("DifNums2222.txt","a");
						fprintf(fp1,"%d %s %d \n", TheTrainingCycle, ThisWord, nphonvowels);
						fclose(fp1);
						return;
						
					} else if (nphoncodas[0] == 0 && nphononsets[1] == 0) { //nothing
						for(k=0;k<ntempcodas2[0];k++) { //copy codas across
							strcpy(tempcodas[0][k],tempcodas2[0][k/*i*/]);
							tempcodasnum[0][k] = tempcodasnum2[0][k+ntempcodas[0]];
						}
						ntempcodas[0] = ntempcodas2[0];
						foundit = 1;
						j = 0;
			
						fp1 = fopen("NoCodNoOns.txt","a");
						fprintf(fp1,"%d %s %d \n", TheTrainingCycle, ThisWord, nphonvowels);
						fclose(fp1);
					} else if(nphononsets[1] == 0) { //no onsets.copy across graphemes.
						for(j=0;j<ntempcodas2[0];j++) {
							strcpy(tempcodas[0][j], tempcodas2[0][j]);
							tempcodasnum[0][j] = tempcodasnum2[0][j];
						}
						ntempcodas[0] = ntempcodas2[0];
						fp1 = fopen("nophononsets.txt","a");
						fprintf(fp1,"%d %s \n",TheTrainingCycle, ThisWord);
						fclose(fp1);
						foundit = 1;

						//// The middle has not been found. 
					}  else if(nphononsets[1] > 0) { // Look for first phoneme
						//Look backwards												
						//impune sextet express
						for(j=nphononsets[1] - 1; j>= 0;j--) {
							thephoneme = PhonemeIndex(phononsets[1][j]);
							for(k=ntempcodas2[0]-1;k>=0;k--) {
								if(Weights[(NMARCOLETTERS * (8+j)) + tempcodasnum2[0][k]][( (8 + j) * NUMBER_OF_PHONEMES) + thephoneme] > 0.7) {																	
									foundit = 1;
									break;
								}
							}
							if(foundit ==1) break;
						}
						int temponsetn = 0, ts1,ts2;

						if(foundit == 1) { // found an onset
							//sextant 1, 1, 2
							//highbrow 2,1,3.k,j,3
							ts1 = k-j;			
						
							ntemponsets[1] = ntempcodas[0]-ts1; // 
						
							for(l=ts1;l<ntempcodas2[0];l++) {
								strcpy(temponsets[1][l-ts1], tempcodas2[0][l]);
								temponsetsnum[1][l-ts1] = tempcodasnum2[0][l];
							}

							ntempcodas[0] = ntempcodas2[0] - ntemponsets[1];
							for(l=0;l<ts1;l++) {
								strcpy(tempcodas[0][l], tempcodas2[0][l]);
								tempcodasnum[0][l] = tempcodasnum2[0][l];
							}
							
							fp1 = fopen("tc7.found.txt","a");
							fprintf(fp1,"%d %s %d %d %d\n",TheTrainingCycle, ThisWord,k,j,ntempcodas2[0]);
							fclose(fp1);
						} else {
							if(nphoncodas[0] == 0) { //just onsets.
								for(l=0;l<ntempcodas[0];l++) {
									strcpy(temponsets[1][l], tempcodas2[0][l]);
									temponsetsnum[1][l] = tempcodasnum2[0][l];
								}
								ntempcodas[0] = 0;
								ntemponsets[1] = ntempcodas2[0];
								foundit = 1;
											
								fp1 = fopen("tc8.guess.txt","a");
								fprintf(fp1,"%d %s %d %s %s\n",TheTrainingCycle, ThisWord, ThisFreq, ThisPhon, PhonString);
								fclose(fp1);
								return;

							} else {	

								fp1 = fopen("Onestoboot.txt","a");
								fprintf(fp1,"%d %s %d %s %s %d %d %d %d %d\n", TheTrainingCycle, ThisWord, ThisFreq, ThisPhon, PhonString, nphonvowels, ntempcodas[0], ntemponsets[1], nphoncodas[0], nphononsets[1]);
								fclose(fp1);


								//fp1 = fopen("tc7.e.txt","a");
								//fprintf(fp1,"%d %s \n",TheTrainingCycle, Word);
								//fclose(fp1);
								return;
							}
						}
				
					} else {
						fp1 = fopen("notfounds11.txt","a");	fprintf(fp1," %d %d [%s] %s ntv=%d ntc0=%d ntc1=%d nto0=%d nto1=%d %d %d %d %d %d \n", TheTrainingCycle, i,ThisWord,OrthSegWord, ntempvowels, ntempcodas[0],ntempcodas[1], ntemponsets[0], ntemponsets[1],  TheBreak, nphononsets[0],nphoncodas[0], nphononsets[1], nphoncodas[1]);
						fprintf(fp1,"v1=%s ", tempvowels[0]);	for(k=0;k<ntempcodas[0];k++) {	fprintf(fp1,"c0.%d=%s ",k,tempcodas[0][k]);	}for(k=0;k<ntemponsets[1];k++) {	fprintf(fp1,"t1.%d=%s ",k,temponsets[1][k]);} fprintf(fp1,"v=%s ",tempvowels[1]);	for(k=0;k<ntempcodas[1];k++) {fprintf(fp1,"c1.%d=%s ",k,tempcodas[1][k]);}	fprintf(fp1,"v2=%s ",tempvowels[2]);for(k=0;k<ntempcodas[2];k++) {fprintf(fp1,"tp3=%s ",tempcodas[2][k]);}	fprintf(fp1,"\n"); fclose(fp1);
						fp1 = fopen("notfoundslist.txt","a");
						fprintf(fp1,"%d %s \n", TheTrainingCycle, ThisWord);
						fclose(fp1);
						return;
					}
				} else { // must be 1.
					//ok, just pile up the consonants, its a monosyllable//should not get here.
				
					fp1 = fopen("consonantpile.txt","a");
					fprintf(fp1,"%d %s %d\n",TheTrainingCycle, ThisWord, nphonvowels);
					fclose(fp1);
					
					foundit = 1;
					ntempcodas[0] = ntempcodas2[0];
					for(j=0;j<ntempcodas[0];j++) {
						strcpy(tempcodas[0][j],tempcodas2[0][j]);
						tempcodasnum[0][j] = tempcodasnum2[0][j];			
					}
					ToTrain = 0;
				}
			}	
}

void ZorziNet::DetermineNumberofSyllables()
{
	int changeit1 = 0,l;

	if(  ( /*ntempcodas[0] > 0 &&*/ ntempvowels == 2)  ) {   
		//HERE IS WHERE WE DECIDED WHERE -ED, -E, and the like should go. MARKER 1. 
		if( ( (strcmp(tempvowels[1],"e")) == 0 || (strcmp(tempvowels[1],"ue")) == 0) && ntempcodas[1] == 0) {//final e.
			if(InTraining == 1) {
				if(ThisNumSylls == 1) { //stick the vowels back.
					ntempvowels--;
					strcpy(tempcodas[0][ntempcodas[0]], tempvowels[1]);
					tempcodasnum[0][ntempcodas[0]] = tempvowelsnum[1];
					ntempcodas[0]++;
					TheBreak++;
					IsE = 1;
				}
			} else { //Running mode.
				l = ntempcodas[0];
				int changeit = 0;	

				if(SplitNetActs[2] > SplitESyllablePlaceVal) changeit = 0; //stickbak
				else changeit = 1;
	
				if(changeit == 0) {
					ntempvowels--;
					strcpy(tempcodas[0][ntempcodas[0]], tempvowels[1]);
					tempcodasnum[0][ntempcodas[0]] = tempvowelsnum[1];
					ntempcodas[0]++;
					TheBreak++;
					IsE = 1;
				}
			}
		} else if( ntempcodas[1] == 1 &&
				( (strcmp(tempvowels[1], "e")) == 0 || (strcmp(tempvowels[1],"ue")) == 0) && 
				( (strcmp(tempcodas[1][ntempcodas[1]-1],"d")) == 0 ||  (strcmp(tempcodas[1][ntempcodas[1]-1],"s")) == 0) )  {											
						//es and ed words.
					
				
			if(/*ntempcodas[0] > 0*/1==1) {	//facedd, bigbed, sifted, breastfed. plagued
				if(InTraining == 0) {
					//Not in training
					if(ntempcodas[0] > 0) {		
					//	SplitForwardTrigrams(Word);//SHOULD BE DONE ONCE AT THE START.
						strcpy(GlobalWord,ThisWord);
						
						if(SplitNetActs[2] > SplitESyllablePlaceVal) changeit1 = 0; //stick bak
						else changeit1 = 1;				
					}
				} else {
					if(ThisNumSylls == 2) {
						changeit1 = 1;
					}
				}
			
				if(changeit1 == 0) {
					l = ntempcodas[0];
					ntempvowels--;	
					strcpy(tempcodas[0][l], tempvowels[1]);
					tempcodasnum[0][l] = tempvowelsnum[1];
					
					if((strcmp(tempcodas[1][ntempcodas[1]-1],"d")) == 0) strcpy(tempcodas[0][l+1],"d");
					else strcpy(tempcodas[0][l+1],"s");
	
					tempcodasnum[0][l+/*2*/1] = tempcodasnum[1][0];
							
					ntempcodas[0] += 2;
					ntempcodas[1] = 0;
					TheBreak+=2;

					IsE = 1;
					IsED = 1;
				} else {
					/*fp1 = fopen("notchanged.txt","a");
					fprintf(fp1,"%s \n",Word);
					fclose(fp1);*/
				}
			}
		} 
	}

}

void ZorziNet::ResetInputs() {
	int i,j;
	
	for(i=0;i<MAXPOS;i++) {
		for(j=0;j< NMARCOLETTERS;j++) {
			Input[i][j] = 0;
		}
	}

	for(i=0;i<MAXPHONE;i++) {
		for(j=0;j<(NUMBER_OF_PHONEMES );j++) {
			Output[i][j] = 0;
			ErrorBuffer[i][j] = 0;
		}
	}

	//ErrorBuffer[MAXPHONE][NUMBER_OF_PHONEMES ]
	OldLen = -1;
	
}


void ZorziNet::ResetAll()
{
	int i,j;

	for(i=0;i</*MAXSYLLABLES*/4;i++) {
		ntempcodas[i] = 0; 
		ntemponsets[i]= 0; 
		ntempcodas2[i] = 0; 
		ntemponsets2[i]= 0; 
	}
	ntempvowels = 0; ntempvowels2 = 0;
	ParseCodasUpTo = 0;
	ParseCodasLast = 0;
	ParseCodasFinalLetter = 0;
	ParseCodaOldNLets = 0;
	AttentionWindow[0] = 0;
	
	for(i=0;i<MAXPOS * NMARCOLETTERS ;i++) NewInput[i] = 0;
	for(i=0;i<MAXPHONE;i++) {
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			Output[i][j] = 0;
		}
	}
	ParseCodasFinalLetter = 0;
	NGraphemeMemory = 0;
	AttentionWindow[0] = 0;
	SplitLetterWord[0] = 0;

	ParseCodasFinalLetter = 0;
	ParseCodasLast = 0;
}


void ZorziNet::MakeOrthographicInput(char *Word)
{
//This function should be re-written. 

    int h,i,j,k,l,m,o,b, UpTo;
	int intonset = 0,nfound = 0;


	int Hack = 0;
	int LetterSpot;
	int O1=0;
	int places[10];
	int graphnumplaces[10];
	char graphsact[10][10], TheCodaString[20];
	int nplaces;
	int n;

	int nmiss,oldm;
	int spots1,nofoundcount , foundcount, foundspot, ons1 , cods1, tempk;
				
	int HasBroken = 0;
	char NewString[50];
	char  numx = 0;
	int WhatBreak = 0,Marker, spec3spot = -1;

	int nusedvowels1 =0, nusedcodas1 = 0;

	FILE *fp1, *fp4;

	IsE=0, IsED = 0,IsE2 = 0, IsED2 = 0;
	GotThrough=0;
	NOrthSylls = 0;
	for(i=0;i</*MAXSYLLABLES*/5;i++) {
		ntempcodas[i] = 0; 
		ntemponsets[i]= 0; 
		ntempcodas2[i] = 0; 
		ntemponsets2[i]= 0; 
	}
	ntempvowels = 0; ntempvowels2 = 0;
	
	for(i=0;i<MAXPOS * NMARCOLETTERS ;i++) NewInput[i] = 0;
	i=0;
	foundit = 0;

	SegmentWord(Word); //turn it into consonants and graphemes
	ParseSegments(); //parse the consonants and graphemes

	ToTrain = 0;
	Marker = 0;

		
	rejected = 0;

//	if(ntemponsets[0] > 3 || ntemponsets[1] >3 || (ntempcodas[0] > 4 && ntempvowels == 1) || ntempcodas[1] > 4 || ntempvowels > 2 ) {
		/*fp1 = fopen("2007rejected2.1.txt","a");
		fprintf(fp1,"%d %d [%s] [%s] %d  [ntv=%d %d %d %d %d\n",TheTrainingCycle, NOrthSylls, newword, Word,j,ntempvowels, ntemponsets[0],ntemponsets[1],ntempcodas[0], ntempcodas[1]  );
		fclose(fp1);	*/	
//		rejected = 1;
//		ToTrain = 0;
//		return;
//	}
	//	if(TheTrainingCycle == 1 && (ntempcodas[0] + ntemponsets[1] == nphoncodas[0]+nphononsets[1]) || (ntempvowels = 1 && nphonvowels == 1) ) FindBestPlace();

	
	CopyInfoAcross(); //into 2.


	//InTraining = 1;
		if(InTraining == 1) { //This leanrs the places automatically, not used in CDP++.
			AutoTrain();
		} else {
		//RUNING IN TEST MODE.
			ParseCodasTesting2();
			//ParseCodasTesting4();
		}
		MakeNewInput();
	
		if(foundit == 1) {
			ToTrain = 1;
		} else {
	//		fp1= fopen("badstim.txt","a");
	//		fprintf(fp1,"%d %s \n",TheTrainingCycle, Word);
	//		fclose(fp1);
		}



	//	strcpy(PhonString,"++++++++++++++++");
	///////////////////////////////////////////////////////////////////////
		int UsedEdRule = 0;


//	fp1 = fopen("Testlook.txt","a");
		
	UpTo = 0;
//	fprintf(fp1,"%s ntv=%d\n",Word,ntempvowels);
	for(i=0;i<MAXPOS;i++) {
		for(j=0;j<NMARCOLETTERS;j++) {
			Input[i][j] = NewInput[UpTo++];
		//	fprintf(fp1,"%1.0f ",Input[i][j]);
		}
	//	fprintf(fp1,"\n");
	}
//	fclose(fp1);

	GlobalVowels = ntempvowels;


	//CreateOrthString();	
//	if(ToTrain == 1) MakePhonStr(PhonString);

	GotThrough = 1;


	if(ToTrain == 1) {
		fp1 = fopen("PhonTests.txt","a");
		fprintf(fp1,"%d %d %d %d [%s] %s %s %s %d %d %s %d %d\n ", TheTrainingCycle, foundit, i, ThisFreq, PhonString,ThisPhon,Word,OrthSegWord, ToTrain, Marker,OrthString, ThisStress, ThisFreq);
		fclose(fp1);

	} else {
		/*fp1 = fopen("PhonTestsBooted.txt","a");
		fprintf(fp1,"%d %d %d [%s] %s %s %s %d %d %s %d %d\n ", TheTrainingCycle, foundit, i, PhonString,ThisPhon,Word,OrthSegWord, ToTrain, Marker,OrthString, ThisStress, ThisFreq);
		fclose(fp1);*/
	}
	k = 0;

	if(ToTrain == 1 /*&& ThisNumSylls == 2*/) {
		strcpy(GlobalWord,Word);
		//TrainSplitWeights();
	}
//	if(TheTrainingCycle == 0 && nphonvowels == 2 && (ntempcodas[0] + ntemponsets[1] == nphoncodas[0]+nphononsets[1]) ) AddGraphemePhonemeInf();  	
}


	/*for(h=0;h<SPLITNETSPOTS;h++) {
		SplitNetSums[h] = 0;
		for(i=0;i< NSEGINPUTS;i++) {
			if(inputs2[i] > .01) { //Speed
				SplitNetSums[h] += SplitNetWeights[i][h] * inputs2[i];			
			}
		}
		SplitNetActs[h] = ActFunc(SplitNetSums[h]);
		if(SplitNetActs[h] > tempmax) {
			SplitMaxPos = h;
			tempmax = SplitNetActs[h];
		}
		if(SplitNetActs[h] < .05) SplitNetActs[h] = 0;
	}	*/












/*void ZorziNet::SplitForwardTrigrams(char *thisword)  //TRIGRAMS
{
}*/



void ZorziNet::SplitNetForward(char *theword)
{
}
void ZorziNet::SplitNetLoadWeights(char *thename)
{
	
}

void ZorziNet::SplitNetSaveWeights()
{
}

void ZorziNet::MakePhonStr(char *Pattern)
{
	int h, i,j,a,Upto = 0;
	FILE *fp;

	/*fp = fopen("thepattern.txt","a");
	fprintf(fp,"[%s] \n",Pattern);
	fclose(fp);*/

	for(i=0;i<MAXSYLLABLES;i++) {
		nphoncodas[i] = 0;
		nphononsets[i] = 0;
	}
	nphonvowels = 0;

	for(h=0;h<MAXSYLLABLES;h++) {
		for(i=(h*MAXSPOTSPERSYLL);i<MAXONSETS + (h*MAXSPOTSPERSYLL);i++) {
			if(Pattern[i] != '*') {
				phononsets[h][nphononsets[h]] = Pattern[i];
				nphononsets[h]++;
			}
		}
		for(i=MAXONSETS + 1 + (h*MAXSPOTSPERSYLL);i<((h+1)*MAXSPOTSPERSYLL);i++) {
			if(Pattern[i] != '*') {
				phoncodas[h][nphoncodas[h]] = Pattern[i];
				nphoncodas[h]++;
			}
		}

		if(Pattern[MAXONSETS + (h*MAXSPOTSPERSYLL)] != '*') {
			phonvowels[h] = Pattern[MAXONSETS + (h*MAXSPOTSPERSYLL)];
			nphonvowels++;
		}		
	}
	/*fp = fopen("phontestsss.txt","a");
	fprintf(fp,"%s %d \n",Pattern,nphonvowels);
	for(i=0;i<nphonvowels;i++) {
		for(j=0;j<nphononsets[i];j++) fprintf(fp,"%c ",phononsets[j]);
		fprintf(fp,"%c ",phonvowels[i]);
		for(j=0;j<nphoncodas[i];j++) fprintf(fp,"%c ",phoncodas[i]);
	}
	fprintf(fp,"\n");
	fclose(fp);*/


	for(i=0;i<MAXSYLLABLES * MAXSPOTSPERSYLL;i++) {
		if(Pattern[i] == '*') {
			for(j=0;j<(NUMBER_OF_PHONEMES);j++) {
				Final[Upto] = 0;
				Upto++;
				NewOutputs[i][j] = 0;
			}
		} else {
			a = PhonemeIndex(Pattern[i]);
			if(a != -1) {
				for(j=0;j<a;j++) {
					Final[Upto] = 0;
					NewOutputs[i][j] = 0;
					Upto++;
				}
				Final[Upto] = 1; Upto++;
				NewOutputs[i][j] = 1;
				for(j = a+1;j<NUMBER_OF_PHONEMES;j++) {
					Final[Upto] = 0; Upto++;
					NewOutputs[i][j] = 0;
				}
			} else {
				for(j=0;j<NUMBER_OF_PHONEMES;j++) {
					Final[Upto] = 0; Upto++;
					NewOutputs[i][j] = 0;
				}
				fp = fopen("phonerrors.txt","a");
				fprintf(fp,"%c %s \n",Pattern[i], Pattern);
				fclose(fp);
			}
		}
	}
}



void ZorziNet::SaveNet()
{
	int X, Y,i,j;
	char names[1000];

	if(ENGLISH == 1) sprintf(names,"EnglishWeights.%d.txt",TheTrainingCycle);
	else if (ITALIAN == 1) sprintf(names,"ItalianWeights.%d.txt",TheTrainingCycle);
	else if (GERMAN == 1) sprintf(names,"GermanWeights.%d.txt",TheTrainingCycle);
	else if (FRENCH == 1) sprintf(names,"FrenchWeights.%d.txt",TheTrainingCycle);

	FILE *fp;

#if ENGLISH
	fp = fopen(names,"w");
#elif GERMAN
	fp = fopen(names,"w");
#elif FRENCH
	fp = fopen(names,"w");
#elif ITALIAN
//	fp = fopen("ItalianWeights2.txt","w");
	fp = fopen(names,"w");
#endif

	for(i=0;i<(NMARCOLETTERS) * MAXPOS;i++) {
		for(j=0;j<(NUMBER_OF_PHONEMES) * MAXPHONE ;j++) {
			fprintf(fp,"%f ",Weights[i][j]);
	//		fprintf(fp2,"%3.3f ",Weights[i][j]);
		}
	}
    fclose(fp);
}


void ZorziNet::MakeRule(char *Word, char *Phon)
{
	int i,j,k, ActualSpot=0;
	FILE *fp1, *fp2;
	
	for(i=0;i<16;i++) {
		for(j=0;j<NMARCOLETTERS;j++) {
			Input[i][j] = 0;
		}
	}

	for(i=0;i<strlen(Word);i++) {
		if(Word[i] == '*') {
			for(j=0;j<NMARCOLETTERS;j++) {
				Input[ActualSpot][j] = 0;
			}
			ActualSpot++;
		} else {
			if(i<3) {
				j = CheckForOnsetGrapheme(Word + i);
				if(j != NONSETS) {
					for(k=0;k<j;k++) Input[ActualSpot][k] = 0;
					Input[ActualSpot][k] = 1; k++;
					for(;k<NMARCOLETTERS;k++) Input[ActualSpot][k] = 0;
					i += strlen(OnsetGraphemes[j]) - 1;
				} else {
					j = AlphabetIndex(toupper(Word[i]));
					for(k=0;k<NONSETS+NVOWELS+NCODAS;k++) Input[ActualSpot][k] = 0;
					for(k=0;k<j+NONSETS+NVOWELS+NCODAS;k++) Input[ActualSpot][k] = 0;
					Input[ActualSpot][k] = 1; k++;
					for(;k<NMARCOLETTERS;k++) Input[ActualSpot][k] = 0;
				}
				ActualSpot++;
			} else if (i == 3) {
				for(j=0;j<NVOWELS;j++) {
					if( (strncmp(Word + i, VowelGraphemes[j], strlen(VowelGraphemes[j]))) == 0) {
						break;
					}
				}
				if(j != NVOWELS) {
					for(k=0;k<NONSETS+j;k++) Input[i][k] = 0;
					Input[i][k] = 1; k++;
					for(;k<NMARCOLETTERS;k++) Input[i][k] = 0;
					i += strlen(VowelGraphemes[j]) - 1;
				} else {
					j = AlphabetIndex(toupper(Word[i]));
					for(k=0;k<NONSETS+NVOWELS+NCODAS;k++) Input[i][k] = 0;
					for(;k<j+NONSETS+NVOWELS+NCODAS;k++) Input[i][k] = 0;
					Input[i][k] = 1;k++;
					for(;k<NMARCOLETTERS;k++) Input[i][k] = 0;
				}
				ActualSpot++;
			} else {
				j = CheckForCodaGrapheme(Word + i);
				if(j != NCODAS) {
					for(k=0; k < NONSETS+NVOWELS+j;k++) Input[ActualSpot][k] = 0;
					Input[ActualSpot][k] = 1; k++;
					for(; k < NMARCOLETTERS;k++) Input[ActualSpot][k] = 0;
					i += strlen(CodaGraphemes[j]) - 1;
//	fp1 = fopen("ttt.txt","a");
//	fprintf(fp1,"%s %d\n",Word,j);
//	fclose(fp1);
				} else {
					j =	AlphabetIndex(toupper(Word[i]));
					for(k=0;k<NONSETS+NVOWELS+NCODAS+j;k++) Input[ActualSpot][k] = 0;
					Input[ActualSpot][k] = 1; k++;
					for(; k < NMARCOLETTERS;k++) Input[ActualSpot][k] = 0;
				}
				ActualSpot++;
//	fp1 = fopen("ttt.txt","a");
//	fprintf(fp1,"%s %d %d\n",Word,j, i);
//	fclose(fp1);
			}
		}
	}

	for(;ActualSpot<16;ActualSpot++) {
		for(j=0;j<NMARCOLETTERS;j++) {
			Input[ActualSpot][j] = 0;
		}
	}
/*	fp1 = fopen("ttt.txt","a");
	fprintf(fp1,"%s \n",Word);
	for(i=0;i<16;i++) {
		for(j=0;j<NMARCOLETTERS;j++) {
			fprintf(fp1, "%1.0f",Input[i][j]);
		}
		fprintf(fp1,"\n");
	}
	fclose(fp1);*/
	

	for(i=0;i<strlen(Phon);i++) {
		if(Phon[i] == '*') {
			for(j=0;j<NUMBER_OF_PHONEMES;j++) NewOutputs[i][j] = 0;
		} else {
			j =	PhonemeIndex(Phon[i]);
			for(k=0;k < j;k++) NewOutputs[i][k] = 0;
			NewOutputs[i][k] = 1; k++;
			for(;k<NUMBER_OF_PHONEMES;k++) NewOutputs[i][k] = 0;
		}
	//fp1 = fopen("ttt.txt","a");
//	fprintf(fp1,"%s %s %d\n",Word,Phon,j);
	//fclose(fp1);
	}
	for(;i<16;i++) {
		for(j=0;j<NUMBER_OF_PHONEMES;j++) NewOutputs[i][j] = 0;
	}


	ForwardPass();
//	TrainRule();
	TrainCycle(Word, Phon, 1, 0);
}

void ZorziNet::TrainRule()
{

	int i,j, allzero;
	float Value;
	float Delta;
	int counter = 0, Spot, Phoneme; 
	char NewWord[100];
	char NewPhon[100];

	FILE *fp;


	NeedForward = TRUE;

	for(i=0;i<MAXPHONE;i++) {
      for(j=0;j<(NUMBER_OF_PHONEMES);j++) {
			if(Output[i][j] < Threshold) Output[i][j] = 0;
	  }
	}

	counter = 0;
	for(i=0;i<MAXPHONE;i++) {
      for(j=0;j<(NUMBER_OF_PHONEMES);j++) {
			Value = (float) /*Final[counter]*/ NewOutputs[i][j];
		    ErrorBuffer[i][j] = Value - Output[i][j];	
			TotalError += fabs(ErrorBuffer[i][j]);
			counter++;
      }
//	  fprintf(fp,"\n");
    }
	//fclose(fp);
//	exit(1);

	allzero = 0;

	 for(Spot = 0;Spot<MAXPHONE;Spot++) {
		 for(Phoneme = 0;Phoneme<(NUMBER_OF_PHONEMES);Phoneme++) {
			 for(i=0;i<MAXPOS;i++) {
				for(j=0;j< (NMARCOLETTERS);j++) {
					if(Input[i][j] > .99) { //For Speed.
					///	if(fabs(Weights[(i * (NMARCOLETTERS)) + j][ (Spot * (NUMBER_OF_PHONEMES - 1)) +  Phoneme]) < .001) {
					//		Delta = LearningRate * ErrorBuffer[Spot][Phoneme] * Input[i][j];
					//	} else {
							Delta = 1.0 * LearningRate * ErrorBuffer[Spot][Phoneme] * Input[i][j];				
					//	}
							if(Delta > 1) exit(1);
						Weights[(i * (NMARCOLETTERS)) + j][ (Spot * (NUMBER_OF_PHONEMES - 1)) +  Phoneme] += Delta;
					}
				}
			}
		 }
	 }
}

void ZorziNet::FindBestPlace()
{
	int i,j,k;
	float topcod, topons;
	int topcodspot, toponsspot, temp;
	FILE *fp;

	if(ntemponsets[1] == 1 && ntempcodas[0] == 0 && nphonvowels == 2) { //double vowels
		topcod = 100;
		
		fp = fopen("dvs.txt","a");
		fprintf(fp,"2 %s %d %d %c %s \n",ThisWord, ntempcodas[0],ntemponsets[1], phononsets[1][0], temponsets[1][0]);
		fclose(fp);
		/*fp = fopen("thestrings.txt","a");
		fprintf(fp,"4 %s %d %d %c  ",ThisWord, ntempcodas[0],ntemponsets[1], phononsets[1][0]);
		fclose(fp);*/
		CreateOrthString();

	} else if(ntempvowels == 2 && nphonvowels == 2 && (ntempcodas[0] + ntemponsets[1] == nphoncodas[0]+nphononsets[1])) {
		ntempcodas[0] = nphoncodas[0];
		ntemponsets[1] = nphononsets[1];
		for(i=0;i<ntempcodas[0]+ntemponsets[1];i++) {
			strcpy(temponsets[1][i],tempcodas[0][i+ntempcodas[0]]);
			temponsetsnum[1][i] = tempcodasnum[0][i+ntempcodas[0]];
		}
		fp = fopen("thestrings22.txt","a");
		fprintf(fp,"1 %s %d %d %c \n ",ThisWord, ntempcodas[0],ntemponsets[1], phononsets[1][0]);
		fclose(fp);

		CreateOrthString();
	} else if(ntempvowels == 1 && nphonvowels == 1) {

		fp = fopen("thestrings33.txt","a");
		fprintf(fp,"2 %s %d %d %c \n ",ThisWord, ntempcodas[0],ntemponsets[1], phononsets[1][0]);
		fclose(fp);

		CreateOrthString();
	}   else if(ntempvowels == 2 && nphonvowels == 2) {
		topcod = 0;
		topcodspot = -1;

		if(nphononsets[1] == 0 && nphoncodas[0] == 0) { /*Double vowel*/
			topcod = 100;
			topcodspot = 0;
		} else {
			for(i=ntempcodas[0]-1; i>= 0; i--) {
				for(j=0;j<nGraphemesUpTo[4];j++) {
					if(tempcodasnum[0][i] == GraphemesInf[4][j] && PhonemeIndex(phononsets[1][0]) == PhonemesInf[4][j]) {
						if(GraphemeFreqsInf[4][j] > topcod) {
							topcod = GraphemeFreqsInf[4][j];
							topcodspot = i;				
							/*fp = fopen("thestrings44.txt","a");
							fprintf(fp,"%s %d %d %d %f [%d %d %c %d %f]\n",ThisWord, ntempcodas[0],ntemponsets[1],topcodspot,topcod, i, j,phononsets[1][0], PhonemesInf[4][j],GraphemeFreqsInf[4][j] );
							fclose(fp);*/
						}
					}
				}
			}
		}

		if(!(topcod >0)) {
			if(ntempcodas[0] == 1) {
				topcod = 100;
				topcodspot = 0;
				fp = fopen("topcod1.txt","a");
				fprintf(fp,"%s %s \n",ThisWord, ThisPhon);
				fclose(fp);
			} else if ((strcmp(tempcodas[0][0],"n")) == 0 && (strcmp(tempcodas[0][1],"x")) == 0) {
				topcod = 100;
				topcodspot = 1;

				fp = fopen("topcod2.txt","a");
				fprintf(fp,"%s %s \n",ThisWord, ThisPhon);
				fclose(fp);
			} else if( (strcmp(tempcodas[0][0],"x")) == 0 || (strcmp(tempcodas[0][0],"qu")) == 0) {
				topcod = 100;
				topcodspot = 0;

				fp = fopen("topcod3.txt","a");
				fprintf(fp,"%s %s \n",ThisWord, ThisPhon);
				fclose(fp);
			} else if (ntempcodas[0] > 1) {
				if((strcmp(tempcodas[0][ntempcodas[0] - 1],"r")) == 0 && phononsets[1][1] == 'r' || (strcmp(tempcodas[0][ntempcodas[0] - 1],"l")) == 0 && phononsets[1][1] == 'l') {
					topcod = 100;
					topcodspot = ntempcodas[0] - 2;
					fp = fopen("topcod4.txt","a");
					fprintf(fp,"%s %d \n",ThisWord, ThisPhon);
					fclose(fp);
				} else {
					fp = fopen("topcod5.txt","a");
					fprintf(fp,"%s %s \n",ThisWord, ThisPhon);
					fclose(fp);
				}
			} else if (ntempcodas[0] == 0 && ntemponsets[1] == 0) {
				topcod = 100;
				fp = fopen("topcod6.txt","a");
				fprintf(fp,"%s %s \n",ThisWord, ThisPhon);
				fclose(fp);
			} 
		}

		if(topcod > 0) {
		//ba.sting
			if( ntemponsets[1] == 1 && ntempcodas[0] == 0 || ntemponsets[1] == 0 && ntempcodas[0] == 0) {
				//do nothing, Must be split vowel. or n mid consonant.
			} else {
				ntemponsets[1] = ntempcodas[0] - topcodspot;
				temp = ntempcodas[0];
				ntempcodas[0] = topcodspot;
			
				for(i=0;i<ntempcodas[0];i++) {
					for(i=0;i<ntempcodas[0];i++) {
						strcpy(tempcodas[0][i], tempcodas2[0][i]);
						tempcodasnum[0][i] = tempcodasnum2[0][i];
					}
				}
				for(i=0;i<ntemponsets[1];i++) {
					strcpy(temponsets[1][i],tempcodas2[0][i+ntempcodas[0]]);
					temponsetsnum[1][i] = tempcodasnum2[0][i+ntempcodas[0]];
				}
			}
		
		} else {
			fp = fopen("TopCod100.txt","a");
			fprintf(fp,"%s ",ThisWord);
			for(i=0;i<ntempcodas[0];i++) {
				fprintf(fp,"<%s> ",tempcodas[0][i]);
			}
			for(i=0;i<ntemponsets[1];i++) {
				fprintf(fp,"[%s] ",temponsets[1][i]);
			}
			fprintf(fp,"\n");
			fclose(fp);
		}
	} else {
		fp = fopen("Theerrors2.txt","a");
		fprintf(fp,"%s \n",ThisWord);
		fclose(fp);
	}

	fp = fopen("matched12.txt","a");
	fprintf(fp,"%s %d %d %d %d  %d %d\n",ThisWord, ntempcodas[0],ntemponsets[1],ntempvowels,nphonvowels,ntempcodas[0] + ntemponsets[1], nphoncodas[0]+nphononsets[1]);
	fclose(fp);



}

void ZorziNet::LoadFreqsInfo()
{
	int i,j,k, pos, numinpos;
	char phonemesstr[10];
	FILE *fp;


	fp = fopen("./EnglishRes/graphinfo.txt","r");

	for(i=0;i<10;i++) {
		fscanf(fp,"%d%d",&pos,&nGraphemesUpTo[i]);
		for(j=0;j<nGraphemesUpTo[i];j++) {
			fscanf(fp,"%d%s%f%d%s",&GraphemesInf[i][j], GraphemesInflets[i][j], &GraphemeFreqsInf[i][j], &PhonemesInf[i][j], /*&PhonemesCInf[i][j]*/ phonemesstr); 
			PhonemesCInf[i][j] = phonemesstr[0];
		}
	}
	fclose(fp);


	fp=fopen("graphinfotest.txt","w");
	for(i=0;i<10;i++) {
		fprintf(fp,"%d %d  \n",i, nGraphemesUpTo[i]);
		for(j=0;j<nGraphemesUpTo[i];j++) {
			fprintf(fp," %d %s %f %d %c \n",GraphemesInf[i][j], GraphemesInflets[i][j], GraphemeFreqsInf[i][j], PhonemesInf[i][j], PhonemesCInf[i][j]); 
		}
		fprintf(fp,"\n");
	}
	fclose(fp);

}

void ZorziNet::AddGraphemePhonemeInf()
{
	int i,j,k;
	char NewWord[100];
	/*void AddGraphemePhonemeInf();
	char GraphemesInf[100][10];
	float GraphemeFreqsInf[100][10];
	int nGraphemesUpTo[10];

	ZNet.ThisFreq = Freq1*/
	
	FILE *fp;

	fp =fopen("lookee.txt","a");
	fprintf(fp,"%s %d ",GlobalWord,ntempcodas[0]);
	for(i=0;i<ntempcodas[0];i++) fprintf(fp,"%d ",tempcodasnum[0][i]);
	fprintf(fp,"\n");
	fclose(fp);

		i = 0;
		for(j=0;j<nphoncodas[i];j++) {
			strcat(NewWord,tempcodas[i][j]);
			strcat(NewWord,".");
		}
		ntempcodas[i] = nphoncodas[i];
		strcat(NewWord,"+");
		for(j=0;j<nphononsets[i+1];j++) {
			strcat(NewWord,tempcodas[i][j+nphoncodas[i]]);
			strcat(NewWord,".");
			strcpy(temponsets[i+1][j],tempcodas[i][j+nphoncodas[i]]);
			temponsetsnum[i+1][j] = tempcodasnum[i][j+nphoncodas[i]];
		}
		ntemponsets[i+1] = nphononsets[i+1];



	for(i=0;i<ntempcodas[0];i++) {
		for(j=0;j<nGraphemesUpTo[i];j++) {
			if(tempcodasnum[0][i] == GraphemesInf[i][j] && PhonemeIndex(phoncodas[0][i]) == PhonemesInf[i][j]) break;
		}
		if(j==nGraphemesUpTo[i]) {
			GraphemesInf[i][j] = tempcodasnum[0][i];
			PhonemesInf[i][j] = PhonemeIndex(phoncodas[0][i]);
			PhonemesCInf[i][j] = phoncodas[0][i];
			GraphemeFreqsInf[i][j] = log10((float)ThisFreq+2.0);
			strcpy(GraphemesInflets[i][j], tempcodas[0][i]);
			nGraphemesUpTo[i]++;
			if(PhonemesInf[i][j] == -1) {
				fp =fopen("probs1.txt","a");
				fprintf(fp,"%s %d ",ThisWord,ntempcodas[0]);
				fclose(fp);
			}
		} else {
			GraphemeFreqsInf[i][j] += log10((float)ThisFreq+2.0);
		}

	}
	for(i=4;i<ntemponsets[1]+4;i++) {
		for(j=0;j<nGraphemesUpTo[i];j++) {
			if(temponsetsnum[1][i-4] == GraphemesInf[i][j] && PhonemeIndex(phononsets[1][i-4]) == PhonemesInf[i][j]) break;
		}
		if(j==nGraphemesUpTo[i]) {
			GraphemesInf[i][j] = temponsetsnum[1][i-4];
			PhonemesInf[i][j] = PhonemeIndex(phononsets[1][i-4]);
			PhonemesCInf[i][j] = phononsets[1][i-4];
			GraphemeFreqsInf[i][j] = log10((float)ThisFreq +2.0);
			strcpy(GraphemesInflets[i][j], temponsets[1][i-4]);


			if(i == 4 && phononsets[1][i-4] == 'g' && GraphemesInflets[i][j][0] == 'l') {
				fp=fopen("WasIstDas.txt","a");
				fprintf(fp,"%s %s\n",ThisWord, ThisPhon);
				fclose(fp);
			}	

			nGraphemesUpTo[i]++;
	
			if(PhonemesInf[i][j] == -1) {
				fp =fopen("probs2.txt","a");
				fprintf(fp,"%s %d ",ThisWord,ntempcodas[0]);
				fclose(fp);
			}

		} else {
			GraphemeFreqsInf[i][j] += log10((float)ThisFreq + 2.0);
		}
	}
}

void ZorziNet::CreateOrthString()

{
	int i,j,k;
	FILE *fp;

	OrthString[0] = 0;
	for(i=0;i<ntempvowels;i++) {	
		for(j=0;j<ntemponsets[i];j++) {
			strcat(OrthString,temponsets[i][j]);
			strcat(OrthString,".");
		}
		strcat(OrthString,"+");
		if(ntempvowels > 0) {
			strcat(OrthString, tempvowels[i]);
		}

		strcat(OrthString,"+");
		for(j=0;j<ntempcodas[i];j++) {
			strcat(OrthString,tempcodas[i][j]);
			strcat(OrthString,".");
		}
		strcat(OrthString,"+");	
	}
	for(;i<5;i++) {
		for(j=0;j<3;j++) {
			strcat(OrthString,"+");
		}
	}
}