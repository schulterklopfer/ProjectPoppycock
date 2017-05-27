//
//  Effect.hpp
//  PoppyCock
//
//  Created by jash on 30.04.17.
//
//

#ifndef Effect_h
#define Effect_h

#include <stdio.h>
#include "Entity.h"
#include "boost/shared_ptr.hpp"

class Effect: public Entity {

public:

    class AOE: public Interactive {
        
    private:
        Effect* mEffect;
        ofPoint mHandlePosition;
        float mRadius;
        
    public:
        
        AOE( Effect* effect ) : Interactive(), mEffect(effect), mHandlePosition(ofPoint(75,75)), mRadius( mHandlePosition.length() ) {};
        void draw( const ImVec2 offset, const float scale );
        int getTypeFlags();
        bool hitTest( const float x, const float y );
        void move( const float x, const float y );
        float getRadius();
    
    };
    
    Effect( const ImVec2 position ) : Entity(position), mAOE(boost::shared_ptr<Effect::AOE>( new AOE(this) )) {};

    virtual void draw( const ImVec2 offset, const float scale );
    virtual int getTypeFlags();

    virtual bool isFinal();
    virtual void recalcBlendValues();
    
    virtual void setPosition( const ImVec2 pos );
    virtual void move( const float x, const float y );
    
    boost::shared_ptr<Effect::AOE> getAOE();

    
private:
    boost::shared_ptr<Effect::AOE> mAOE;
    
};

typedef boost::shared_ptr<Effect> EffectRef;
typedef boost::shared_ptr<Effect::AOE> AOERef;

#endif /* Effect_h */
