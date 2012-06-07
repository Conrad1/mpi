// DRC_Overall.cpp: implementation of the DRC_Overall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <math.h>
#include "DRC_Overall.h"
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CONSTRAIN(X)  ( ((X) < (0.0)) ? (0.0) : ( ((X) > (1.0)) ? (1.0) : (X)  ) )
#define ACTFUNC(X,Y,Z) (  ((X) > (0.0)) ? ( (Y) + (1 - (Y)) * (X) * (Z) ) : ( (Y) + ((Y) * (X) * (Z)) ))

//netinput,OrthLex.Activations[i],GlobalStuff.act_rate);

// Get rid of this!
int N;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



DRC_Overall::DRC_Overall()
{

	int i;
	Cycle = 1;

	LetterOff = OrthOff = PhonOff = PhonemeOff = GPCOff = PGCOff =FALSE;
	SpecSaveNumber = -1;
	SpecSaveNumberPhon = -1;
	ClampingCycles = 0;
	CyclesBeforeNaming = 35;
	UsingZorzi = 1;
	DerivCycles = 5;
	CyclesTrained = 0;
	randomized = 0;
	OldN = 0;
	resetcyc = GPCRoute.gpc_cyc_ph_ph;
	for(i=0;i<MAXPOS;i++) LastLetterSpotMax[i] = 0;

}

DRC_Overall::~DRC_Overall()
{

}


int DRC_Overall::Initialize_Rules()
{
	int ok;
	int i;


	ok = GPCRoute.UsingRules.LoadRules();
	
	if(ok == FALSE) return FALSE;

	ok = PGCRoute.UsingRules.LoadRules();

	return ok;
}

int DRC_Overall::Initialize_It(int OldFreq)
{
	int ok, i, j;
	FILE *test;

	/*ok = TheWords.MarcoLoadData();

	Num_Word = TheWords.Word_Count;
	Num_Phon = TheWords.Pron_Count;*/

	Num_Word = 0;
	Num_Phon = 0;


/*	for(i=0;i<Num_Word;i++) {
//		fprintf(test,"%s\n",TheWords.Word[i]);
		if( (strcmp(TheWords.Word[i],"RED+++++")) == 0) OrthLex.ORTHRED = i;	
		else if( (strcmp(TheWords.Word[i],"GREEN+++")) == 0) OrthLex.ORTHGREEN = i; 
		else if( (strcmp(TheWords.Word[i],"BLUE++++")) == 0) OrthLex.ORTHBLUE = i; 
		else if( (strcmp(TheWords.Word[i],"WHITE+++")) == 0) OrthLex.ORTHWHITE = i; 
		else if( (strcmp(TheWords.Word[i],"BLACK+++")) == 0) OrthLex.ORTHBLACK = i; 
	}
	for(i=0;i<Num_Phon;i++) {
		if( (strcmp(TheWords.Phon[i],"rEd+++++")) == 0) PhonLex.PHONRED = i;
		else if((strcmp(TheWords.Phon[i],"grin++++")) ==0) PhonLex.PHONGREEN = i;
		else if((strcmp(TheWords.Phon[i],"blu+++++")) ==0) PhonLex.PHONBLUE = i;
		else if((strcmp(TheWords.Phon[i],"w2t+++++")) ==0) PhonLex.PHONWHITE= i;
		else if((strcmp(TheWords.Phon[i],"bl{k++++")) ==0) PhonLex.PHONBLACK = i;
	}*/
//	fclose(test);
	//OrthLex.Initialize_It(/*Num_Word*/34500);
	//PhonLex.Initialize_It(/*Num_Phon*/34500);


	// Will need to be fixed in future //
//	SemSys.InitializeSem(OrthLex.ORTHRED,OrthLex.ORTHGREEN,OrthLex.ORTHBLUE,OrthLex.ORTHWHITE,OrthLex.ORTHBLACK,PhonLex.PHONRED,PhonLex.PHONGREEN,PhonLex.PHONBLUE,PhonLex.PHONWHITE,PhonLex.PHONBLACK);

	return 1;
}


void DRC_Overall::SetLatInhibition() 
{	
	int i, j;

//	TestLat = 0;

	LetterSpots.ResetLatInhibition();	
	for(i = 0;i<MAXPOS; i++) {	
		for(j=0;j<NLETTERS;j++) {
			LetterSpots.LatInhibition[i] += LetterSpots.Letters[i][j];
		}
		LetterSpots.LatInhibition[i] *= LetterSpots.Letter_Letter;
		
	}
   
	// Set up the phonological buffer 
	PhonBuf.ResetLatInhibition();	
	for(i = 0;i<MAXPHONE; i++) {	
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			//EXCLUDE LAST TWO SPOTS FOR STRESS
			if(! ((i==MAXPHONE-1) && j > (NUMBER_OF_PHONEMES-NSTRESS-1) )  ) {
				PhonBuf.LatInhibition[i] += PhonBuf.Buffer[i][j];
			} else {
				PhonBuf.StressLatInhibition += PhonBuf.Buffer[i][j];
			}
		}
		PhonBuf.LatInhibition[i] *= PhonBuf.Phon_Phon;
		PhonBuf.StressLatInhibition *= PhonBuf.StressPhon_Phon;
	}

	//Set up the orth lex
	OrthLex.ResetLatInhibition();
	for(i=0;i<Num_Word;i++) {
		OrthLex.LatInhibition += OrthLex.Activations[i];
	}
	OrthLex.LatInhibition *= OrthLex.Orth_Orth;
		
	// Set up the phon lex
	PhonLex.ResetLatInhibition();	
	for(i=0;i<Num_Phon;i++) {
			PhonLex.LatInhibition += PhonLex.Activations[i];
	}
	PhonLex.LatInhibition *= PhonLex.PhonUnit_PhonUnit;

	SemSys.ResetLatInhibition();
	for(i=0;i<SemSys.CurrentSem;i++) {
		SemSys.LatInhibition += SemSys.SemAct[i];
	}
	SemSys.LatInhibition *= SemSys.inh_sem_sem;
}

void DRC_Overall::FtOtLetter()
{
	int i,j,k;
	float tinput;
	
	for(i=0; i < MAXPOS;i++) {
		for(j=0;j < NLETTERS;j++) {
			// Collect Feature to Letter Input
			if( !(OrthLex.PointDecayOn && OrthLex.OverPoint == TRUE) && !(PhonLex.PhonPointDecayOn && PhonLex.OverPoint == TRUE)) {
				tinput = 0;
				for(k=0;k<NUMBER_OF_FEATURES;k++) {
					if(Features.FeatureSet[i].Block[k] == 1/* > .05*/) {
						if(GlobalStuff.LetterBlock[j][k] == 1) {
							tinput += Features.exc_feature_letter  /** Features.FeatureSet[i].Block[k]*/;
						} else {
							tinput += Features.inh_feature_letter  /** Features.FeatureSet[i].Block[k]*/;
						}
					} else if (Features.FeatureSet[i].ABlock[k]  == 1/* > .05*/) {
						if(GlobalStuff.LetterBlock[j][k] == 1) {
							tinput += Features.inh_feature_letter /** Features.FeatureSet[i].ABlock[k]*/ ;
						} else {
							tinput += Features.exc_feature_letter /** Features.FeatureSet[i].ABlock[k]*/;
						}
					}				
				}
			} else tinput = 0;
			// Total Letter-Letter Inihibition
			tinput += (LetterSpots.LatInhibition[i] - (LetterSpots.Letter_Letter * LetterSpots.Letters[i][j]));
			
			 //Total Input for Letter Unit from all words
			for(k=0;k<Num_Word;k++) {
				if(GlobalStuff.Alphabet[j] == TheWords.Word[k][i]  ) {
					tinput += (OrthLex.exc_word_letter * OrthLex.Activations[k]);
				} else {
					tinput += (OrthLex.inh_word_letter * OrthLex.Activations[k]);
				}
			}
			LetterSpots.NetInput[i][j] = tinput;			
		}
	}
}

