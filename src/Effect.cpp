//
//  Effect.cpp
//  PoppyCock
//
//  Created by jash on 30.04.17.
//
//

#include "Effect.h"

int Effect::getTypeFlags() {
    return Effect::Type::EFFECT|Effect::Type::ENTITY;
}

void Effect::draw( ImVec2 offset, float scale ) {
    
    Entity::draw(offset, scale);
    ImGui::GetWindowDrawList()->AddCircleFilled(getDrawPosition(offset, scale), 10*scale, 0xff000000, 64 );
    
}
