/* 
 * File:   ResultSender.cpp
 * Author: matt
 * 
 * Created on 27 May 2012, 8:04 PM
 */

#include "ResultSender.h"
#include "MapSerializer.h"
#include "common.h"

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
    
    MPI_Send((void *)buf, serialized.length() + 1, MPI_CHAR, 0, MESSAGE, MPI_COMM_WORLD);
    _sendCount++;
}

void ResultSender::processComplete()
{
    MPI_Send(&_sendCount, 1, MPI_INT, 0, COMPLETE, MPI_COMM_WORLD);
}