void DRC_Overall::LtPhtOrthography()
{
	float tinput;
	float Max  = -1;
	int i,j, k;
	FILE *shit;
	
	for(i=0;i<Num_Word;i++) {
		// Letter to word
		/*shit = fopen("wordsinhere.txt","a");
		fprintf(shit,"%s \n",TheWords.Word[i]);
		fclose(shit);*/
		tinput = 0;
		for(j=0;j<MAXPOS;j++) {
			for(k=0;k<NLETTERS;k++) {
				if(GlobalStuff.Alphabet[k] == TheWords.Word[i][j] ) {		
					tinput += (LetterSpots.exc_letter_word * LetterSpots.Letters[j][k]);
				} else {
					tinput += (LetterSpots.inh_letter_word * LetterSpots.Letters[j][k]);
				}
			}
		}
		// Lateral inhibition 
		tinput += (OrthLex.LatInhibition - (OrthLex.Orth_Orth * OrthLex.Activations[i]));
		
		// Adjacent Units in Phonological Set
		for(j=0;j<TheWords.NWord_to_Phon[i];j++) {
			tinput += (PhonLex.exc_phonU_orth * PhonLex.Activations[TheWords.Word_to_Phon[i][j]]);  	
		}

		OrthLex.NetInput[i] = tinput;
	}
#if ENGLISH
	if(Cycle >= SemSys.SemStart) {
		for(j=0;j<SemSys.CurrentSem;j++) {
			for(k=0;k<SemSys.OrthNConnects[j];k++) {
				OrthLex.NetInput[SemSys.OrthConnects[j][k]] += (SemSys.SemAct[j] * SemSys.exc_sem_orth);
			}
		}
	}
#endif
}

//	for(i=0;i<Num_Phon;i++) {
		// Phoneme to phon Unit
		
//		tinput = 0;
//		for(j=0;j<MAXPHONE;j++) {
//			for(k=0;k<NUMBER_OF_PHONEMES;k++) {
//				if(TheWords.Phon[i][j] == PhonBuf.Phonemes[k]) {
//					tinput += (PhonBuf.exc_phon_word * PhonBuf.Buffer[j][k]);
//				} else {
//					tinput += (PhonBuf.inh_phon_word * PhonBuf.Buffer[j][k]);
//				}
//			}
//		}
//	}



void DRC_Overall::PhtOtPhonLex()
{
	int h,i,j,k;
	int snumber;

	float tinput;
	FILE *fp1;

/*	fp1 = fopen("shit2.txt","a");
	fprintf(fp1,"%f \n",PhonLex.LatInhibition);
	fclose(fp1);*/

	for(i=0;i<Num_Phon;i++) {
		// Phoneme to phon Unit
		
		tinput = 0;

//#if ISMARCO	

		for(j=0;j<MAXPHONE;j++) {
			for(k=0;k<NUMBER_OF_PHONEMES;k++) {
				if(! ((j==MAXPHONE-1) && k > (NUMBER_OF_PHONEMES-3) )) { /*Lasttwo spots */	
					if(TheWords.Phon[i][j] == PhonBuf.Phonemes[k]) {
						tinput += (PhonBuf.exc_phon_word * PhonBuf.Buffer[j][k]);
					} else {
						tinput += (PhonBuf.inh_phon_word * PhonBuf.Buffer[j][k]);
					}
				} else {
					//Stress inhibition
				}
			}
		}
//#endif
		// Lateral inhibition 
		tinput += (PhonLex.LatInhibition - (PhonLex.PhonUnit_PhonUnit * PhonLex.Activations[i]));
		
		// Adjacent Unit in Orthographic Set 
		for(j=0;j<TheWords.NPhon_to_Word[i];j++) {
			tinput += (OrthLex.exc_orth_phon * OrthLex.Activations[TheWords.Phon_to_Word[i][j]]);  
		}
		PhonLex.NetInput[i] = tinput;
	}

/*#if ENGLISH
	if(Cycle >= SemSys.SemStart) {
		for(j=0;j<SemSys.CurrentSem;j++) {
			for(k=0;k<SemSys.NConnects[j];k++) {
				PhonLex.NetInput[SemSys.Connects[j][k]] += (SemSys.SemAct[j] * SemSys.exc_sem_phon);
			}
		}
	}
#endif */

}
		

void DRC_Overall::PhUtPhoneme()
{
	float tinput, multiplier, stresstinput = 0;
	int h,i,j,k;
	int upto, temp;
	FILE *fp1;
	int snumber;
	

	if(ClampingCycles == 0) upto = MAXPHONE;
	else upto = ( (Cycle - 1) / ClampingCycles) + 1;
	//if(upto > MAXPHONE) upto = MAXPHONE;



	for(i=0; i < MAXPHONE;i++) {
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			tinput = 0;
			stresstinput = 0;
			for(k=0; k < Num_Phon;k++) {
				if(! ((i==MAXPHONE-1) && j > (NUMBER_OF_PHONEMES-NSTRESS-1) ))  { //stress
					if(PhonBuf.Phonemes[j] == TheWords.Phon[k][i]  )  {
						tinput += (PhonLex.exc_phonU_phon * PhonLex.Activations[k]);
					} else {
						tinput += (PhonLex.inh_phonU_phon * PhonLex.Activations[k]);
					}
				} else {
					if(j + TheWords.StressPos[k] == NUMBER_OF_PHONEMES) { // 3 2 1 
						/*fp1 = fopen("stresstest.txt","a");
						fprintf(fp1,"%d %d %s %d %d \n",i,j,TheWords.Phon[k],TheWords.StressPos[k], j);
						fclose(fp1);*/
						stresstinput += (PhonLex.exc_stress * PhonLex.Activations[k]);
					} else {
						stresstinput += (PhonLex.inh_stress * PhonLex.Activations[k]);
					}
				}
			}
			// Phoneme to Phoneme

			if(! ((i==MAXPHONE-1) && j > (NUMBER_OF_PHONEMES-NSTRESS-1)) )  { //stress
				tinput += (PhonBuf.LatInhibition[i] - (PhonBuf.Phon_Phon * PhonBuf.Buffer[i][j]));					
				PhonBuf.NetInput[i][j] = tinput;
			} else {
			/*	fp1 = fopen("stressinh.txt","a");
				fprintf(fp1,"%d %d %d %f %f %f %f  \n",i,j,Cycle, stresstinput, PhonBuf.StressLatInhibition,PhonBuf.Buffer[i][j],(PhonBuf.StressLatInhibition - (PhonBuf.StressPhon_Phon * PhonBuf.Buffer[i][j])) );
				fclose(fp1);*/
				stresstinput += (PhonBuf.StressLatInhibition - (PhonBuf.StressPhon_Phon * PhonBuf.Buffer[i][j]));
				PhonBuf.NetInput[i][j] = stresstinput;
			}
		}
	}
}

void DRC_Overall::GPCOtP()
{
	int i,Index, temp;
	float Input;
	float Before = 0, After;

	
	FILE *fp, *crud;


#if ISMARCO
	if(strlen(GPCRoute.FinalWord) > 0) {
	/*	
		TheWords.GetPhonBod(GPCRoute.FinalWord);
		
		for(i=0;i<strlen(TheWords.LastPhonOnset);i++) {
			Input = GPCRoute.PhAct[i];
			Index = PhonBuf.PhonemeIndex(TheWords.LastPhonOnset[i]);
			if(Index != -1) PhonBuf.NetInput[i][Index] += (Input * GPCRoute.exc_gpc_phone);
		}
		temp = strlen(GPCRoute.FinalWord);
		if(GPCRoute.FinalWord[temp - 1] == '+') temp = strlen(TheWords.LastPhonBod) + 1;
		else temp =strlen(TheWords.LastPhonBod);
		for(i=0;i< temp && i < MAXPHONE;i++) {
			Input = GPCRoute.PhAct[i+strlen(TheWords.LastPhonOnset)];
			Index = PhonBuf.PhonemeIndex(GPCRoute.FinalWord[i+strlen(TheWords.LastPhonOnset)]);
			if(Index != -1) PhonBuf.NetInput[i + 3][Index] += (Input * GPCRoute.exc_gpc_phone);
		} */

		GPCInf(Cycle);
	} 

#else

	for(i=0;i<strlen(GPCRoute.FinalWord) && i < MAXPHONE;i++) {
		Input = GPCRoute.PhAct[i];
		Index = PhonBuf.PhonemeIndex(GPCRoute.FinalWord[i]);
		if(Index != -1) PhonBuf.NetInput[i][Index] += (Input * GPCRoute.exc_gpc_phone);
	}
	GPCInf(Cycle); 
#endif
}


