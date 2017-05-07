//
//  GPUEffect.h
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#ifndef GPUEffect_h
#define GPUEffect_h

#include <stdio.h>
#include "Effect.h"


class GPUEffect: public Effect {

protected:
    
public:
    
    GPUEffect( const ImVec2 position ) : Effect(position) {};
    virtual int getTypeFlags();

};

typedef boost::shared_ptr<GPUEffect> GPUEffectRef;

#endif /* GPUEffect_h */
