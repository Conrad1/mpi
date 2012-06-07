/* 
 * File:   ResultSender.cpp
 * Author: matt
 * 
 * Created on 27 May 2012, 8:04 PM
 */

#include "ResultSender.h"
#include "MapSerializer.h"
#include <mpi/mpi.h>

ResultSender::ResultSender(const MPIRunner &runner) : _runner(runner), _sendCount(0)
{
}

ResultSender::ResultSender(const ResultSender& orig) : _runner(orig._runner)
{
}

ResultSender::~ResultSender()
{
}

void ResultSender::sendResult(std::map<std::string, std::string> & result) const
{
    MapSerializer serializer;
    std::string serialized = serializer.serialize(result);
    const char * buf = serialized.c_str();
    
    MPI::COMM_WORLD.Send(buf, serialized.length() + 1, MPI::CHAR, 0, MESSAGE);
    _sendCount++;
}

void ResultSender::processComplete()
{
    MPI::COMM_WORLD.Send(&_sendCount, 1, MPI::INT, 0, COMPLETE);
}


