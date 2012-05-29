/* 
 * File:   Channel.cpp
 * Author: matt
 * 
 * Created on 28 May 2012, 10:37 PM
 */

#include "CommChannel.h"
#include "ResultSender.h"


CommChannel::CommChannel(const int endpoint): _endpoint(endpoint), _closed(false), _totalExpected(0)
{
    bind();
}

CommChannel::CommChannel(const CommChannel& orig)
{
}

void CommChannel::bind()
{
    _streamRequest = MPI::COMM_WORLD.Irecv(&_totalExpected, 1, MPI::INT, _endpoint, COMPLETE);
    _dataRequest = MPI::COMM_WORLD.Irecv(_receiveBuffer, 1024, MPI::CHAR, _endpoint, MESSAGE);
    tick();
}

void CommChannel::tick()
{
    if (_dataRequest.Test())
    {
        char temp[1024];
        strcpy(temp, _receiveBuffer);
        
        _data.push_back(temp);
        
        _dataRequest = MPI::COMM_WORLD.Irecv(_receiveBuffer, 1024, MPI::CHAR, _endpoint, MESSAGE);
    }
    
    if (_streamRequest.Test() && !_closed)
    {
        _closed = true;
        DEBUG_LOG << "Stream terminated: " << _endpoint << std::endl;
    }
}

bool CommChannel::closed()
{
    return _closed && _data.size() == _totalExpected;
}

CommChannel::~CommChannel()
{
}

