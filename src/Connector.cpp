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
    const int animationDurationMS = 500;
    const float moveDistance = 24*scale;
    const float lineSize = 6;
    const float animatorSize = 4;
    const ofPoint sourcePosition = (ofPoint)mSource->getDrawPosition(offset, scale);
    const ofPoint targetPosition = (ofPoint)mTarget->getDrawPosition(offset, scale);
    const ofPoint deltaPosition = (targetPosition - sourcePosition);
    const ofPoint deltaPositionNormalized = deltaPosition.getNormalized();
    const float deltaLength = deltaPosition.length() - 2.f*20.f*scale;
    const ImVec2 socketPosition = (ImVec2)(sourcePosition + deltaPositionNormalized*20*scale);
    const float perc = (float)(ofGetElapsedTimeMillis()%animationDurationMS)/(float)animationDurationMS;
    //const ofPoint halfSizeVec = deltaPositionNormalized*animatorSize*0.5*scale;
    const ofPoint halfSizeVec = deltaPositionNormalized*lineSize*0.5*scale;
    const ofPoint moveDistVec = deltaPositionNormalized*moveDistance;
    const ofPoint moveDistVecPerc = deltaPositionNormalized*moveDistance*perc;
    int count = deltaLength/moveDistance;
    
    int color = isFader()?0xffaaffaa:0xffffffff;
    
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

    
    ImGui::GetWindowDrawList()->AddLine(socketPosition,
                                        (ImVec2)((ofPoint)socketPosition+deltaPositionNormalized*deltaLength), color, 1);
    
    ImVec2 ap0;
    ImVec2 ap1;

    for( int i=0; i<count; i++ ) {
        ap0.x = moveDistVec.x*i + socketPosition.x + moveDistVecPerc.x - halfSizeVec.x;
        ap0.y = moveDistVec.y*i + socketPosition.y + moveDistVecPerc.y - halfSizeVec.y;
        ap1.x = moveDistVec.x*i + socketPosition.x + moveDistVecPerc.x + halfSizeVec.x;
        ap1.y = moveDistVec.y*i + socketPosition.y + moveDistVecPerc.y + halfSizeVec.y;
        
        //ImGui::GetWindowDrawList()->AddLine(ap0, ap1, 0xff000000, animatorSize*scale);
        ImGui::GetWindowDrawList()->AddLine(ap0, ap1, color, lineSize*scale);

    }
    
    ap0.x = sourcePosition.x;
    ap0.y = sourcePosition.y;
    ap1.x = socketPosition.x;
    ap1.y = socketPosition.y;
   
    //ImGui::GetWindowDrawList()->AddLine(ap0, ap1, color, 6.f*scale);
    //ImGui::GetWindowDrawList()->AddCircleFilled(ap0, 3*scale, color, 16 );

    //ImGui::GetWindowDrawList()->AddCircleFilled(socketPosition,7.f*scale, color, 32);
}

bool Connector::hitTest( float x, float y ) {
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

bool Connector::isFader() {
    return (mTarget->getTypeFlags()&Entity::Type::OBSERVER)==Entity::Type::OBSERVER;
}

EntityRef Connector::getSource() {
    return mSource;
}


EntityRef Connector::getTarget() {
    return mTarget;
}
