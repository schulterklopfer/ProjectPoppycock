//
//  Effect.cpp
//  PoppyCock
//
//  Created by jash on 30.04.17.
//
//

#include "Connector.h"
#include "Entity.h"
#include "Globals.h"

const char* Connector::sBlendModeLabels[sBlendModeCount] = {
    "Add", "Average", "Color burn", "Color dodge", "Darken",
    "Difference", "Exclusion", "Glow", "Hard light", "Hard mix",
    "Lighten", "Linear burn", "Linear dodge", "Linear light", "Multiply",
    "Negation", "Normal", "Overlay", "Phoenix", "Pin light",
    "Reflect", "Screen", "Soft light", "Subtract", "Vivid light",
    "Mask r", "Mask g", "Mask b"
};



void Connector::draw(const ofPoint sourcePosition,
                     const ofPoint targetPosition,
                     const float scale,
                     const float startOffset,
                     const float endOffset,
                     const int stateFlags,
                     const bool isFader,
                     const bool animate ) {

    const int animationDurationMS = 500;
    const float moveDistance = 24*scale;
    const float lineSize = 6;
    const float animatorSize = 4;
    const ofPoint deltaPosition = (targetPosition - sourcePosition);
    const ofPoint deltaPositionNormalized = deltaPosition.getNormalized();
    const float deltaLength = deltaPosition.length() - (startOffset+endOffset)*scale;
    const ImVec2 socketPosition = (ImVec2)(sourcePosition + deltaPositionNormalized*startOffset*scale);
    const float perc = animate?((float)(Globals::getElapsedTimeMillis()%animationDurationMS))/(float)animationDurationMS:0.5f;
    const ofPoint halfSizeVec = deltaPositionNormalized*lineSize*0.5*scale;
    const ofPoint moveDistVec = deltaPositionNormalized*moveDistance;
    const ofPoint moveDistVecPerc = deltaPositionNormalized*moveDistance*perc;
    int count = deltaLength/moveDistance;
    
    int color = isFader?0xffaaffaa:0xffffffff;
    
    if( (stateFlags&State::OVER) == State::OVER ) {
        color = 0xff0000ff;
    }
    if( (stateFlags&State::DOWN) == State::DOWN ) {
        color = 0xffff00ff;
    }
    if( (stateFlags&State::SELECT) == State::SELECT ) {
        color = 0xff00ff00;
    }
    if( (stateFlags&State::DRAG) == State::DRAG ) {
        color = 0xaa00ff00;
    }
    if( (stateFlags&State::SOURCE) == State::SOURCE ) {
        color = 0xffc3c3c3;
    }
    if( (stateFlags&State::TARGET) == State::TARGET ) {
        color = 0xffff0000;
    }
    if( (stateFlags&State::GHOST) == State::GHOST ) {
        color = 0x55ffffff;
    }

    
    ImGui::GetWindowDrawList()->AddLine(socketPosition,
                                        (ImVec2)((ofPoint)socketPosition+deltaPositionNormalized*deltaLength), color, 3.f*scale);
    
    ImVec2 ap0;
    ImVec2 ap1;

    const float moveDistanceRemainder = deltaLength - moveDistance*count;
    
    for( int i=0; i<=count; i++ ) {
        ap0.x = moveDistVec.x*i + socketPosition.x + moveDistVecPerc.x - halfSizeVec.x;
        ap0.y = moveDistVec.y*i + socketPosition.y + moveDistVecPerc.y - halfSizeVec.y;
        ap1.x = moveDistVec.x*i + socketPosition.x + moveDistVecPerc.x + halfSizeVec.x;
        ap1.y = moveDistVec.y*i + socketPosition.y + moveDistVecPerc.y + halfSizeVec.y;
        
        //ImGui::GetWindowDrawList()->AddLine(ap0, ap1, 0xff000000, animatorSize*scale);
        
        // clip moving dots to connector line
        if( i<count || (i==count && moveDistance*perc < moveDistanceRemainder) ) {
            ImGui::GetWindowDrawList()->AddLine(ap0, ap1, color, lineSize*scale);
        }

    }
    
    ap0.x = sourcePosition.x;
    ap0.y = sourcePosition.y;
    ap1.x = socketPosition.x;
    ap1.y = socketPosition.y;

}

