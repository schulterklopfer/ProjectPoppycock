//
//  Interactive.cpp
//  PoppyCock
//
//  Created by jash on 01.05.17.
//
//

#include "Interactive.h"
#include "ofxImGui.h"

Interactive::Interactive() : stateFlags(0) {
    Poco::UUIDGenerator gen;
    Poco::UUID uuid = gen.createRandom();
    mId = uuid.toString();
};

char* Interactive::getId() {
    return &mId[0];
}

bool Interactive::hitTest( const float x, const float y ) {
    return false;
}

int Interactive::getTypeFlags() {
    return Interactive::Type::NONE;
}

ofRectangle* Interactive::getBounds() {
    return &mBounds;
}

bool Interactive::isOfType( Interactive::Type t ) {
    return (getTypeFlags()&t)==t;
}

void Interactive::recalcBounds() {}

void Interactive::inspectorContent() {
    ImGui::Text("Inspector content");
}
