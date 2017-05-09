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
        NONE       = 0,
        ENTITY     = (1 << 0),
        CONNECTOR  = (1 << 1),
        OBSERVER   = (1 << 2),
        AOE        = (1 << 3),
        EFFECT     = (1 << 4),
        GPU_EFFECT = (1 << 5),
    } Type;
    
    typedef enum State {
        OVER   = (1 << 0),
        DOWN   = (1 << 1),
        DRAG   = (1 << 2),
        SELECT = (1 << 3),
        SOURCE = (1 << 4),
        TARGET = (1 << 5),
        GHOST  = (1 << 6),
        IDLE   = (1 << 7)
    } State;

    Interactive() : stateFlags(0) {};

    bool isOfType( Interactive::Type t );
    
    virtual bool hitTest( const float x, const float y );
    virtual int getTypeFlags();
    virtual void recalcBounds();
    virtual ofRectangle* getBounds();
    virtual void inspectorContent();
    int stateFlags;
    
};

typedef boost::shared_ptr<Interactive> InteractiveRef;
typedef std::vector<InteractiveRef> InteractiveList;
typedef InteractiveList::iterator InteractiveListIterator;

#endif /* Interactive_h */
