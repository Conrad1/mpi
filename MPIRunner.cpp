/* 
 * File:   MPIRunner.cpp
 * Author: matt
 * 
 * Created on 27 May 2012, 5:20 PM
 */

#include "MPIRunner.h"
#include <mpi/mpi.h>
#include "common.h"
#include "ResultSender.h"
#include "CommChannel.h"
#include "MapSerializer.h"


MPIRunner::MPIRunner(int &argc, char **&argv)
{
    MPI::Init(argc, argv);
    _myNode = MPI::COMM_WORLD.Get_rank();
    
    _totalNodes = MPI::COMM_WORLD.Get_size();
    DEBUG_LOG << "Started up - node " << _myNode << std::endl;

}

MPIRunner::MPIRunner(const MPIRunner& orig)
{
    
}

void MPIRunner::startup(Task & task, Aggregator & aggregator)
{
    ResultSender sender(*this);
    
    if (!isMaster())
    {
        task.run(sender);
        sender.processComplete();
    }
    else
    {
        waitToAggregate(aggregator);
    }
}

void MPIRunner::waitToAggregate(Aggregator & aggregator)
{
    DEBUG_LOG << "Waiting to aggregate results" << std::endl;
    DEBUG_LOG << "Total nodes " << _totalNodes << std::endl;
    
    CommChannel * channels[_totalNodes];
    MapSerializer serializer;
    for (int node = 1; node < _totalNodes; node++ )
    {
        channels[node - 1] = new CommChannel(node);
    }
    
    bool allClosed = false;
    while (!allClosed)
    {
        usleep(1000);
        allClosed = true;
        for (int ch = 0; ch < (_totalNodes - 1); ch++)
        {
            channels[ch]->tick();
            allClosed &= channels[ch]->closed();
        }
    }
    
    DEBUG_LOG << "All processes done" << std::endl;
    std::vector<std::map<std::string, std::string> > results;
    
    for (int ch = 0; ch < (_totalNodes - 1); ch++)
    {
        const std::vector<std::string> & data = channels[ch]->data();
        
        DEBUG_LOG << "Channel " << ch + 1 << ", results " << data.size() << std::endl;
        
        for (int i =0; i < data.size(); i++)
        {
            results.push_back(serializer.deserialize(data[i]));
        }
    }
    
    aggregator.aggregate(results);
}

MPIRunner::~MPIRunner()
{
    MPI::Finalize();
}

