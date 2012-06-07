// WordDBFuncs.cpp: implementation of the WordDBFuncs class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WordDBFuncs.h"
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

WordDBFuncs::WordDBFuncs()
{
	OverFreq = -1000;
}

WordDBFuncs::~WordDBFuncs()
{

}

void WordDBFuncs::SaveBodies()
{

	FILE *Fp1;
	char new_word[10];
	char the_bod[10];
	char old_pron[10];
	int i,j,k;

	Fp1 = fopen("Bodies","w");

	for(i=0;i<Word_Count;i++) {
 	//  for(k=0;k<NWord_to_Phon[i];k++) {
		
		strcpy(new_word, Word[i]);
		j = 0;
		while(j < MAXPOS && new_word[j] != '+') j++;
		new_word[j] = 0;
		j = 0;
		if(new_word[j] == 'Y') j++; 
		while( ConsonantNum(tolower(new_word[j])) != -1) j++; 
		if(new_word[j] == 'U' && new_word[j-1] == 'Q') j++;
		strcpy(the_bod, new_word + j);
		if(strlen(the_bod) > 0) fprintf(Fp1,"%s %s %s %8.0f ",new_word, Phon[Word_to_Phon[i][0]], the_bod, pow(10, (Freq[i] + 1)* word_log_divisor) );
		else fprintf(Fp1,"%s %s * %8.0f ",new_word, Phon[Word_to_Phon[i][0]],  pow(10, (Freq[i] + 1)* word_log_divisor) );
	
		k = 0;
		strcpy(new_word, Phon[Word_to_Phon[i][k]]);
		j = 0;
		while(j < MAXPHONE && new_word[j] != '+') j++;
		new_word[j] = 0;
		j = 0;
		//if(new_word[j] == 'Y') j++; 
		while( PhonemeVowelIndex(new_word[j]) == -1 && new_word[j] != '+') j++; 
		strcpy(the_bod, new_word + j);
		if(strlen(the_bod) == 0) fprintf(Fp1,"%s * 0\n",new_word);
		else fprintf(Fp1,"%s %s %f\n ",new_word, the_bod, (pow(10, (PhFreq[Word_to_Phon[i][k]] + 1) * phon_log_divisor) ) / NWord_to_Phon[Word_to_Phon[i][k] ]);
	//  }
	}
	fclose(Fp1);
}


void WordDBFuncs::SaveCCVC()
{
	FILE *Fp1, *Fp2;
	char SWord[10];

	int i,j,k,l;

	Fp1 = fopen("CVCC","w");
	Fp2 = fopen("CCVC","w");

	for(i=0;i<Pron_Count;i++) {
		j = 0;
		while(Phon[i][j] != WORDPAD) j++;
		if(j == 4) {
			if((PhonemeVowelIndex(Phon[i][1])) != -1) {
				for(k=0;k<NPhon_to_Word[i];k++) {
					strcpy(SWord, Word[Phon_to_Word[i][k]]);	
					for(l=0;l<strlen(SWord);l++) if(SWord[l] == WORDPAD) SWord[l] = 0;
					fprintf(Fp1,"%s %8.0f\n", SWord , 	pow(10.0, (Freq[Phon_to_Word[i][k]] + 1.0) * word_log_divisor) ); 
				}
			}
			if((PhonemeVowelIndex(Phon[i][2])) != -1) {
				for(k=0;k<NPhon_to_Word[i];k++) {
					strcpy(SWord, Word[Phon_to_Word[i][k]]);	
					for(l=0;l<strlen(SWord);l++) if(SWord[l] == WORDPAD) SWord[l] = 0;
					fprintf(Fp2,"%s %8.0f\n", SWord , 	pow(10.0, (Freq[Phon_to_Word[i][k]] + 1.0) * word_log_divisor) ); 
				}
			}
		}
	}
	fclose(Fp1);
	fclose(Fp2);
}

int WordDBFuncs::countit(char *line) 
{	
	int i, len, the_count = 0;

	
	
	len = strlen(line);	
	for(i=0;i<len;i++) {
		if(line[i] == ' ') the_count++;
	}
	return (the_count);
} 

void WordDBFuncs::SaveHomophones()
{
//	float HomCount[8000][5];
//	int GoodCounts[8000];
	char HomLine[22][15], TheWord[20], crapname[200];
	int i,j,k,l, Found, Good_Count, Homs;
	char TempWord1[20],TempWord2[20];

	char tempword[20], templine[200], line[200], Flecs[20], Morphs[20], MorphData[30], FlecData[30];
	FILE *Fp1, *Fp0;
	FILE *Fp2, *Fp3, *Fp4, *Fp5; 

	if( (Fp0 = fopen("JustHoms.txt","w")) == NULL) exit(1);
	

	Homs = 0;
	for(i=0;i<Pron_Count;i++) {
		if(NPhon_to_Word[i] > 1) {		
			fprintf(Fp0,"%d %s ",NPhon_to_Word[i], NPhon_to_Word[i]);
		
			for(j=0;j<NPhon_to_Word[i];j++) {
				fprintf(Fp0,"%s ",Phon[Phon_to_Word[i][j]]);
			}
			fprintf(Fp0,"\n");
		}
	}
	fclose(Fp0);

}



void WordDBFuncs::SaveSplitHomophones()
{
	int i,j,k, shit;
	FILE *Fp1, *Fp2;
	float min;
	float less_than = -100;
	int count;

	Fp1 = fopen("Homophones1","w");
	Fp2 = fopen("Homophones2","w");

	for(i=0;i<Pron_Count;i++) {
		if(NPhon_to_Word[i] > 1) {
			count = 0;
			min = 2;
			for(j=0;j<NPhon_to_Word[i];j++) {
				if( (Freq[Phon_to_Word[i][j]] + 1.0) >= less_than) {
					count++;
				}
			}
			
			if(count >= -1) {
				fprintf(Fp1,"%s ",Phon[i]);
				for(j=0;j<NPhon_to_Word[i];j++) {
					k = 0;
					while(Word[Phon_to_Word[i][j]][k] != '+' && k < MAXPOS) {
						fprintf(Fp1,"%c",Word[Phon_to_Word[i][j]][k]);
						k++;
					}
					fprintf(Fp1,"%6.0f %0.2f ", pow(10.0, (Freq[Phon_to_Word[i][j]] + 1.0) * word_log_divisor), Freq[Phon_to_Word[i][j] ] + 1.0);
				}
				k = 0;
				while(Phon[i][k] != '+' && k < MAXPHONE) {
					fprintf(Fp1,"%c",Phon[i][k]);
					k++;
				}
				fprintf(Fp1,"\n",Phon[k]);
			} else {
				for(j=0;j<NPhon_to_Word[i];j++) {
					k = 0;
					while(Word[Phon_to_Word[i][j]][k] != '+' && k < MAXPOS) {
						fprintf(Fp2,"%c",Word[Phon_to_Word[i][j]][k]);
						k++;
					}
					fprintf(Fp2," %5.0f ", Freq[Phon_to_Word[i][j] ] + 1.0);
					fprintf(Fp2,"%d %d ",Neighbours(Word[Phon_to_Word[i][j]]), BodNeighbours(Word[Phon_to_Word[i][j]]) );
				}
				k = 0;
				while(Phon[i][k] != '+' && k < MAXPHONE) {
					fprintf(Fp2,"%c",Phon[i][k]);
					k++;
				}
				fprintf(Fp2,"\n",Phon[k]);
			}
		}
	}
	fclose(Fp2);
	fclose(Fp1);
}


void WordDBFuncs::SaveContingencyHomophones()
{
	int i,j,k;
	FILE *Fp1;
	float min;
	float less_than = 0.05;
	char TempWord1[20],TempWord2[20];
	int count;

	Fp1 = fopen("Homophones1","w");
	//Fp2 = fopen("Homophones2","w");
//	for(i=0;i<20;i++) {
//	fprintf(Fp1,"->%c ",WDBPhonemes[i]);
//	}

	for(i=0;i<Pron_Count;i++) {
		if(NPhon_to_Word[i] > 1) {
				for(j=0;j<NPhon_to_Word[i];j++) {
					strcpy(TempWord1, Word[Phon_to_Word[i][j]]);
					strcpy(TempWord2, Phon[i]);
						
					GetBod(TempWord1);
					GetPhonBod(TempWord2);
					//CalcContingency(Word[Phon_to_Word[i][j]], Phon[i]);

					
					CalcVowelContingency(TempWord1,TempWord2);
					fprintf(Fp1," *%3.2f ",MaxContingency);		

					
					if(strlen(LastOnset) != 0) {
						CalcContingency(TempWord1,TempWord2);
						fprintf(Fp1," 1 %3.2f %3.2f ",AvgContingency,MaxContingency);		
					
					} else {
						fprintf(Fp1," 0 0 0 ");

					}

					k = 0;
					while(Word[Phon_to_Word[i][j]][k] != '+' && k < MAXPOS) {
						fprintf(Fp1,"%c",Word[Phon_to_Word[i][j]][k]);
						k++;
					}
				}
				k = 0;
				fprintf(Fp1," ");
				while(Phon[i][k] != '+' && k < MAXPHONE) {
					fprintf(Fp1,"%c",Phon[i][k]);
					k++;
				}
				fprintf(Fp1,"\n",Phon[k]);
		}
	}
//	fclose(Fp2);
	fclose(Fp1);


}

void WordDBFuncs::Onsets() 
{

	int i,j,k;

	char TempWord;
	char onsets[100][10];
	int onsetcount[100];
	int nonsets = 0;
	FILE *fp1;

	for(i=0;i<100;i++) {
		for(j=0;j<10;j++) {
			onsets[i][j] = 0;
		}
		onsetcount[i] = 0;
	}

	for(i=0;i<Word_Count;i++) {
		GetBod(Word[i]);
		for(j=0;j<nonsets;j++) {
			if(strcmp(LastOnset, onsets[j]) == 0) break;
		}
		if(j == nonsets) {
			strcpy(onsets[j], LastOnset);
			onsetcount[j] = 1;
			nonsets++;
		} else {
			onsetcount[j]++;
		}
	}

	fp1 = fopen("Theonsets.txt","w");
	for(i=0;i<nonsets;i++) {
		fprintf(fp1, "%s %d \n", onsets[i], onsetcount[i]);
	}
	fclose(fp1);
}

int WordDBFuncs::TLCs(char *The_Word)
{
	char new_word[15];
	char first, second;
	int i,j,k;
	int TLC = 0;

	for(j=0;j<strlen(The_Word) - 1;j++) {
		first = The_Word[j];
		second = The_Word[j+1];
		strcpy(new_word, The_Word);
		new_word[j] = second;
		new_word[j+1] = first;
		if(strcmp(new_word, The_Word) != 0) {
			for(k = strlen(new_word);k<9;k++) {
				new_word[k] = '+';
			}
			new_word[k] = 0;

			for(i=0;i<Word_Count;i++) {
				if((strcmp(new_word, Word[i])) == 0) {
					TLC++;
				}
			}
		}
	}
	return(TLC);			
}
			

int WordDBFuncs::BodNeighbours(char *The_Word)
{
	char new_word[15];
	char the_wordbod[10];
	char old_pron[10], the_bod[100];
	NResults = 0;

	int i,j,k, count = 0;


	if(strcmp(The_Word,"UGH++++++") != 0 && strcmp(The_Word,"NTH++++++") != 0 && strcmp(The_Word,"LL+++++++") != 0 && strcmp(The_Word,"OZ+++++++") != 0 && strcmp(The_Word,"LB+++++++") != 0 && strcmp(The_Word,"KT+++++++") != 0 && strcmp(The_Word,"HZ+++++++") != 0 && strcmp(The_Word,"HT+++++++") != 0 && strcmp(The_Word,"WT+++++++") != 0 && strcmp(The_Word,"YD+++++++") != 0 && strcmp(The_Word, "ST+++++++") !=0 && strcmp(The_Word,"RD+++++++") != 0 && strcmp(The_Word,"MT+++++++") != 0 && strcmp(The_Word,"MS+++++++") != 0&& strcmp(The_Word,"FT+++++++") != 0 && strcmp(The_Word,"SQ+++++++") != 0 && strncmp(The_Word,"CWM",3) != 0) {	
//		j =0;
	
//		while(j < MAXPOS && The_Word[j] != '+') j++;
//		The_Word[j] = 0;	
	
//		j = 0;
//		if(The_Word[j] == 'Y' || The_Word[j] == 'y') j++; 
//		if(strncmp(The_Word,"QU",2)) j+= 2;
//		while(ConsonantNum(tolower(The_Word[j])) != -1 && j < MAXPOS) j++; 

		GetBod(The_Word);
		strcpy(the_wordbod,LastBod);

//		strcpy(the_wordbod, The_Word+j);
	
		for(i=0;i<Word_Count;i++) {
			GetBod(Word[i]);
			strcpy(the_bod,LastBod);
			if(strcmp(the_bod,the_wordbod) == 0) {
				for(j=0;j<NWord_to_Phon[i];j++) {
					sprintf(Results[NResults],"%s %s", Word[i], Phon[Word_to_Phon[i][j]]);
					NResults++;
					count++;
				}
			}

		}
		return count;
	} else {
		return -1;
	}
}

void WordDBFuncs::SplitIt()
{

}


void WordDBFuncs::SplitVowels()
{

}


void WordDBFuncs::SplitVowels2()
{

}


void WordDBFuncs::CalcVowelContingency(char *TheWord, char *The_Pron)
{
	int i,j,k,TheSpot, ThePronSpot;
	int TheLetCount;
	char NewWord[20], NewPron[20];;
	float SumContinge = 0;
	FILE *fp2;

	fp2 = fopen("crap","a");
	fprintf(fp2,"before1 %s %s\n", TheWord, The_Pron);
	fclose(fp2);

//	GetBod(TheWord);
//	GetPhonBod(The_Pron);
	
	for(i=0;i<9;i++) {
		NewWord[i] = '+';
	}
	for(i=0;i<strlen(TheWord);i++) {
		NewWord[i] = TheWord[i];
	}
	NewWord[9] = 0;

	TheSpot = WordPos(NewWord);

	for(i=0;i<9;i++) NewPron[i] = '+';
	for(i=0;i<strlen(The_Pron);i++) NewPron[i] = The_Pron[i];
	NewPron[9] = 0;

	
	ThePronSpot = 0;
	
	if(TheSpot == -1) {
		MaxContingency= -1;
		return;
	}

	while( strcmp(Phon[Word_to_Phon[TheSpot][ThePronSpot]], NewPron) != 0) {
		ThePronSpot++;
	}

	for(i=0;i<UpToVowel;i++) {
		if(VowelPhonemes[i] == TheVowel[TheSpot][0][ThePronSpot]) break;
	}

	for(j=0;j<VowelPhonemeLettersCount[i];j++) {
		if( strcmp(TheVowelLetters[TheSpot][0][ThePronSpot], VowelPhonemeLetters[i][j]) == 0) {
			TheLetCount = NVowelPhonemeLettersCount[i][j];
		}
		SumContinge += NVowelPhonemeLettersCount[i][j];
	}
	
	
	MaxContingency = TheLetCount / SumContinge ;

	
}



