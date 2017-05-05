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
    
    Effect( const ImVec2 position ) : Entity(position) {};

    virtual void draw( const ImVec2 offset, const float scale );
    virtual int getTypeFlags();

};

#endif /* Effect_h */
