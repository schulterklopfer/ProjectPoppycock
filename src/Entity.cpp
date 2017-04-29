//
//  Entity.cpp
//  PoppyCock
//
//  Created by jash on 27.04.17.
//
//

#include "Entity.h"

Entity::Entity( string id, ImVec2 position ) :
    mId(id),
    mPosition(position),
    mSize(40),
    mBoundsDirty(false),
    stateFlags(0)
{
    recalcBounds();
}

ImVec2 Entity::getPosition() {
    return mPosition;
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
    ImVec2 position = ImVec2( mPosition.x*scale + offset.x,
                              mPosition.y*scale + offset.y );
    
    int color = 0xffffffff;
    
    if( (stateFlags&ST_OVER) == ST_OVER ) {
        color = 0xff0000ff;
    }
    if( (stateFlags&ST_DOWN) == ST_DOWN ) {
        color = 0xffff00ff;
    }
    if( (stateFlags&ST_SLCT) == ST_SLCT ) {
        color = 0xff00ff00;
    }
    if( (stateFlags&ST_DRAG) == ST_DRAG ) {
        color = 0xaa00ff00;
    }
    
    ImGui::GetWindowDrawList()->AddCircleFilled(position, mSize*0.5*scale, color, 64 );

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