void WordDBFuncs::CalcContingency(char *The_Word, char *The_Pron)
{
	int i,j,k, Total, thespot, Word_Total, ThePronSpot;
	FILE *shit, *fp2;
	float TempContinge, SumContinge;
	char NewWord[15], NewPron[15];

	fp2 = fopen("crap","a");
	fprintf(fp2,"in  \n");
	fclose(fp2);

	for(i=0;i<9;i++) {
		NewWord[i] = '+';
	}
	for(i=0;i<strlen(The_Word);i++) {
		NewWord[i] = The_Word[i];
	}
	NewWord[9] = 0;

	for(i=0;i<9;i++) {
		NewPron[i] = '+';
	}
	for(i=0;i<strlen(The_Pron);i++) {
		NewPron[i] = The_Pron[i];
	}
	NewPron[9] = 0;



	thespot = WordPos(NewWord);

	//fp2 = fopen("crap","a");
//	fprintf(fp2,"before \n");
//	fclose(fp2);

	if(thespot == -1) {
		MaxContingency = -1;
		AvgContingency = -1;
		return;
	}


	ThePronSpot = 0;
	while( strcmp(Phon[Word_to_Phon[thespot][ThePronSpot]], NewPron) != 0) ThePronSpot++;


	MaxContingency = 100;
	SumContinge = 0;

	TempContinge = 0;
	SumContinge = 0;


	for(i=0;i<OnsetNumber[thespot][ThePronSpot];i++) {
		Total = 0;
		Word_Total = 0;
		for(j=0;j<UpToPhonemes;j++) {
			
			if(WDBPhonemes[j] == OnsetPartsPhon[thespot][ThePronSpot][i]) {
				for(k=0;k<UpToLetters[j];k++) {
					Total += LettersCount[j][k];							
					if(strcmp(OnsetParts[thespot][ThePronSpot][i], WDBLetters[j][k]) == 0) {					
						Word_Total = LettersCount[j][k];
					}

				}	
				// break;
			}
		}
		TempContinge =  (float) Word_Total / (float) Total ;
		SumContinge += TempContinge;
	
		if(TempContinge < MaxContingency) {
			MaxContingency = TempContinge;
		}
		
	}
	AvgContingency = SumContinge / OnsetNumber[thespot][ThePronSpot];
}

void WordDBFuncs::CalcCodaContingency(char *The_Word, char *The_Pron)
{
	int i,j,k, Total, thespot, Word_Total, ThePronSpot;
	int offcount;
	FILE *shit, *fp2;
	float TempContinge, SumContinge;
	char NewWord[15], NewPron[15];

	fp2 = fopen("crap","a");
	fprintf(fp2,"in  \n");
	fclose(fp2);

	for(i=0;i<9;i++) {
		NewWord[i] = '+';
	}
	for(i=0;i<strlen(The_Word);i++) {
		NewWord[i] = The_Word[i];
	}
	NewWord[9] = 0;

	for(i=0;i<9;i++) {
		NewPron[i] = '+';
	}
	for(i=0;i<strlen(The_Pron);i++) {
		NewPron[i] = The_Pron[i];
	}
	NewPron[9] = 0;

	thespot = WordPos(NewWord);

	if(thespot == -1) {
		MaxCodaContingency = -1;
		AvgCodaContingency = -1;
		return;
	}

	ThePronSpot = 0;
	while( strcmp(Phon[Word_to_Phon[thespot][ThePronSpot]], NewPron) != 0) ThePronSpot++;


	MaxCodaContingency = 100;
	SumContinge = 0;

	TempContinge = 0;
	SumContinge = 0;
	offcount = 0;

	for(i=0;i<CodaNumber[thespot][ThePronSpot];i++) {
		Total = 0;
		Word_Total = 0;
		for(j=0;j<UpToCoda;j++) {
//X !
			if(CodaPhonemes[j] == CodaPartsPhon[thespot][ThePronSpot][i]) {
				for(k=0;k<CodaPhonemeLettersCount[j];k++) {
					Total += NCodaPhonemeLettersCount[j][k];							
					if(strcmp(CodaParts[thespot][ThePronSpot][offcount], CodaPhonemeLetters[j][k]) == 0) {					
						Word_Total = NCodaPhonemeLettersCount[j][k];
					}
				}	
				// break;
			}
		}
		TempContinge =  (float) Word_Total / (float) Total ;
		SumContinge += TempContinge;
	
		if(TempContinge < MaxCodaContingency) {
			MaxCodaContingency = TempContinge;
		}
		if(CodaParts[thespot][ThePronSpot][i][0] == 'X') i++;
		offcount++;
	}

	AvgCodaContingency = SumContinge / CodaNumber[thespot][ThePronSpot];
}

int WordDBFuncs::CountStartPron(char *The_Word) 
{
	int i;
	
	for(i=0;i<strlen(The_Word) && i < MAXPOS;i++) {
		if(PhonemeVowelIndex(The_Word[i]) != -1) return i;
	}
	return(strlen(The_Word));
	return(-1);
}

