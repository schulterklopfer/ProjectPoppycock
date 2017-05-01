//
//  Entity.cpp
//  PoppyCock
//
//  Created by jash on 27.04.17.
//
//

#include "Entity.h"

Entity::Entity( string id, ImVec2 position ) :
    Interactive(),
    mId(id),
    mPosition(position),
    mSize(40),
    mBoundsDirty(false)
{
    recalcBounds();
}

ImVec2 Entity::getPosition() {
    return mPosition;
}

ImVec2 Entity::getDrawPosition( ImVec2 offset, float scale ) {
    return  ImVec2( mPosition.x*scale + offset.x,
                    mPosition.y*scale + offset.y );
}


void Entity::setPosition( ImVec2 p) {
    mPosition.x = p.x;
    mPosition.y = p.y;
    recalcBounds();
}

void Entity::move( float x, float y ) {
    mPosition.x += x;
    mPosition.y += y;
    recalcBounds();
}

bool Entity::getBoundsDirty() {
    return mBoundsDirty;
}

void Entity::clearBoundsDirty() {
    mBoundsDirty = false;
}

string Entity::getId() {
    return mId;
}

void Entity::setId( string id ) {
    mId = id;
}

void Entity::draw( ImVec2 offset, float scale ) {
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
        color = 0xaa00ff00;
    }
    if( (stateFlags&State::SOURCE) == State::SOURCE ) {
        color = 0xffc3c3c3;
    }
    if( (stateFlags&State::TARGET) == State::TARGET ) {
        color = 0xffff0000;
    }
    
    ImGui::GetWindowDrawList()->AddCircleFilled(getDrawPosition(offset, scale), mSize*0.5*scale, color, 64 );

}

void Entity::drawBoundingBox(  ImVec2 offset, float scale  ) {
    ImVec2 upperLeft = ImVec2( mBounds.getMinX()*scale+offset.x, mBounds.getMinY()*scale+offset.y );
    ImVec2 lowerRight = ImVec2( mBounds.getMaxX()*scale+offset.x, mBounds.getMaxY()*scale+offset.y );
    ImGui::GetWindowDrawList()->AddRect(upperLeft, lowerRight, 0xff0000ff );
}

ofRectangle* Entity::getBounds() {
    return &mBounds;
}

void Entity::recalcBounds() {
    mBounds.setFromCenter(mPosition.x, mPosition.y, mSize, mSize);
    mBoundsDirty = true;
}

bool Entity::hitTest( float x, float y ) {
    return mBounds.inside(x, y);
}

int Entity::getTypeFlags() {
    return Entity::Type::ENTITY;
}

ConnectorRef Entity::getInput() {
    return mInput;
}

ConnectorList* Entity::getOutputs() {
    return &mOutputs;
}

void Entity::setInput( ConnectorRef input ) {
    if( input == mInput ) return;
    mInput = input;
}

void Entity::addOutput( ConnectorRef output ) {
    
    // connector doesn't have this object as source
    if( output->getSource().get() != this ) {
        ofLogVerbose(__FUNCTION__ ) << "connector has wrong source " << output->getSource()->getId();
        return;
    }
    
    if( outputExists( output->getTarget().get() ) ) {
        return;
    }
    
    mOutputs.push_back(output);
    
}

bool Entity::outputExists( Entity* e ) {
    // does a output with this object as source and e exist?
    for( ConnectorListIterator iter = mOutputs.begin(); iter != mOutputs.end(); ++iter ) {
        if( ((*iter)->getSource()).get() == this && ((*iter)->getTarget()).get() == e ) {
            ofLogVerbose(__FUNCTION__ ) << "out out to " << e->getId() << " exists.";
            return true;
        }
    }
    return false;
}

bool Entity::connectorIsCircular( Entity* target ) {
    // if nothing is connected to source, early out
    if( mInput == NULL ) return false;
    
    Entity* prevSourceEntity = mInput->getSource().get();
    
    while( prevSourceEntity != NULL ) {
        if( prevSourceEntity == target ) {
            return true;
        }
        if( prevSourceEntity->getInput() != NULL ) {
            prevSourceEntity = prevSourceEntity->getInput()->getSource().get();
        } else {
            prevSourceEntity = NULL;
        }
    }
    return false;
}

bool Entity::acceptsInputFrom( EntityRef &source ) {
    return this != source.get() && mInput == NULL && !source.get()->connectorIsCircular( this );
}

bool Entity::providesOutputTo( EntityRef &target ) {
    return this != target.get() && !connectorIsCircular( target.get() );
}

bool Entity::acceptsInput() {
    return true;
}

bool Entity::providesOutput() {
    return true;
}

