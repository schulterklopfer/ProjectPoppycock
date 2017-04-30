//
//  Effect.hpp
//  PoppyCock
//
//  Created by jash on 30.04.17.
//
//

#ifndef Connector_h
#define Connector_h

#include <stdio.h>
#include <vector>
#include "boost/shared_ptr.hpp"

#include "Interactive.h"
#include "ofxImGui.h"

class Entity;

class Connector : public Interactive {

protected:
    // cant use EntityRef since it's defined in Entity.h and including that would
    // create circular include pattern and break compilation
    boost::shared_ptr<Entity> mSource;
    boost::shared_ptr<Entity> mTarget;
    
public:
    
    Connector(boost::shared_ptr<Entity> source,
              boost::shared_ptr<Entity> target ) : Interactive(), mSource(source), mTarget(target) {};
    
    virtual void draw( ImVec2 offset, float scale );
    
    boost::shared_ptr<Entity> getSource();
    boost::shared_ptr<Entity> getTarget();
    
    virtual bool hitTest( float x, float y );
    
};


typedef boost::shared_ptr<Connector> ConnectorRef;
typedef std::vector<ConnectorRef> ConnectorList;
typedef ConnectorList::iterator ConnectorListIterator;

#endif /* Connector_h */