void WordDBFuncs::RimeBods()
{
	char AllRimes[2000][10];
	char AllBods[2000][20][10];
	int UpToRimes = 0;
	int UpToBods[2000];
	int NRimes[2000];
	int NBods[2000][20];
	int i,j,k,l,m,n;
	int highest, highestspot;

	FILE *fp;


	for(i=0;i<Pron_Count;i++) {
		GetPhonBod(Phon[i]);
		for(j=0;j<UpToRimes;j++) {
			if((strcmp(LastPhonBod,AllRimes[j])) == 0) {
				break;
			}
		}
		if(j==UpToRimes) {
			strcpy(AllRimes[j],LastPhonBod);
			NRimes[j] = 0;
			UpToBods[j] = 0;
			for(k=0;k<NPhon_to_Word[i];k++) {
				GetBod(Word[Phon_to_Word[i][k]]);
				strcpy(AllBods[j][UpToBods[j]],LastBod);
				NBods[j][UpToBods[j]] = 1;
				NRimes[j]++;
				UpToBods[j]++;
			}
			UpToRimes++;
		} else {
			for(k=0;k<NPhon_to_Word[i];k++) {
				GetBod(Word[Phon_to_Word[i][k]]);
				for(l=0;l<UpToBods[j];l++) {
					if((strcmp(AllBods[j][l],LastBod)) ==0) break;
				}
				if(l == UpToBods[j]) {
					strcpy(AllBods[j][l],LastBod);
					NBods[j][l] = 1;
					NRimes[j]++;
					UpToBods[j]++;
				} else {
					NBods[j][l]++;
					NRimes[j]++;
				}
			}
		}
	}
	fp = fopen("RimeBods.txt","w");
	for(i=0;i<UpToRimes;i++) {
		highest = -1;
		fprintf(fp,"%s %d %d ", AllRimes[i],NRimes[i], UpToBods[i]);
		for(j=0;j<UpToBods[i];j++) {
			if(NBods[i][j] > highest) {
				highest = NBods[i][j];
				highestspot = j;
			}
		}
		fprintf(fp,"%f %d ",(float)NBods[i][highestspot]/(float)NRimes[i], NBods[i][highestspot]);
	
		for(j=0;j<UpToBods[i];j++) {
			fprintf(fp,"%s %d ", AllBods[i][j], NBods[i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);

}

void WordDBFuncs::ChopAllPhons()
{
	int i,j,k,a, upto, startsyll;
	int NSylls, whichSyll;
	char TempSyll[10];

	FILE *fp;

	
	for(i=0;i<Pron_Count;i++) {

		for(j=0;j<3;j++) TheOnset[i][0][j] = 0;
		for(j=0;j<3;j++) TheOnset[i][1][j] = 0;
		for(j=0;j<5;j++) TheVowel[i][0][j] = 0;
		for(j=0;j<5;j++) TheVowel[i][1][j] = 0;



		whichSyll = 0;
		a = strlen(PhonSeg[i]);
		
//		fp = fopen("Teststs.txt","a");
//		fprintf("%d %d  %s \n",i,a, Phon);
//		fclose(fp);


		NumberSylls[i] = 1;
		for(j=0;j<a;j++) {
			if(PhonSeg[i][j] == '.') NumberSylls[i]++;
		}
		upto = 0;
		startsyll = 0;
		TempSyll[0] = 0;
		while(upto < a ) {
			if(PhonSeg[i][upto] == '.') {
				TempSyll[startsyll] = 0;
				GetPhonBod(TempSyll);
			
				//fp = fopen("Teststs2.txt","a");
				//fprintf(fp,"%s %s %s \n",TempSyll, LastPhonOnset,LastPhonBod);
				//fclose(fp);

				for(j=0;j<3;j++) TheOnset[i][0][j] = 0;
				//TheVowel[i][0][0] = 0;
				TheOnset[i][whichSyll][0] = 0;
				TheVowel[i][whichSyll][0] = 0;
				strcpy(TheOnset[i][whichSyll], LastPhonOnset);
				strcpy(TheVowel[i][whichSyll], LastPhonBod);
				whichSyll++;
				startsyll = 0;
				upto++;
			} else {
				TempSyll[startsyll] = PhonSeg[i][upto];
				startsyll++;
				upto++;	
			}
		}
		TempSyll[startsyll] = 0;

		GetPhonBod(TempSyll);
		//fp = fopen("Teststs2.txt","a");
		//fprintf(fp,"%d %s %s %s \n",i, TempSyll, LastPhonOnset,LastPhonBod);
		//fclose(fp);
		strcpy(TheOnset[i][whichSyll], LastPhonOnset);
		strcpy(TheVowel[i][whichSyll], LastPhonBod);

//		fprintf(fp,"%d %s %s %s \n",i,Phon[i], TheOnset[i], TheVowel[i]);
	}


//	fp = fopen("Teststs.txt","w");	
//	fprintf(fp,"%d \n",Pron_Count);
//	for(i=0;i<Pron_Count;i++) {
//		for(j=0;j<NumberSylls[i];j++) {
//			fprintf(fp,"%d %d %s %s \n",i, NumberSylls[i], Phon[i], PhonSeg[i]);
//			fprintf(fp,"-->%s %s \n",TheOnset[i][j], TheVowel[i][j]);
//		}
//	}

//	fclose(fp);

}


void WordDBFuncs::GetPhonBod(char *The_Word)
{
	int i,j,k, count = 0;
	char temp[10];
	char Temp_The_Word[100];
//	FILE *fp;

	strcpy(Temp_The_Word, The_Word);
	j =0;
	
	//if(strlen(The_Word) > 2) {
	//	if(strncmp(The_Word,"kw",2) == 0) j = 2;
	//	else if(strncmp(The_Word,"skw",3) == 0) j = 3;
	//	else while(j < MAXPOS && The_Word[j] != '+') j++;
	/*} else */ 
	 if(The_Word[1] == 'j') {
		strcpy(temp,Temp_The_Word + 2);
		Temp_The_Word[2] = 0;
		strcat(Temp_The_Word,temp);
	}
	if(The_Word[2] == 'j') {
		strcpy(temp,Temp_The_Word + 3);
		Temp_The_Word[3] = 0;
		strcat(Temp_The_Word,temp);
	}
	while(j < MAXPOS && Temp_The_Word[j] != '+') j++;
	Temp_The_Word[j] = 0;	
	j = CountStartPron(Temp_The_Word);
	//	fp=fopen("fuck","a");
	//	fprintf(fp,"hre  %d %d \n",j, strlen(The_Word) - j);
	//	fclose(fp);
	LastPhonOnsetLength = j;
	LastPhonBodLength = strlen(Temp_The_Word) - j;
	strcpy(LastPhonBod, Temp_The_Word+j);
	strncpy(LastPhonOnset, Temp_The_Word, j);
	LastPhonBod[LastPhonBodLength] = 0;
	LastPhonOnset[j] = 0;
}

int WordDBFuncs::NPhonBods(char *The_Word)
{
	char this_bod[20], compbod[20];
	int NBods = 0,i;
	FILE *fp;

	WordDBFuncs::GetPhonBod(The_Word);
	strcpy(this_bod,LastPhonBod);
	for(i=0;i<Pron_Count;i++) {
		WordDBFuncs::GetPhonBod(Phon[i]);
		strcpy(compbod,LastPhonBod);
		if( (strcmp(compbod,this_bod)) == 0) NBods++;	
	}
	return(NBods);
}


void WordDBFuncs::GetBod(char *The_Word)
{
	int i,j,k, count = 0;
	char Bak_Word[20];
	FILE *fp;

	strcpy(Bak_Word,The_Word);


	j =0;
	
	while(j < 9 && Bak_Word[j] != '+') j++;

	Bak_Word[j] = 0;	
	
	j = 0;

	if(Bak_Word[0] == 'Y' || Bak_Word[0] == 'y') j++; 
	if(strlen(Bak_Word) > 2) { 
		if(strncmp(Bak_Word, "QU",2) == 0) j = 2;
		else if(strncmp(Bak_Word, "SQU",3) == 0) j = 3;
		else while(ConsonantNum(tolower(Bak_Word[j])) != -1) j++; 
	} else while(ConsonantNum(tolower(Bak_Word[j])) != -1) j++;
	LastOnsetLength = j;
	strncpy(LastOnset, Bak_Word, j);
	LastBodLength = strlen(Bak_Word) - j;

	if(LastBodLength != 0) {
		strcpy(LastBod, Bak_Word+j);
		LastBodLength = strlen(Bak_Word) - j;
	} else {
		LastBod[0] = 0;
		LastBodLength = 0;
	}
	LastOnset[j] = 0;
}

void WordDBFuncs::ConsistentBodies()
{
	char TheWordBod[20], ThePhonBod[20];
	char AllOrthBodies[2000][10];
	char AllPhons[2000][30][10];
	int NPhonbods[2000];
	int PhonBodCount[2000][30];
	int OrthSums[2000];
	int UpToOrth = 0;

	
	int h,i,j,k;

	FILE *Fp;

	for(i=0;i<Word_Count;i++) {
		for(h=0;h<NWord_to_Phon[i];h++) {
			//	Fp = fopen("AllConsis.txt","a");
			//	fprintf(Fp,"%d %d \n",i,UpToOrth);
			//	fclose(Fp);

			WordDBFuncs::GetBod(Word[i]);
		//	strcpy(TheWordBod, LastBod);
			WordDBFuncs::GetPhonBod(Phon[Word_to_Phon[i][h]]);
		//	strcpy(ThePhonBod,LastPhonBod);

			strcpy(ThePhonBod, LastBod);
			strcpy(TheWordBod,LastPhonBod);

			//Consistency(TheWordBod, ThePhonBod);

			for(j=0;j<UpToOrth;j++) {
				if( strcmp(TheWordBod,AllOrthBodies[j]) == 0) break;
			}
			if(j == UpToOrth) {
				strcpy(AllOrthBodies[j],TheWordBod);
				strcpy(AllPhons[j][0], ThePhonBod);
				PhonBodCount[j][0] = 1;
				NPhonbods[j] = 1;
				UpToOrth++;
			} else {
				for(k=0;k<NPhonbods[j];k++) {
					if( strcmp(ThePhonBod, AllPhons[j][k]) == 0) {
						break;
					}
				}
				if(k == NPhonbods[j]) {
					strcpy(AllPhons[j][k], ThePhonBod);
					PhonBodCount[j][k] = 1;
					NPhonbods[j]++;
				} else {
					PhonBodCount[j][k]++;
				}
			}
		}	
	}
	for(i=0;i<UpToOrth;i++) {
		OrthSums[i] = 0;
		for(j=0;j<NPhonbods[i];j++) {
			OrthSums[i] += PhonBodCount[i][j];
		}
	}


	Fp = fopen("AllConsis.txt","a");
	fprintf(Fp,"==>%d \n",UpToOrth);
	for(i=0;i<UpToOrth;i++) {
	//	fprintf(Fp,"->%d \n",i);
		for(j=0;j<NPhonbods[i];j++) {
			fprintf(Fp,"%s %s %d %d \n",AllOrthBodies[i],AllPhons[i][j], OrthSums[i],PhonBodCount[i][j]);
		}
	}
	fclose(Fp);
}

float WordDBFuncs::Consistency(char *The_Word, char *The_Phon) 
{
	char TheWordBod[20], ThePhonBod[20];
	int i,j;

	SameConsis = 0, DifConsis = 0;
	SameConsisFreq = 0, DifConsisFreq = 0;
	
	FILE *fp;

//	fp = fopen("test","a");

	WordDBFuncs::GetBod(The_Word);
	strcpy(TheWordBod, LastBod);
	WordDBFuncs::GetPhonBod(The_Phon);
	strcpy(ThePhonBod,LastPhonBod);
	
	for(i=0;i<Word_Count;i++) {
 		WordDBFuncs::GetBod(Word[i]);
		if(strcmp(LastBod,TheWordBod) == 0) {
			for(j=0;j<NWord_to_Phon[i];j++) {
				WordDBFuncs::GetPhonBod(Phon[Word_to_Phon[i][j]]);
				if( strcmp(LastPhonBod,ThePhonBod) == 0) {
					SameConsis++;
					SameConsisFreq += OldFreq[i];
			//		fprintf(fp,"s = %s %s   ", Word[i], Phon[Word_to_Phon[i][j]]);
				} else {
					DifConsis++;
					DifConsisFreq += OldFreq[i];
			//		fprintf(fp,"d = %s %s   ", Word[i], Phon[Word_to_Phon[i][j]]);

				}
			}
		}
	}
//	fclose(fp);
	return( (float) SameConsis / (float) (DifConsis + SameConsis) );
}


float WordDBFuncs::FBConsistency(char *The_Word, char *The_Phon) 
{
	char TheWordBod[20], ThePhonBod[20];
	int i,j;

	SameConsis = 0, DifConsis = 0;
	SameConsisFreq = 0, DifConsisFreq = 0;
	
	FILE *fp;

//	fp = fopen("test","a");

	WordDBFuncs::GetBod(The_Word);
	strcpy(TheWordBod, LastBod);
	WordDBFuncs::GetPhonBod(The_Phon);
	strcpy(ThePhonBod,LastPhonBod);
	
	for(i=0;i<Pron_Count;i++) {
 		WordDBFuncs::GetPhonBod(Phon[i]);
		if( (strcmp(LastPhonBod,ThePhonBod)) == 0) {
			for(j=0;j<NPhon_to_Word[i];j++) {
				WordDBFuncs::GetBod(Word[Phon_to_Word[i][j]]);
				if(strcmp(LastBod,TheWordBod) == 0) {
					SameConsis++;
					SameConsisFreq += OldFreq[i];
				} else {
					DifConsis++;
					DifConsisFreq += OldFreq[i];
				}
			}
		}
	}
	TempTokenConsis = SameConsisFreq / (SameConsisFreq + DifConsisFreq);

	return( (float) SameConsis / (float) (DifConsis + SameConsis) );
}


int WordDBFuncs::PhonNeighbours(char *NeighWord) 
{
	FILE *fp, *fp2, *fp3;

	//These two are for later
	int Number;

	char wd[50];
	char ph[50];
	char NewNeighWord[50];
	char NewAssPron[50];
	char AssPron[50];

	float F, SF;
	int Word_Len, bit_count, Neigh_Count, NeighCount1, wordid;
	float SumFreq = 0;
	
	int nthisword = 0;
	//int thisword[50];


	int h,i, j = 0, k,l, count = 0, len;

	char FileWord[100], FilePhon[100], FilePhon2[100], Orth[100], NewWord[100], NormalPhon[100];
	float Freq1, Freq2, TempFreq = 0;
	int TheStress, NumSylls;
	int startspot = 0;

	char TheFileName[1000];
	char Test2[1000], Test3[1000];

	strcpy(AssPron,"XXX");

	for(i=0;i<strlen(LoadDirectory);i++) {
		if(LoadDirectory[i] == '\\') Test2[i] = '/';
		else Test2[i] = LoadDirectory[i];
	}
	Test2[i] = 0;
	
		
	//if(ENGLISH == 1) sprintf(Test3,"%s/EnglishRes/EnglishNewDB2.txt",Test2);
	if(ENGLISH == 1) sprintf(Test3,"./cdp/EnglishNewDB.txt");
	else if (ITALIAN == 1) sprintf(Test3,"%s/ItalianRes/ItalianDB.txt",Test2);
	else if (GERMAN == 1) sprintf(Test3,"%s/GermanRes/GermanDB.txt",Test2);
	else if (FRENCH == 1) sprintf(Test3,"%s/FrenchRes/FrenchDB.txt",Test2);


/*	fp2 = fopen("test1112.txt","w");
	fprintf(fp2,"%s \n",Test3);
	fclose(fp2);*/

	i = 0;
	while(NeighWord[i] != '+' && i < MAXPOS) i++;
	
	//if(NeighWord[i] == '+') i--;
	for(j=0;j<i;j++) {
		if(ENGLISH == 1) NewWord[j] = tolower(NeighWord[j]);
		else NewWord[j] = NeighWord[j];
	}
	NewWord[j] = 0;

	/*fp2 = fopen("resinfo22.txt","a");
	fprintf(fp2,"%s %s %d\n",NeighWord, NewWord, j);
	fclose(fp2);*/

//	fp = fopen("c:/Work/NewModels/NewMultiSyllStress2009.Jan/NewLex4.3.2.txt","r");
	fp = fopen(Test3,"r");

	strcpy(TheFoundWord,"XXXXXX");
	if(fp == 0) exit(1);

	Word_Count = 0;
	Pron_Count = 0;
	FoundWords = 0;
	startspot = 0;

	//for(i=0;i<32270 /*34012*/;i++) {
	//	fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid);		
	while ( (fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid)) != EOF) {		
		if(ENGLISH == 1 || GERMAN == 1) {
			FilePhon2[16] = 0;
		}
	
	/*fp3 = fopen("Allinfo22.txt","a");
			if(fp3 == 0) exit(1);
			fprintf(fp3,"%d %s %s %d %f %f %s %d \n",i, FileWord,FilePhon2,TheStress, Freq1, Freq2, NormalPhon, wordid);		
			fclose(fp3);*/
	
		
		if( (strcmp(NewWord, FileWord)) == 0) {						
			
			/*fp3 = fopen("Allinfo22.txt","a");
			if(fp3 == 0) exit(1);
			fprintf(fp3,"%d %s %s %d %f %f %s %d \n",i, FileWord,FilePhon2,TheStress, Freq1, Freq2, NormalPhon, wordid);		
			fclose(fp3);*/
			
		/*	if(TheStress == 1 || TheStress == 2) {
				NewStressPos[startspot] = TheStress - 1;
			} else {
				NewStressPos[startspot] = 0; //Trochaic Default
			}*/
			
			if(Freq1 < 1) Freq1 = 1;
			if(Freq2 < 1) Freq2 = 1;
			
			FoundWords=1;
			thisword[nthisword] = wordid;
			nthisword++;
			strcpy(NewTempWord[startspot],FileWord);
			strcpy(NewTempPron[startspot],FilePhon2);
			NewTempFreq[startspot] = Freq1;
			NewTempSFreq[startspot] = Freq2;
			NewStressPos[startspot] = TheStress;
			startspot++;			
		}
	}

/*	fp3 = fopen("Allinfo22.txt","a");
	fprintf(fp3,"**\n");
	fclose(fp3);*/
	//return 1;

	//*add neighbors etc. *//
	
	strcpy(NewAssPron,AssPron);
	//strcpy(NewNeighWord,NeighWord);
	for(i=0;i<strlen(NewAssPron);i++) {
		if(NewAssPron[i] == '+') {
			NewAssPron[i] = 0;
			break;
		}
	}

	/*fp3 = fopen("Allinfo111.txt","a");
	fprintf(fp3,"%s %s\n", NewAssPron, AssPron);
	fclose(fp3);*/


	//Phon neighbors.
	NeighCount1 = 0;
	for(h=0;h<startspot;h++) {
		rewind(fp);
		while ( (fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid)) != EOF) {		
			if(ENGLISH == 1 || GERMAN == 1) FilePhon2[16] = 0;
			if(strlen(NewTempPron[h]) <= strlen(NormalPhon)) {
				for(j=0;j<nthisword;j++) {
					if(thisword[j] == wordid) break;
				}
				if(j==nthisword) {
					NeighCount1 = 0;
					for(j=0;j<strlen(NewTempPron[h]);j++) {
						if(NewTempPron[h][j] != NormalPhon[j]) NeighCount1++;
						if(NeighCount1 == 2) break;
					}
					NeighCount1 += (strlen(NormalPhon) - strlen(NewTempPron[h]));
					if(NeighCount1 > 0 && NeighCount1 < 2) {
						for(k=0;k<startspot;k++) {
							if( ((strcmp(NewTempWord[k], FileWord)) == 0) &&((strcmp(NewTempPron[k], FilePhon2)) == 0) && (NewStressPos[k] == TheStress - 1)) break;
						}
						if(k==startspot) {
							strcpy(NewTempWord[startspot],FileWord);
							strcpy(NewTempPron[startspot],FilePhon2);
							NewTempFreq[startspot] = Freq1;
							NewTempSFreq[startspot] = Freq2;
	
							if(TheStress == 1 || TheStress == 2) NewStressPos[startspot] = TheStress - 1;
							else NewStressPos[startspot] = 0; //Trochaic Default			
							startspot++;
						}
					}
				}
			}
		}
	}


	/*	fp2 = fopen("resinfo2222.txt","a");
	fprintf(fp2,"ss = %d %s  \n",startspot, NewAssPron);
	for(i=0;i<startspot;i++) {
		fprintf(fp2,"%s %s %d\n",NewTempWord[i],NewTempPron[i], NewStressPos[i]);
	}
	fclose(fp2);*/

	count = startspot;
	//if(count > 5000) count = 5000;

	for(i=0;i<count;i++) WTaken[i] = 0;
	Word_Count = 0;

	//Squish Orth - Take Out Dups
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(WTaken[j] == 1) {
					if( (strcmp(NewTempWord[i], NewTempWord[j]) ) == 0 ) {
						break;
					}
				}
			}
		}
		if(j == count) {
			strcpy(Word[Word_Count], NewTempWord[i]);
			WTaken[i] = 1;
			Freq[Word_Count] = NewTempFreq[i]; 
			Word_Count++;
		} else {
			//k = 0;
		//	while( strcmp(NewTempWord[i], Word[k]) != 0) k++; 
		//	Freq[k] = NewTempFreq[i];
		}
	}


	/*fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"1 %d %d %d \n",Word_Count, Pron_Count, count);
	fclose(fp2);*/

	//Squish Phon - Take Out Dups
	for(j=0;j<count;j++) Taken[j] = 0;

	Pron_Count = 0;
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(Taken[j] == 1) {
					if( (strcmp(NewTempPron[i], NewTempPron[j]) ) == 0 && (NewStressPos[i] == NewStressPos[j]) ) {				
						break;
					}
				}		
			}
		}
		if(j == count) {
			strcpy(Phon[Pron_Count], NewTempPron[i]);
			StressPos[Pron_Count] = NewStressPos[i];
			Taken[i] = 1;
			PhFreq[Pron_Count] = NewTempSFreq[i];
			Pron_Count++;
		} else {
			//k = 0;
		//	while(  strcmp(NewTempPron[i], Phon[k]) != 0) k++;
		//	PhFreq[k] = NewTempSFreq[i];
		}
	}

/*	fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"2 %d %d %d \n",Word_Count, Pron_Count, count);
	fclose(fp2);*/

	if(ENGLISH == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0)) ;  
	} else if (ITALIAN == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(606000000.0+2.0)) / log10(606000000.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(606000000.0+2.0)) / log10(606000000.0+2.0)) ;  
	} else if (GERMAN == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(150507.0+2.0)) / log10(150507.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(150507.0+2.0)) / log10(150507.0+2.0)) ;  
	} else if (FRENCH == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(867040.78+2.0)) / log10(867040.78+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(867040.78+2.0)) / log10(867040.78+2.0)) ;  
	}
	//for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0) ) ;
	//for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0)) ;  
	
