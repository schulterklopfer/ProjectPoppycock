//
//  Serializable.hpp
//  PoppyCock
//
//  Created by jash on 07.06.17.
//
//

#ifndef Serializable_h
#define Serializable_h

#include <stdio.h>
#include <string>
#include "ofxJSON.h"

class Serializable {
    
    
public:
    virtual void serialize( Json::Value* outJSON);
    virtual void deserialize( Json::Value* inJSON );
    virtual void save( std::string fileName );
    virtual void load( std::string fileName );

};

#endif /* Serializable_h */
