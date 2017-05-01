//
//  EntityManager.cpp
//  PoppyCock
//
//  Created by jash on 27.04.17.
//
//

#include "EntityManager.h"

void EntityManager::createEntity( Entity::Type type, string id, ofVec2f position) {
    
    switch( type ) {
        case Entity::Type::EFFECT:
            mEntities.push_back(EntityRef(new Effect(id, position)));
            break;
        case Entity::Type::OBSERVER:
            mEntities.push_back(EntityRef(new Observer(id, position)));
            break;
        default:
            mEntities.push_back(EntityRef(new Entity(id, position)));
    }
    
    
    recalcBounds();
};

void EntityManager::createConnector( EntityRef source, EntityRef target ) {
    // check if there is already a connector between source and target
    if( connectorExists( source, target ) ) return;
    if( !target->acceptsInputFrom(source) || !source->providesOutputTo(target) ) return;
    ConnectorRef cRef = ConnectorRef( new Connector( source, target ) );
    source->addOutput(cRef);
    target->addInput(cRef);
    mConnectors.push_back(cRef);
    
}

bool EntityManager::connectorExists( EntityRef &source, EntityRef &target ) {
    for( ConnectorListIterator iter = mConnectors.begin(); iter != mConnectors.end(); ++iter ) {
        if(((*iter)->getSource() == source && (*iter)->getTarget() == target) ||
           ((*iter)->getSource() == target && (*iter)->getTarget() == source)) {
            return true;
        }
    }
    return false;
}

EntityList* EntityManager::getEntities() {
    return &mEntities;
}

ConnectorList* EntityManager::getConnectors() {
    return &mConnectors;
}

InteractiveList EntityManager::getInteractives() {
    InteractiveList list;
    for( ConnectorListIterator iter = mConnectors.begin(); iter != mConnectors.end(); ++iter ) {
        list.push_back((*iter));
    }

    for( EntityListIterator iter = mEntities.begin(); iter != mEntities.end(); ++iter ) {
        list.push_back((*iter));
    }
    
    return list;
    
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