/*	fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"3 %d %d %d \n",Word_Count, Pron_Count, count);
	fclose(fp2);*/

	// Connect the new lexicons //
	for(i=0;i<Word_Count;i++) NWord_to_Phon[i] = 0;
	for(i=0;i<Pron_Count;i++) NPhon_to_Word[i] = 0;


	//exit(1);

	for(i=0;i<count;i++) {
		for(j=0;j<Pron_Count;j++) {
			if( ((strcmp(NewTempPron[i], Phon[j])) == 0) && (NewStressPos[i] == StressPos[j])) break;
		}
		for(k=0;k<Word_Count;k++) {
			if( (strcmp(NewTempWord[i], Word[k])) == 0 ) break;
		}
		//if(j == Pron_Count) exit(1); //Something has gone wrong :(
		Word_to_Phon[k][ NWord_to_Phon[k] ] = j;
		NWord_to_Phon[k] += 1;
		Phon_to_Word[j][ NPhon_to_Word[j] ] = k;
		NPhon_to_Word[j] += 1;
	}

//	exit(1);

	for(i=0;i<Word_Count;i++) {
		for(j=0;j<strlen(Word[i]);j++) {
			if(ENGLISH == 1) Word[i][j] = toupper(Word[i][j]);
		}
		for(;j<MAXPOS;j++) {
			Word[i][j] = '+';
		}
	}
	Word[i][MAXPOS] = 0;

	fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"%d %d %d %s \n",Word_Count, Pron_Count, count, NewAssPron);
	for(i=0;i<Word_Count;i++) {
		fprintf(fp2,"%d %s %f ",i, Word[i], Freq[i]);
		for(j=0;j<NWord_to_Phon[i];j++) {
			fprintf(fp2,"[%s] %d %f %d ",Phon[Word_to_Phon[i][j]],Word_to_Phon[i][j], PhFreq[Word_to_Phon[i][j]],NewStressPos[Word_to_Phon[i][j]]);
		}
		fprintf(fp2,"\n");
	}
	fclose(fp2);

/*	fp2 = fopen("test111111111.txt","w");
	fprintf(fp2,"%s \n",LoadDirectory);
	fclose(fp2);*/
	return TRUE;
}

int WordDBFuncs::PhonNeighboursFromPhon(char *NeighWord) 
{
	FILE *fp, *fp2, *fp3;

	//These two are for later
	int Number;

	char wd[50];
	char ph[50];
	char NewNeighWord[50];
	char NewAssPron[50];
	char AssPron[50];

	float F, SF;
	int Word_Len, bit_count, Neigh_Count, NeighCount1, wordid;
	float SumFreq = 0;
	
	int nthisword = 0;
	//int thisword[50];


	int h,i, j = 0, k,l, count = 0, len;

	char FileWord[100], FilePhon[100], FilePhon2[100], Orth[100], NewWord[100], NormalPhon[100];
	float Freq1, Freq2, TempFreq = 0;
	int TheStress, NumSylls;
	int startspot = 0;

	char TheFileName[1000];
	char Test2[1000], Test3[1000];

	strcpy(AssPron,"XXX");

	for(i=0;i<strlen(LoadDirectory);i++) {
		if(LoadDirectory[i] == '\\') Test2[i] = '/';
		else Test2[i] = LoadDirectory[i];
	}
	Test2[i] = 0;
	
		
	if(ENGLISH == 1) sprintf(Test3,"%s/EnglishRes/EnglishNewDB2.txt",Test2);
	//if(ENGLISH == 1) sprintf(Test3,"/home/Conrad/CDP/EnglishNewDB.txt");
	else if (ITALIAN == 1) sprintf(Test3,"%s/ItalianRes/ItalianDB.txt",Test2);
	else if (GERMAN == 1) sprintf(Test3,"%s/GermanRes/GermanDB.txt",Test2);
	else if (FRENCH == 1) sprintf(Test3,"%s/FrenchRes/FrenchDB.txt",Test2);


/*	fp2 = fopen("test1112.txt","w");
	fprintf(fp2,"%s \n",Test3);
	fclose(fp2);*/

	i = 0;
	while(NeighWord[i] != '+' && i < MAXPOS) i++;
	
	//if(NeighWord[i] == '+') i--;
	for(j=0;j<i;j++) {
		if(ENGLISH == 1) NewWord[j] = tolower(NeighWord[j]);
		else NewWord[j] = NeighWord[j];
	}
	NewWord[j] = 0;

	/*fp2 = fopen("resinfo22.txt","a");
	fprintf(fp2,"%s %s %d\n",NeighWord, NewWord, j);
	fclose(fp2);*/

//	fp = fopen("c:/Work/NewModels/NewMultiSyllStress2009.Jan/NewLex4.3.2.txt","r");
	fp = fopen(Test3,"r");
	strcpy(TheFoundWord,"XXXXXX");
	if(fp == 0) exit(1);

	Word_Count = 0;
	Pron_Count = 0;
	FoundWords = 0;
	startspot = 0;

	
	
	//startspot = 1;
	strcpy(NewTempWord[0],"NOWORDENT");
	strcpy(NewTempPron[0],NeighWord);
	NeighCount1 = 0;
	startspot = 1;

	//for(h=0;h<startspot;h++) {
		rewind(fp);
		while ( (fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid)) != EOF) {		
			if(ENGLISH == 1 || GERMAN == 1) FilePhon2[16] = 0;
			if( strlen(NewTempPron[0]) == strlen(NormalPhon) ) {
				NeighCount1 = 0;
				for(j=0;j<strlen(NewTempPron[0]);j++) {
					if(NewTempPron[0][j] != NormalPhon[j]) NeighCount1++;
					if(NeighCount1 == 2) break;
				}
				//NeighCount1 += (strlen(NormalPhon) - strlen(NewTempPron[0]));
				if(NeighCount1 > 0 && NeighCount1 < 2) {
					strcpy(NewTempWord[startspot],FileWord);
					strcpy(NewTempPron[startspot],FilePhon2);
					NewTempFreq[startspot] = Freq1;
					NewTempSFreq[startspot] = Freq2;
	
					if(TheStress == 1 || TheStress == 2) NewStressPos[startspot] = TheStress - 1;
					else NewStressPos[startspot] = 0; //Trochaic Default			
					startspot++;		
				}
			}
		}

	

	count = startspot;
	//if(count > 5000) count = 5000;

	for(i=0;i<count;i++) WTaken[i] = 0;
	Word_Count = 0;

	//Squish Orth - Take Out Dups
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(WTaken[j] == 1) {
					if( (strcmp(NewTempWord[i], NewTempWord[j]) ) == 0 ) {
						break;
					}
				}
			}
		}
		if(j == count) {
			strcpy(Word[Word_Count], NewTempWord[i]);
			WTaken[i] = 1;
			Freq[Word_Count] = NewTempFreq[i]; 
			Word_Count++;
		} else {
			//k = 0;
		//	while( strcmp(NewTempWord[i], Word[k]) != 0) k++; 
		//	Freq[k] = NewTempFreq[i];
		}
	}


	/*fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"1 %d %d %d \n",Word_Count, Pron_Count, count);
	fclose(fp2);*/

	//Squish Phon - Take Out Dups
	for(j=0;j<count;j++) Taken[j] = 0;

	Pron_Count = 0;
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(Taken[j] == 1) {
					if( (strcmp(NewTempPron[i], NewTempPron[j]) ) == 0 && (NewStressPos[i] == NewStressPos[j]) ) {				
						break;
					}
				}		
			}
		}
		if(j == count) {
			strcpy(Phon[Pron_Count], NewTempPron[i]);
			StressPos[Pron_Count] = NewStressPos[i];
			Taken[i] = 1;
			PhFreq[Pron_Count] = NewTempSFreq[i];
			Pron_Count++;
		} else {
			//k = 0;
		//	while(  strcmp(NewTempPron[i], Phon[k]) != 0) k++;
		//	PhFreq[k] = NewTempSFreq[i];
		}
	}

/*	fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"2 %d %d %d \n",Word_Count, Pron_Count, count);
	fclose(fp2);*/

	if(ENGLISH == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0)) ;  
	} else if (ITALIAN == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(606000000.0+2.0)) / log10(606000000.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(606000000.0+2.0)) / log10(606000000.0+2.0)) ;  
	} else if (GERMAN == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(150507.0+2.0)) / log10(150507.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(150507.0+2.0)) / log10(150507.0+2.0)) ;  
	} else if (FRENCH == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(867040.78+2.0)) / log10(867040.78+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(867040.78+2.0)) / log10(867040.78+2.0)) ;  
	}
	//for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0) ) ;
	//for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0)) ;  
	
/*	fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"3 %d %d %d \n",Word_Count, Pron_Count, count);
	fclose(fp2);*/

	// Connect the new lexicons //
	for(i=0;i<Word_Count;i++) NWord_to_Phon[i] = 0;
	for(i=0;i<Pron_Count;i++) NPhon_to_Word[i] = 0;


	//exit(1);

	for(i=0;i<count;i++) {
		for(j=0;j<Pron_Count;j++) {
			if( ((strcmp(NewTempPron[i], Phon[j])) == 0) && (NewStressPos[i] == StressPos[j])) break;
		}
		for(k=0;k<Word_Count;k++) {
			if( (strcmp(NewTempWord[i], Word[k])) == 0 ) break;
		}
		//if(j == Pron_Count) exit(1); //Something has gone wrong :(
		Word_to_Phon[k][ NWord_to_Phon[k] ] = j;
		NWord_to_Phon[k] += 1;
		Phon_to_Word[j][ NPhon_to_Word[j] ] = k;
		NPhon_to_Word[j] += 1;
	}

//	exit(1);

	for(i=0;i<Word_Count;i++) {
		for(j=0;j<strlen(Word[i]);j++) {
			if(ENGLISH == 1) Word[i][j] = toupper(Word[i][j]);
		}
		for(;j<MAXPOS;j++) {
			Word[i][j] = '+';
		}
	}
	Word[i][MAXPOS] = 0;

	fp2 = fopen("Neighinfo.txt","a");

	fprintf(fp2,"NWords=%d NProns=%d Phon=%s \n",Word_Count-1, Pron_Count-1, NewTempPron[0]);
	for(i=1;i<Word_Count;i++) {
		fprintf(fp2,"%d %s %f ",i, Word[i], Freq[i]);
		for(j=0;j<NWord_to_Phon[i];j++) {
			if(j>0) fprintf(fp2,"%d %s %f ",i, Word[i], Freq[i]);
			fprintf(fp2,"[%s] %d %f %d \n",Phon[Word_to_Phon[i][j]],Word_to_Phon[i][j], PhFreq[Word_to_Phon[i][j]],NewStressPos[Word_to_Phon[i][j]]);
		}
		//fprintf(fp2,"\n");
	}
	fclose(fp2);

/*	fp2 = fopen("test111111111.txt","w");
	fprintf(fp2,"%s \n",LoadDirectory);
	fclose(fp2);*/
	return Pron_Count;
}



void WordDBFuncs::RimeOnsetLength() 
{
	int i, freq, freq2;
	char word1[100],pron1[100];



	FILE *fp, *fp2;
	fp2 = fopen("Spokefre","r");
	fp = fopen("OnsetLengths","w");

	for(i=0;i<7446;i++) {
		fscanf(fp2,"%s%s%d%d",word1,pron1,&freq,&freq2);
		GetBod(word1);
		fprintf(fp,"%s %s %d %d %d %d \n",word1,pron1,freq,freq2,LastOnsetLength,LastBodLength);
	}
	fclose(fp);
}
		
int WordDBFuncs::HFNNeighbours(char *The_Word, float WordFreq)
{
	FILE *word;
	int i,j,k,bit_count = 0, Word_Len = 0;
	
//	word = fopen("crap","a");
	
	 neighbourfreqs = 0;

	//while( (Word_Len < MAXPOS) && (The_Word[Word_Len] != '+') ) Word_Len++;
	Word_Len = strlen(The_Word);

	for(j=0;j<Word_Count;j++) {
	
		k = 0;
		while(Word[j][k] != WORDPAD && k < MAXPOS) k++;
//		fprintf(word,"%s %s %d %d\n",Word[j],The_Word, k, Word_Len);
		if(Word_Len == k && WordFreq < Freq[j]) {
			count = 0;
			for(i=0;i<k;i++) {
				if(Word[j][i] != The_Word[i]) count++;
				if(count == 2) break;
			}
			if(count == 1 ) {
				bit_count++;
				neighbourfreqs += pow(10.0, (Freq[j] + 1.0) * word_log_divisor);
			}
		}
	}
//	fclose(word);
	return(bit_count);
}





int WordDBFuncs::levenshtein( char *a, char *b )
{
	int i, low;
	int j;
	int cost;
	int d[50][50];
	int x,y,z;
 
	if ( strlen(a) == 0 ) return strlen(b);
	if ( strlen(b)== 0) return strlen(a);

 
	for ( i = 0; i <= strlen(a); i++ ) {
		d[ i ][ 0 ] = i;
	}
 	for ( j = 0; j <= strlen(b); j++ ) {
		d[ 0 ][ j ] = j;
	}
 
	for ( i = 1; i <= strlen(a); i++ )	{
		for ( j = 1; j <= strlen(b); j++ ) {
			if ( a[i - 1] == b[j - 1] ) cost = 0;
			else cost = 1;
			//if(d[ i - 1 ][ j ] + 1 <  d[ i ][ j - 1 ] + 1)
			x = d[ i - 1 ][ j ] + 1;
			y = d[ i ][ j - 1 ] + 1;
			z = d[ i - 1 ][ j - 1 ] + cost;
			if(x>y) x = y;
			else if (x > z) x = z;
			d[i][j] = x;
			//d[ i ][ j ] = (std::min)(d[ i - 1 ][ j ] + 1, d[ i ][ j - 1 ] + 1, d[ i - 1 ][ j - 1 ] + cost );
	
		}
	}
 
	return d[ strlen(a)][ strlen(b)];
}

//int WordDBFuncs::compare(struct Lev *a, struct Lev *b)
int /*WordDBFuncs::*/compare(const void * a, const void * b)
{
	struct WordDBFuncs::Lev *sp1 = (WordDBFuncs::Lev*)a;
	struct WordDBFuncs::Lev *sp2 = (WordDBFuncs::Lev*)b;

	if(sp1->LOD > sp2->LOD) return 1;
	else if (sp1->LOD < sp2->LOD) return -1;
	else return 0;
}