void Connector::draw( const ImVec2 offset, const float scale ) {
    
    const bool animate =
        ( mSource->getMaxEdgeDistanceFromObserver() > -1 &&
          mTarget->getMaxEdgeDistanceFromObserver() > -1 ) ||
        mTarget->isOfType(Interactive::Type::OBSERVER);
    
    Connector::draw((ofPoint)Entity::getDrawPosition(mSource->getPosition(),offset,scale),
                    (ofPoint)Entity::getDrawPosition(mTarget->getPosition(),offset,scale),
                    scale,
                    20.f, 20.f,
                    stateFlags,
                    isFader(),
                    animate );
}

bool Connector::hitTest( const float x, const float y ) {
    const float lineSize = 6;

    const ofPoint sourcePosition = (ofPoint)mSource->getPosition();
    const ofPoint targetPosition = (ofPoint)mTarget->getPosition();
    const ofPoint deltaPosition = ((ofPoint)targetPosition - (ofPoint)sourcePosition);
    const ofPoint deltaPositionNormalized = deltaPosition.getNormalized();
    const ofPoint perpendicularNormalized = ofPoint( deltaPositionNormalized.y, -deltaPositionNormalized.x );
    const float deltaLength = deltaPosition.length();
    
    vector<ofPoint> points;
    
    points.push_back( sourcePosition + perpendicularNormalized*lineSize*0.5 );
    points.push_back( targetPosition + perpendicularNormalized*lineSize*0.5 );
    points.push_back( targetPosition - perpendicularNormalized*lineSize*0.5 );
    points.push_back( sourcePosition - perpendicularNormalized*lineSize*0.5 );
    
    return ofInsidePoly(x, y, points);
    
}

void Connector::inspectorContent() {
    
    
        ImGui::PushID(this);
    int index = 0;
    
    if( ImGui::CollapsingHeader("Connector")) {
        if( mSource->isOfType(Interactive::Type::EFFECT) && mTarget->isOfType(Interactive::Type::EFFECT) ) {
            
            if (ImGui::TreeNode("Blending"))
            {
                
                ImGui::Columns(2);
                ImGui::Separator();
                
                // blend mode
                ImGui::PushID( index++ ); // Use field index as identifier.
                
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::Bullet();
                ImGui::Selectable( "Blend mode" );
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                
                if( ImGui::Combo("##value", (int*)&mBlendMode, sBlendModeLabels, sBlendModeCount) ) {
                    ofLogVerbose(__FUNCTION__) << "blendMode changed to "<<mBlendMode;
                }
                
                
                
                ImGui::PopItemWidth();
                ImGui::NextColumn();
                
                ImGui::PopID();
                
                // blend opacity
                ImGui::PushID( index++ ); // Use field index as identifier.
                
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::Bullet();
                ImGui::Selectable("Blend opacity");
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                if( ImGui::DragFloat( "##value", &mBlendOpacity, 0.01, 0.0f, 1.0f ) ) {
                    ofLogVerbose(__FUNCTION__) << "blend opacity changed to " << mBlendOpacity;
                }
                ImGui::PopItemWidth();
                ImGui::NextColumn();
                
                ImGui::PopID();
                
                
                
                ImGui::Columns(1);
                ImGui::TreePop();
                
            }
        }
        
    }
    ImGui::PopID();


}


bool Connector::isFader() {
    return mTarget->isOfType(Interactive::Type::OBSERVER);
}

int Connector::getTypeFlags() {
    return Interactive::Type::CONNECTOR;
}

EntityRef Connector::getSource() {
    return mSource;
}


EntityRef Connector::getTarget() {
    return mTarget;
}

int Connector::getBlendMode() {
    return mBlendMode;
}

float Connector::getBlendOpacity() {
    return mBlendOpacity;
}


void Connector::recalcBounds() {
    mBounds.set((ofPoint)mSource->getPosition(), (ofPoint)mTarget->getPosition());
}
