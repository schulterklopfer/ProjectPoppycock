//
//  ShaderEffect.hpp
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#ifndef ShaderEffect_h
#define ShaderEffect_h

#include <stdio.h>
#include "Effect.h"


class ShaderEffect: public Effect {

protected:
    
public:
    
    ShaderEffect( const ImVec2 position ) : Effect(position) {};
    virtual int getTypeFlags();

};

typedef boost::shared_ptr<ShaderEffect> ShaderEffectRef;

#endif /* ShaderEffect_h */