void DRC_Overall::PGCPtO()
{
	int i,Index;
	float Input;
	float Before = 0, After;
	char letter;

	for(i=0;i<strlen(PGCRoute.TheWord) && i < MAXPOS;i++) {
		Input = PGCRoute.WdAct[i];
		letter = GlobalStuff.Language_Data_Hack(PGCRoute.TheWord[i]);
		Index = GlobalStuff.AlphabetIndex(letter);
		if(Index != -1) LetterSpots.NetInput[i][Index] += (Input * PGCRoute.exc_pgc_phone);
		//else exit(1);
	}
	PGCInf(Cycle);
}

void DRC_Overall::UpdateSemantic()
{
	int i,j;
	FILE *fp1;

	for(i=0;i<SemSys.CurrentSem;i++) {
		SemSys.NetInput[i] = SemSys.LatInhibition - (SemSys.inh_sem_sem * SemSys.SemAct[i]);
	}

	
	for(i=0;i<SemFeatures.NumFeatures;i++) {
		SemFeatures.NetInput[i] = 0.0;

		//SemFeatures.FeatureAct[i] = 0.0;
	}

	

	if(TheColor == ISRED) {
		SemFeatures.FeatureAct[0] = 1.0;
	} else if(TheColor == ISGREEN) {
		SemFeatures.FeatureAct[1] = 1.0;
	} else if(TheColor == ISBLUE) {
		SemFeatures.FeatureAct[2] = 1.0;
	} else if(TheColor == ISWHITE) {
		SemFeatures.FeatureAct[5] = 1.0;
	} else if(TheColor == ISBLACK) {
		SemFeatures.FeatureAct[6] = 1.0;
	} else if(TheColor == ISBREAD) {
		SemFeatures.NetInput[3] = 1.0 * SemFeatures.exc_out_sem;
	} else if(TheColor == ISCOOK) {
		SemFeatures.NetInput[4] = 1.0 * SemFeatures.exc_out_sem;
	} else if(TheColor == ISTOAST) {
		SemFeatures.NetInput[3] =  0.7 * SemFeatures.exc_out_sem;
		SemFeatures.NetInput[4] =  0.7 * SemFeatures.exc_out_sem;
	} 
	
//	fp1 = fopen("Colour","a");
//   fprintf(fp1,"%d %f %f \n",TheColor, SemFeatures.NetInput[3], SemFeatures.NetInput[4]);
//	fclose(fp1);

	//Hack here ! //
	//for(i=3;i<SemSys.CurrentSem;i++) {
	//	for(j=0;j<SemSys.NSem_to_SemFeat[i];j++) {
	//		SemFeatures.NetInput[SemSys.Sem_to_SemFeat[i][j]] += SemSys.exc_sem_semfeatures * SemSys.SemAct[i] * SemSys.Sem_to_SemFeatWeight[i][j];
	//	}
	//}


	for(i=0;i<SemFeatures.NumFeatures;i++) {
		for(j=0;j<SemFeatures.NSemFeat_to_SemWeight[i];j++) {
			SemSys.NetInput[SemFeatures.SemFeat_to_Sem[i][j]] += SemFeatures.exc_semfeat_sem * SemFeatures.SemFeat_to_SemWeight[i][j] * SemFeatures.FeatureAct[i];
		}
	}

	for(i=0;i<Num_Word;i++) {
		for(j=0;j<OrthLex.OrthNConnects[i];j++) {
			SemSys.NetInput[OrthLex.OrthConnects[i][j]] += OrthLex.exc_orth_sem * OrthLex.Activations[i];
		}
	}

	for(i=0;i<Num_Phon;i++) {
		for(j=0;j<PhonLex.PhNConnects[i];j++) {
			SemSys.NetInput[PhonLex.PhConnects[i][j]] += PhonLex.exc_phon_sem * PhonLex.Activations[i];
		}
	}
}		
	
inline float DRC_Overall::ActivationFunc(float actinput, float Initial_Act)
{
	if((actinput) >= 0.0) {
		return (Initial_Act) + ((1.0 - (Initial_Act)) * (actinput) * (GlobalStuff.act_rate));
	} else {
		return (Initial_Act) + Initial_Act * (actinput) * GlobalStuff.act_rate;
	}
}

