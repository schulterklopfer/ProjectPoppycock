//
//  ImEasyCam.cpp
//  PoppyCock
//
//  Created by jash on 14.05.17.
//
//

#include "ImEasyCam.h"
#include "ofMath.h"
#include "ofUtils.h"

// when an ImEasyCam is moving due to momentum, this keeps it
// from moving forever by assuming small values are zero.
static const float minDifference = 0.1e-5f;

// this is the default on windows os
static const unsigned long doubleclickTime = 200;

//----------------------------------------
ImEasyCam::ImEasyCam(){
    lastTap	= 0;
    lastDistance = 0;
    drag = 0.9f;
    sensitivityRot = 1.0f;//when 1 moving the mouse from one side to the other of the arcball (min(viewport.width, viewport.height)) will rotate 180degrees. when .5, 90 degrees.
    sensitivityXY = .5f;
    sensitivityZ= .7f;
    
    bDistanceSet = false;
    bApplyInertia =false;
    bDoScrollZoom = false;
    bInsideArcball = true;
    bAutoDistance = true;
    bDoRotate = false;
    events = nullptr;
    rotationFactor = 0.0f;
    
    reset();
}

//----------------------------------------
ImEasyCam::~ImEasyCam(){
}

//----------------------------------------
void ImEasyCam::update( ImVec2 offset ){
    
    
    const ofRectangle viewport = getViewport(this->viewport);
    
    if(!bDistanceSet && bAutoDistance){
        setDistance(getImagePlaneDistance(viewport), true);
    }

    if(ImGui::IsMouseHoveringBox(ImGui::GetItemBoxMin(), ImGui::GetItemBoxMax()) ) {
        const ImGuiIO io = ImGui::GetIO();
        ImGui::CaptureMouseFromApp(true);
        
        rotationFactor = sensitivityRot * 180 / min(viewport.width, viewport.height);

        const ImVec2 relMouse = ImVec2( io.MousePos.x - offset.x, io.MousePos.y - offset.y );

        
        lastMouse = mouse;
        mouse = (ofVec2f)relMouse;
        
        if( io.MouseDown[0] ) {
            mousePressed(mouse,lastMouse);
        }
        
        if( ImGui::IsMouseDragging() ) {
            mouseDragged(mouse,lastMouse);
        }
        
        if( io.MouseReleased[0] ) {
            mouseReleased(mouse,lastMouse);
        }
        
        if( io.MouseWheel != 0.0 ) {
            mouseScrolled(io.MouseWheel);
        }
        
        if( bDoRotate ) {
            updateRotation();
        } else if ( bDoScrollZoom) {
            updateTranslation();
            bDoScrollZoom = false;
        }
        ImGui::CaptureMouseFromApp(false);

    }
}

//----------------------------------------
void ImEasyCam::begin(ofRectangle _viewport){
    viewport = getViewport(_viewport);
    ofCamera::begin(viewport);
}

//----------------------------------------
void ImEasyCam::reset(){
    target.resetTransform();
    
    target.setPosition(0,0, 0);
    lookAt(target);
    
    resetTransform();
    setPosition(0, 0, lastDistance);
    
    xRot = 0;
    yRot = 0;
    zRot = 0;
    
    moveX = 0;
    moveY = 0;
    moveZ = 0;
    
    bDoRotate = false;
    bApplyInertia = false;
}

//----------------------------------------
void ImEasyCam::setTarget(const ofVec3f& targetPoint){
    target.setPosition(targetPoint);
    lookAt(target);
}

//----------------------------------------
void ImEasyCam::setTarget(ofNode& targetNode){
    target = targetNode;
    lookAt(target);
}

//----------------------------------------
ofNode& ImEasyCam::getTarget(){
    return target;
}

//----------------------------------------
void ImEasyCam::setDistance(float distance){
    setDistance(distance, true);
}

//----------------------------------------
void ImEasyCam::setDistance(float distance, bool save){//should this be the distance from the camera to the target?
    if(distance > 0.0f){
        if(save){
            this->lastDistance = distance;
        }
        setPosition(target.getPosition() + (distance * getZAxis()));
        bDistanceSet = true;
    }
}

//----------------------------------------
float ImEasyCam::getDistance() const{
    return target.getPosition().distance(getPosition());
}

//----------------------------------------
void ImEasyCam::setAutoDistance(bool bAutoDistance){
    this->bAutoDistance = bAutoDistance;
    if(bAutoDistance){
        bDistanceSet = false;
    }
}

//----------------------------------------
void ImEasyCam::setDrag(float drag){
    this->drag = drag;
}

//----------------------------------------
float ImEasyCam::getDrag() const{
    return drag;
}

