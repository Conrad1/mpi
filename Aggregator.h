/* 
 * File:   Aggregator.h
 * Author: matt
 *
 * Created on 27 May 2012, 6:52 PM
 */

#ifndef AGGREGATOR_H
#define	AGGREGATOR_H
#include "common.h"

class Aggregator {
public:
    Aggregator();
    Aggregator(const Aggregator& orig);
    virtual ~Aggregator();
    
    virtual void aggregate(std::vector<std::map<std::string, std::string> > result) = 0;
private:

};

#endif	/* AGGREGATOR_H */