inline float DRC_Overall::ConstrainAct(float Act)
{
	if((Act) < 0) return 0;
	else if((Act) > 1) return 1;
	else return (Act);
}
	
	
void DRC_Overall::UpdateActivation()
{
	int i,j, k,l, GraphCat;
	float netinput, the_length, tempact, totphoneme;

	FILE *crap;

	if(SaveType == 1) WordInf(Cycle, ActualWord);


	//NB This has to be here as the global threading is a bit fucked, and
	//resets the noise value everytim it is called out of a procedure

	srand( (unsigned)time( NULL ) );
/* GET RID OF SEMANTICS FOR NOW
	//Update Semantics
	for(i=0;i<SemFeatures.NumFeatures;i++) {
		netinput = SemFeatures.NetInput[i];
		//SemFeatures.FeatureAct[i] = ActivationFunc(netinput, SemFeatures.FeatureAct[i]);
		SemFeatures.FeatureAct[i] = ACTFUNC(netinput, SemFeatures.FeatureAct[i], GlobalStuff.act_rate);
		//	SemFeatures.FeatureAct[i] = ConstrainAct(SemFeatures.FeatureAct[i]);
		SemFeatures.FeatureAct[i] = CONSTRAIN(SemFeatures.FeatureAct[i]);
	}
//
	the_length = VectLength(SemFeatures.FeatureAct, SemFeatures.NumFeatures);

	for(i=0;i<SemFeatures.NumFeatures;i++) {
//		SemFeatures.FeatureAct[i] /= the_length;
		SemFeatInf(i,Cycle);
	}
	if(Cycle >= SemSys.SemStart) {
		for(i=0;i<SemSys.CurrentSem;i++) {
	//		if(OrthLex.PointDecayOn && OrthLex.OverPoint == TRUE) SemSys.SemAct[i] = SemSys.SemAct[i] * (1 - SemSys.SemDecay);
			if(DecayEveryCycle != 0) SemSys.SemAct[i] = SemSys.SemAct[i] * (1 - SemSys.SemDecay);
			netinput = SemSys.NetInput[i];
			//SemSys.SemAct[i] = ActivationFunc(netinput,SemSys.SemAct[i]);
			SemSys.SemAct[i] = ACTFUNC(netinput,SemSys.SemAct[i],GlobalStuff.act_rate);
			
			//SemSys.SemAct[i] = ConstrainAct(SemSys.SemAct[i]);
			SemSys.SemAct[i] = CONSTRAIN(SemSys.SemAct[i]);
			SemInf(i,Cycle);				
		}
	}
*/

	testcycle1 = -1;
	//for(i=0;i<MAXPOS;i++) LastLetterSpotMax[i] = 0;
	for(i=0;i<MAXPOS;i++) {
		for(j=0;j<NLETTERS;j++) {
//			if(OrthLex.PointDecayOn && OrthLex.OverPoint == TRUE) LetterSpots.Letters[i][j] = LetterSpots.Letters[i][j] * (1 - LetterSpots.Decay); 				
			if(DecayEveryCycle != 0) LetterSpots.Letters[i][j] = LetterSpots.Letters[i][j] * (1 - LetterSpots.Decay);
			netinput = LetterSpots.NetInput[i][j] + Box_Muller_Polar(LetterSpots.Noise);
			LetterSpots.Letters[i][j] = ACTFUNC(netinput,LetterSpots.Letters[i][j],GlobalStuff.act_rate);
			LetterSpots.Letters[i][j] = CONSTRAIN(LetterSpots.Letters[i][j]);					
			LetterInf(i,j,Cycle);
		}	

		if(LetterSpots.LetterOver == FALSE && i == 0) {
			/*crap=fopen("under.txt","a");
				fprintf(crap,"-->%d %\n",Cycle);
				fclose(crap);*/
			
			
			LastLetterSpotMax[0] = -100;
			LastLetterSpot[0] = -1;
			for(k=0;k<NLETTERS;k++) { //Find the highest
				if(LetterSpots.Letters[0][k] > LastLetterSpotMax[0]) {
					LastLetterSpot[0] = k;
					LastLetterSpotMax[0] = LetterSpots.Letters[0][k];
				}
			}
			//start parser // m,ake sure there is a break if it has been reset
			if(resetcyc == GPCRoute.gpc_cyc_ph_ph) {
				if(LastLetterSpotMax[0] > LetterSpots.LetterOverPar) {
					LetterSpots.LetterOver = TRUE;
					GPCRoute.gpc_act_offset = Cycle + 1;
					testcycle1 = Cycle;
					/*crap=fopen("ONIT.txt","a");
					fprintf(crap,"%d %s %f %d\n",Cycle,ActualWord, LastLetterSpotMax[0],LastLetterSpot[0]);
					fclose(crap);*/
					//	N = (int) ( ( ( (float) (Cycle - 0) - GPCRoute.gpc_act_offset ) / GPCRoute.gpc_cyc_ph_ph ) + 1.0);
				}
			} else resetcyc++;
		}

		if(/*i == 0  && */ (LetterSpots.LetterOver == TRUE)) {
			if(testcycle1 != Cycle && i > OldN-4 && i < OldN) {
				/*crap=fopen("tesssss133.txt","a");
				fprintf(crap,"-->%d %d %d i=%d %d %d  %f \n",Cycle,testcycle1,OldN,i,JustChanged,LastLetterSpot[i],LastLetterSpotMax[i]);
				fclose(crap);*/
				OldLastLetterSpot[i] = LastLetterSpot[i];
				OldLastLetterSpotMax[i] = LastLetterSpotMax[i];
				LastLetterSpot[i] = -1;
				LastLetterSpotMax[i] = -1;

				for(j=0;j<NLETTERS;j++) {
					if(LetterSpots.Letters[i][j] > LastLetterSpotMax[i]) {
						LastLetterSpot[i] = j;
						LastLetterSpotMax[i] = LetterSpots.Letters[i][j];
					//	crap=fopen("lettersinfo.txt","a");
					//	fprintf(crap,"%d %d %d %d %d %f \n",Cycle, i,j, LastLetterSpot[i],OldLastLetterSpot[i], LastLetterSpotMax[i]);
					//	fclose(crap);
					}
				}
				if(LastLetterSpot[i] != OldLastLetterSpot[i] && JustChanged == 0) {
					LetterSpots.LetterOver = FALSE;
					//resetcyc = 1;
					resetcyc = GPCRoute.gpc_cyc_ph_ph;
					alldone = 0;

					ZNet.ResetAll();

					//N = (int) ( ( ( (float) (Cycle - 0) - GPCRoute.gpc_act_offset ) / GPCRoute.gpc_cyc_ph_ph ) + 1.0);					
					//need to way 11 cycles!!
				/*	crap=fopen("tereset.txt","a");
					fprintf(crap,"%d %d %d %d LLS=%d OLLS=%d %d %f \n",Cycle,OldN,i,JustChanged,LastLetterSpot[i],OldLastLetterSpot[i],Cycle,LastLetterSpotMax[i]);		
					fclose(crap);*/
			//		exit(1);
				}
				if(LastLetterSpotMax[i] < LetterSpots.LetterOverPar) {
					LetterSpots.LetterOver = FALSE;	
					alldone = 0;
					resetcyc = GPCRoute.gpc_cyc_ph_ph;
					ZNet.ResetAll();
				/*	crap=fopen("teresethere.txt","a");
					fprintf(crap,"%d %d %d %d LLS=%d OLLS=%d %d %f \n",Cycle,OldN,i,JustChanged,LastLetterSpot[i],OldLastLetterSpot[i],Cycle,LastLetterSpotMax[i]);		
					fclose(crap);*/

	
				}
			}
		}
		/*crap=fopen("tesssss133.txt","a");
		fprintf(crap,"---->%d %d %d %d %d %d  %f \n",Cycle,testcycle1,OldN,i,JustChanged,LastLetterSpot[i],LastLetterSpotMax[i]);
		fclose(crap);*/
	}




		/*crap=fopen("tess2.txt","a");
		for(i=0;i<8;i++) {
			fprintf(crap,"11 %d 313133 [%d] %d %d %d %d %d %f \n",Num_Word, LetterSpots.LetterOver,OldN,i,JustChanged,LastLetterSpot[i],Cycle,LastLetterSpotMax[i]);			
		}	
		fclose(crap);*/

	//Update Orthographic Input Lex
	OrthLex.VLexM = -1;		  // M Criterion
	OrthLex.VTotal_Sum = 0;  // Total_Sum
	OrthLex.VLexM_Spot = -1;
	for(i=0;i<Num_Word;i++) {
//		if(OrthLex.PointDecayOn && OrthLex.OverPoint == TRUE) OrthLex.Activations[i] = OrthLex.Activations[i] * (1 - OrthLex.Decay); 
		if(DecayEveryCycle != 0) OrthLex.Activations[i] = OrthLex.Activations[i] * (1 - OrthLex.Decay);
	
		netinput = OrthLex.NetInput[i] /*+ Box_Muller_Polar(OrthLex.Noise)*/ + TheWords.Freq[i] * TheWords.fgain_tmp ;
	
//		OrthLex.Activations[i] = ActivationFunc(netinput,OrthLex.Activations[i]);
		OrthLex.Activations[i] = ACTFUNC(netinput,OrthLex.Activations[i],GlobalStuff.act_rate);

		//OrthLex.Activations[i] = ConstrainAct(OrthLex.Activations[i]);
		OrthLex.Activations[i] = CONSTRAIN(OrthLex.Activations[i]);
		
		OrthLex.VTotal_Sum += OrthLex.Activations[i];
		if(OrthLex.Activations[i] > OrthLex.VLexM) {
			OrthLex.VLexM = OrthLex.Activations[i];		  // M Criterion
			OrthLex.VLexM_Spot = i;
		}
		if(OrthLex.VLexM > OrthLex.OrthDecayPoint) OrthLex.OverPoint = TRUE;
		OrthLexInf(i, Cycle);
	}
	if(SpecSaveNumber != -1) OrthLexSpecSave(SpecSaveNumber, Cycle);
	OrthLexSumInf(i, Cycle);

	if(SpecTotOrthSave == TRUE) TotOrthSpecSave(Cycle);

	PhonLex.VTotal_Sum = 0;  // Total_Sum
	PhonLex.MaxPhon = -1;

	/*crap=fopen("tess2.txt","a");
	fprintf(crap,"->%d [%d] %d %d %d %d %d %f \n",Num_Phon, LetterSpots.LetterOver,OldN,i,JustChanged,LastLetterSpot[i],Cycle,LastLetterSpotMax[i]);			
	fclose(crap);*/


	for(i=0;i<Num_Phon;i++) {

	/*crap=fopen("tess2.txt","a");
	fprintf(crap,"->%f %f %d %d [%d] %d %d %d %d %d %f \n",PhonLex.Activations[i],PhonLex.NetInput[i],i,Num_Phon, LetterSpots.LetterOver,OldN,i,JustChanged,LastLetterSpot[i],Cycle,LastLetterSpotMax[i]);			
	fclose(crap);*/

//		if(OrthLex.PointDecayOn && OrthLex.OverPoint == TRUE) PhonLex.Activations[i] = PhonLex.Activations[i] * (1 - PhonLex.Decay);
		if(DecayEveryCycle != 0) PhonLex.Activations[i] = PhonLex.Activations[i] * (1 - PhonLex.Decay);
		netinput = PhonLex.NetInput[i] /* + Box_Muller_Polar(PhonLex.Noise) */+ TheWords.PhFreq[i] * TheWords.fgain_tmp;

//		PhonLex.Activations[i] = ActivationFunc(netinput,PhonLex.Activations[i]);
		PhonLex.Activations[i] = ACTFUNC(netinput,PhonLex.Activations[i],GlobalStuff.act_rate);

		//		PhonLex.Activations[i] = ConstrainAct(PhonLex.Activations[i]);
		PhonLex.Activations[i] = CONSTRAIN(PhonLex.Activations[i]);

		if(PhonLex.Activations[i] > PhonLex.MaxPhon) PhonLex.MaxPhon = PhonLex.Activations[i];
		PhonLex.VTotal_Sum += PhonLex.Activations[i];
		PhonLexInf(i, Cycle);
	}		
	if(PhonLex.MaxPhon > PhonLex.PhonDecayPoint) PhonLex.OverPoint = TRUE;
	PhonLexSumInf(i, Cycle);
	//if(SpecSaveNumberPhon != -1) PhonLexSpecSave(SpecSaveNumberPhon, Cycle);
	//if(SpecTotPhonSave == TRUE) TotPhonSpecSave(Cycle);

	/*crap=fopen("tess2.txt","a");
	for(i=0;i<8;i++) {
		fprintf(crap,"3 3  [%d] %d %d %d %d %d %f \n",LetterSpots.LetterOver,OldN,i,JustChanged,LastLetterSpot[i],Cycle,LastLetterSpotMax[i]);			
	}	
	fclose(crap);*/


	totphoneme = 0;
	for(i=0;i<MAXPHONE;i++) {
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
//			if(OrthLex.PointDecayOn && OrthLex.OverPoint == TRUE) PhonBuf.Buffer[i][j] = PhonBuf.Buffer[i][j] * (1 - PhonBuf.Decay); 
		//	if(DecayEveryCycle != 0) PhonBuf.Buffer[i][j] = PhonBuf.Buffer[i][j] * (1 - PhonBuf.Decay);
			if(DecayEveryCycle != 0) PhonBuf.Buffer[i][j] = PhonBuf.Buffer[i][j] - PhonBuf.Decay;
		
			netinput = PhonBuf.NetInput[i][j] /*+ Box_Muller_Polar(PhonBuf.Noise)*/;

			if(netinput > 0.0) {
				PhonBuf.Buffer[i][j] += (1.0 - PhonBuf.Buffer[i][j]) * netinput * GlobalStuff.act_rate;  
			} else {
				PhonBuf.Buffer[i][j] += PhonBuf.Buffer[i][j] * netinput * GlobalStuff.act_rate;  
			}
			totphoneme += PhonBuf.Buffer[i][j];
			//THERE SEEMS TO BE SOME BUG HERE!!!!!! CAREFUL!!

			//tempact = ActivationFunc(netinput, PhonBuf.Buffer[i][j]);
			//PhonBuf.Buffer[i][j] = ActivationFunc(netinput, PhonBuf.Buffer[i][j]);
		//	PhonBuf.Buffer[i][j] = ACTFUNC(netinput, PhonBuf.Buffer[i][j],GlobalStuff.act_rate);
	
			//	PhonBuf.Buffer[i][j] = tempact;
			//PhonBuf.Buffer[i][j] = ConstrainAct(PhonBuf.Buffer[i][j]);
			PhonBuf.Buffer[i][j] = CONSTRAIN(PhonBuf.Buffer[i][j]);
#if USEDERIV	
			PhonBuf.OldBuffer[Cycle][i][j] = PhonBuf.Buffer[i][j];		
			if(Cycle > DerivCycles) {
				PhonBuf.Derivatives[i][j] = (PhonBuf.OldBuffer[Cycle][i][j] - PhonBuf.OldBuffer[Cycle - DerivCycles][i][j]);
			} else {
				PhonBuf.Derivatives[i][j] = 0;
			}
#endif
			PhonemeInf(i,j, Cycle);		
		}
	} 
	PhonemeSumInf(Cycle, totphoneme);

	//Stuff For Next Cycle
	if(OrthLex.VLexM > OrthLex.OrthDecayPoint) OrthLex.OverPoint = TRUE;
	
	/*crap=fopen("tess2.txt","a");
		for(i=0;i<8;i++) {
			fprintf(crap,"4444 [%d] %d %d %d %d %d %f \n",LetterSpots.LetterOver,OldN,i,JustChanged,LastLetterSpot[i],Cycle,LastLetterSpotMax[i]);			
		}	
		fclose(crap);*/
}

