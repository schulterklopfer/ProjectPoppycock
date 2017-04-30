//
//  Effect.hpp
//  PoppyCock
//
//  Created by jash on 30.04.17.
//
//

#ifndef Effect_h
#define Effect_h

#include <stdio.h>
#include "Entity.h"

class Effect: public Entity {

public:
    
    Effect( string id, ImVec2 position ) : Entity(id, position) {};

    virtual void draw( ImVec2 offset, float scale );
    virtual Type getType();
    
};

#endif /* Effect_h */