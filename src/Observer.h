//
//  Observer.hpp
//  PoppyCock
//
//  Created by jash on 01.05.17.
//
//

#ifndef Observer_h
#define Observer_h

#include <stdio.h>
#include "Entity.h"

class Observer: public Entity {
    
public:
    
    Observer( string id, ImVec2 position ) : Entity(id, position) {};
    
    virtual void draw( ImVec2 offset, float scale );
    virtual int getTypeFlags();
    virtual bool acceptsInputFrom(boost::shared_ptr<Entity> &source );
    virtual bool providesOutputTo(boost::shared_ptr<Entity> &source );
    virtual bool providesOutput();
};
#endif /* Observer_h */
