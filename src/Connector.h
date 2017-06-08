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
        ADD = 0,
        AVERAGE = 1,
        COLOR_BURN = 2,
        COLOR_DODGE = 3,
        DARKEN = 4,
        DIFFERENCE = 5,
        EXCLUSION = 6,
        GLOW = 7,
        HARD_LIGHT = 8,
        HARD_MIX = 9,
        LIGHTEN = 10,
        LINEAR_BURN = 11,
        LINEAR_DODGE = 12,
        LINEAR_LIGHT = 13,
        MULTIPLY = 14,
        NEGATION = 15,
        NORMAL = 16,
        OVERLAY = 17,
        PHOENIX = 18,
        PIN_LIGHT = 19,
        REFLECT = 20,
        SCREEN = 21,
        SOFT_LIGHT = 22,
        SUBTRACT = 23,
        VIVID_LIGHT = 24,
        MASK_R = 25,
        MASK_G = 26,
        MASK_B = 27
    } BlendMode;
    
    static const int sBlendModeCount = 28;
    static const char* sBlendModeLabels[sBlendModeCount];
    
    Connector(const boost::shared_ptr<Entity> source,
              const boost::shared_ptr<Entity> target ) : Interactive(), mSource(source), mTarget(target), mBlendMode(BlendMode::ADD), mBlendOpacity(1.0f) {};
    
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
    
    int getBlendMode();
    float getBlendOpacity();
    
    virtual bool hitTest( const float x, const float y );
    virtual int getTypeFlags();
    virtual void recalcBounds();
    virtual void inspectorContent();
    virtual void serialize( Json::Value* outJSON);
    virtual void deserialize( Json::Value* inJSON );

    bool isFader();
    
protected:
    // cant use EntityRef since it's defined in Entity.h and including that would
    // create circular include pattern and break compilation
    boost::shared_ptr<Entity> mSource;
    boost::shared_ptr<Entity> mTarget;
    
    int mBlendMode;
    float mBlendOpacity;
};


typedef boost::shared_ptr<Connector> ConnectorRef;
typedef std::vector<ConnectorRef> ConnectorList;
typedef ConnectorList::iterator ConnectorListIterator;

#endif /* Connector_h */
