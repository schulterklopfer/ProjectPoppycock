//
//  Observer.cpp
//  PoppyCock
//
//  Created by jash on 01.05.17.
//
//

#include "Observer.h"
#include "GPUEffect.h"

int Observer::getTypeFlags() {
    return Observer::Type::OBSERVER|Observer::Type::ENTITY;
}

void Observer::update() {
    
    if( mInputs.size() > 0 ) {
        if( mInputs[0]->getSource()->isOfType(Interactive::Type::GPU_EFFECT) ) {
            GPUEffectRef eRef = TO_GPU_EFFECT(mInputs[0]->getSource());
            (eRef->getBuffer())->readFromDevice();
            ofLogVerbose(__FUNCTION__) << "Observer inputs[0] result: " << (*(eRef->getBuffer()))[0];
        }
    }

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
