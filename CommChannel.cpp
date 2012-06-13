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
    MPI_Irecv(&_totalExpected, 1, MPI_INT, _endpoint, COMPLETE, MPI_COMM_WORLD, &_streamRequest);
    MPI_Irecv(_receiveBuffer, 1024, MPI_CHAR, _endpoint, MESSAGE, MPI_COMM_WORLD, &_dataRequest);
    tick();
}

void CommChannel::tick()
{
	int dataTest = 0;
	MPI_Test(&_dataRequest, &dataTest,  MPI_STATUS_IGNORE);
    if (dataTest)
    {
        char temp[1024];
        strcpy(temp, _receiveBuffer);
        
        _data.push_back(temp);
        
        _dataRequest = MPI_Irecv(_receiveBuffer, 1024, MPI_CHAR, _endpoint, MESSAGE, MPI_COMM_WORLD, &_dataRequest);
    }
    
	int streamTest = 0;
	MPI_Test(&_streamRequest, &streamTest,  MPI_STATUS_IGNORE);
    if (streamTest && !_closed)
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

