/* 
 * File:   CDPAggregator.cpp
 * Author: matt
 * 
 * Created on 6 June 2012, 10:53 PM
 */

#include "CDPAggregator.h"
#include <string>
#include <vector>
#include <map>

CDPAggregator::CDPAggregator()
{
}

CDPAggregator::CDPAggregator(const CDPAggregator& orig)
{
}

CDPAggregator::~CDPAggregator()
{
}

void CDPAggregator::aggregate(std::vector<std::map<std::string, std::string> > result)
{
    DEBUG_LOG << "Aggregate - total = " << result.size() << std::endl;
    
    for (std::vector<std::map<std::string, std::string> >::iterator iter = result.begin(); iter != result.end(); iter++)
    {
        std::map<std::string, std::string> wordRes = *iter;
        
        DEBUG_LOG << "Result " << wordRes["word"] << std::endl;
    }
    
    
}

