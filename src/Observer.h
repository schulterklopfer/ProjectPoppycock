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
    
    Observer( const ImVec2 position ) : Entity(position) {};
    
    virtual void draw( const ImVec2 offset, const float scale );
    virtual int getTypeFlags();
    virtual bool acceptsInputFrom(const boost::shared_ptr<Entity> &source );
    virtual bool providesOutputTo(const boost::shared_ptr<Entity> &source );
    virtual bool providesOutput();

    virtual void update();

};
#endif /* Observer_h */
