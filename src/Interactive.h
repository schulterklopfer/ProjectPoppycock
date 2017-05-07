//
//  Interactive.hpp
//  PoppyCock
//
//  Created by jash on 01.05.17.
//
//

#ifndef Interactive_h
#define Interactive_h

#include <stdio.h>
#include <vector>
#include "ofRectangle.h"
#include "boost/shared_ptr.hpp"


#define TO_(i,tFlag,T) (i==NULL?NULL:(i->isOfType(tFlag)?boost::static_pointer_cast<T>(i):NULL))

#define TO_ENTITY(i)        TO_(i, Interactive::Type::ENTITY, Entity)
#define TO_EFFECT(i)        TO_(i, Interactive::Type::EFFECT, Effect)
#define TO_SHADER_EFFECT(i) TO_(i, Interactive::Type::SHADER_EFFECT, ShaderEffect)
#define TO_AOE(i)           TO_(i, Interactive::Type::AOE, Effect::AOE)
#define TO_OBSERVER(i)      TO_(i, Interactive::Type::OBSERVER, Observer)
#define TO_CONNECTOR(i)     TO_(i, Interactive::Type::CONNECTOR, Connector)

class Interactive {

protected:
    ofRectangle mBounds;
    
public:
    
    typedef enum Type {
        NONE       =  0,
        ENTITY     =  1,
        CONNECTOR  =  2,
        OBSERVER   =  4,
        AOE        =  8,
        EFFECT     = 16,
        GPU_EFFECT = 32,
    } Type;
    
    typedef enum State {
        OVER   =  1,
        DOWN   =  2,
        DRAG   =  4,
        SELECT =  8,
        SOURCE = 16,
        TARGET = 32,
        GHOST  = 64
    } State;

    Interactive() : stateFlags(0) {};

    bool isOfType( Interactive::Type t );
    
    virtual bool hitTest( const float x, const float y );
    virtual int getTypeFlags();
    virtual void recalcBounds();
    virtual ofRectangle* getBounds();
    int stateFlags;
    
};

typedef boost::shared_ptr<Interactive> InteractiveRef;
typedef std::vector<InteractiveRef> InteractiveList;
typedef InteractiveList::iterator InteractiveListIterator;

#endif /* Interactive_h */
