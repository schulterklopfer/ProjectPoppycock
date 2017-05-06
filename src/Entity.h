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
    
    bool __r_targetEntityInInputs( Entity* const target );
    
protected:
    
    bool connectorIsCircular( Entity* const target );
    bool inCircle( const float x, const float y );
    bool outputExists( Entity* const e );
    bool inputExists( Entity* const e );
    void recalcConnectionBounds();

    ImVec2 mPosition;
    bool mBoundsDirty;
    float mSize;
    
    ConnectorList mInputs;
    ConnectorList mOutputs;
    
    
public:

    Entity( const ImVec2 position );

    static ImVec2 getDrawPosition( const ImVec2 position, const ImVec2 offset, float scale );
    
    virtual void draw( const ImVec2 offset, const float scale );
    void drawBoundingBox( const ImVec2 offset, const float scale );
    virtual bool hitTest( const float x, const float y );
    virtual int getTypeFlags();
    ImVec2 getPosition();
    
    void setPosition( const ImVec2 p);
    void move( const float x, const float y );
    bool getBoundsDirty();
    void clearBoundsDirty();
    ConnectorList* getInputs();
    ConnectorList* getOutputs();
    void addInput( const ConnectorRef input );
    void addOutput( const ConnectorRef output );
    
    virtual bool acceptsInputFrom(const boost::shared_ptr<Entity> &source );
    virtual bool providesOutputTo(const boost::shared_ptr<Entity> &source );
    virtual bool acceptsInput();
    virtual bool providesOutput();
    virtual boost::shared_ptr<Entity> clone();
    virtual void recalcBounds();

};

typedef boost::shared_ptr<Entity> EntityRef;
typedef std::vector<EntityRef> EntityList;
typedef EntityList::iterator EntityListIterator;


#endif /* Entity_h */
