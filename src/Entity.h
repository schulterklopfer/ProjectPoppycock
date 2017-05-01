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

private:
    
    bool __r_targetEntityInInputs( Entity* target );
    
protected:
    
    virtual void recalcBounds();
    bool connectorIsCircular( Entity* target );
    bool inCircle( float x, float y );
    bool outputExists( Entity* e );
    bool inputExists( Entity* e );

    
    string mId;
    ImVec2 mPosition;
    ofRectangle mBounds;
    bool mBoundsDirty;
    float mSize;
    
    ConnectorList mInputs;
    ConnectorList mOutputs;
    
    
public:

    Entity( string id, ImVec2 position );

    virtual void draw( ImVec2 offset, float scale );
    void drawBoundingBox( ImVec2 offset, float scale );
    virtual bool hitTest( float x, float y );
    virtual int getTypeFlags();
    ImVec2 getPosition();
    ImVec2 getDrawPosition( ImVec2 offset, float scale );
    void setPosition( ImVec2 p);
    void move( float x, float y );
    bool getBoundsDirty();
    void clearBoundsDirty();
    string getId();
    void setId( string id );
    ofRectangle* getBounds();
    ConnectorList* getInputs();
    ConnectorList* getOutputs();
    void addInput( ConnectorRef input );
    void addOutput( ConnectorRef output );
    
    virtual bool acceptsInputFrom(boost::shared_ptr<Entity> &source );
    virtual bool providesOutputTo(boost::shared_ptr<Entity> &source );
    virtual bool acceptsInput();
    virtual bool providesOutput();

};

typedef boost::shared_ptr<Entity> EntityRef;
typedef std::vector<EntityRef> EntityList;
typedef EntityList::iterator EntityListIterator;


#endif /* Entity_h */