void DRC_Overall::UpdatePgcRoute() {

	float divisor;
	int  i,j, MaxU;
	float max;

	char TheName[30];
	FILE *Fp;
	
	divisor = PGCRoute.pgc_cyc_ph_ph;
	
	N = (int) ( ( ( (float) (Cycle) - PGCRoute.pgc_act_offset ) / PGCRoute.pgc_cyc_ph_ph ) + 1.0);
	
	PGCRoute.EOW = FALSE;
		
	for(i = 0; i < N && i < MAXPHONE;i++) {
		max = -1;
		for(j=0; j<NUMBER_OF_PHONEMES;j++) {
			if(PhonBuf.Buffer[i][j] > max) {
				max = PhonBuf.Buffer[i][j];
				MaxU = j;
			}
		}
		if(/*PhonBuf.XXX*/Phonemes[MaxU] == WORDPAD) {
			PGCRoute.EOW = TRUE;
			break;
		}

		PhonemeStr[i] = /*PhonBuf.XXX*/Phonemes[MaxU];
		PGCRoute.PhAct[i] = PhonBuf.Buffer[i][MaxU];		
	}
	LetterStr[i] = 0;

	while( (i < N) &&  (i < MAXPHONE) ) {
		PGCRoute.PhAct[i] = PhonBuf.Buffer[i][NUMBER_OF_PHONEMES ];
		PhonemeStr[i] = WORDPAD;
		i++;
	}
	PhonemeStr[i] = 0;

	while(i < MAXPHONE) {
		PGCRoute.PhAct[i++] = 0;
	}

	PGCRoute.InputPgc(PhonemeStr);
}

	
void DRC_Overall::UpdateGpcRoute() 
{
	float divisor;
	int  i,j, MaxU, TempCount,uptolet=0;
	float max, sum;

	char TheName[30], TempWord[30];
	FILE *Fp, *crud;

	int a, k, NCons = 0, WordPos;
	int Pos = 0, used;

	
//	for(i=0;i<MAXPOS;i++) LetterStr[i] = 0;

	divisor = GPCRoute.gpc_cyc_ph_ph;
	GPCRoute.EOW = FALSE;

	/*if(GPCRoute.gpc_cyc_ph_ph == 0) {
	//	if(Cycle > GPCRoute.gpc_act_offset) {
		if(LetterSpots.LetterOver == TRUE) {
			N = MAXPOS;
		} else N = 0;
	} else {*/

		if(LetterSpots.LetterOver == TRUE) {

			N = (int) ( ( ( (float) (Cycle - 0) - GPCRoute.gpc_act_offset ) / GPCRoute.gpc_cyc_ph_ph ) + 1.0 /* 3 */);
			N = N /* + X */;
			if(N < 0) N = 0;
		} else {
			N = 0;
		}
	//}
	/*Fp = fopen("Testcycles111.txt","a");
	fprintf(Fp,"%d %d %d %d\n", Cycle, N,OldN, alldone,GPCRoute.gpc_act_offset);
	fclose(Fp);*/

	if(MarcoGrapheme) {
	} else if(MarcoNormal || MarcoDelay) {
		if(alldone == 0) {
		//Don't change letters out of attention.
			if(N-3 < 0) i = 0;
			else i = N - 3;
		
			for(/*i = 0*/; i < N && i < MAXPOS;i++) {
				max = -1;
				for(j=0; j<NLETTERS;j++) {
					if(LetterSpots.Letters[i][j] > max) {
						max = LetterSpots.Letters[i][j];
						MaxU = j;
					}
				}
				if(GlobalStuff.Alphabet[MaxU] == WORDPAD) {
					GPCRoute.EOW = TRUE;
					break;
				}
				LetterStr[i] = GlobalStuff.Alphabet[MaxU];
				GPCRoute.WdAct[i] = LetterSpots.Letters[i][MaxU];		
			}
			LetterStr[i] = 0;

			while( (i < N)  &&  (i < MAXPOS) ) {
				GPCRoute.WdAct[i] = LetterSpots.Letters[i][NLETTERS - 1];
				LetterStr[i] = WORDPAD;
				LetterStr[i+1] = 0;

/*				Fp = fopen("letterstrs.txt","a");
				fprintf(Fp,"* %s %d %d %d %d %d\n",LetterStr, Cycle,alldone,i,N,MAXPOS);
				fclose(Fp);*/
				i++;
			}
			LetterStr[i] = 0;

/*			Fp = fopen("letterstrs.txt","a");
			fprintf(Fp,"%s %d %d %d %d %d\n",LetterStr, Cycle,alldone,i,N,MAXPOS);
			fclose(Fp);*/

			while(i < MAXPOS) {
				GPCRoute.WdAct[i++] = 0;
			}

			//	GPCRoute.InputGpc(LetterStr);
			//	GPCRoute.OutputWord();
		
			//for(i=0;i<MAXPOS;i++) GPCRoute.WdAct[i] = 0;

/*			Fp = fopen("letterstrs.txt","a");
			fprintf(Fp,"%s %d %d %d %d %d\n",LetterStr, Cycle,alldone,i,N,MAXPOS);
			fclose(Fp);*/

		
		/*	for(i=0;i<strlen(LetterStr);i++) {
				if(LetterStr[i] == '+' ) {
					LetterStr[i] = 0;
					alldone = 1;
					ZNet.ParseCodasFinalLetter = 1;
					//break;
				}
			}*/

			if(strlen(LetterStr) == strlen(lowerword) ) {
					LetterStr[i] = 0;
					alldone = 1;
					ZNet.ParseCodasFinalLetter = 1;	
			}
		}

		if(ENGLISH == 1) {
			for(i=0;i<strlen(LetterStr);i++) LetterStr[i] = tolower(LetterStr[i]);
		}
		strcpy(ZNet.SplitLetterWord, LetterStr);
	//	if(OldN != N) ZNet.SetNet(LetterStr); //removed 2009.Nov

		/******** Attention Window **************/
		used = N - strlen(LetterStr);
		uptolet = 0;

		 JustChanged = 0;
		 if(N>0) {	
			 if(OldN != N) { //Could optimize more get rid of new Ns that don't change the letter length.
				
				 /*	Fp = fopen("letterstrs2.txt","a");
					fprintf(Fp,"%s %d %d %d N=%d %d\n",LetterStr, Cycle,alldone,i,N,MAXPOS);
					fclose(Fp);*/
		
				ZNet.SetNet(LetterStr); //added 2009.nov PARAMETER DOES NOTHING!!!!
				ZNet.ForwardPass();

				for(i=0;i<MAXPHONE;i++) {
					 for(j=0;j<(NUMBER_OF_PHONEMES );j++) {
						if(ZNet.Output[i][j] < 0.05) ZNet.Output[i][j] = 0;
					 }
				}
			 }
			 for(i=0;i<MAXPHONE;i++) {
				for(j=0;j<(NUMBER_OF_PHONEMES );j++) {
					if(! ((i==MAXPHONE-1) && j > (NUMBER_OF_PHONEMES-NSTRESS-1)) ) {
							PhonBuf.NetInput[i][j] += (ZNet.Output[i][j] * GPCRoute.exc_gpc_phone);
						/*	Fp = fopen("valsacts.txt","a");
							fprintf(Fp,"%d %d %d %c ZNO=%f znexph=%f pbni=%f znout%f %f \n",i,j,Cycle,Phonemes[j],\
									ZNet.Output[i][j],\
									ZNet.Output[i][j] * ZNet.exc_ZNet_phone,\
									PhonBuf.NetInput[i][j],\
									ZNet.Output[i][j] * GPCRoute.exc_gpc_phone,\
									GPCRoute.exc_gpc_phone);
							fclose(Fp);	*/
					} else {
						//WHEN TO START ADDING STRESS INPUT FROM THE NET.
						//	Fp = fopen("valsacts.txt","a");
						////	fprintf(Fp,"%s %d %d \n",LetterStr, Cycle,alldone);
						//	fclose(Fp);
						if(/*ZNet.ntempvowels == 2*/ alldone == 1) {
							PhonBuf.NetInput[i][j] += (ZNet.Output[i][j] * /*ZNet.exc_ZNet_phone*/GPCRoute.exc_gpc_phone );
						/*	Fp = fopen("valsacts.txt","a");
							fprintf(Fp,"%d %d %d %f %f %f \n",i,j,Cycle,ZNet.Output[i][j],ZNet.Output[i][j] * ZNet.exc_ZNet_phone, PhonBuf.NetInput[i][j]);
							fclose(Fp);*/					
						}
					}
				}
			}
		}
		if(OldN != N) {
			 OldN = N;
			 JustChanged = 1;
		}
	} else {
		/*Early DRC */
	}
}