//----------------------------------------
void ImEasyCam::updateTranslation(){
    if(bApplyInertia){
        moveX *= drag;
        moveY *= drag;
        moveZ *= drag;
        if(ABS(moveX) <= minDifference && ABS(moveY) <= minDifference && ABS(moveZ) <= minDifference){
            bApplyInertia = false;
        }
        move((getXAxis() * moveX) + (getYAxis() * moveY) + (getZAxis() * moveZ));
    }else{
        setPosition(prevPosition + ofVec3f(prevAxisX * moveX) + (prevAxisY * moveY) + (prevAxisZ * moveZ));
    }
}

//----------------------------------------
void ImEasyCam::updateRotation(){
    if(bApplyInertia){
        xRot *=drag;
        yRot *=drag;
        zRot *=drag;
        
        if(ABS(xRot) <= minDifference && ABS(yRot) <= minDifference && ABS(zRot) <= minDifference){
            bApplyInertia = false;
            bDoRotate = false;
        }
        curRot = ofQuaternion(xRot, getXAxis(), yRot, getYAxis(), zRot, getZAxis());
        setPosition((getGlobalPosition()-target.getGlobalPosition())*curRot +target.getGlobalPosition());
        rotate(curRot);
    }else{
        curRot = ofQuaternion(xRot, prevAxisX, yRot, prevAxisY, zRot, prevAxisZ);
        setPosition((prevPosition-target.getGlobalPosition())*curRot +target.getGlobalPosition());
        setOrientation(prevOrientation * curRot);
    }
}

void ImEasyCam::mousePressed(const ofVec2f mouse, const ofVec2f lastMouse){
    const ofRectangle viewport = getViewport(this->viewport);

    prevAxisX = getXAxis();
    prevAxisY = getYAxis();
    prevAxisZ = getZAxis();
    prevPosition = ofCamera::getGlobalPosition();
    prevOrientation = ofCamera::getGlobalOrientation();
    

    if(ofVec2f(mouse.x - viewport.x - (viewport.width/2), mouse.y - viewport.y - (viewport.height/2)).length() < min(viewport.width/2, viewport.height/2)){
        bInsideArcball = true;
    }else{
        bInsideArcball = false;
    }
    
    bDoRotate = false;
    bApplyInertia = false;
    
}

void ImEasyCam::mouseReleased(const ofVec2f mouse, const ofVec2f lastMouse ){
    
    const unsigned long curTap = ofGetElapsedTimeMillis();
    const ofRectangle viewport = getViewport(this->viewport);
    if(lastTap != 0 && curTap - lastTap < doubleclickTime){
        reset();
        return;
    }
    lastTap = curTap;
    bApplyInertia = true;
    mouseVel = mouse  - lastMouse;
    
    updateMouse(mouse,lastMouse);
    ofVec2f center(viewport.width/2, viewport.height/2);
    int vFlip;
    if(isVFlipped()){
        vFlip = -1;
    }else{
        vFlip =  1;
    }
    zRot = -vFlip * ofVec2f(mouse.x - viewport.x - center.x, mouse.y - viewport.y - center.y).angle(lastMouse - ofVec2f(viewport.x, viewport.y) - center);
    
}

void ImEasyCam::mouseDragged(const ofVec2f mouse, const ofVec2f lastMouse){
    mouseVel = mouse  - lastMouse;
    bDoRotate = true;
    updateMouse(mouse, lastMouse);

}

void ImEasyCam::mouseScrolled(const float mouse){
    const ofRectangle viewport = getViewport(this->viewport);
    prevPosition = ofCamera::getGlobalPosition();
    prevAxisZ = getZAxis();
    moveZ = mouse * 30 * sensitivityZ * (getDistance() + FLT_EPSILON)/ viewport.height;
    bDoScrollZoom = true;
}

void ImEasyCam::updateMouse(const ofVec2f mouse, const ofVec2f lastMouse){
    const ofRectangle viewport = getViewport(this->viewport);
    int vFlip;
    if(isVFlipped()){
        vFlip = -1;
    }else{
        vFlip =  1;
    }

    xRot = 0;
    yRot = 0;
    zRot = 0;
    if(bInsideArcball){
        xRot = vFlip * -mouseVel.y * rotationFactor;
        yRot = -mouseVel.x * rotationFactor;
    }else{
        ofVec2f center(viewport.width/2, viewport.height/2);
        zRot = -vFlip * ofVec2f(mouse.x - viewport.x - center.x, mouse.y - viewport.y - center.y).angle(lastMouse - ofVec2f(viewport.x, viewport.y) - center);
    }
    

}
