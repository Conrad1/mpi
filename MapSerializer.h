/* 
 * File:   MapSerializer.h
 * Author: matt
 *
 * Created on 27 May 2012, 8:09 PM
 */

#ifndef MAPSERIALIZER_H
#define	MAPSERIALIZER_H

#include <map>

class MapSerializer {
public:
    MapSerializer();
    MapSerializer(const MapSerializer& orig);
    std::string serialize(std::map<std::string, std::string> & theMap);
    std::map<std::string, std::string> deserialize(const std::string &);
    virtual ~MapSerializer();
private:

};

#endif	/* MAPSERIALIZER_H */

