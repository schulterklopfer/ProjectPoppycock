//
//  Serializable.cpp
//  PoppyCock
//
//  Created by jash on 07.06.17.
//
//

#include "Serializable.h"

void Serializable::load(std::string fileName) {
    ofFile file = ofFile(fileName);
    
    if( file.isFile() ) {
        ofxJSONElement JSON;
        const bool parsingSuccessful = JSON.open(file.path());
        if( parsingSuccessful ) {
            deserialize(&JSON);
        }
    }
}

void Serializable::save(std::string fileName) {
    ofxJSONElement JSON;
    serialize( &JSON );
    const bool savingSuccessful = JSON.save(fileName);
}

void Serializable::serialize( Json::Value* outJSON) {}
void Serializable::deserialize( Json::Value* inJSON ) {}
