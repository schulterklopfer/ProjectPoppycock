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
#include "Entity.h"
#include <vector>
#include "ofRectangle.h"

typedef std::vector<EntityRef> EntityList;
typedef std::vector<EntityRef>::iterator EntityListIterator;
typedef std::vector<EntityRef>::reverse_iterator EntityListReverseIterator;

class EntityManager {
private:
    
    void recalcBounds();
    
    EntityList mEntities;
    ofRectangle mBounds;
    
public:
    
    void createEntity( string id, ofVec2f position );
    EntityList* getEntities();
    void drawBoundingBox( ImVec2 offset, float scale );
    ofRectangle* getBounds();
    void checkDirtyBounds();

    EntityRef hotEntity;
    EntityRef activeEntity;
    EntityRef draggingEntity;
    EntityRef selectedEntity;
    
};


#endif /* EntityManager_h */
