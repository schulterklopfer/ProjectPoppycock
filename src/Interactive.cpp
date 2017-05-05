//
//  Interactive.cpp
//  PoppyCock
//
//  Created by jash on 01.05.17.
//
//

#include "Interactive.h"

bool Interactive::hitTest( const float x, const float y ) {
    return false;
}

int Interactive::getTypeFlags() {
    return Interactive::Type::NONE;
}

ofRectangle* Interactive::getBounds() {
    return &mBounds;
}

void Interactive::recalcBounds() {}
