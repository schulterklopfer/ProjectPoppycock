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
    return inCircle(x, y);
}

inline bool Entity::inCircle( float x, float y ) {
    const ofPoint c = mBounds.getCenter();
    const float radius = mSize*0.5;
    float dx = fabs(x-c.x);
    if (    dx >  radius ) return false;
    float dy = fabs(y-c.y);
    if (    dy >  radius ) return false;
    if ( dx+dy <= radius ) return true;
    return ( dx*dx + dy*dy <= radius*radius );
}



int Entity::getTypeFlags() {
    return Entity::Type::ENTITY;
}

ConnectorList* Entity::getInputs() {
    return &mInputs;
}

ConnectorList* Entity::getOutputs() {
    return &mOutputs;
}

void Entity::addInput( ConnectorRef input ) {
    
    // connector doesn't have this object as source
    if( input->getTarget().get() != this ) {
        ofLogVerbose(__FUNCTION__ ) << "connector has wrong target " << input->getTarget()->getId();
        return;
    }
    
    if( inputExists( input->getSource().get() ) ) {
        return;
    }
    
    mInputs.push_back(input);
    
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

bool Entity::inputExists( Entity* e ) {
    // does an input with this object as target and e exist?
    for( ConnectorListIterator iter = mInputs.begin(); iter != mInputs.end(); ++iter ) {
        if( ((*iter)->getTarget()).get() == this && ((*iter)->getSource()).get() == e ) {
            ofLogVerbose(__FUNCTION__ ) << "input from " << e->getId() << " exists.";
            return true;
        }
    }
    return false;
}

bool Entity::outputExists( Entity* e ) {
    // does a output with this object as source and e exist?
    for( ConnectorListIterator iter = mOutputs.begin(); iter != mOutputs.end(); ++iter ) {
        if( ((*iter)->getSource()).get() == this && ((*iter)->getTarget()).get() == e ) {
            ofLogVerbose(__FUNCTION__ ) << "output to " << e->getId() << " exists.";
            return true;
        }
    }
    return false;
}

// look recursevely for entity matching *target going backwards in input sources
bool Entity::__r_targetEntityInInputs( Entity* target ) {
    for( ConnectorListIterator iter = mInputs.begin(); iter != mInputs.end(); ++iter ) {
        if( (*iter)->getSource().get() == target ) {
            return true;
        }
        return (*iter)->getSource()->__r_targetEntityInInputs(target);
    }
    return false;
}


bool Entity::connectorIsCircular( Entity* target ) {
    // if nothing is connected to source, early out
    if( mInputs.size() == 0 ) return false;
    return __r_targetEntityInInputs(target);
}

bool Entity::acceptsInputFrom( EntityRef &source ) {
    return this != source.get() && mInputs.size() == 0 && !source.get()->connectorIsCircular( this );
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