float WordDBFuncs::levenshteintop20( char *TheWord, int OorP)
{
	FILE *fp, *fp2;
	char Test2[100], Test3[100];

	typedef int (*compfn)(const void*, const void*);

	char FileWord[100],ParsedOrth[100],FilePhon[100],FilePhon2[100],NormalPhon[100];
	int TheStress, NumSylls, bit_count, NeighCount1, i,j, UpTo = 0, wordid;
	float Freq1, Freq2;
	
	float sum20;

	for(i=0;i<strlen(LoadDirectory);i++) {
		if(LoadDirectory[i] == '\\') Test2[i] = '/';
		else Test2[i] = LoadDirectory[i];
	}
	Test2[i] = 0;

	if(ENGLISH == 1) sprintf(Test3,"%s/EnglishRes/EnglishNewDB.txt",Test2);
	else if (ITALIAN == 1)sprintf(Test3,"%s/ItalianRes/ItalianDB.txt",Test2);
	else if (GERMAN == 1) sprintf(Test3,"%s/GermanRes/GermanDB.txt",Test2);

	fp = fopen(Test3,"r");

    
	while ( (fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid)) != EOF) {		
		if(ENGLISH == 1 || GERMAN == 1) {
			FilePhon2[16] = 0;
		}

		if(OorP == 0) { //letters
			if(strcmp(FileWord, TheWord) != 0) {	
				LevValues[UpTo].LOD = levenshtein(TheWord, FileWord);
				LevValues[UpTo].freq = /*Freq1*/ log(Freq1 + 1.0);
				strcpy(LevValues[UpTo].Word, FileWord);
				UpTo++;
			}
		} else { //phonemes
			if(strcmp(FileWord, TheWord) != 0) {	
				LevValues[UpTo].LOD = levenshtein(TheWord, FilePhon);
				LevValues[UpTo].freq = /*Freq2*/ log(Freq2 + 1.0);
				strcpy(LevValues[UpTo].Word, /*FilePhon*/NormalPhon);
				UpTo++;
			}
		}
	}
	qsort((void *)LevValues, UpTo, sizeof(struct Lev), /*(compfn)*/ compare);
	fclose(fp);

	freq20 = 0;
	sum20 = 0;
	for(i=0;i<20;i++) {
		sum20 += LevValues[i].LOD;
		freq20 += LevValues[i].freq;
		/*fp2 = fopen("testing22.txt","a");
		fprintf(fp2,"%d %s \n",LevValues[i].LOD,LevValues[i].Word);
		fclose(fp2);*/
	}
	sum20 = sum20/20;
	freq20 = freq20/20.0;

	return(sum20);
}





int WordDBFuncs::Neighbours(char *The_Word) 
{
	FILE *word, *fp,*fp2;
	char FileWord[100],ParsedOrth[100],FilePhon[100],NormalPhon[100];
	char FilePhon2[100], Test3[100], Test2[100];
	int TheStress, NumSylls, bit_count, NeighCount1, i,j,wordid;
	float Freq1, Freq2;


	for(i=0;i<strlen(LoadDirectory);i++) {
		if(LoadDirectory[i] == '\\') Test2[i] = '/';
		else Test2[i] = LoadDirectory[i];
	}
	Test2[i] = 0;

	if(ENGLISH == 1) sprintf(Test3,"%s/EnglishRes/EnglishNewDB.txt",Test2);
	else if (ITALIAN == 1)sprintf(Test3,"%s/ItalianRes/ItalianDB.txt",Test2);
	else if (GERMAN == 1) sprintf(Test3,"%s/GermanRes/GermanDB.txt",Test2);

	fp = fopen(Test3,"r");
    
	bit_count = 0;
	neighbourfreqs = 0;
	while ( (fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid)) != EOF) {		
		if(ENGLISH == 1 || GERMAN == 1) {
			FilePhon2[16] = 0;
		}
		if(strlen(The_Word) == strlen(FileWord)) {
			if( (strcmp(FileWord,The_Word)) == 0) break; 
			NeighCount1 = 0;
			for(j=0;j<strlen(FileWord);j++) {
				if(The_Word[j] != FileWord[j]) NeighCount1++;
				if(NeighCount1 == 2) break;
			}
			if(NeighCount1 == 1) {
				//fp2 = fopen("here2.txt","a");fprintf(fp2,"bbb %s %s\n",The_Word, FileWord);fclose(fp2);
				bit_count++;
				neighbourfreqs += /*pow((float) 10.0, (float) Freq1 * word_log_divisor )*/log(Freq1+1.0);
			}
		}
	}
	fclose(fp);
	return(bit_count);
}

void WordDBFuncs::CalcAllBods()
{
	int i,j,k;
	char bods[2000][10];
	int bodcount[2000];
	int UpTo;

	char tempvowel[100];
	char Tvowels[2000][10];
	int Tvowelcount[2000];
	int UpToVowel = 0;


	FILE *fp;


	UpTo = 0;

	for(i=0;i<Word_Count;i++) {
		GetBod(Word[i]);
		for(j=0;j<UpTo;j++) {
			if( (strcmp(bods[j],LastBod)) == 0) {
				break;
			}
		}
		fp = fopen("AllBods.txt","a");
		fprintf(fp,"-->%s %s \n",Word[i], LastBod);
		fclose(fp);

		if(j==UpTo) {
			strcpy(bods[j],LastBod);
			bodcount[j] = BodNeighbours(LastBod);
		//	fp = fopen("AllBods","a");
		//	fprintf(fp,"%s %d \n",LastBod,bodcount[j]);
		//	fclose(fp);
			UpTo++;

		}
	}
	fp = fopen("AllBods2.txt","w");

	for(i=0;i<UpTo;i++) {
		fprintf(fp,"%s %d \n",bods[i],bodcount[i]);
	}
	fclose(fp);
//f	exit(1);
	for(i=0;i<UpTo;i++) {
		for(j=0;j<strlen(bods[i]);j++) {
			if(ConsonantNum(bods[i][j]) != -1) {
				tempvowel[j] = 0;
				break;
			} else {
				tempvowel[j] = bods[i][j];
			}
		}
		for(j=0;j<UpToVowel;j++) {
			if( (strcmp(tempvowel,Tvowels[j])) == 0) break;
		}
		if(j==UpToVowel) {
			strcpy(Tvowels[j], tempvowel);
			UpToVowel++;
		}
	}
	fp = fopen("Vowelstest.txt","w");
	for(i=0;i<UpToVowel;i++) {
		fprintf(fp,"%s \n",Tvowels[i]);
	}
	fclose(fp);

}


void WordDBFuncs::OnePlusNeigh()
{
	FILE *fp;
	int i,j,k;
	char Word1[10],Word2[10];

}


void WordDBFuncs::FunnyNeigh(char *The_Word)
{
	char wd[50];
	char ph[50];
	char NewNeighWord[50];
	char NewAssPron[50];

	float F, SF;
	int Word_Len, bit_count, Neigh_Count, NeighCount1, wordid;
	float SumFreq = 0;
	
	int nthisword = 0;
	int thisword[50];


	int h,i, j = 0, k,l, count = 0, len;

	char FileWord[100], FilePhon[100], FilePhon2[100], Orth[100], NewWord[100], NormalPhon[100];
	float Freq1, Freq2, TempFreq = 0;
	FILE *fp;
	int TheStress;
	
}

int WordDBFuncs::ReLoadData(char *NeighWord)
{
	return TRUE;
}

void WordDBFuncs::CalcSecondSyllConsis() 
{
	FILE *fp,*fp2;
	char Word[100];
}


void WordDBFuncs::AddWordOrth(char *theword, char *thephon, int thestress, float freq, float phonfreq)
{
	int i,j;
	char NewWord[30];
	char NewPhon[30];

	for(i=0;i<strlen(theword);i++) NewWord[i] = theword[i];
	for(i=strlen(thephon);i<16;i++) NewWord[i] = '+';
	NewWord[i] = 0;
	for(i=0;i<strlen(thephon);i++) NewPhon[i] = thephon[i];
	for(i=strlen(thephon);i<16;i++) NewPhon[i] = '+';
	NewPhon[i] = 0;

	strcpy(Phon[Pron_Count], NewPhon);
	strcpy(Word[Word_Count], NewWord);

	NWord_to_Phon[Word_Count] = 1;   
	NPhon_to_Word[Pron_Count] = 1;
	Word_to_Phon[Word_Count][0] = Pron_Count;
	Phon_to_Word[Pron_Count][0] = Word_Count;

	if(thestress == 1 || thestress == 2) {
		StressPos[Word_Count] = thestress - 1;
	} else {
		StressPos[Word_Count] = 0; //Trochaic Default
	}

	Pron_Count++;
	Word_Count++;
	//Word_Count

}


int WordDBFuncs::ReLoadDataSlow(char *NeighWord, char *AssPron)
{
	FILE *fp, *fp2, *fp3;

	//These two are for later
	int Number;

	char wd[50];
	char ph[50];
	char NewNeighWord[50];
	char NewAssPron[50];

	float F, SF;
	int Word_Len, bit_count, Neigh_Count, NeighCount1, wordid;
	float SumFreq = 0;
	
	int nthisword = 0;
//	int thisword[50];


	int h,i, j = 0, k,l, count = 0, len;

	char FileWord[100], FilePhon[100], FilePhon2[100], Orth[100], NewWord[100], NormalPhon[100];
	float Freq1, Freq2, TempFreq = 0;
	int TheStress, NumSylls;
	int startspot = 0;

	char TheFileName[1000];
	char Test2[1000], Test3[1000];

	for(i=0;i<strlen(LoadDirectory);i++) {
		if(LoadDirectory[i] == '\\') Test2[i] = '/';
		else Test2[i] = LoadDirectory[i];
	}
	Test2[i] = 0;
	
		
	//if(ENGLISH == 1) sprintf(Test3,"%s/EnglishRes/EnglishNewDB2.txt",Test2);
	if(ENGLISH == 1) sprintf(Test3,"./cdp/./EnglishRes/EnglishNewDB2.txt");
	else if (ITALIAN == 1) sprintf(Test3,"%s/ItalianRes/ItalianDB.txt",Test2);
	else if (GERMAN == 1) sprintf(Test3,"%s/GermanRes/GermanDB.txt",Test2);
	else if (FRENCH == 1) sprintf(Test3,"%s/FrenchRes/FrenchDB.txt",Test2);

/*	fp2 = fopen("test1112.txt","w");
	fprintf(fp2,"%s \n",Test3);
	fclose(fp2);*/

	i = 0;
	while(NeighWord[i] != '+' && i < MAXPOS) i++;
	
	//if(NeighWord[i] == '+') i--;
	for(j=0;j<i;j++) {
		if(ENGLISH == 1) NewWord[j] = tolower(NeighWord[j]);
		else NewWord[j] = NeighWord[j];
	}
	NewWord[j] = 0;

	/*fp2 = fopen("resinfo22.txt","a");
	fprintf(fp2,"%s %s %d\n",NeighWord, NewWord, j);
	fclose(fp2);*/

//	fp = fopen("c:/Work/NewModels/NewMultiSyllStress2009.Jan/NewLex4.3.2.txt","r");
	fp = fopen(Test3,"r");

	//printf("File  %s %d \n",Test3,fp);

	strcpy(TheFoundWord,"XXXXXX");
	if(fp == 0) 
    {
        _exit(1);
    }

	Word_Count = 0;
	Pron_Count = 0;
	FoundWords = 0;
	startspot = 0;

	//for(i=0;i<32270 /*34012*/;i++) {
	//	fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid);		
	while ( (fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid)) != EOF) {		
		if(ENGLISH == 1 || GERMAN == 1) {
			FilePhon2[16] = 0;
		}
	
	/*fp3 = fopen("Allinfo22.txt","a");
			if(fp3 == 0) exit(1);
			fprintf(fp3,"%d %s %s %d %f %f %s %d \n",i, FileWord,FilePhon2,TheStress, Freq1, Freq2, NormalPhon, wordid);		
			fclose(fp3);*/
	
		
		if( (strcmp(NewWord, FileWord)) == 0) {						
			
			/*fp3 = fopen("Allinfo22.txt","a");
			if(fp3 == 0) exit(1);
			fprintf(fp3,"%d %s %s %d %f %f %s %d \n",i, FileWord,FilePhon2,TheStress, Freq1, Freq2, NormalPhon, wordid);		
			fclose(fp3);*/
			
		/*	if(TheStress == 1 || TheStress == 2) {
				NewStressPos[startspot] = TheStress - 1;
			} else {
				NewStressPos[startspot] = 0; //Trochaic Default
			}*/
			
			if(Freq1 < 1) Freq1 = 1;
			if(Freq2 < 1) Freq2 = 1;
			
			FoundWords=1;
			thisword[nthisword] = wordid;
			nthisword++;
			strcpy(NewTempWord[startspot],FileWord);
			strcpy(NewTempPron[startspot],FilePhon2);
			NewTempFreq[startspot] = Freq1;
			NewTempSFreq[startspot] = Freq2;
			NewStressPos[startspot] = TheStress;
			startspot++;			
		}
	}
	count = startspot;
	//if(count > 5000) count = 5000;

	fclose(fp);

	for(i=0;i<count;i++) WTaken[i] = 0;
	Word_Count = 0;

	//Squish Orth - Take Out Dups
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(WTaken[j] == 1) {
					if( (strcmp(NewTempWord[i], NewTempWord[j]) ) == 0 ) {
						break;
					}
				}
			}
		}
		if(j == count) {
			strcpy(Word[Word_Count], NewTempWord[i]);
			WTaken[i] = 1;
			Freq[Word_Count] = NewTempFreq[i]; 
			Word_Count++;
		} else {
			//k = 0;
		//	while( strcmp(NewTempWord[i], Word[k]) != 0) k++; 
		//	Freq[k] = NewTempFreq[i];
		}
	}


	/*fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"1 %d %d %d \n",Word_Count, Pron_Count, count);
	fclose(fp2);*/

	//Squish Phon - Take Out Dups
	for(j=0;j<count;j++) Taken[j] = 0;

	Pron_Count = 0;
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(Taken[j] == 1) {
					if( (strcmp(NewTempPron[i], NewTempPron[j]) ) == 0 && (NewStressPos[i] == NewStressPos[j]) ) {				
						break;
					}
				}		
			}
		}
		if(j == count) {
			strcpy(Phon[Pron_Count], NewTempPron[i]);
			StressPos[Pron_Count] = NewStressPos[i];
			Taken[i] = 1;
			PhFreq[Pron_Count] = NewTempSFreq[i];
			Pron_Count++;
		} else {
			//k = 0;
		//	while(  strcmp(NewTempPron[i], Phon[k]) != 0) k++;
		//	PhFreq[k] = NewTempSFreq[i];
		}
	}

