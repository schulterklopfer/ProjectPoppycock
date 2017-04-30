//
//  Effect.cpp
//  PoppyCock
//
//  Created by jash on 30.04.17.
//
//

#include "Connector.h"
#include "Entity.h"

void Connector::draw( ImVec2 offset, float scale ) {
    const ImVec2 sourcePosition = mSource->getDrawPosition(offset, scale);
    const ImVec2 targetPosition = mTarget->getDrawPosition(offset, scale);
    
    ImGui::GetWindowDrawList()->AddLine(sourcePosition, targetPosition, 0xffffffff,5.f*scale);
    
}

EntityRef Connector::getSource() {
    return mSource;
}


EntityRef Connector::getTarget() {
    return mTarget;
}
