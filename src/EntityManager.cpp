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

void EntityManager::deleteInteractive( InteractiveRef& interactive ) {
    if( (interactive->getTypeFlags()&Interactive::Type::ENTITY) == Interactive::Type::ENTITY ) {
        const EntityRef eRef = boost::static_pointer_cast<Entity>(interactive);
        ConnectorList* const inputs = eRef->getInputs();
        ConnectorList* const outputs = eRef->getOutputs();
        
        // for each input connection, go to source entitiy of this connection and delete it from outputs
        for( ConnectorListIterator iter = inputs->begin(); iter != inputs->end(); ++iter ) {
            ConnectorList* const inputSourceOutputs = (*iter)->getSource()->getOutputs();
            inputSourceOutputs->erase(std::remove(inputSourceOutputs->begin(), inputSourceOutputs->end(), (*iter)), inputSourceOutputs->end());
            mConnectors.erase(std::remove(mConnectors.begin(), mConnectors.end(), (*iter)), mConnectors.end());
        }
        
        // for each output connection, go to target entitiy of this connection and delete it from inputs
        for( ConnectorListIterator iter = outputs->begin(); iter != outputs->end(); ++iter ) {
            ConnectorList* const outputTargetInputs = (*iter)->getTarget()->getInputs();
            outputTargetInputs->erase(std::remove(outputTargetInputs->begin(), outputTargetInputs->end(), (*iter)), outputTargetInputs->end());
            mConnectors.erase(std::remove(mConnectors.begin(), mConnectors.end(), (*iter)), mConnectors.end());
        }

        // just to be sure, cleanup inputs and outpus
        inputs->clear();
        outputs->clear();
        
        // finally remove the entity from the list of entities
        mEntities.erase(std::remove(mEntities.begin(), mEntities.end(), eRef), mEntities.end());
        
    }
    else if( (interactive->getTypeFlags()&Interactive::Type::CONNECTOR) == Interactive::Type::CONNECTOR ) {
        const ConnectorRef cRef = boost::static_pointer_cast<Connector>(interactive);
        ConnectorList* const sourceOutputs = cRef->getSource()->getOutputs();
        ConnectorList* const targetInputs = cRef->getTarget()->getInputs();
        
        // remove all refs to connector
        sourceOutputs->erase(std::remove(sourceOutputs->begin(), sourceOutputs->end(), cRef), sourceOutputs->end());
        targetInputs->erase(std::remove(targetInputs->begin(), targetInputs->end(), cRef), targetInputs->end());
        mConnectors.erase(std::remove(mConnectors.begin(), mConnectors.end(), cRef), mConnectors.end());
        
    }
}

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
