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
    const int animationDurationMS = 1000;
    const float moveDistance = 20*scale;
    const float lineSize = 6;
    const float animatorSize = 4;
    const ImVec2 sourcePosition = mSource->getDrawPosition(offset, scale);
    const ImVec2 targetPosition = mTarget->getDrawPosition(offset, scale);
    const ofPoint deltaPosition = ((ofPoint)targetPosition - (ofPoint)sourcePosition);
    const ofPoint deltaPositionNormalized = deltaPosition.getNormalized();
    const float deltaLength = deltaPosition.length();
    const ImVec2 socketPosition = (ImVec2)((ofPoint)sourcePosition + deltaPositionNormalized*20*scale);
    const float perc = (float)(ofGetElapsedTimeMillis()%animationDurationMS)/(float)animationDurationMS;
    const ofPoint halfSizeVec = deltaPositionNormalized*animatorSize*0.5*scale;
    const ofPoint moveDistVec = deltaPositionNormalized*moveDistance;
    const ofPoint moveDistVecPerc = deltaPositionNormalized*moveDistance*perc;
    int count = deltaLength/moveDistance;
    
    int color = 0xffffffff;
    
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

    
    ImGui::GetWindowDrawList()->AddLine(sourcePosition, targetPosition, color,lineSize*scale);
    
    ImVec2 ap0;
    ImVec2 ap1;

    for( int i=0; i<count; i++ ) {
        ap0.x = moveDistVec.x*i + sourcePosition.x + moveDistVecPerc.x - halfSizeVec.x;
        ap0.y = moveDistVec.y*i + sourcePosition.y + moveDistVecPerc.y - halfSizeVec.y;
        ap1.x = moveDistVec.x*i + sourcePosition.x + moveDistVecPerc.x + halfSizeVec.x;
        ap1.y = moveDistVec.y*i + sourcePosition.y + moveDistVecPerc.y + halfSizeVec.y;
        
        ImGui::GetWindowDrawList()->AddLine(ap0, ap1, 0xff000000, animatorSize*scale);

    }
   
    ImGui::GetWindowDrawList()->AddCircleFilled(socketPosition,7.f*scale, color, 32);
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

EntityRef Connector::getSource() {
    return mSource;
}


EntityRef Connector::getTarget() {
    return mTarget;
}
