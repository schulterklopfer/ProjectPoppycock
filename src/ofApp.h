#pragma once

//#define IMGUI_INCLUDE_IMGUI_USER_H 0


#include "ofMain.h"
#include "ofxImGui.h"
#include "ThemeTest.h"
#include "EntityManager.h"

#define ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void mouseScrolled(float x, float y);
    
    
private:
    
    void GUI_entityArea_backgroundGrid();
    void GUI_entityMenu();
    void GUI_entityArea();
    void GUI_sidebar();
    
    EntityManager mEntityManager;
    
    bool mDragAEntityrea;
    bool mKeyShift;

    ofRectangle mEntityAreaViewRect;
    float mEntityAreaScale;
    
    ofxImGui::Gui mGui;
    ImVec4 mBackgroundColor;
    
    bool mToggleEntityMenu;
    bool mActivateFilter;
    
    bool mEntityMenuIsOpen;
    ImGuiTextFilter mEntityMenuFilter;

    
    
    
    bool show_test_window;
    bool show_another_window;
    
    float floatValue;
    ofImage imageButtonSource;
    GLuint imageButtonID;

    ofPixels pixelsButtonSource;
    GLuint pixelsButtonID;
    
    ofTexture textureSource;
    GLuint textureSourceID;
    
};