void DRC_Overall::DecayCycle()
{

	int i,j, GraphCat;
	float netinput;

	//Update Letters Could just calculate via 1-X ^ n
	for(i=0;i<MAXPOS;i++) {
		for(j=0;j<NLETTERS;j++) {
			LetterSpots.Letters[i][j] = LetterSpots.Letters[i][j] * (1 - LetterSpots.Decay);
		}
	}

	for(i=0;i<Num_Word;i++) {
		OrthLex.Activations[i] = OrthLex.Activations[i] * (1 - OrthLex.Decay);
	}

	for(i=0;i<Num_Phon;i++) {
		PhonLex.Activations[i] = PhonLex.Activations[i] * (1 - PhonLex.Decay);
	}		

	for(i=0;i<MAXPHONE;i++) {
		for(j=0;j<NUMBER_OF_PHONEMES;j++) {
			PhonBuf.Buffer[i][j] = PhonBuf.Buffer[i][j] * (1 - PhonBuf.Decay);
		}
	}
	
}
		
//Gets the actual phonology generated and turns it into nice phon, using output rules
void DRC_Overall::FixPron()
{
	GPCRoute.OutputWord();
}

			
void DRC_Overall::ClearRun()
{
	int i,j;
	FILE *fp;

	//fp = fopen("here1.txt","a");fprintf(fp,"1\n"); fclose(fp);
	OldN = -1;
	Cycle = 1;

	Features.resetfeatures();
	LetterSpots.ResetLetters();
	OrthLex.ResetOrth();
	PhonLex.ResetPhon();
	PhonBuf.ResetBuf();
	PhonFeatures.resetfeatures();
	SemSys.ResetFeatures();
	SemFeatures.ResetFeatures();
	ZNet.ResetInputs();
	ZNet.ResetAll();

	//fp = fopen("here7.txt","a");fprintf(fp,"1\n"); fclose(fp);
	for(i=0;i<MAXPHONE+1;i++) GPCRoute.WdAct[i] = 0;

	OrthLex.OverPoint = FALSE;
	PhonLex.OverPoint = FALSE;

	if(ClearGraph == TRUE) {
		ClearAll();
	} else {
		ClearAllNoRedraw();
	}
	alldone = 0;

	LetterStr[0] =0;
}

