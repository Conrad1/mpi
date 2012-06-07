/* 
 * File:   CDPAggregator.h
 * Author: matt
 *
 * Created on 6 June 2012, 10:53 PM
 */

#ifndef CDPAGGREGATOR_H
#define	CDPAGGREGATOR_H

#include "../Aggregator.h"

class CDPAggregator : public Aggregator
{
public:
    CDPAggregator();
    CDPAggregator(const CDPAggregator& orig);
    virtual ~CDPAggregator();
    
    virtual void aggregate(std::vector<std::map<std::string, std::string> > result);
private:

};

#endif	/* CDPAGGREGATOR_H */

