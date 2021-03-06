//
//  EntityManager.hpp
//  PoppyCock
//
//  Created by jash on 27.04.17.
//
//

#ifndef EntityManager_h
#define EntityManager_h

#include <stdio.h>
#include "ofRectangle.h"
#include "ofxImGui.h"
#include "ofxSingleton.h"

#include "Entity.h"
#include "Effect.h"
#include "GPUEffect.h"
#include "Observer.h"
#include "Connector.h"
#include "Serializable.h"

typedef std::vector<InteractiveRef> Selection;
typedef Selection::iterator SelectionIterator;


class EntityManager: public ofxSingleton::UnmanagedSingleton<EntityManager>, public Serializable {
private:
    
    void recalcBounds();
    void regenerateInteractivesList();
    void recalcMaxEdgeDistances();
    void __updateMaxEdgeDistance__( EntityRef &start, EntityRef &current, int distance );
    EntityList mEntities;
    EntityList mEntitiesToUpdate;
    ConnectorList mConnectors;
    InteractiveList mInteractives;
    ofRectangle mBounds;
    
public:
    
    void createEntity( Entity::Type type, ofVec2f position );
    void createConnector( EntityRef source, EntityRef target );
    void selectInteractive( const InteractiveRef& interactive );
    void deleteInteractive( const InteractiveRef& interactive );
    bool connectorExists( EntityRef &source, EntityRef &target );
    bool isInSelection( const InteractiveRef& interactive );
    void deselectInteractive( const InteractiveRef& interactive );
    EntityList* const getEntities();
    EntityList* const getEntitiesToUpdate();
    ConnectorList* const getConnectors();
    InteractiveList* const getInteractives();
    const InteractiveRef getInteractive( const string id );
    void drawBoundingBox( ImVec2 offset, float scale );
    ofRectangle* getBounds();
    void checkDirtyBounds();

    InteractiveRef hotInteractive;
    InteractiveRef activeInteractive;
    InteractiveRef draggingInteractive;
    Selection selectedInteractives;
    
    // serializable
    
    void serialize( Json::Value* outJSON);
    void deserialize( Json::Value* inJSON );
        
};

#define EntityManagerInstance (EntityManager::getInstance())


#endif /* EntityManager_h */