void DRC_Overall::AddWord(char *Word)
{
	int i;
	FILE *fp;

	for(i=0;i<MAXPOS && i < strlen(Word);i++) {
		if(Word[i] == '+') break;
	}
	InitialWordLength = i;
	//Word[i] = 0;
	strcpy(ActualWord, Word);
	GPCRoute.EOW = FALSE;
	resetcyc = GPCRoute.gpc_cyc_ph_ph;
	//strcpy(ZNet.WholeWord, Word);

	//for(i=0;i<InitialWordLength;i++) lowerword[i] = tolower(Word[i]);
	strcpy(lowerword,Word);
	lowerword[InitialWordLength] = 0;
	
	strcpy(ZNet.GlobalWord,lowerword);
	ZNet.ToTrain = 0;

	Features.ClampWord(Word);
}

void  DRC_Overall::AddWords(char *Word, char *Word2, float amount1, float amount2)
{
	int i;
	FILE *fp;

	for(i=0;i<MAXPOS && i < strlen(Word);i++) if(Word[i] == '+') break;
	InitialWordLength = i;
	strcpy(lowerword,Word);
	lowerword[InitialWordLength] = 0;

	//Word[i] = 0;
	strcpy(ActualWord, Word);
	GPCRoute.EOW = FALSE;
	resetcyc = GPCRoute.gpc_cyc_ph_ph;
	//strcpy(ZNet.WholeWord, Word);
	strcpy(ZNet.GlobalWord,lowerword);
	ZNet.ToTrain = 0;

	Features.ClampWords(Word,Word2,amount1,amount2);
}


void DRC_Overall::AddPron(char Pron[])
{
	PGCRoute.EOW = FALSE;
	//PhonBuf.ClampPron(Pron);		
	PhonFeatures.ClampPhon(Pron);
}	




int DRC_Overall::RunCycle()
{
	int Named;
	int i,j, k, Spot, Pos;
	float ProbErr;
	FILE *Fp, *crud;

	srand( (unsigned)time( NULL ) );

	ZNet.HitDeadNode = 0;
////	Fp = fopen("Runcycle.txt","a");
//	fprintf(Fp,"%d \n",Cycle);
//	fclose(Fp);


	k = 0;
	SetLatInhibition();
	
//	Fp = fopen("Runcycle.txt","a");
//	fprintf(Fp,"LI \n");
//	fclose(Fp);
//
	if(!LetterOff) FtOtLetter(); 

//	Fp = fopen("Runcycle.txt","a");
//	fprintf(Fp,"LE \n");
//	fclose(Fp);

	if(!OrthOff) LtPhtOrthography(); 

//	Fp = fopen("Runcycle.txt","a");
//	fprintf(Fp,"LPO \n");
//	fclose(Fp);


	if(!PhonOff) PhtOtPhonLex(); 

//	Fp = fopen("Runcycle.txt","a");
//	fprintf(Fp,"PHPL \n");
//	fclose(Fp);


	if(!PhonemeOff) PhUtPhoneme();

	/*Fp = fopen("Runcycle.txt","a");
	fprintf(Fp,"11 %d Cycle \n", Cycle);
	fclose(Fp);*/


	if(!GPCOff) {
		UpdateGpcRoute();
		GPCOtP();
	}
	if(!PGCOff) {
		//UpdatePgcRoute();
		//PGCPtO();
	}
	/*Fp = fopen("Runcycle.txt","a");
	fprintf(Fp,"22 %d Cycle \n", Cycle);
	fclose(Fp);*/
	UpdateActivation(); 

	/*Fp = fopen("Runcycle.txt","a");
	fprintf(Fp,"33 %s \n",ActualWord);
	fclose(Fp);*/

	PhonBuf.NOrthSylls = ZNet.ntempvowels;
	Named = PhonBuf.PronWord();
	
//	Fp = fopen("TestWords.txt","a");
//	fprintf(Fp,"%f %f \n",PhonBuf.Buffer[15][NUMBER_OF_PHONEMES-2] > PhonBuf.Buffer[15][NUMBER_OF_PHONEMES-1);
//	fclose(Fp);

	Cycle++;



#if ISMARCO //XXX
//	return Named;

	if(/*Cycle > CyclesBeforeNaming*/1==1) {
		//return(Named);

		PhonBuf.WordSaid = 0;
		return Named;

	} else return FALSE;
#else
	return(Named);
#endif
}

int DRC_Overall::RunCycleSpell()
{
	int Named;



//	ClampingCycles;

	SetLatInhibition(); 

	if(!LetterOff) FtOtLetter(); 
	if(!OrthOff) LtPhtOrthography(); 
	if(!PhonOff) PhtOtPhonLex(); 
	if(!PhonemeOff) PhUtPhoneme(); 

	if(!GPCOff) {
		UpdateGpcRoute();
		GPCOtP();
	}
	//if(!PGCOff) {
		UpdatePgcRoute();
		PGCPtO();
//	}

	UpdateActivation(); 
	
	Named = LetterSpots.SpellWord();
	Cycle++;

	return(Named);
}

int DRC_Overall::TestWord(char *theword, int spot, int runcycles, int Fast)
{
	FILE *crud,*fp;
	int OverallCycles ;
	int Cycles = 0;
	int Clear = TRUE;
	int TrainMore;
	int i,j;
	int NamingStop = TRUE, SpellingStop, NoStop;
	int Named;
	char Word[200], start1[500], start2[500], OldWord[500], Word2[50];
	char bit[100];

	TheColor = 0;

	LetterSpots.LetterOver = FALSE;
	PhonBuf.StartChecking = FALSE;
	strcpy(Word, theword);    	
	DisLetters =FALSE ;
	DisOrthLex =FALSE ;
	DisPhonLex =FALSE ;
	DisPhonemes =FALSE ;
	DisGPC =FALSE ;
	DisPGC =FALSE ;
	DisTotalSum =FALSE ;
	
	LetterOff = FALSE;
	OrthOff = FALSE;
	PhonOff = FALSE;
	PhonemeOff = FALSE;
	GPCOff = FALSE;
	PGCOff = TRUE;

	ClearGraph = FALSE;
	SaveGraphs = FALSE;
	SpecSave = FALSE;
	SpecOrthSave = FALSE;
	SpecTotOrthSave = FALSE;

	DecayEveryCycle = FALSE;
	OrthLex.PointDecayOn = FALSE;
	PhonLex.PhonPointDecayOn = FALSE;
	SwitchBoth = FALSE;

	MarcoDelay = FALSE;
	MarcoNormal = TRUE;
	MarcoGrapheme = FALSE;

	NamingStop = TRUE;
	SpellingStop = FALSE;
	NoStop = FALSE;
	Cycles = runcycles;
	Clear = TRUE;
	AddNoise = FALSE;

		//XX	
	strcpy(OldWord,Word);
	if( strlen(Word) > MAXPOS) Word[MAXPOS] = 0;
	else {	
		for(i=strlen(Word);i<MAXPOS;i++) {
			Word[i] = WORDPAD;
		}
		Word[i] = 0;
	}	
	//strcpy(GlobalWord, Word);

	GPCRoute.NLetters = strlen(OldWord);
	GPCRoute.EOW = TRUE;
	GPCRoute.InputGpc(OldWord);
	GPCRoute.OutputWord();	

	if(Fast == 1) TheWords.ReLoadDataSlow(Word, GPCRoute.FinalWord);
	else TheWords.ReLoadDataSlow3(Word, GPCRoute.FinalWord);
	
	Num_Word = TheWords.Word_Count;
	Num_Phon = TheWords.Pron_Count ;
	OrthLex.Num_Word = TheWords.Word_Count;
	PhonLex.Num_Phon =  TheWords.Pron_Count; 
	
	ClearRun(); 
    
	OrthLex.OverPoint = FALSE;
	OverallCycles = 0;
	
	AddWord(Word);
	Named = FALSE;
	TrainMore = FALSE;
	PhonBuf.StartChecking = FALSE;
	LetterSpots.LetterOver = FALSE;

	for(i=0;i<Cycles;i++) {
		Named = RunCycle();
		OverallCycles++;
		if(Named == TRUE)	{	
			WordRes[spot] = OverallCycles;
			strcpy(PhonRes[spot],PhonBuf.WordSoFar);
			strcpy(OrthRes[spot],Word);
			StressRes[spot] = PhonBuf.MaxStress;	
			break;
		}
	
		if(i == Cycles) {
			WordRes[spot] = OverallCycles;
			strcpy(PhonRes[spot],PhonBuf.WordSoFar);
			StressRes[spot] = PhonBuf.MaxStress;	
			strcpy(OrthRes[spot],Word);
		}	
	} 
	return 1;
    
}




