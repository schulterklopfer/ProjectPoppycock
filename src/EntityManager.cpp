//
//  EntityManager.cpp
//  PoppyCock
//
//  Created by jash on 27.04.17.
//
//

#include "EntityManager.h"
#include "Effect.h"

void EntityManager::createEntity(string id, ofVec2f position) {
    mEntities.push_back(EntityRef(new Entity(id, position)));
    recalcBounds();
};

void EntityManager::createEffect(string id, ofVec2f position) {
    mEntities.push_back(EntityRef(new Effect(id, position)));
    recalcBounds();
};

std::vector<EntityRef>* EntityManager::getEntities() {
    return &mEntities;
}

void EntityManager::recalcBounds() {
    for( EntityListIterator iter = mEntities.begin(); iter != mEntities.end(); ++iter ) {
        if( iter == mEntities.begin() ) {
            mBounds.set(*(*iter)->getBounds());
        } else {
            mBounds.growToInclude(*(*iter)->getBounds());
        }
    }
}

void EntityManager::checkDirtyBounds() {
    bool dirty = false;
    for( EntityListIterator iter = mEntities.begin(); iter != mEntities.end(); ++iter ) {
        dirty = dirty || (*iter)->getBoundsDirty();
        (*iter)->clearBoundsDirty();
    }
    if( dirty ) {
        recalcBounds();
    }
    
}

void EntityManager::drawBoundingBox(  ImVec2 offset, float scale  ) {
    ImVec2 upperLeft = ImVec2( mBounds.getMinX()*scale+offset.x, mBounds.getMinY()*scale+offset.y );
    ImVec2 lowerRight = ImVec2( mBounds.getMaxX()*scale+offset.x, mBounds.getMaxY()*scale+offset.y );
    ImGui::GetWindowDrawList()->AddRect(upperLeft, lowerRight, 0xff0000ff );
}

ofRectangle* EntityManager::getBounds() {
    return &mBounds;
}
