/* 
 * File:   newsimpletest.cpp
 * Author: matt
 *
 * Created on 27/05/2012, 8:38:50 PM
 */

#include <stdlib.h>
#include <iostream>
#include "common.h"
#include "MapSerializer.h"


static int failCount = 0;

void assertTrue(const char * message, bool cond)
{
    if (!cond)
    {
        failCount++;
        std::cout << "Failed: " << message << std::endl;
    }
}

void testSerializeDeserialize()
{
    MapSerializer serializer;
    std::map<std::string, std::string> map;
    
    map["key1"] = "val1";
    map["key2"] = "val2";
    
    std::string serialized = serializer.serialize(map);
    std::map<std::string, std::string> deserialized = serializer.deserialize(serialized);
    
    assertTrue("val1", deserialized["key1"] == map["key1"]);
    assertTrue("val2", deserialized["key2"] == map["key2"]);
}

void testSerializeDeserializeWithSpacesInValues()
{
    MapSerializer serializer;
    std::map<std::string, std::string> map;
    
    map["key1"] = "val1 hghg";
    map["key2"] = "val2 ghghg";
    
    std::string serialized = serializer.serialize(map);
    std::map<std::string, std::string> deserialized = serializer.deserialize(serialized);
    
    assertTrue("val1", deserialized["key1"] == map["key1"]);
    assertTrue("val2", deserialized["key2"] == map["key2"]);
}

void testSerializeDeserializeWithSpacesInKeys()
{
    MapSerializer serializer;
    std::map<std::string, std::string> map;
    
    map["key1 a"] = "val1 hghg";
    map["key2 b"] = "val2 ghghg";
    
    std::string serialized = serializer.serialize(map);
    std::map<std::string, std::string> deserialized = serializer.deserialize(serialized);
    
    assertTrue("val1", deserialized["key1 a"] == map["key1 a"]);
    assertTrue("val2", deserialized["key2 b"] == map["key2 b"]);
}


int main(int argc, char** argv)
{
    testSerializeDeserialize();
    testSerializeDeserializeWithSpacesInValues();
    testSerializeDeserializeWithSpacesInKeys();

    if (failCount)
    {
        std::cout << failCount << " Assertion failures!!" << std::endl;
    }
    else
    {
        std::cout << "Success" << std::endl;
    }
    return (!failCount ? EXIT_SUCCESS : -1);
}

