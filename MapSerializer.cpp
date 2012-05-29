/* 
 * File:   MapSerializer.cpp
 * Author: matt
 * 
 * Created on 27 May 2012, 8:09 PM
 */

#include <iosfwd>
#include <ios>
#include <sstream>
#include <iostream>

#include "MapSerializer.h"

MapSerializer::MapSerializer()
{
}

MapSerializer::MapSerializer(const MapSerializer& orig)
{
}

MapSerializer::~MapSerializer()
{
}

std::string MapSerializer::serialize(std::map<std::string, std::string> & theMap)
{
    std::stringstream out(std::stringstream::out);
    
    for (std::map<std::string, std::string>::iterator iter = theMap.begin(); 
         iter != theMap.end(); 
         ++iter)
    {
        out << (*iter).first << "=" << (*iter).second << "\n";
    }
    
    return out.str();
}

std::map<std::string, std::string> MapSerializer::deserialize(const std::string & mashed)
{
    std::map<std::string, std::string> result;
    
    std::string temp = mashed;
    
    std::string line;
    for (line = temp.substr(0, temp.find("\n")); line != ""; line = temp.substr(0, temp.find("\n")))
    {
        temp.erase(0, temp.find("\n") + 1);
        std::string key = line.substr(0, line.find("="));
        std::string value = line.substr(line.find("=") + 1);
        
        result[key] = value;
    }
    
    return result;
}



