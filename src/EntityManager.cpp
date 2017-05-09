//
//  EntityManager.cpp
//  PoppyCock
//
//  Created by jash on 27.04.17.
//
//

#include "EntityManager.h"

void EntityManager::createEntity( Entity::Type type, ofVec2f position) {
    
    switch( type ) {
        case Entity::Type::EFFECT:
            mEntities.push_back(EntityRef(new Effect(position)));
            break;
        case Entity::Type::GPU_EFFECT:
            mEntities.push_back(EntityRef(new GPUEffect(position)));
            break;
        case Entity::Type::OBSERVER:
            mEntities.push_back(EntityRef(new Observer(position)));
            break;
        default:
            mEntities.push_back(EntityRef(new Entity(position)));
    }
    regenerateInteractivesList();
    recalcBounds();
};

void EntityManager::deleteInteractive( const InteractiveRef& interactive ) {
    if( interactive->isOfType(Interactive::Type::ENTITY) ) {
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
    else if( interactive->isOfType(Interactive::Type::CONNECTOR) ) {
        const ConnectorRef cRef = boost::static_pointer_cast<Connector>(interactive);
        ConnectorList* const sourceOutputs = cRef->getSource()->getOutputs();
        ConnectorList* const targetInputs = cRef->getTarget()->getInputs();
        
        // remove all refs to connector
        sourceOutputs->erase(std::remove(sourceOutputs->begin(), sourceOutputs->end(), cRef), sourceOutputs->end());
        targetInputs->erase(std::remove(targetInputs->begin(), targetInputs->end(), cRef), targetInputs->end());
        mConnectors.erase(std::remove(mConnectors.begin(), mConnectors.end(), cRef), mConnectors.end());
        
        
        if( cRef->getTarget()->isOfType(Interactive::Type::OBSERVER) ||
            cRef->getTarget()->getMaxEdgeDistanceFromObserver() > -1 ) {
            recalcMaxEdgeDistances();
        }
        
    }
    regenerateInteractivesList();
}

bool EntityManager::isInSelection(const InteractiveRef &interactive) {
    return std::find( selectedInteractives.begin(), selectedInteractives.end(), interactive ) != selectedInteractives.end();
}

void EntityManager::deselectInteractive(const InteractiveRef& interactive) {
    interactive->stateFlags &= ~Interactive::State::SELECT;
    for ( SelectionIterator iter = selectedInteractives.begin(); iter != selectedInteractives.end(); ) {
        if ( (*iter) == interactive ) {
            iter = selectedInteractives.erase( iter );
        } else {
            ++iter;
        }
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
    regenerateInteractivesList();
    if( target->isOfType(Interactive::Type::OBSERVER) || target->getMaxEdgeDistanceFromObserver() > -1 ) {
        recalcMaxEdgeDistances();
    }
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

EntityList* const EntityManager::getEntities() {
    return &mEntities;
}

EntityList* const EntityManager::getEntitiesToUpdate() {
    return &mEntitiesToUpdate;
}

ConnectorList* const EntityManager::getConnectors() {
    return &mConnectors;
}

InteractiveList* const EntityManager::getInteractives() {
    return &mInteractives;
}

void EntityManager::regenerateInteractivesList() {
    mInteractives.clear();
    for( ConnectorListIterator iter = mConnectors.begin(); iter != mConnectors.end(); ++iter ) {
        mInteractives.push_back((*iter));
    }
    
    for( EntityListIterator iter = mEntities.begin(); iter != mEntities.end(); ++iter ) {
        mInteractives.push_back((*iter));
        if( (*iter)->isOfType(Interactive::Type::EFFECT) ) {
            const EffectRef eRef = boost::static_pointer_cast<Effect>(*iter);
            if( eRef->isFinal() ) {
                mInteractives.push_back(eRef->getAOE());
            }
        }
    }
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

void EntityManager::recalcMaxEdgeDistances() {
    // 1) find observers
    mEntitiesToUpdate.clear();
    
    EntityList observers;
    
    for( EntityListIterator iter = mEntities.begin(); iter != mEntities.end(); ++iter ) {
        if( (*iter)->isOfType(Interactive::Type::OBSERVER) ) {
            observers.push_back(*iter);
        } else {
            // reset max edge distances;
            (*iter)->setMaxEdgeDistanceFromObserver(-1,true);
            (*iter)->stateFlags |= Interactive::State::IDLE;
        }
    }
    // 2) walk back inputs of observers and set maxEdgeDistances
    for( EntityListIterator iter = observers.begin(); iter != observers.end(); ++iter ) {
        __updateMaxEdgeDistance__( (*iter), (*iter), 0 );
    }
    
    // push entities with mMaxEdgeDistanceFromObserver>0 and observers with inputs onto
    // mEntitiesToUpdate.
    for( EntityListIterator iter = mEntities.begin(); iter != mEntities.end(); ++iter ) {
        if( (*iter)->isOfType(Interactive::Type::OBSERVER) ) {
            if( (*iter)->getInputs()->size()>0 )
                mEntitiesToUpdate.push_back(*iter);
        } else {
            if( (*iter)->getMaxEdgeDistanceFromObserver()>-1 ) {
                mEntitiesToUpdate.push_back(*iter);
            }
        }
    }
    
    // sort by inverse mMaxEdgeDistanceFromObserver to generate update order
    std::sort(mEntitiesToUpdate.begin(), mEntitiesToUpdate.end(),
              [] (EntityRef const& a, EntityRef const& b) { return a->getMaxEdgeDistanceFromObserver() > b->getMaxEdgeDistanceFromObserver(); });
    
    
}

void EntityManager::__updateMaxEdgeDistance__( EntityRef &start, EntityRef &current, int distance) {
    ConnectorList* inputs = current->getInputs();
    for( ConnectorListIterator iter = inputs->begin(); iter != inputs->end(); ++iter ) {
        EntityRef source = (*iter)->getSource();
        // not there or circular?
        if( source == NULL || source == start ) continue;
        source->setMaxEdgeDistanceFromObserver(distance);
        source->stateFlags &= ~Interactive::State::IDLE;
        __updateMaxEdgeDistance__( start, source, distance+1 );
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
