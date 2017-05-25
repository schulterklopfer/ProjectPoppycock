//
//  ImEasyCam.hpp
//  PoppyCock
//
//  Created by jash on 14.05.17.
//
//

#ifndef ImEasyCam_h
#define ImEasyCam_h

#include "ofCamera.h"
#include "ofEvents.h"
#include "ImGui.h"


/// \brief A super simple camera for interacting with objects in 3D space.
class ImEasyCam : public ofCamera {
public:
    /// \name Constructor and Destructor
    /// \{
    
    /// \brief Create a default camera.
    ImEasyCam();
    
    /// \brief Destroy the camera.
    ~ImEasyCam();
    
    /// \}
    /// \name Rendering
    /// \{
    
    virtual void begin(ofRectangle viewport = ofRectangle());
    
    /// \brief Reset the camera position and orientation.
    void reset();
    
    /// \}
    /// \name Camera Target
    /// \{
    
    /// \brief Set the camera's target.
    /// \param target The position of the target.
    void setTarget(const ofVec3f& target);
    
    /// \brief Set the camera's target.
    /// \param target The position of the target.
    void setTarget(ofNode& target);
    
    /// \brief Get the camera's target node reference.
    /// \returns a reference the the camera's target node.
    ofNode& getTarget();
    
    /// \}
    /// \name Getters
    /// \{
    
    /// \brief Set the camera's distance to the target.
    /// \param distance The distance to the target.
    void setDistance(float distance);
    
    /// \brief Get the distance to the target.
    /// \returns the distance to the target.
    float getDistance() const;
    
    /// \brief Set the camera's drag coefficient.
    ///
    /// Drag is affects how quickly the camera starts up and slows down when
    /// a user interacts with its position using a pointer.
    ///
    /// \param drag The normalized coefficient value between 0 and 1.
    void setDrag(float drag);
    
    /// \brief Get the camera's drag coefficient.
    /// \returns the camera's drag coefficient.
    float getDrag() const;
    
    /// \}
    /// \name Setters
    /// \{
    
    /// \brief Enable or disable camera autodistance.
    ///
    /// Allow the camera to attempt to set the distance based on the camera's
    /// current viewpoirt.
    ///
    /// \param bAutoDistance true to enable auto distance.
    void setAutoDistance(bool bAutoDistance);
    
    void update( ImVec2 offset );

    
private:
    void setDistance(float distance, bool save);
    
    ofNode target;
    
    bool bApplyInertia;
    bool bDoScrollZoom;
    bool bInsideArcball;
    bool bDistanceSet;
    bool bAutoDistance;
    float lastDistance;
    
    float drag;
    
    float xRot;
    float yRot;
    float zRot;
    
    float moveX;
    float moveY;
    float moveZ;
    
    float sensitivityXY;
    float sensitivityZ;
    float sensitivityRot;
    
    float rotationFactor;
    
    ofVec2f mouseVel;
    
    void updateRotation();
    void updateTranslation();
    void mousePressed(const ofVec2f mouse, const ofVec2f lastMouse);
    void mouseReleased(const ofVec2f mouse, const ofVec2f lastMouse);
    void mouseDragged(const ofVec2f mouse, const ofVec2f lastMouse);
    void mouseScrolled(const float mouse);
    void updateMouse(const ofVec2f mouse, const ofVec2f lastMouse);
        
    /// \brief The time of the last pointer down event.
    unsigned long lastTap;
    
    /// \brief The current rotation quaternion.
    ofQuaternion curRot;
    
    /// \brief The previous X axis.
    ofVec3f prevAxisX;
    
    /// \brief The previous Y axis.
    ofVec3f prevAxisY;
    
    /// \brief The previous Z axis.
    ofVec3f prevAxisZ;
    
    /// \brief the previous camera position.
    ofVec3f prevPosition;
    
    /// \brief The previous camera orientation.
    ofQuaternion prevOrientation;
    
    ofRectangle viewport;
    
    bool mouseDown;
    bool bDoRotate;
    ofVec2f mouse;
    ofVec2f lastMouse;
        
    ofCoreEvents * events;
};


#endif /* ImEasyCam_h */
