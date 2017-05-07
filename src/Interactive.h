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


#define TO_ENTITY boost::static_pointer_cast<Entity>
#define TO_EFFECT boost::static_pointer_cast<Effect>
#define TO_AOE boost::static_pointer_cast<Effect::AOE>
#define TO_OBSERVER boost::static_pointer_cast<Observer>
#define TO_CONNECTOR boost::static_pointer_cast<Connector>

class Interactive {

protected:
    ofRectangle mBounds;
    
public:
    
    typedef enum Type {
        NONE      =  0,
        ENTITY    =  1,
        CONNECTOR =  2,
        EFFECT    =  4,
        OBSERVER  =  8,
        AOE       = 16,
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