/*	fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"2 %d %d %d \n",Word_Count, Pron_Count, count);
	fclose(fp2);*/

	if(ENGLISH == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0)) ;  
	} else if (ITALIAN == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(606000000.0+2.0)) / log10(606000000.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(606000000.0+2.0)) / log10(606000000.0+2.0)) ;  
	} else if (GERMAN == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(150507.0+2.0)) / log10(150507.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(150507.0+2.0)) / log10(150507.0+2.0)) ;  
	} else if (FRENCH == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(867040.78+2.0)) / log10(867040.78+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(867040.78+2.0)) / log10(867040.78+2.0) ) ;  
	}
	//for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0) ) ;
	//for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0)) ;  
	
/*	fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"3 %d %d %d \n",Word_Count, Pron_Count, count);
	fclose(fp2);*/

	// Connect the new lexicons //
	for(i=0;i<Word_Count;i++) NWord_to_Phon[i] = 0;
	for(i=0;i<Pron_Count;i++) NPhon_to_Word[i] = 0;


	//exit(1);

	for(i=0;i<count;i++) {
		for(j=0;j<Pron_Count;j++) {
			if( ((strcmp(NewTempPron[i], Phon[j])) == 0) && (NewStressPos[i] == StressPos[j])) break;
		}
		for(k=0;k<Word_Count;k++) {
			if( (strcmp(NewTempWord[i], Word[k])) == 0 ) break;
		}
		//if(j == Pron_Count) exit(1); //Something has gone wrong :(
		Word_to_Phon[k][ NWord_to_Phon[k] ] = j;
		NWord_to_Phon[k] += 1;
		Phon_to_Word[j][ NPhon_to_Word[j] ] = k;
		NPhon_to_Word[j] += 1;
	}

//	exit(1);

	for(i=0;i<Word_Count;i++) {
		for(j=0;j<strlen(Word[i]);j++) {
			if(ENGLISH == 1) Word[i][j] = toupper(Word[i][j]);
		}
		for(;j<MAXPOS;j++) {
			Word[i][j] = '+';
		}
	}
	Word[i][MAXPOS] = 0;

	/*fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"%d %d %d %s \n",Word_Count, Pron_Count, count, NewAssPron);
	for(i=0;i<Word_Count;i++) {
		fprintf(fp2,"%d %s %f ",i, Word[i], Freq[i]);
		for(j=0;j<NWord_to_Phon[i];j++) {
			fprintf(fp2,"[%s] %d %f %d ",Phon[Word_to_Phon[i][j]],Word_to_Phon[i][j], PhFreq[Word_to_Phon[i][j]],NewStressPos[Word_to_Phon[i][j]]);
		}
		fprintf(fp2,"\n");
	}
	fclose(fp2);*/

/*	fp2 = fopen("test111111111.txt","w");
	fprintf(fp2,"%s \n",LoadDirectory);
	fclose(fp2);*/
	return TRUE;

}




/////////////////////////////// This is the one
int WordDBFuncs::MarcoLoadData()
{

	FILE *fp;

	//These two are for later

	char wd[40];
	char ph[40];
	char Seg[40];
	float F, SF;


	FILE *fp1, *fp2;
	int i, j = 0, k, count = 0, len, len2;


	return TRUE; 
}





int WordDBFuncs::ReLoadDataSlow2(char *NeighWord, char *AssPron)
{
	FILE *fp, *fp2, *fp3;

	//These two are for later
	int Number;

	char wd[50];
	char ph[50];
	char NewNeighWord[50];
	char NewAssPron[50];

	float F, SF;
	int Word_Len, bit_count, Neigh_Count, NeighCount1, wordid;
	float SumFreq = 0;
	
	int nthisword = 0;
	//int thisword[50];


	int h,i, j = 0, k,l, count = 0, len;

	char FileWord[100], FilePhon[100], FilePhon2[100], Orth[100], NewWord[100], NormalPhon[100];
	float Freq1, Freq2, TempFreq = 0;
	int TheStress, NumSylls;
	int startspot = 0;

	char TheFileName[1000];
	char Test2[1000], Test3[1000];

	for(i=0;i<strlen(LoadDirectory);i++) {
		if(LoadDirectory[i] == '\\') Test2[i] = '/';
		else Test2[i] = LoadDirectory[i];
	}
	Test2[i] = 0;
	
		
	if(ENGLISH == 1) sprintf(Test3,"%s/EnglishRes/EnglishNewDB2.txt",Test2);
	//if(ENGLISH == 1) sprintf(Test3,"/home/Conrad/CDP/EnglishNewDB.txt");
	else if (ITALIAN == 1) sprintf(Test3,"%s/ItalianRes/ItalianDB.txt",Test2);
	else if (GERMAN == 1) sprintf(Test3,"%s/GermanRes/GermanDB.txt",Test2);
	else if (FRENCH == 1) sprintf(Test3,"%s/FrenchRes/FrenchDB.txt",Test2);


	/*fp2 = fopen("test1112.txt","w");
	fprintf(fp2,"%s \n",Test3);
	fclose(fp2);*/

	i = 0;
	while(NeighWord[i] != '+' && i < MAXPOS) i++;
	
	//if(NeighWord[i] == '+') i--;
	for(j=0;j<i;j++) {
		if(ENGLISH == 1) NewWord[j] = tolower(NeighWord[j]);
		else NewWord[j] = NeighWord[j];
	}
	NewWord[j] = 0;

	/*fp2 = fopen("resinfo22.txt","a");
	fprintf(fp2,"%s %s %d\n",NeighWord, NewWord, j);
	fclose(fp2);*/

//	fp = fopen("c:/Work/NewModels/NewMultiSyllStress2009.Jan/NewLex4.3.2.txt","r");
	fp = fopen(Test3,"r");

	strcpy(TheFoundWord,"XXXXXX");
	if(fp == 0) exit(1);

	Word_Count = 0;
	Pron_Count = 0;
	FoundWords = 0;
	startspot = 0;

	//for(i=0;i<32270 /*34012*/;i++) {
	//	fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid);		
	while ( (fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid)) != EOF) {		
		if(ENGLISH == 1 || GERMAN == 1) {
			FilePhon2[16] = 0;
		}
	
	/*fp3 = fopen("Allinfo22.txt","a");
			if(fp3 == 0) exit(1);
			fprintf(fp3,"%d %s %s %d %f %f %s %d \n",i, FileWord,FilePhon2,TheStress, Freq1, Freq2, NormalPhon, wordid);		
			fclose(fp3);*/
	
		
		if( (strcmp(NewWord, FileWord)) == 0) {						
			
			/*fp3 = fopen("Allinfo22.txt","a");
			if(fp3 == 0) exit(1);
			fprintf(fp3,"%d %s %s %d %f %f %s %d \n",i, FileWord,FilePhon2,TheStress, Freq1, Freq2, NormalPhon, wordid);		
			fclose(fp3);*/
			
		/*	if(TheStress == 1 || TheStress == 2) {
				NewStressPos[startspot] = TheStress - 1;
			} else {
				NewStressPos[startspot] = 0; //Trochaic Default
			}*/
			
			if(Freq1 < 1) Freq1 = 1;
			if(Freq2 < 1) Freq2 = 1;
			
			FoundWords=1;
			thisword[nthisword] = wordid;
			nthisword++;
			strcpy(NewTempWord[startspot],FileWord);
			strcpy(NewTempPron[startspot],FilePhon2);
			NewTempFreq[startspot] = Freq1;
			NewTempSFreq[startspot] = Freq2;
			NewStressPos[startspot] = TheStress;
			startspot++;			
		}
	}

/*	fp3 = fopen("Allinfo22.txt","a");
	fprintf(fp3,"**\n");
	fclose(fp3);*/
	//return 1;

	//*add neighbors etc. *//
	
	strcpy(NewAssPron,AssPron);
	//strcpy(NewNeighWord,NeighWord);
	for(i=0;i<strlen(NewAssPron);i++) {
		if(NewAssPron[i] == '+') {
			NewAssPron[i] = 0;
			break;
		}
	}

	/*fp3 = fopen("Allinfo111.txt","a");
	fprintf(fp3,"%s %s\n", NewAssPron, AssPron);
	fclose(fp3);*/


	//Phon neighbors.
	NeighCount1 = 0;
	for(h=0;h<startspot;h++) {
		rewind(fp);
		while ( (fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid)) != EOF) {		
			if(ENGLISH == 1 || GERMAN == 1) FilePhon2[16] = 0;
			if(strlen(NewTempPron[h]) <= strlen(NormalPhon)) {
				for(j=0;j<nthisword;j++) {
					if(thisword[j] == wordid) break;
				}
				if(j==nthisword) {
					NeighCount1 = 0;
					for(j=0;j<strlen(NewTempPron[h]);j++) {
						if(NewTempPron[h][j] != NormalPhon[j]) NeighCount1++;
						if(NeighCount1 == /*2*/ 3) break;
					}
					NeighCount1 += (strlen(NormalPhon) - strlen(NewTempPron[h]));
					if(NeighCount1 > 0 && NeighCount1 < 3) {
						for(k=0;k<startspot;k++) {
							if( ((strcmp(NewTempWord[k], FileWord)) == 0) &&((strcmp(NewTempPron[k], FilePhon2)) == 0) && (NewStressPos[k] == TheStress - 1)) break;
						}
						if(k==startspot) {
							strcpy(NewTempWord[startspot],FileWord);
							strcpy(NewTempPron[startspot],FilePhon2);
							NewTempFreq[startspot] = Freq1;
							NewTempSFreq[startspot] = Freq2;
	
							if(TheStress == 1 || TheStress == 2) NewStressPos[startspot] = TheStress - 1;
							else NewStressPos[startspot] = 0; //Trochaic Default			
							startspot++;
						}
					}
				}
			}
		}

	}


	///Orthographic Neighbors
	rewind(fp);
	while ( (fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid)) != EOF) {		
		if(ENGLISH == 1 || GERMAN == 1) FilePhon2[16] = 0;
		if(strlen(NewWord) <= strlen(FileWord) || NewWord[0] == FileWord[0]) {
			for(j=0;j<nthisword;j++) {
				if(thisword[j] == wordid) break;
			}
			if(j==nthisword) {
				NeighCount1 = 0;
				for(j=0;j<strlen(NewWord);j++) {
					if(NewWord[j] != FileWord[j]) NeighCount1++;
					if(NeighCount1 == 3) break;
				}
				NeighCount1 += (strlen(FileWord) - strlen(NewWord));
				if(NeighCount1 > 0 && NeighCount1 < 3 || NewWord[0] == FileWord[0]) {
					for(k=0;k<startspot;k++) {
						if( ((strcmp(NewTempWord[k], FileWord)) == 0) &&((strcmp(NewTempPron[k], FilePhon2)) == 0) && (NewStressPos[k] == TheStress - 1)) break;
					}
					if(k==startspot) {
						strcpy(NewTempWord[startspot],FileWord);
						strcpy(NewTempPron[startspot],FilePhon2);
						NewTempFreq[startspot] = Freq1;
						NewTempSFreq[startspot] = Freq2;

						if(TheStress == 1 || TheStress == 2) NewStressPos[startspot] = TheStress - 1;
						else NewStressPos[startspot] = 0; //Trochaic Default			
						startspot++;
					}
				}
			}
		}
	}


	//Phonlogical Neighbors
	rewind(fp);
	while ( (fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid)) != EOF) {		
		if(ENGLISH == 1 || GERMAN == 1) FilePhon2[16] = 0;
		if(strlen(NewAssPron) < strlen(NormalPhon) || NewAssPron[0] == NormalPhon[0]) {
	//	if(strlen(NewNeighWord) == strlen(NormalPhon)) {
			for(j=0;j<nthisword;j++) {
				if(thisword[j] == wordid) break;
			}
			if(j==nthisword) {
				NeighCount1 = 0;
				for(j=0;j<strlen(NewAssPron);j++) {
				//for(j=0;j<strlen(NewNeighWord);j++) {
					if(NewAssPron[j] != NormalPhon[j]) NeighCount1++;
				//	if(NewNeighWord[j] != NormalPhon[j]) NeighCount1++;
					if(NeighCount1 == 3) break;
				}
				NeighCount1 += (strlen(NormalPhon) - strlen(NewAssPron));
				if(NeighCount1 < 3 || NewAssPron[0] == NormalPhon[0]) {
			//	exit(1);
					for(k=0;k<startspot;k++) {
						if( ((strcmp(NewTempWord[k], FileWord)) == 0) && ((strcmp(NewTempPron[k], FilePhon2)) == 0) && (NewStressPos[k] == TheStress - 1)) break;
					}
					if(k==startspot) {
						strcpy(NewTempWord[startspot],FileWord);
						strcpy(NewTempPron[startspot],FilePhon2);
						NewTempFreq[startspot] = Freq1;
						NewTempSFreq[startspot] = Freq2;
	
						if(TheStress == 1 || TheStress == 2) NewStressPos[startspot] = TheStress - 1;
						else NewStressPos[startspot] = 0; //Trochaic Default			
						startspot++;


//	fp2 = fopen("resinfo333.txt","a");
//	fprintf(fp2,"%s %s %d \n", FileWord, FilePhon2, TheStress - 1);
//	fclose(fp2);
					}
				}
			}
		}
	}

	fclose(fp);
	


/*	fp2 = fopen("resinfo2222.txt","a");
	fprintf(fp2,"ss = %d %s  \n",startspot, NewAssPron);
	for(i=0;i<startspot;i++) {
		fprintf(fp2,"%s %s %d\n",NewTempWord[i],NewTempPron[i], NewStressPos[i]);
	}
	fclose(fp2);*/

	count = startspot;
	//if(count > 5000) count = 5000;

	for(i=0;i<count;i++) WTaken[i] = 0;
	Word_Count = 0;

	//Squish Orth - Take Out Dups
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(WTaken[j] == 1) {
					if( (strcmp(NewTempWord[i], NewTempWord[j]) ) == 0 ) {
						break;
					}
				}
			}
		}
		if(j == count) {
			strcpy(Word[Word_Count], NewTempWord[i]);
			WTaken[i] = 1;
			Freq[Word_Count] = NewTempFreq[i]; 
			Word_Count++;
		} else {
			//k = 0;
		//	while( strcmp(NewTempWord[i], Word[k]) != 0) k++; 
		//	Freq[k] = NewTempFreq[i];
		}
	}


	/*fp2 = fopen("Neighinfo.2.txt","a");
	fprintf(fp2,"1 %d %d %d \n",Word_Count, Pron_Count, count);
	fclose(fp2);*/

	//Squish Phon - Take Out Dups
	for(j=0;j<count;j++) Taken[j] = 0;

	Pron_Count = 0;
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(Taken[j] == 1) {
					if( (strcmp(NewTempPron[i], NewTempPron[j]) ) == 0 && (NewStressPos[i] == NewStressPos[j]) ) {				
						break;
					}
				}		
			}
		}
		if(j == count) {
			strcpy(Phon[Pron_Count], NewTempPron[i]);
			StressPos[Pron_Count] = NewStressPos[i];
			Taken[i] = 1;
			PhFreq[Pron_Count] = NewTempSFreq[i];
			Pron_Count++;
		} else {
			//k = 0;
		//	while(  strcmp(NewTempPron[i], Phon[k]) != 0) k++;
		//	PhFreq[k] = NewTempSFreq[i];
		}
	}

