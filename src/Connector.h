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

public:
    typedef enum BlendMode {
        ADD = 1,
        AVERAGE = 2,
        COLOR_BURN = 3,
        COLOR_DODGE = 4,
        DARKEN = 5,
        DIFFERENCE = 6,
        EXCLUSION = 7,
        GLOW = 8,
        HARD_LIGHT = 9,
        HARD_MIX = 10,
        LIGHTEN = 11,
        LINEAR_BURN = 12,
        LINEAR_DODGE = 13,
        LINEAR_LIGHT = 14,
        MULTIPLY = 15,
        NEGATION = 16,
        NORMAL = 17,
        OVERLAY = 18,
        PHOENIX = 19,
        PIN_LIGHT = 20,
        REFLECT = 21,
        SCREEN = 22,
        SOFT_LIGHT = 23,
        SUBTRACT = 24,
        VIVID_LIGHT = 25
    } BlendMode;
    
    Connector(const boost::shared_ptr<Entity> source,
              const boost::shared_ptr<Entity> target ) : Interactive(), mSource(source), mTarget(target), mBlendMode(BlendMode::NORMAL) {};
    
    void draw( const ImVec2 offset, const float scale );
    
    static void draw( const ofPoint sp,
                      const ofPoint tp,
                      const float scale = 1.0,
                      const float startOffset = 0.0,
                      const float endOffset = 0.0,
                      const int stateFlags = 0,
                      const bool isFader = false,
                      const bool animate = false );
    
    boost::shared_ptr<Entity> getSource();
    boost::shared_ptr<Entity> getTarget();
    
    virtual bool hitTest( const float x, const float y );
    virtual int getTypeFlags();
    virtual void recalcBounds();

    bool isFader();
    
protected:
    // cant use EntityRef since it's defined in Entity.h and including that would
    // create circular include pattern and break compilation
    boost::shared_ptr<Entity> mSource;
    boost::shared_ptr<Entity> mTarget;
    
    BlendMode mBlendMode;
};


typedef boost::shared_ptr<Connector> ConnectorRef;
typedef std::vector<ConnectorRef> ConnectorList;
typedef ConnectorList::iterator ConnectorListIterator;

#endif /* Connector_h */
