#pragma once

//#define IMGUI_INCLUDE_IMGUI_USER_H 0


#include "ofMain.h"
#include "ofxImGui.h"
#include "ThemeTest.h"
#include "EntityManager.h"
#include "Globals.h"

typedef struct EntityMenuListEntry {
    string label;
    Interactive::Type type;
} EntityMenuListEntry;

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
    
    std::vector<EntityMenuListEntry> mEntityMenuEntries = {
        {"Shader Effect", Interactive::Type::SHADER_EFFECT },
        {"Observer", Interactive::Type::OBSERVER }
    };

    
private:
    
    bool GUI_entityMenu();
    void GUI_entityArea();
    void GUI_entityArea_backgroundGrid();
    void GUI_sidebar();
    
    static bool __failsEntityMenuFilter (string s);
    
    EntityManager mEntityManager;
    
    bool mShortcutKeyDown;
    bool mMouseSelectionActive;
    ofPoint mMouseSelectionStartPoint;
    

    ofRectangle mEntityAreaViewRect;
    float mEntityAreaScale;
    
    ofxImGui::Gui mGui;
    ImVec4 mBackgroundColor;
    
    //bool mToggleEntityMenu;
    //bool mActivateFilter;
    
    bool mEntityMenuIsOpen;
    ImGuiTextFilter mEntityMenuFilter;
    int mEntityMenuSelectedOption;
    int mEntityMenuCreate;
    
    bool show_test_window;
    
};
