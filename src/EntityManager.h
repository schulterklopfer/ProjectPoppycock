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

#include "Entity.h"
#include "Effect.h"
#include "Connector.h"

class EntityManager {
private:
    
    void recalcBounds();
    
    EntityList mEntities;
    ConnectorList mConnectors;
    ofRectangle mBounds;
    
public:
    
    void createEntity( string id, ofVec2f position );
    void createEffect( string id, ofVec2f position );
    void createConnector( EntityRef source, EntityRef target );
    bool connectorExists( EntityRef &source, EntityRef &target );
    bool connectorIsCircular( EntityRef &source, EntityRef &target );
    EntityList* getEntities();
    ConnectorList* getConnectors();
    void drawBoundingBox( ImVec2 offset, float scale );
    ofRectangle* getBounds();
    void checkDirtyBounds();

    InteractiveRef hotInteractive;
    InteractiveRef activeInteractive;
    InteractiveRef draggingInteractive;
    InteractiveRef selectedInteractive;
        
};


#endif /* EntityManager_h */
