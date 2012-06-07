/* 
 * File:   CDPTask.h
 * Author: matt
 *
 * Created on 6 June 2012, 10:15 PM
 */

#ifndef CDPTASK_H
#define	CDPTASK_H

#include "../Task.h"
#include "DRC_Overall.h"

typedef struct {
	int Stress;
	char name[50];
	char pron[50];
	int cycles;
} DBRes;

//DBRes TheRes[10][2000];

typedef struct  {
	int ProcWithin;
	int NItems;
	char DataFile[50];
	char SavingNam[50];
} ProcInfo;

class CDPTask : public Task
{
public:
    CDPTask(const char * filename);
    CDPTask(const CDPTask& orig);
    virtual ~CDPTask();
    
    virtual void run(const ResultSender & resultSender, const MPIRunner & runner);
private:
    void runBatch(const ResultSender & resultSender);
    void computeBatch(const int node, const int totalNodes);
    DRC_Overall A_DRC;
    int start;
    int finish;
    
    const char * _filename;
};



#endif	/* CDPTASK_H */

