/* 
 * File:   main.cpp
 * Author: matt
 *
 * Created on 27 May 2012, 11:37 AM
 */

#include <cstdlib>
#include <mpi/mpi.h>
#include "MPIRunner.h"
#include "common.h"
#include "ResultSender.h"

using namespace std;

class StubTask : public Task
{
public:
    virtual void run(ResultSender & sender)
    {
        DEBUG_LOG << "Running" << std::endl;
        for (int i = 0; i < 10; i++)
        {
            map<string, string> data;
        
            data["a"] = "b";
            sender.sendResult(data);
        }
        
        sleep(1);
    }
};

class StubAggregator : public Aggregator
{
public:
    virtual void aggregate(vector<map<string, string> > results)
    {
        DEBUG_LOG << "Aggregating" << results.size() << "results" << std::endl;
    }
};


int main(int argc, char** argv)
{
    MPIRunner runner(argc, argv);
    StubTask task;
    StubAggregator aggregator;
    
    runner.startup(task, aggregator);
    return 0;
}

