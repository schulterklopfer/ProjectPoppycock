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
#include "boost/shared_ptr.hpp"
#include "ofxImGui.h"
#include "ofRectangle.h"

class Entity {

    
private:
    
    void recalcBounds();
    
    string mId;
    ImVec2 mPosition;
    ofRectangle mBounds;
    bool mBoundsDirty;
    float mSize;
    
    
public:

    static const int ST_OVER =  1;
    static const int ST_DOWN =  2;
    static const int ST_DRAG =  4;
    static const int ST_SLCT =  8;
    static const int ST_SRC  = 16;
    static const int ST_TRGT = 32;

    
    Entity( string id, ImVec2 position );

    virtual void draw( ImVec2 offset, float scale );
    void drawBoundingBox( ImVec2 offset, float scale );

    virtual bool hitTest( float x, float y );
    
    int stateFlags;
    
    
    ImVec2 getPosition();
    void setPosition( ImVec2 p);
    void move( float x, float y );
    bool getBoundsDirty();
    void clearBoundsDirty();
    
    string getId();
    void setId( string id );
    
    ofRectangle* getBounds();
    
};

typedef boost::shared_ptr<Entity> EntityRef;


#endif /* Entity_h */
