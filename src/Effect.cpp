//
//  Effect.cpp
//  PoppyCock
//
//  Created by jash on 30.04.17.
//
//

#include "Effect.h"
#include "Observer.h"

int Effect::getTypeFlags() {
    return Interactive::Type::EFFECT|Interactive::Type::ENTITY;
}

void Effect::draw( const ImVec2 offset, const float scale ) {
    
    Entity::draw(offset, scale);
    // draw area of effect
    if( isFinal() ) {
        mAOE->draw(offset, scale);
    }
}

void Effect::move(const float x, const float y) {
    Entity::move(x,y);
    recalcBlendValues();
}

void Effect::setPosition(const ImVec2 pos) {
    Entity::setPosition(pos);
    recalcBlendValues();
}

void Effect::recalcBlendValues() {
    
    if( mMaxEdgeDistanceFromObserver == -1 ) return;
    
    for( ConnectorListIterator iter = mOutputs.begin(); iter != mOutputs.end(); ++iter ) {
        boost::shared_ptr<Observer> oRef = TO_OBSERVER((*iter)->getTarget());
        if( oRef != NULL ) {
            oRef->recalcBlendValues();
        }
    }
}

// is Effect connected to an observer?
bool Effect::isFinal() {
    for( ConnectorListIterator iter = mOutputs.begin(); iter != mOutputs.end(); ++iter ) {
        if( (*iter)->isFader() )
            return true;
    }
    return false;
}

AOERef Effect::getAOE() {
    return mAOE;
}

// Area of Effect

void Effect::AOE::draw( const ImVec2 offset, const float scale ) {
    int color = 0xffffffff;
    
    if( (stateFlags&State::OVER) == State::OVER ) {
        color = 0xff0000ff;
    }
    if( (stateFlags&State::DOWN) == State::DOWN ) {
        color = 0xffff00ff;
    }
    if( (stateFlags&State::SELECT) == State::SELECT ) {
        color = 0xff00ff00;
    }
    if( (stateFlags&State::DRAG) == State::DRAG ) {
        color = 0xffffffff;
    }
    if( (stateFlags&State::SOURCE) == State::SOURCE ) {
        color = 0xffc3c3c3;
    }
    if( (stateFlags&State::TARGET) == State::TARGET ) {
        color = 0xffff0000;
    }
    
    ImGui::GetWindowDrawList()->AddCircle(Entity::getDrawPosition(mEffect->getPosition(), offset, scale), mRadius*scale, color, 64 );
    ImGui::GetWindowDrawList()->AddCircleFilled(Entity::getDrawPosition((ImVec2)((ofPoint)mEffect->getPosition() + mHandlePosition), offset, scale), 5*scale, color, 16 );
}

int Effect::AOE::getTypeFlags() {
    return Interactive::Type::AOE;
}

bool Effect::AOE::hitTest(const float x, const float y) {
    return Entity::inCircle((ofPoint)mEffect->getPosition() + mHandlePosition, 5, x, y);
}

void Effect::AOE::move( const float x, const float y ) {
    
    // look ahead and check if handle will be inside effect
    const ofPoint toCheck = (ofPoint)mEffect->getPosition() + mHandlePosition + ofPoint(x,y);
    
    if( !mEffect->hitTest(toCheck.x, toCheck.y) ) {
        mHandlePosition.x += x; mHandlePosition.y += y;
        mRadius = mHandlePosition.length();
        mEffect->recalcBlendValues();
    }
    
}

float Effect::AOE::getRadius() {
    return mRadius;
}

