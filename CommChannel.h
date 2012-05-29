/* 
 * File:   Channel.h
 * Author: matt
 *
 * Created on 28 May 2012, 10:37 PM
 */

#ifndef CHANNEL_H
#define	CHANNEL_H

#include <mpi/mpi.h>
#include "common.h"


class CommChannel {
public:
    CommChannel(const int endpoint);
    CommChannel(const CommChannel& orig);
    virtual ~CommChannel();
    void tick();
    bool closed();
    
    const std::vector<std::string> & data() { return _data; }
    
private:
    int _endpoint;
    
    void bind();
    MPI::Request _streamRequest;
    MPI::Request _dataRequest;
    char _receiveBuffer[1024];
    bool _closed;
    int _totalExpected;
    std::vector<std::string> _data;
};

#endif	/* CHANNEL_H */

