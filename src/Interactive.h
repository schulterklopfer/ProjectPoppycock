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

#include "boost/shared_ptr.hpp"

class Interactive {

public:
    
    typedef enum Type {
        NONE,
        EFFECT,
        OBSERVER,
        CONNECTOR
    } Type;
    
    typedef enum State {
        OVER   =  1,
        DOWN   =  2,
        DRAG   =  4,
        SELECT =  8,
        SOURCE = 16,
        TARGET = 32
    } State;

    virtual bool hitTest( float x, float y );
    virtual Type getType();

    int stateFlags;
    
};

typedef boost::shared_ptr<Interactive> InteractiveRef;
typedef std::vector<InteractiveRef> InteractiveList;
typedef InteractiveList::iterator InteractiveListIterator;

#endif /* Interactive_h */