/*	fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"2 %d %d %d \n",Word_Count, Pron_Count, count);
	fclose(fp2);*/

	if(ENGLISH == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0)) ;  
	} else if (ITALIAN == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(606000000.0+2.0)) / log10(606000000.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(606000000.0+2.0)) / log10(606000000.0+2.0)) ;  
	} else if (GERMAN == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(150507.0+2.0)) / log10(150507.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(150507.0+2.0)) / log10(150507.0+2.0)) ;  
	} else if (FRENCH == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(867040.78+2.0)) / log10(867040.78+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(867040.78+2.0)) / log10(867040.78+2.0)) ;  
	}
	//for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0) ) ;
	//for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0)) ;  
	
/*	fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"3 %d %d %d \n",Word_Count, Pron_Count, count);
	fclose(fp2);*/

	// Connect the new lexicons //
	for(i=0;i<Word_Count;i++) NWord_to_Phon[i] = 0;
	for(i=0;i<Pron_Count;i++) NPhon_to_Word[i] = 0;


	//exit(1);

	for(i=0;i<count;i++) {
		for(j=0;j<Pron_Count;j++) {
			if( ((strcmp(NewTempPron[i], Phon[j])) == 0) && (NewStressPos[i] == StressPos[j])) break;
		}
		for(k=0;k<Word_Count;k++) {
			if( (strcmp(NewTempWord[i], Word[k])) == 0 ) break;
		}
		//if(j == Pron_Count) exit(1); //Something has gone wrong :(
		Word_to_Phon[k][ NWord_to_Phon[k] ] = j;
		NWord_to_Phon[k] += 1;
		Phon_to_Word[j][ NPhon_to_Word[j] ] = k;
		NPhon_to_Word[j] += 1;
	}

//	exit(1);

	for(i=0;i<Word_Count;i++) {
		for(j=0;j<strlen(Word[i]);j++) {
			if(ENGLISH == 1) Word[i][j] = toupper(Word[i][j]);
		}
		for(;j<MAXPOS;j++) {
			Word[i][j] = '+';
		}
	}
	Word[i][MAXPOS] = 0;

/*	fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"%d %d %d %s \n",Word_Count, Pron_Count, count, NewAssPron);
	for(i=0;i<Word_Count;i++) {
		fprintf(fp2,"%d %s %f ",i, Word[i], Freq[i]);
		for(j=0;j<NWord_to_Phon[i];j++) {
			fprintf(fp2,"[%s] %d %f %d ",Phon[Word_to_Phon[i][j]],Word_to_Phon[i][j], PhFreq[Word_to_Phon[i][j]],NewStressPos[Word_to_Phon[i][j]]);
		}
		fprintf(fp2,"\n");
	}
	fclose(fp2);*/

/*	fp2 = fopen("test111111111.txt","w");
	fprintf(fp2,"%s \n",LoadDirectory);
	fclose(fp2);*/
	return TRUE;
}


//allwords
int WordDBFuncs::ReLoadDataSlow3(char *NeighWord, char *AssPron)
{
		FILE *fp, *fp2, *fp3;

	//These two are for later
	int Number;

	char wd[50];
	char ph[50];
	char NewNeighWord[50];
	char NewAssPron[50];

	float F, SF;
	int Word_Len, bit_count, Neigh_Count, NeighCount1, wordid;
	float SumFreq = 0;
	
	int nthisword = 0;
//	int thisword[50];


	int h,i, j = 0, k,l, count = 0, len;

	char FileWord[100], FilePhon[100], FilePhon2[100], Orth[100], NewWord[100], NormalPhon[100];
	float Freq1, Freq2, TempFreq = 0;
	int TheStress, NumSylls;
	int startspot = 0;

	char TheFileName[1000];
	char Test2[1000];
	char Test3[1000];

	for(i=0;i<strlen(LoadDirectory);i++) {
		if(LoadDirectory[i] == '\\') Test2[i] = '/';
		else Test2[i] = LoadDirectory[i];
	}
	Test2[i] = 0;
		
	if(ENGLISH == 1) sprintf(Test3,"%s/EnglishRes/EnglishNewDB2.txt",Test2);
	else if (ITALIAN == 1)sprintf(Test3,"%s/ItalianRes/ItalianDB.txt",Test2);
	else if (GERMAN == 1) sprintf(Test3,"%s/GermanRes/GermanDB.txt",Test2);
	else if (FRENCH == 1) sprintf(Test3,"%s/FrenchRes/FrenchDB.txt",Test2);
	
	//sprintf(Test3,"%s/NewLex4.3.2.txt",Test2);
/*	fp2 = fopen("test11111.txt","w");
	fprintf(fp2,"%s %s \n",LoadDirectory,Test3);
	fclose(fp2);*/

	i = 0;
	while(NeighWord[i] != '+' && i < 16) i++;
	
	//if(NeighWord[i] == '+') i--;
	
	for(j=0;j<i;j++) {
		if(ENGLISH == 1) NewWord[j] = tolower(NeighWord[j]);
		else NewWord[j] = NeighWord[j];
	}
	NewWord[j] = 0;

/*	fp2 = fopen("resinfo22.txt","a");
	fprintf(fp2,"1 %s %s %s %d\n",LoadDirectory, NeighWord, NewWord, j);
	fclose(fp2);*/

//	fp = fopen("c:/Work/NewModels/NewMultiSyllStress2009.Jan/NewLex4.3.2.txt","r");
	fp = fopen(Test3,"r");

	strcpy(TheFoundWord,"XXXXXX");
	if(fp == 0) exit(1);

	Word_Count = 0;
	Pron_Count = 0;
	FoundWords = 0;
	startspot = 0;
	i =0;
	//for(i=0;i<32270 /*34012*/;i++) {
		//fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid);		
	while ( (fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid)) != EOF) {		
//	if( (strcmp(NewWord, FileWord)) == 0) {						
			//if(strlen(FileWord) < 5) {
				i++;
				if(ENGLISH == 1 || GERMAN == 1) FilePhon2[16] = 0;
				if(1==1) {
			/*fp3 = fopen("Allinfo2.txt","a");
			if(fp3 == 0) exit(1);
			fprintf(fp3,"++ %d %s %s %d %f %f %s %d \n",i, FileWord,FilePhon2,TheStress, Freq1, Freq2, NormalPhon, wordid);		
			fclose(fp3);*/
	
					if(Freq1 < 1) Freq1 = 1;
					if(Freq2 < 1) Freq2 = 1;
			
					FoundWords=1;
					thisword[nthisword] = wordid;
					nthisword++;
					strcpy(NewTempWord[startspot],FileWord);
					strcpy(NewTempPron[startspot],FilePhon2);
					NewTempFreq[startspot] = Freq1;
					NewTempSFreq[startspot] = Freq2;
					startspot++;	
				}
			//}
	//	}
	}
	fclose(fp);

	/*	fp2 = fopen("resinfo22.txt","a");
	fprintf(fp2,"loaded %s %s %s %d\n",LoadDirectory, NeighWord, NewWord, j);
	fclose(fp2);*/

	count = startspot;

	for(i=0;i<count;i++) WTaken[i] = 0;
	Word_Count = 0;

	//Squish Orth - Take Out Dups
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(WTaken[j] == 1) {
					if( (strcmp(NewTempWord[i], NewTempWord[j]) ) == 0 ) {
						break;
					}
				}
			}
		}
		if(j == count) {
			strcpy(Word[Word_Count], NewTempWord[i]);
			WTaken[i] = 1;
			Freq[Word_Count] = NewTempFreq[i]; 
			Word_Count++;
		} else {
			//k = 0;
		//	while( strcmp(NewTempWord[i], Word[k]) != 0) k++; 
		//	Freq[j] = NewTempFreq[i];
		}
	}


	//Squish Phon - Take Out Dups
	for(j=0;j<count;j++) Taken[j] = 0;

	Pron_Count = 0;
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(Taken[j] == 1) {
					if( (strcmp(NewTempPron[i], NewTempPron[j]) ) == 0 && (NewStressPos[i] == NewStressPos[j]) ) {
						break;
					}
				}		
			}
		}
		if(j == count) {
			strcpy(Phon[Pron_Count], NewTempPron[i]);
			StressPos[Pron_Count] = NewStressPos[i];
			Taken[i] = 1;
			PhFreq[Pron_Count] = NewTempSFreq[i];
			Pron_Count++;
		} else {
			//k = 0;
		//	while(  strcmp(NewTempPron[i], Phon[k]) != 0) k++;
		//	PhFreq[j] = NewTempSFreq[i];
		}
	}
	//if(ENGLISH ==1) CreateGraphemePattern( log(thisfreq + 2)/log(1093547.0 + 2) );	
	//			else if (ITALIAN == 1) CreateGraphemePattern( log(thisfreq + 2)/log(606000000.0 + 2) );
	//			else if (GERMAN == 1) {
	//				if(thisnosylls < 4)	CreateGraphemePattern( log(thisfreq + 2)/log(150507.0 + 2) );
	//			}
	
	if(ENGLISH == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0)) ;  
	} else if (ITALIAN == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(606000000.0+2.0)) / log10(606000000.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(606000000.0+2.0)) / log10(606000000.0+2.0)) ;  
	} else if (GERMAN == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(150507.0+2.0)) / log10(150507.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(150507.0+2.0)) / log10(150507.0+2.0)) ;  
	} else if (FRENCH == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(867040.78+2.0)) / log10(867040.78+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(867040.78+2.0)) / log10(867040.78+2.0)) ;  
	}

	// Connect the new lexicons //
	for(i=0;i<Word_Count;i++) NWord_to_Phon[i] = 0;
	for(i=0;i<Pron_Count;i++) NPhon_to_Word[i] = 0;

	//exit(1);

	for(i=0;i<count;i++) {
		for(j=0;j<Pron_Count;j++) {
			if( ((strcmp(NewTempPron[i], Phon[j])) == 0) && (NewStressPos[i] == StressPos[j])) break;
		}
		for(k=0;k<Word_Count;k++) {
			if( (strcmp(NewTempWord[i], Word[k])) == 0 ) break;
		}
		//if(j == Pron_Count) exit(1); //Something has gone wrong :(
		Word_to_Phon[k][ NWord_to_Phon[k] ] = j;
		NWord_to_Phon[k] += 1;
		Phon_to_Word[j][ NPhon_to_Word[j] ] = k;
		NPhon_to_Word[j] += 1;
	}

//	exit(1);

	for(i=0;i<Word_Count;i++) {
		for(j=0;j<strlen(Word[i]);j++) {
			if(ENGLISH==1) Word[i][j] = toupper(Word[i][j]);
		}
		for(;j<MAXPOS;j++) {
			Word[i][j] = '+';
		}
	}

	//fp2 = fopen("resinfo2222.txt","a");
//	fprintf(fp2,"7 ss = %d \n",startspot);
	//for(i=0;i<startspot;i++) {
	//	fprintf(fp2,"%s %s %d\n",NewTempWord[i],NewTempPron[i], NewStressPos[i]);
	//}
//	fclose(fp2);


/*	fp2 = fopen("Neighinfo.txt","a");
	fprintf(fp2,"%d %d %d %s %s \n",Word_Count, Pron_Count, count, NewNeighWord,NewAssPron);
	/*for(i=0;i<Word_Count;i++) {
		fprintf(fp2,"%d %s %f ",i, Word[i], Freq[i]);
		for(j=0;j<NWord_to_Phon[i];j++) {
			fprintf(fp2,"[%s] %d %f ",Phon[Word_to_Phon[i][j]],Word_to_Phon[i][j], PhFreq[Word_to_Phon[i][j]]);
		}
		fprintf(fp2,"\n");
	}*/
/*	fclose(fp2);*/

/*	fp2 = fopen("test111111111.txt","w");
	fprintf(fp2,"%s \n",LoadDirectory);
	fclose(fp2);*/
	/*fp2 = fopen("resinfo22.txt","a");
	fprintf(fp2,"2 %s %s %s %d\n",LoadDirectory, NeighWord, NewWord, j);
	fclose(fp2);*/

	return TRUE;
}