void DRC_Overall::TrainNetwork(int Cycles)
{
	int i, j,k,l,m, sampling = 2, num, NumSylls;

//	int Overall_Count;
	int onetoone = 0, more = 0, less = 0, totaltrained = 0;
	float sum;
	FILE *fp,*fp1,*fp2;
	char FileWord[100], FilePhon[100], FilePhon2[100], Orth[100], ParsedOrth[100];
	int  TheStress;
	float Freq1, Freq2;
	char word1[100], word2[100], crap[100];
	char littlebit[20];	

	ZNet.TotalError = 0;

	/*fp = fopen("EnglishManyRules3.txt","r");
	for(i=0;i<115;i++) {
		fscanf(fp,"%s%s",word1,word2);
		ZNet.MakeRule(word1,word2);
	}
	fclose (fp);
	
	CyclesTrained++;
	fp = fopen("TrainRes.txt","a");
	fprintf(fp,"->%d %f \n",CyclesTrained, ZNet.TotalError);
	fclose(fp);

	return;*/


	ZNet.InTraining = 1;
//	srand(10);

	ZNet.TotalError = 0;

	//srand( (unsigned)time( NULL ) );
	if(sampling ==1 ) {
	} else if (sampling == 2) {
		
	//	fp = fopen("NewDB.DEC.2008.txt","r");

		
	//	fp = fopen("ItalianTrainingDB3.txt","r");
		
		if(ENGLISH == 1) fp = fopen("./EnglishRes/EnglishNewTraining.txt","r");
		else if (ITALIAN == 1) fp = fopen("./ItalianRes/ItalianTrainingDB3.txt","r");
		else if (GERMAN == 1) fp = fopen("./GermanRes/GermanTraining.txt","r"); 
		else if (FRENCH == 1) fp = fopen("./FrenchRes/FrenchTraining.txt","r"); 
		if(fp == 0) exit(1);

		//ZNet.Phonologies = fopen("phonologies.txt","r");
	
		//for(i=0;i<32266;i++) {
#if ENGLISH
		for(i=0;i<32196;i++) {
			fscanf(fp,"%s%s%s%d%f%f%d",FileWord,ParsedOrth, FilePhon, &TheStress, &Freq1, &Freq2, &NumSylls ); //2008.training
			j = strlen(ParsedOrth);
			ParsedOrth[j-6] = 0;
#elif ITALIAN
		for(i=0;i<63245;i++) {
			fscanf(fp,"%s%s%s%d%f%f%d",FileWord,ParsedOrth, FilePhon, &TheStress, &Freq1, &Freq2, &NumSylls ); //2008.training	
#elif GERMAN
		for(i=0;i<51765;i++) {
			fscanf(fp,"%s%s%s%d%f%f%d",FileWord,ParsedOrth, FilePhon, &TheStress, &Freq1, &Freq2, &NumSylls ); //2008.training
			j = strlen(ParsedOrth);
			ParsedOrth[j-6] = 0;
#elif FRENCH
		for(i=0;i<101092;i++) {
			fscanf(fp,"%s%s%s%d%f%f%d%s",FileWord,ParsedOrth, FilePhon, &TheStress, &Freq1, &Freq2, &NumSylls,crap ); //2008.training
#endif
			/*fp2 = fopen("test111111.txt","a");
			fprintf(fp2,"[%s] [%s] %d %d %f\n",ParsedOrth, FilePhon,NumSylls,TheStress,Freq1);
			fclose(fp2);*/


			if( (strcmp(ParsedOrth,"++++++++++++")) != 0) {
			
				if(ENGLISH ==1) {
					j = strlen(FilePhon);
					FilePhon[j-16] = 0;
				} else if(GERMAN ==1 ) {
					j = strlen(FilePhon);
					FilePhon[j-16] = 0;
				} else if (ITALIAN == 1) {
					if(Freq1 == -1) Freq1 = 0;
					if(Freq2 == -1) Freq2 = 0;
				} else if (FRENCH == 1) {
					j=strlen(FilePhon);
					//FilePhon[j-9] = 0;
					//j=strlen(ParsedOrth);
					//ParsedOrth[j-3] = 0;
				}
			

		//	if(TheStress == 1) TheStress = 2;



		/*	if(NumSylls == 1) TheStress = 1;
			if(NumSylls == 2) {
				if(TheStress == 2) TheStress = 1;
				else TheStress = 2;
			}
			if(NumSylls == 3) {
				if(TheStress == 3) TheStress = 1;
				else if(TheStress == 1) TheStress = 3;
			}*/


			//if( (strcmp(FilePhon,"X")) != 0 && strlen(ParsedOrth) > 12) {
			
			/*	fp1 = fopen("try3.txt","a");
				fprintf(fp1,"%s %s %s %d %f %f %d\n",FileWord,ParsedOrth, FilePhon, TheStress, Freq1, Freq2, NumSylls ); //2008.training
				fclose(fp1);*/
				
					ZNet.InTraining = 0;
					ZNet.ToTrain = 0;
					ZNet.ThisFreq = Freq1;
					ZNet.ThisStress = TheStress;
					ZNet.ThisNumSylls = NumSylls;
					strcpy(ZNet.ThisWord, FileWord);
//			ZNet.TrainCycle("dog","dQg",.5,.5);
			//ZNet.TrainCycleFromAll2("dog","DQG","dog",.5,1,1);
					if(ENGLISH == 1) ZNet.TrainCycleFromAll2(FileWord,ParsedOrth,FilePhon, log(Freq1 +2.0)/log(1093547.0 +2.0), NumSylls, TheStress);
					else if (ITALIAN ==1) ZNet.TrainCycleFromAll2(FileWord,ParsedOrth,FilePhon, log(Freq1 +2.0)/log(606000000.0 +2.0), NumSylls, TheStress);
					else if (GERMAN == 1) ZNet.TrainCycleFromAll2(FileWord,ParsedOrth,FilePhon, log(Freq1 +2.0)/log(150507.0 + 2.0), NumSylls, TheStress);
					else if (FRENCH == 1) ZNet.TrainCycleFromAll2(FileWord,ParsedOrth,FilePhon, log( Freq1 +2.0)/log(867040.78 + 2.0), NumSylls, TheStress);
			}
				//	}
		}
	
		fclose(fp);
		//fclose(ZNet.Phonologies);

		
		/*if(ZNet.TheTrainingCycle == 0) {
				fp=fopen("graphinfo.txt","w");
				fprintf(fp,"%d \n",ZNet.nGraphemesUpTo[0]);
				fclose(fp);

				fp=fopen("graphinfo.txt","w");
				for(i=0;i<10;i++) {
					fprintf(fp,"%d %d    ",i, ZNet.nGraphemesUpTo[i]);
					for(j=0;j<ZNet.nGraphemesUpTo[i];j++) {
						fprintf(fp," [%d %s %f %d %c] \n",ZNet.GraphemesInf[i][j], ZNet.GraphemesInflets[i][j], ZNet.GraphemeFreqsInf[i][j], ZNet.PhonemesInf[i][j], ZNet.PhonemesCInf[i][j]); 
					}
					fprintf(fp,"\n");
				}
				fclose(fp);
		}*/

	} else if (sampling == 3) { //Need better accuracy
	}

	ZNet.InTraining = 0;
	CyclesTrained++;
	fp = fopen("TrainRes.txt","a");
	fprintf(fp,"%d %f %d\n",CyclesTrained, ZNet.TotalError,Overall_Count);
	fclose(fp);
	
}
