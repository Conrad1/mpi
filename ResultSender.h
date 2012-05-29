/* 
 * File:   ResultSender.h
 * Author: matt
 *
 * Created on 27 May 2012, 8:04 PM
 */

#ifndef RESULTSENDER_H
#define	RESULTSENDER_H
#include <map>
#include <string>
#include "MPIRunner.h"

const int COMPLETE = 11;
const int MESSAGE = 13;

class ResultSender {
public:
    
    ResultSender(const MPIRunner & runner);
    ResultSender(const ResultSender& orig);
    virtual ~ResultSender();
    
    void sendResult(std::map<std::string, std::string> & result);
    void processComplete();
private:
    
    const MPIRunner & _runner;
    int _sendCount;
    
};

#endif	/* RESULTSENDER_H */

