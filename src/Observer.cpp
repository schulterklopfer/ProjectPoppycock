//
//  Observer.cpp
//  PoppyCock
//
//  Created by jash on 01.05.17.
//
//

#include "Observer.h"

int Observer::getTypeFlags() {
    return Observer::Type::OBSERVER|Observer::Type::ENTITY;
}

void Observer::draw( const ImVec2 offset, const float scale ) {
    
    Entity::draw(offset, scale);
    ImGui::GetWindowDrawList()->AddCircleFilled(getDrawPosition(mPosition, offset, scale), 18*scale, 0xffaaffaa, 64 );
    
}

// Observer has no outputs.
bool Observer::providesOutputTo( const EntityRef &target ) {
    return false;
}

bool Observer::providesOutput() {
    return false;
}

// accepts more than one input
bool Observer::acceptsInputFrom( const EntityRef &source ) {
    return this != source.get();
}
