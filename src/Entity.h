//
//  Entity.hpp
//  PoppyCock
//
//  Created by jash on 27.04.17.
//
//

#ifndef Entity_h
#define Entity_h

#include <stdio.h>
#include <vector>
#include "boost/shared_ptr.hpp"

#include "ofxImGui.h"
#include "ofRectangle.h"
#include "Interactive.h"
#include "Connector.h"

class Entity : public Interactive {

    
protected:
    
    virtual void recalcBounds();
    
    string mId;
    ImVec2 mPosition;
    ofRectangle mBounds;
    bool mBoundsDirty;
    float mSize;
    
    ConnectorRef mInput;
    ConnectorList mOutputs;
    
    
public:

    Entity( string id, ImVec2 position );

    virtual void draw( ImVec2 offset, float scale );
    void drawBoundingBox( ImVec2 offset, float scale );
    virtual bool hitTest( float x, float y );
    virtual Interactive::Type getType();
    ImVec2 getPosition();
    ImVec2 getDrawPosition( ImVec2 offset, float scale );
    void setPosition( ImVec2 p);
    void move( float x, float y );
    bool getBoundsDirty();
    void clearBoundsDirty();
    string getId();
    void setId( string id );
    ofRectangle* getBounds();
    ConnectorRef getInput();
    ConnectorList* getOutputs();
    void setInput( ConnectorRef input );
    void addOutput( ConnectorRef output );
    bool outputExists( Entity* e );
    
    int stateFlags;
    
};

typedef boost::shared_ptr<Entity> EntityRef;
typedef std::vector<EntityRef> EntityList;
typedef EntityList::iterator EntityListIterator;


#endif /* Entity_h */