int WordDBFuncs::ReLoadDataSlowPrime(char *NeighWord, char *AssPron, char *PrimeWord, char *AssPronPrime)
{
	FILE *fp, *fp2, *fp3;

	//These two are for later
	int Number;

	char wd[50], Test2[50], Test3[50];
	char ph[50];
	char NewNeighWord[50];
	char NewAssPron[50];

	float F, SF;
	int Word_Len, bit_count, Neigh_Count, NeighCount1, wordid;
	float SumFreq = 0;
	
	int nthisword = 0;
	int thisword[50];


	int h,i, j = 0, k,l, count = 0, len;

	char FileWord[100], FilePhon[100], FilePhon2[100], Orth[100], NewWord[100],  NormalPhon[100],NewWord2[100], FileWord2[100];
	float Freq1, Freq2, TempFreq = 0;


	int TheStress, NumSylls;
	int startspot = 0;

	i=0;

	NeighWord[16] = 0;
	AssPronPrime[16] = 0;
	PrimeWord[16] = 0;
	AssPron[16] = 0;


	while(NeighWord[i] != '+' && i < 16) i++;

	for(j=0;j<i;j++) {
		NewWord[j] = tolower(NeighWord[j]);
	}
	NewWord[j] = 0;

	i = 0;
	while(PrimeWord[i] != '+' && i < 16) i++;
	for(j=0;j<i;j++) {
		NewWord2[j] = tolower(PrimeWord[j]);
	}
	NewWord2[j] = 0;


	for(i=0;i<strlen(LoadDirectory);i++) {
		if(LoadDirectory[i] == '\\') Test2[i] = '/';
		else Test2[i] = LoadDirectory[i];
	}
	Test2[i] = 0;


	if(ENGLISH == 1) sprintf(Test3,"%s/EnglishRes/EnglishNewDB.txt",Test2);
	else if (ITALIAN == 1) sprintf(Test3,"%s/ItalianRes/ItalianDB.txt",Test2);
	else if (GERMAN == 1) sprintf(Test3,"%s/GermanRes/GermanDB.txt",Test2);
	else if (FRENCH == 1) sprintf(Test3,"%s/FrenchRes/FrenchDB.txt",Test2);

	fp = fopen(Test3,"r");

	strcpy(TheFoundWord,"XXXXXX");
	if(fp == 0) exit(1);

	Word_Count = 0;
	Pron_Count = 0;
	FoundWords = 0;


	startspot = 0;

	while ( (fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid)) != EOF) {		
		fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid);		
		if(ENGLISH == 1 || GERMAN == 1) FilePhon2[16] = 0;
	
		if( (strcmp(NewWord, FileWord)) == 0 || (strcmp(NewWord2, FileWord)) == 0) {				
			if(TheStress == 1 || TheStress == 2) {
				NewStressPos[startspot] = TheStress - 1;
			} else {
				NewStressPos[startspot] = 0; //Trochaic Default
			}
			if(Freq1 < 1) Freq1 = 1;
			if(Freq2 < 1) Freq2 = 1;

			FoundWords=1;
			thisword[nthisword] = wordid;
			nthisword++;
			strcpy(NewTempWord[startspot],FileWord);
			strcpy(NewTempPron[startspot],FilePhon2);
			NewTempFreq[startspot] = Freq1;
			NewTempSFreq[startspot] = Freq2;
			startspot++;			
		}
	}

	
/*	fp2 = fopen("resinfo321.txt","a");
	fprintf(fp2,"O = %d %s\n",startspot, NewNeighWord);
	for(i=0;i<startspot;i++) {
		fprintf(fp2,"%s %s \n",NewTempWord[i],NewTempPron[i]);
	}
	fclose(fp2);
	rewind(fp);*/

/*	fp3 = fopen("Allpseuds.txt","a");
	fprintf(fp3,"\n");
	fclose(fp3);
	fclose(fp);
	return(0);*/

//	fp2 = fopen("test555.txt","a");
//	fprintf(fp2,"%s %s  O\n",NeighWord, AssPron);
//	fclose(fp2);


	NeighCount1 = 0;
	//Phonlogical Neighbors of Assembled Phons
	
	//assembled prob
	strcpy(NewAssPron,AssPron);
	strcpy(NewNeighWord,NeighWord);
	for(i=0;i<strlen(NewAssPron);i++) {
		if(NewAssPron[i] == '+') {
			NewAssPron[i] = 0;
			break;
		}
	}

	//Adds the neighbors of the assembled phonologies
	for(h=0;h<startspot;h++) {
		rewind(fp);
		for(i=0;i<8000/*32270 *//*34012*/;i++) {
			fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid);		
			if(ENGLISH == 1 || GERMAN == 1) FilePhon2[16] = 0;
			if(strlen(NewTempPron[h]) == strlen(NormalPhon)) {
				for(j=0;j<nthisword;j++) {
					if(thisword[j] == wordid) break;
				}
				if(j==nthisword) {
					NeighCount1 = 0;
					for(j=0;j<strlen(NewTempPron[h]);j++) {
						if(NewTempPron[h][j] != NormalPhon[j]) NeighCount1++;
						if(NeighCount1 == 2) break;
					}
					if(NeighCount1 == 1) {
						for(k=0;k<startspot;k++) {
							if( ((strcmp(NewTempWord[k], FileWord)) == 0) &&((strcmp(NewTempPron[k], FilePhon2)) == 0) && (NewStressPos[k] == TheStress - 1)) break;
						}			
						if(k==startspot) {
							strcpy(NewTempWord[startspot],FileWord);
							strcpy(NewTempPron[startspot],FilePhon2);
							NewTempFreq[startspot] = Freq1;
							NewTempSFreq[startspot] = Freq2;
	
							if(TheStress == 1 || TheStress == 2) NewStressPos[startspot] = TheStress - 1;
							else NewStressPos[startspot] = 0; //Trochaic Default			
							startspot++;
						}
					}
				}
			}
		}
	}


/*	fp2 = fopen("resinfo32.txt","a");
	fprintf(fp2,"A = %d \n",startspot);
	for(i=0;i<startspot;i++) {
		fprintf(fp2,"%s %s \n",NewTempWord[i],NewTempPron[i]);
	}
	fclose(fp2);*/


	///Orthographic Neighbors
	rewind(fp);
	for(i=0;i<32270 /*34012*/;i++) {
		fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid);		
		if(ENGLISH == 1 || GERMAN == 1) FilePhon2[16] = 0;
		if(strlen(NewWord) == strlen(FileWord)) {
			for(j=0;j<nthisword;j++) {
				if(thisword[j] == wordid) break;
			}
			if(j==nthisword) {
				NeighCount1 = 0;
				for(j=0;j<strlen(NewWord);j++) {
					if(NewWord[j] != FileWord[j]) NeighCount1++;
					if(NeighCount1 == 2) break;
				}
				if(NeighCount1 == 1) {
		//	exit(1);
					for(k=0;k<startspot;k++) {
						if( ((strcmp(NewTempWord[k], FileWord)) == 0) &&((strcmp(NewTempPron[k], FilePhon2)) == 0) && (NewStressPos[k] == TheStress - 1)) break;
					}
					if(k==startspot) {
						strcpy(NewTempWord[startspot],FileWord);
						strcpy(NewTempPron[startspot],FilePhon2);
						NewTempFreq[startspot] = Freq1;
						NewTempSFreq[startspot] = Freq2;

						if(TheStress == 1 || TheStress == 2) NewStressPos[startspot] = TheStress - 1;
						else NewStressPos[startspot] = 0; //Trochaic Default			
						startspot++;
					}
				}
			}
		}
	}

	
	/*fp2 = fopen("resinfo32.txt","a");
	fprintf(fp2,"O = %d %s\n",startspot, NewNeighWord);
	for(i=0;i<startspot;i++) {
		fprintf(fp2,"%s %s \n",NewTempWord[i],NewTempPron[i]);
	}
	fclose(fp2);
	rewind(fp);*/


	for(i=0;i<32270 /*34012*/;i++) {
		fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid);		
		if(ENGLISH == 1 || GERMAN == 1) FilePhon2[16] = 0;
		if(strlen(NewNeighWord) == strlen(FileWord)) {
			for(j=0;j<nthisword;j++) {
				if(thisword[j] == wordid) break;
			}
			if(j==nthisword) {
				NeighCount1 = 0;
				for(j=0;j<strlen(FileWord);j++) {
					if(NewNeighWord[j] != FileWord[j]) NeighCount1++;
					if(NeighCount1 == 2) break;
				}
				if(NeighCount1 < 2) {
			//	exit(1);
					if(k==startspot) {
						for(k=0;k<startspot;k++) {
							if( ((strcmp(NewTempWord[k], FileWord)) == 0) &&((strcmp(NewTempPron[k], FilePhon2)) == 0) && (NewStressPos[k] == TheStress - 1)) break;
						}		
						strcpy(NewTempWord[startspot],FileWord);
						strcpy(NewTempPron[startspot],FilePhon2);
						NewTempFreq[startspot] = Freq1;
						NewTempSFreq[startspot] = Freq2;
	
						if(TheStress == 1 || TheStress == 2) NewStressPos[startspot] = TheStress - 1;
						else NewStressPos[startspot] = 0; //Trochaic Default			
						startspot++;
					}
				}
			}
		}
	}
//	fclose(fp);

	/*fp2 = fopen("resinfo33.txt","a");
	fprintf(fp2,"TP = %d \n",startspot);
	for(i=0;i<startspot;i++) {
		fprintf(fp2,"%s %s \n",NewTempWord[i],NewTempPron[i]);
	}
	fclose(fp2);*/

	//assembled prob

	strcpy(NewAssPron,AssPronPrime);
	//strcpy(NewNeighWord,NeighWord);
	for(i=0;i<strlen(NewAssPron);i++) {
		if(NewAssPron[i] == '+') {
			NewAssPron[i] = 0;
			break;
		}
	}

	
	rewind(fp);
	for(i=0;i<32270 /*34012*/;i++) {
		fscanf(fp,"%s%s%d%f%f%s%d",FileWord,FilePhon2,&TheStress, &Freq1, &Freq2, NormalPhon, &wordid);		
		if(ENGLISH == 1 || GERMAN == 1) FilePhon2[16] = 0;
		if(strlen(NewAssPron) == strlen(NormalPhon)) {
			for(j=0;j<nthisword;j++) {
				if(thisword[j] == wordid) break;
			}
			if(j==nthisword) {
				NeighCount1 = 0;
				for(j=0;j<strlen(NewAssPron);j++) {
					if(NewAssPron[j] != NormalPhon[j]) NeighCount1++;
					if(NeighCount1 == 2) break;
				}
				if(NeighCount1 < 2) {
			//	exit(1);
					for(k=0;k<startspot;k++) {
						if( ((strcmp(NewTempWord[k], FileWord)) == 0) &&((strcmp(NewTempPron[k], FilePhon2)) == 0) && (NewStressPos[k] == TheStress - 1)) break;
					}	
					if(k==startspot) {
						strcpy(NewTempWord[startspot],FileWord);
						strcpy(NewTempPron[startspot],FilePhon2);
						NewTempFreq[startspot] = Freq1;
						NewTempSFreq[startspot] = Freq2;
	
						if(TheStress == 1 || TheStress == 2) NewStressPos[startspot] = TheStress - 1;
						else NewStressPos[startspot] = 0; //Trochaic Default			
						startspot++;

					}
				}
			}
		}
	}
	fclose(fp);

/*	fp2 = fopen("resinfo32.txt","a");
	fprintf(fp2,"L = %d \n",startspot);
	for(i=0;i<startspot;i++) {
		fprintf(fp2,"%s %s \n",NewTempWord[i],NewTempPron[i]);
	}
	fclose(fp2);*/




	count = startspot;

	for(i=0;i<count;i++) WTaken[i] = 0;
	Word_Count = 0;

	//Squish Orth - Take Out Dups
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(WTaken[j] == 1) {
					if( (strcmp(NewTempWord[i], NewTempWord[j]) ) == 0 ) {
						break;
					}
				}
			}
		}
		if(j == count) {
			strcpy(Word[Word_Count], NewTempWord[i]);
			WTaken[i] = 1;
			Freq[Word_Count] = NewTempFreq[i]; 
			Word_Count++;
		} else {
			//k = 0;
		//	while( strcmp(NewTempWord[i], Word[k]) != 0) k++; 
		//	Freq[k] = NewTempFreq[i];
		}
	}



	//Squish Phon - Take Out Dups
	for(j=0;j<count;j++) Taken[j] = 0;

	Pron_Count = 0;
	for(i=0;i<count;i++) {
		for(j=0;j<count;j++) {
			if(i != j) {
				if(Taken[j] == 1) {
					if( (strcmp(NewTempPron[i], NewTempPron[j]) ) == 0 && (NewStressPos[i] == NewStressPos[j]) ) {
						break;
					}
				}		
			}
		}
		if(j == count) {
			strcpy(Phon[Pron_Count], NewTempPron[i]);
			StressPos[Pron_Count] = NewStressPos[i];
			Taken[i] = 1;
			PhFreq[Pron_Count] = NewTempSFreq[i];
			Pron_Count++;
		} else {
			//k = 0;
		//	while(  strcmp(NewTempPron[i], Phon[k]) != 0) k++;
		//	PhFreq[k] = NewTempSFreq[i];
		}
	}


	if(ENGLISH == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(1093547.0+2.0)) / log10(1093547.0+2.0)) ;  
	} else if (ITALIAN == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(606000000.0+2.0)) / log10(606000000.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(606000000.0+2.0)) / log10(606000000.0+2.0)) ;  
	} else if (GERMAN == 1) {
		for(i=0;i<Word_Count;i++) Freq[i] = ( (log10(Freq[i]+2.0) - log10(150507.0+2.0)) / log10(150507.0+2.0) ) ;
		for(i=0;i<Pron_Count;i++) PhFreq[i] = ( (log10(PhFreq[i]+2.0) - log10(150507.0+2.0)) / log10(150507.0+2.0)) ;  
	}



	// Connect the new lexicons //
	for(i=0;i<Word_Count;i++) NWord_to_Phon[i] = 0;
	for(i=0;i<Pron_Count;i++) NPhon_to_Word[i] = 0;


	//exit(1);

	for(i=0;i<count;i++) {
		for(j=0;j<Pron_Count;j++) {
			if( ((strcmp(NewTempPron[i], Phon[j])) == 0) && (NewStressPos[i] == NewStressPos[j])) break;
		}
		for(k=0;k<Word_Count;k++) {
			if( (strcmp(NewTempWord[i], Word[k])) == 0 ) break;
		}
		//if(j == Pron_Count) exit(1); //Something has gone wrong :(
		Word_to_Phon[k][ NWord_to_Phon[k] ] = j;
		NWord_to_Phon[k] += 1;
		Phon_to_Word[j][ NPhon_to_Word[j] ] = k;
		NPhon_to_Word[j] += 1;
	}

//	exit(1);

	for(i=0;i<Word_Count;i++) {
		for(j=0;j<strlen(Word[i]);j++) {
			Word[i][j] = toupper(Word[i][j]);
		}
		for(;j<16;j++) {
			Word[i][j] = '+';
		}
	}

/*	fp2 = fopen("resinfo1222.txt","a");
	fprintf(fp2,"%d %d %d %s %s \n",Word_Count, Pron_Count, count, NewNeighWord,NewAssPron);
	for(i=0;i<Word_Count;i++) {
		fprintf(fp2,"%d %s %f ",i, Word[i], Freq[i]);
		for(j=0;j<NWord_to_Phon[i];j++) {
			fprintf(fp2,"[%s] %d %f ",Phon[Word_to_Phon[i][j]],Word_to_Phon[i][j], PhFreq[Word_to_Phon[i][j]]);
		}
		fprintf(fp2,"\n");
	}
	fclose(fp2);*/

	/* exit(1);*/
	
//	ChopAllPhons();
		

	return TRUE;
}





