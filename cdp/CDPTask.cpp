/* 
 * File:   CDPTask.cpp
 * Author: matt
 * 
 * Created on 6 June 2012, 10:15 PM
 */

#include "CDPTask.h"
#include <string.h>
#include "../common.h"
#include "../MPIRunner.h"
#include "../ResultSender.h"


#define TRUE 1
#define FALSE 0

static const char * WORKING_DIR = "/home/matt/work/mpi_wrapper";

CDPTask::CDPTask(const char * filename) : _filename(filename)
{
    start = 0;
    finish = 10;
    A_DRC.Initialize_It(34000);
    strcpy(A_DRC.TheWords.LoadDirectory, WORKING_DIR);
    A_DRC.Initialize_DRCOS(A_DRC.Num_Word, A_DRC.Num_Phon);
}

CDPTask::CDPTask(const CDPTask& orig)
{
}

CDPTask::~CDPTask()
{
}

void CDPTask::run(const ResultSender & resultSender, const MPIRunner & runner)
{
    computeBatch(runner.node(), runner.total());
    runBatch(resultSender);
}

void CDPTask::runBatch(const ResultSender & resultSender)
{
    DEBUG_LOG << "Running batch" << std::endl;
	FILE *crud,*fp;
    
    int lineCount = 0;
    
	int OverallCycles ;
	int Cycles = 0,Cyc, UpToCount = -1;
	int Clear = TRUE;
	int TrainMore;
	int i,j;
	int NamingStop = TRUE, SpellingStop, NoStop;
	int Named;
	char Word[200], start1[500], start2[500], OldWord[500], Word2[50];
	char bit[100];
	char GlobWord[200];

	//int compare( const void *arg1, const void *arg2 );

	A_DRC.TheColor = 0;

	//strcpy(A_DRC.QuickSaveName,"QuickSave.txt");
	//cout << "QSN " << A_DRC.QuickSaveName << endl;
	A_DRC.QuickSaveOn = TRUE;

	A_DRC.LetterSpots.LetterOver = FALSE;
	A_DRC.PhonBuf.StartChecking = FALSE;
	//p = dlg.TheWord.GetBuffer( 200 );
		//dlg.TheWord.ReleaseBuffer();

	A_DRC.DisLetters = 0;
	A_DRC.DisOrthLex = 0;
	A_DRC.DisPhonLex = 0;
	A_DRC.DisPhonemes = 0;
	A_DRC.DisGPC = 0;
	A_DRC.DisPGC = 0;
	A_DRC.DisTotalSum = 0;
	
	A_DRC.LetterOff = 0;
	A_DRC.OrthOff = 0;
	A_DRC.PhonOff = 0;
	A_DRC.PhonemeOff = 0;
	A_DRC.GPCOff = 0;
	A_DRC.PGCOff = 0;

	A_DRC.ClearGraph = 0;
	A_DRC.SaveGraphs = 0;
	A_DRC.SpecSave = 0;
	A_DRC.SpecOrthSave = 0;
	A_DRC.SpecTotOrthSave = 0;

	A_DRC.DecayEveryCycle = 0;
	A_DRC.OrthLex.PointDecayOn = 0;
	A_DRC.PhonLex.PhonPointDecayOn = 0;
	A_DRC.SwitchBoth = 0;

	A_DRC.MarcoDelay = 0;
	A_DRC.MarcoNormal = 1;
	A_DRC.MarcoGrapheme = 0;

	NamingStop = 0;
	SpellingStop = 0;
	NoStop = 0;
	Cycles = 300;
	Clear = 1;
	A_DRC.AddNoise =0;
	
	//printf("here %s \n", FileName);
	//scanf("%d",&i);
	fp = fopen(_filename,"r");
    DEBUG_LOG << fp << std::endl;
	fscanf(fp,"%s",Word);
	//printf("---fp= %s %d \n",Word,fp);

	if( (strcmp(Word,"naming")) != 0) {
		printf("error loading file!!! \n");
		return;
	}
	while (fscanf(fp, "%s%d", Word, &Cyc) != EOF) {
        DEBUG_LOG << "WORD: " << Word << std::endl;
		UpToCount++;
		if(UpToCount > finish) return;
		if(UpToCount >= start && UpToCount <= finish) {
			printf("-->%s\n",Word);fflush(stdout);
			strcpy(OldWord,Word);
			Cycles = Cyc;
			if( strlen(Word) > MAXPOS) Word[MAXPOS] = 0;
			else {	
				for(i=strlen(Word);i<MAXPOS;i++) {
					Word[i] = WORDPAD;
				}
				Word[i] = 0;
			}	
			strcpy(GlobWord, Word);
			A_DRC.TestWord(GlobWord,UpToCount,Cyc,1);
            std::map<std::string, std::string> res;
            res["word"] = Word;
            
            resultSender.sendResult(res);
			/*printf("here2 %s \n", Word);

			A_DRC.GPCRoute.NLetters = strlen(OldWord);
			A_DRC.GPCRoute.EOW = TRUE;
			A_DRC.GPCRoute.InputGpc(OldWord);
			A_DRC.GPCRoute.OutputWord();	

			A_DRC.TheWords.ReLoadDataSlow(Word, "test" );
			A_DRC.Num_Word = A_DRC.TheWords.Word_Count;
			A_DRC.Num_Phon = A_DRC.TheWords.Pron_Count ;
			A_DRC.OrthLex.Num_Word = A_DRC.TheWords.Word_Count;
			A_DRC.PhonLex.Num_Phon =  A_DRC.TheWords.Pron_Count; 
	
			A_DRC.ClearRun(); 
			A_DRC.OrthLex.OverPoint = FALSE;
	
			OverallCycles = 0;

			printf("-->%s \n",Word);
			
			A_DRC.AddWord(Word);
			Named = FALSE;
			TrainMore = FALSE;

		
			for(i=0;i<Cycles;i++) {
				Named = A_DRC.RunCycle();
				OverallCycles++;

				if(Named == TRUE)	{	
					A_DRC.QuickSave( Word, A_DRC.PhonBuf.WordSoFar, TRUE, OverallCycles);

					break;
				}
			}
			if(i == Cycles) {
				sprintf(bit,"  Not Named after %d Cycles", OverallCycles);
				A_DRC.QuickSave(Word, A_DRC.PhonBuf.WordSoFar, TRUE, OverallCycles);
			}*/
		}
	} 
    
    DEBUG_LOG << "Done Running batch" << std::endl;;
}

void CDPTask::computeBatch(const int node, const int totalNodes)
{
    int lineCount = 0, batchSize;
    
    FILE * fp = fopen(_filename,"r");
    char buf[1024];
    double cyc;
    
    while (fscanf(fp, "%s%d", buf, &cyc) != EOF)
    {
        lineCount++;
    }
    
    batchSize = lineCount / (totalNodes - 1);
    
    start = batchSize * (node - 1);
    finish = batchSize * node;
}

