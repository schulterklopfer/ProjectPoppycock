//
//  Entity.cpp
//  PoppyCock
//
//  Created by jash on 27.04.17.
//
//

#include "Entity.h"

Entity::Entity( const ImVec2 position ) :
    Interactive(),
    mPosition(position),
    mSize(40),
    mBoundsDirty(false),
    mMaxEdgeDistanceFromObserver(-1)
{
    recalcBounds();
    stateFlags |= Interactive::State::IDLE;
}

ImVec2 Entity::getPosition() {
    return mPosition;
}

ImVec2 Entity::getDrawPosition( const ImVec2 position, const ImVec2 offset, const float scale ) {
    return  ImVec2( position.x*scale + offset.x,
                    position.y*scale + offset.y );
}


void Entity::setPosition( ImVec2 p) {
    mPosition.x = p.x;
    mPosition.y = p.y;
    recalcBounds();
    recalcConnectionBounds();
}

void Entity::setMaxEdgeDistanceFromObserver( int d, bool force ) {
    if( force || d>mMaxEdgeDistanceFromObserver ) {
        mMaxEdgeDistanceFromObserver = d;
    }
}

int Entity::getMaxEdgeDistanceFromObserver() {
    return mMaxEdgeDistanceFromObserver;
}

void Entity::move( const float x, const float y ) {
    mPosition.x += x;
    mPosition.y += y;
    recalcBounds();
    recalcConnectionBounds();
}

bool Entity::getBoundsDirty() {
    return mBoundsDirty;
}

void Entity::clearBoundsDirty() {
    mBoundsDirty = false;
}

void Entity::update() {

}

void Entity::draw( const ImVec2 offset, const float scale ) {
    int color = ((stateFlags&State::IDLE) == State::IDLE)?0x33ffffff:0xffffffff;
    
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
    
    ImGui::GetWindowDrawList()->AddCircle(getDrawPosition(mPosition, offset, scale), mSize*0.5*scale, color, 64, 3*scale );

}

void Entity::drawBoundingBox( const ImVec2 offset, const float scale  ) {
    ImVec2 upperLeft = ImVec2( mBounds.getMinX()*scale+offset.x, mBounds.getMinY()*scale+offset.y );
    ImVec2 lowerRight = ImVec2( mBounds.getMaxX()*scale+offset.x, mBounds.getMaxY()*scale+offset.y );
    ImGui::GetWindowDrawList()->AddRect(upperLeft, lowerRight, 0xff0000ff );
}

void Entity::recalcBounds() {
    mBounds.setFromCenter(mPosition.x, mPosition.y, mSize, mSize);
    mBoundsDirty = true;
}

void Entity::recalcConnectionBounds() {
    for( ConnectorListIterator iter = mInputs.begin(); iter != mInputs.end(); ++iter ) {
        (*iter)->recalcBounds();
    }
    for( ConnectorListIterator iter = mOutputs.begin(); iter != mOutputs.end(); ++iter ) {
        (*iter)->recalcBounds();
    }
}

bool Entity::hitTest( const float x, const float y ) {
    return inCircle(mBounds.getCenter(), mSize*0.5, x, y);
}

bool Entity::inCircle( const ofPoint center, const float radius, const float x, const float y ) {
    float dx = fabs(x-center.x);
    if (    dx >  radius ) return false;
    float dy = fabs(y-center.y);
    if (    dy >  radius ) return false;
    if ( dx+dy <= radius ) return true;
    return ( dx*dx + dy*dy <= radius*radius );
}



int Entity::getTypeFlags() {
    return Interactive::Type::ENTITY;
}

ConnectorList* Entity::getInputs() {
    return &mInputs;
}

ConnectorList* Entity::getOutputs() {
    return &mOutputs;
}

void Entity::addInput( const ConnectorRef input ) {
    
    // connector doesn't have this object as source
    if( input->getTarget().get() != this ) {
        return;
    }
    
    if( inputExists( input->getSource().get() ) ) {
        return;
    }
    
    mInputs.push_back(input);
    recalcConnectionBounds();
}

void Entity::addOutput( const ConnectorRef output ) {
    
    // connector doesn't have this object as source
    if( output->getSource().get() != this ) {
        return;
    }
    
    if( outputExists( output->getTarget().get() ) ) {
        return;
    }
    
    mOutputs.push_back(output);
    recalcConnectionBounds();
}

bool Entity::inputExists( Entity* const e ) {
    // does an input with this object as target and e exist?
    for( ConnectorListIterator iter = mInputs.begin(); iter != mInputs.end(); ++iter ) {
        if( ((*iter)->getTarget()).get() == this && ((*iter)->getSource()).get() == e ) {
            ofLogVerbose(__FUNCTION__ ) << "input exists.";
            return true;
        }
    }
    return false;
}

bool Entity::outputExists( Entity* const e ) {
    // does a output with this object as source and e exist?
    for( ConnectorListIterator iter = mOutputs.begin(); iter != mOutputs.end(); ++iter ) {
        if( ((*iter)->getSource()).get() == this && ((*iter)->getTarget()).get() == e ) {
            ofLogVerbose(__FUNCTION__ ) << "output exists.";
            return true;
        }
    }
    return false;
}

// look recursevely for entity matching *target going backwards in input sources
bool Entity::__r_targetEntityInInputs( Entity* const target ) {
    for( ConnectorListIterator iter = mInputs.begin(); iter != mInputs.end(); ++iter ) {
        if( (*iter)->getSource().get() == target ) {
            return true;
        }
        return (*iter)->getSource()->__r_targetEntityInInputs(target);
    }
    return false;
}


bool Entity::connectorIsCircular( Entity* const target ) {
    // if nothing is connected to source, early out
    if( mInputs.size() == 0 ) return false;
    return __r_targetEntityInInputs(target);
}

bool Entity::acceptsInputFrom( const EntityRef &source ) {
    return this != source.get() && mInputs.size() == 0 && !source.get()->connectorIsCircular( this );
}

bool Entity::providesOutputTo( const EntityRef &target ) {
    return this != target.get() && !connectorIsCircular( target.get() );
}

bool Entity::acceptsInput() {
    return true;
}

bool Entity::providesOutput() {
    return true;
}


EntityRef Entity::clone() {
    EntityRef clone = EntityRef( new Entity(mPosition) );
    clone->mSize = mSize;
    return clone;
}

// inverse sqrt approximation
float Entity::Q_rsqrt( float number )
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;
    
    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
    //	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
    
    return y;
}

