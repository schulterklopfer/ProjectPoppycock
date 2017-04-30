#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetEscapeQuitsApp(false);
    //required call
    mGui.setup();
    
    ImGui::GetIO().MouseDrawCursor = false;
    //backgroundColor is stored as an ImVec4 type but can handle ofColor
    mBackgroundColor = ofColor(114, 144, 154);
    show_test_window = true;
    show_another_window = false;
    floatValue = 0.0f;
    mEntityAreaScale = 1.0;
    mDragAEntityrea = false;
    
    mEntityMenuIsOpen = mToggleEntityMenu = mActivateFilter = false;
    
    //load your own ofImage
    imageButtonSource.load("of.png");
    imageButtonID = mGui.loadImage(imageButtonSource);
    
    //or have the loading done for you if you don't need the ofImage reference
    //imageButtonID = gui.loadImage("of.png");
    
    //can also use ofPixels in same manner
    ofLoadImage(pixelsButtonSource, "of_upside_down.png");
    pixelsButtonID = mGui.loadPixels(pixelsButtonSource);
    
    //and alt method
    //pixelsButtonID = gui.loadPixels("of_upside_down.png");
    
    //pass in your own texture reference if you want to keep it
    textureSourceID = mGui.loadTexture(textureSource, "of_upside_down.png");
    
    //or just pass a path
    //textureSourceID = gui.loadTexture("of_upside_down.png");

    ofLogVerbose() << "textureSourceID: " << textureSourceID;
    
    mEntityManager.createEffect("test0", ofVec2f(20,20));
    mEntityManager.createEntity("test1", ofVec2f(100.0f,100.0f));
    mEntityManager.createEffect("test2", ofVec2f(100.0f,200.0f));
    mEntityManager.createEntity("test3", ofVec2f(200.0f,100.0f));
    mEntityManager.createEntity("test4", ofVec2f(1030,1030));

    
}

bool doSetTheme = false;
//--------------------------------------------------------------
void ofApp::update(){
    
    if(doSetTheme)
    {
        doSetTheme = false;
        mGui.setTheme(new ThemeTest());
        
    }
    
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    
    //backgroundColor is stored as an ImVec4 type but is converted to ofColor automatically
    
    ofSetBackgroundColor(mBackgroundColor);
    
    //required to call this at beginning
    mGui.begin();
    
   
    
   
    
    //In between gui.begin() and gui.end() you can use ImGui as you would anywhere else
    
    // 1. Show a simple window
    {
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("Float", &floatValue, 0.0f, 1.0f);
        
        //this will change the app background color
        ImGui::ColorEdit3("Background Color", (float*)&mBackgroundColor);
        if(ImGui::Button("Test Window"))
        {
            show_test_window = !show_test_window;
        }
        
        if (ImGui::Button("Another Window"))
        {
            //bitwise OR
            show_another_window ^= 1;
            
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    
    // 2. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    if (show_test_window)
    {
        ImGui::SetNextWindowPos(ofVec2f(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&show_test_window);
    }
    
    
    bool pressed = ImGui::ImageButton((ImTextureID)(uintptr_t)imageButtonID, ImVec2(200, 200));
    pressed = ImGui::ImageButton((ImTextureID)(uintptr_t)pixelsButtonID, ImVec2(200, 200));
    pressed = ImGui::ImageButton((ImTextureID)(uintptr_t)textureSourceID, ImVec2(200, 200));
    
    
    GUI_entityArea();
    GUI_sidebar();
    GUI_entityMenu();

    
    //required to call this at end
    mGui.end();
    
    if(textureSource.isAllocated())
    {
        //textureSource.draw(ofRandom(200), ofRandom(200));
    }
}

void ofApp::GUI_entityArea() {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoSavedSettings;
    
    float margin = 10;
    float width = ofGetWidth()-(400+2*margin);
    float height = ofGetHeight()-2*margin;
    
    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::SetNextWindowPos(ImVec2(margin, margin));
    
    ImGuiIO io = ImGui::GetIO();
    
    if( ImGui::Begin("entities", NULL, window_flags) ) {
 
        ImGui::BeginChild("draw area");
        ImVec2 drawAreaSize = ImGui::GetWindowContentRegionMax();
        mEntityAreaViewRect.setSize(drawAreaSize.x/mEntityAreaScale,
                                    drawAreaSize.y/mEntityAreaScale);
        const ImVec2 cPos = ImGui::GetCursorScreenPos();
        ImVec2 relativeOffset = ImVec2( mEntityAreaViewRect.getX() + cPos.x,
                                        mEntityAreaViewRect.getY() + cPos.y );

        EntityList* entities = mEntityManager.getEntities();
        ConnectorList* connectors = mEntityManager.getConnectors();

        mEntityManager.hotInteractive = NULL;

        if (ImGui::IsWindowFocused() ) {
            const ImVec2 relMousePosition = ImVec2( io.MousePos.x - cPos.x,
                                                   io.MousePos.y - cPos.y );
            
            // target mode is entered when entity is dragged and shit key is held down
            // this is used to draw connections from draggingEntity to hotEntity
            const bool targetMode = (io.KeyShift && mEntityManager.draggingInteractive != NULL);
            
            // check if we hover an entity in reverse so
            // top entities are selected first
            if( targetMode || (mEntityManager.draggingInteractive == NULL && !mDragAEntityrea ) ) {
                EntityList* list = mEntityManager.getEntities();
                for (unsigned i = list->size(); i-- > 0; ) {
                    // hover
                    EntityRef eRef = list->at(i);
                    eRef->stateFlags &= ~(Entity::State::DOWN|Entity::State::OVER|Entity::State::TARGET);

                    if( eRef->hitTest((relMousePosition.x - mEntityAreaViewRect.getX())/mEntityAreaScale,
                                      (relMousePosition.y - mEntityAreaViewRect.getY())/mEntityAreaScale) ) {
                        
                        // mark entity under mouse as hot
                        if( mEntityManager.hotInteractive == NULL ) {
                            mEntityManager.hotInteractive = eRef;
                            mEntityManager.hotInteractive->stateFlags |= Entity::State::OVER;
                        }
                        
                        // mark entity under mouse when left button is down as active
                        if( io.MouseDown[0] && mEntityManager.hotInteractive == eRef ) {
                            mEntityManager.activeInteractive = eRef;
                            mEntityManager.activeInteractive->stateFlags |= Entity::State::DOWN;
                            if( targetMode && mEntityManager.activeInteractive != mEntityManager.draggingInteractive ) {
                                mEntityManager.activeInteractive->stateFlags |= Entity::State::TARGET;
                            }
                        }
                        // select entity under mouse when button was pressed and released over the same entity
                        // but only when not in target mode
                        else if( io.MouseReleased[0] && mEntityManager.activeInteractive == eRef && !targetMode ) {
                            if( mEntityManager.selectedInteractive != NULL &&
                                mEntityManager.selectedInteractive != eRef ) {
                                mEntityManager.selectedInteractive->stateFlags &= ~Entity::State::SELECT;
                            }
                            mEntityManager.selectedInteractive = eRef;
                            mEntityManager.selectedInteractive->stateFlags |= Entity::State::SELECT;
                        }
                    }
                }
            }


            // reset drag mode when mouse is release or shift key is released
            if( io.MouseReleased[0] || mKeyShift != io.KeyShift ) {
                if( mEntityManager.draggingInteractive != NULL ) {
                    mEntityManager.draggingInteractive->stateFlags &= ~(Entity::State::DRAG|Entity::State::SOURCE);
                }
                if( mEntityManager.activeInteractive != NULL ) {
                    mEntityManager.activeInteractive->stateFlags &= ~(Entity::State::DOWN);
                }
                if( mEntityManager.hotInteractive != NULL ) {
                    mEntityManager.hotInteractive->stateFlags &= ~(Entity::State::TARGET);

                }
                
                if( mEntityManager.draggingInteractive != NULL && mEntityManager.hotInteractive != NULL && io.KeyShift ) {
                    
                    // check if hot and dragging interactive are Entities
                    if((mEntityManager.draggingInteractive->getTypeFlags()&Interactive::Type::ENTITY) == Interactive::Type::ENTITY &&
                       (mEntityManager.hotInteractive->getTypeFlags()&Interactive::Type::ENTITY) == Interactive::Type::ENTITY ) {
                        EntityRef draggingEntity = boost::static_pointer_cast<Entity>(mEntityManager.draggingInteractive);
                        EntityRef hotEntity = boost::static_pointer_cast<Entity>(mEntityManager.hotInteractive);
                        
                        // we have a connection, add connection object to data structs
                        // connections have a direction. there can be multiple connections out
                        // but only one connection in.
                        ofLogVerbose(__FUNCTION__) << "connection from " << draggingEntity << " to " << hotEntity;
                        mEntityManager.createConnector(draggingEntity, hotEntity);
                    }
                    
                    
                }
                
                mEntityManager.activeInteractive = mEntityManager.draggingInteractive = NULL;
                mDragAEntityrea = false;
            }
            
            if( ImGui::IsMouseDragging() ) {
                if( mEntityManager.activeInteractive != NULL && mEntityManager.draggingInteractive == NULL ) {
                    mEntityManager.draggingInteractive = mEntityManager.activeInteractive;
                    mEntityManager.draggingInteractive->stateFlags |= io.KeyShift?Entity::State::SOURCE:Entity::State::DRAG;
                }
                
                EntityRef draggingEntity= NULL;
                EntityRef hotEntity = NULL;
                
                if(mEntityManager.draggingInteractive != NULL &&
                   (mEntityManager.draggingInteractive->getTypeFlags()&Interactive::Type::ENTITY) == Interactive::Type::ENTITY ) {
                    draggingEntity = boost::static_pointer_cast<Entity>(mEntityManager.draggingInteractive);
                }
                
                if(mEntityManager.hotInteractive != NULL &&
                   (mEntityManager.hotInteractive->getTypeFlags()&Interactive::Type::ENTITY) == Interactive::Type::ENTITY ) {
                    hotEntity = boost::static_pointer_cast<Entity>(mEntityManager.hotInteractive);
                }
                
                if( draggingEntity != NULL ) {
                    
                    // drag connection to another entity
                    if( io.KeyShift ) {
                        if( mEntityManager.draggingInteractive != NULL ) {
                            // we have a connection, draw a line ...
                            if( mEntityManager.hotInteractive != NULL ) {
                                // ... to center of target
                                const ImVec2 start = ImVec2(
                                                            draggingEntity->getPosition().x*
                                                            mEntityAreaScale+relativeOffset.x,
                                                            draggingEntity->getPosition().y*
                                                            mEntityAreaScale+relativeOffset.y
                                                            );
                                const ImVec2 end = ImVec2(
                                                          hotEntity->getPosition().x*
                                                          mEntityAreaScale+relativeOffset.x,
                                                          hotEntity->getPosition().y*
                                                          mEntityAreaScale+relativeOffset.y
                                                          );
                                ImGui::GetWindowDrawList()->AddLine(start, end, 0xff00ff00);
                            } else {
                                // ... to mouse pointer
                                const ImVec2 start = ImVec2(
                                                            draggingEntity->getPosition().x*
                                                            mEntityAreaScale+relativeOffset.x,
                                                            draggingEntity->getPosition().y*
                                                            mEntityAreaScale+relativeOffset.y
                                                            );
                                ImGui::GetWindowDrawList()->AddLine(start, io.MousePos, 0xff00ffff);
                            }
                            
                        }
                    }
                    // drag entity around
                    else {
                        draggingEntity->move(
                                             io.MouseDelta.x/mEntityAreaScale,
                                             io.MouseDelta.y/mEntityAreaScale
                                             );
                    }

                } else {
                    // drag view around
                    // TODO: Limit to bounds of EntityManager
                    mDragAEntityrea = true;
                    mEntityAreaViewRect.position.x += io.MouseDelta.x;
                    mEntityAreaViewRect.position.y += io.MouseDelta.y;
                }
                
            }
            
            // scale view with mouse wheel around mouse pointer
            if( io.MouseWheel != 0 ) {
                
                // view rect
                ofRectangle rect = mEntityAreaViewRect;

                // compute the mouse position in the unscaled entity view based on the area view rect and
                // scale factor
                // this will act as an anchor when scaling is done
                const ImVec2 viewAreaMouse = ImVec2((relMousePosition.x - rect.position.x)/mEntityAreaScale,
                                                    (relMousePosition.y - rect.position.y)/mEntityAreaScale );
                
                // set new scale factor
                mEntityAreaScale += io.MouseWheel*0.1;
                if( mEntityAreaScale < 0.1 ) {
                    mEntityAreaScale = 0.1;
                } else if( mEntityAreaScale > 4.0 ) {
                    mEntityAreaScale = 4.0;
                }
                
                // compute new view rect position based on mouse anchor and new scale factor
                rect.position.set(relMousePosition.x - viewAreaMouse.x*mEntityAreaScale,
                                  relMousePosition.y - viewAreaMouse.y*mEntityAreaScale);
                
                rect.setSize(drawAreaSize.x*mEntityAreaScale, drawAreaSize.y*mEntityAreaScale);
                
                mEntityAreaViewRect.set(rect);
             
                relativeOffset.x = mEntityAreaViewRect.getX() + cPos.x;
                relativeOffset.y = mEntityAreaViewRect.getY() + cPos.y;
                
                mEntityAreaViewRect.setSize(drawAreaSize.x/mEntityAreaScale,
                                            drawAreaSize.y/mEntityAreaScale);


            }
            
            // hit test on connectors
            for( ConnectorListIterator iter = connectors->begin(); iter != connectors->end(); ++iter ) {
                if( (*iter)->hitTest((relMousePosition.x - mEntityAreaViewRect.getX())/mEntityAreaScale,
                                     (relMousePosition.y - mEntityAreaViewRect.getY())/mEntityAreaScale) ) {
                    (*iter)->stateFlags |= Connector::State::OVER;
                } else {
                    (*iter)->stateFlags &= ~Connector::State::OVER;

                }
                
                
            }

        }
        
        GUI_entityArea_backgroundGrid();
        
        // draw connectors
        for( ConnectorListIterator iter = connectors->begin(); iter != connectors->end(); ++iter ) {
            (*iter)->draw( relativeOffset, mEntityAreaScale );
            //(*iter)->drawBoundingBox( relativeOffset, mEntityAreaScale );
            
        }

        // draw entities
        for( EntityListIterator iter = entities->begin(); iter != entities->end(); ++iter ) {
            (*iter)->draw( relativeOffset, mEntityAreaScale );
            //(*iter)->drawBoundingBox( relativeOffset, mEntityAreaScale );

        }
                
        mEntityManager.checkDirtyBounds();
        //mEntityManager.drawBoundingBox(relativeOffset, mEntityAreaScale);
        
        ImGui::EndChild();
        mKeyShift = io.KeyShift;
        
    }
    
    ImGui::End();
}

void ofApp::GUI_entityArea_backgroundGrid() {
    
    
    const ImVec2 cPos = ImGui::GetCursorScreenPos();
    
    const float minX = 0;
    const float minY = 0;
    const float maxX = mEntityAreaViewRect.getWidth();
    const float maxY = mEntityAreaViewRect.getHeight();
    
    const int step = 20;

    const ImVec2 gridOffset = ImVec2((int)(mEntityAreaViewRect.position.x/mEntityAreaScale)%step,
                                     (int)(mEntityAreaViewRect.position.y/mEntityAreaScale)%step);

    
    ImVec2 tl;
    ImVec2 br;
    
    const int color = 0x55ffffff;
    
    for( int y=minY; y<=maxY; y+=step ) {
        for( int x=minX; x<=maxX; x+=step ) {
            
            tl.x = x + gridOffset.x;
            tl.y = y + gridOffset.y;
            
            tl.x*=mEntityAreaScale;
            tl.y*=mEntityAreaScale;

            tl.x += cPos.x;
            tl.y += cPos.y;
            
            br.x = tl.x+1;
            br.y = tl.y+1;
            
            ImGui::GetWindowDrawList()->AddRectFilled(tl, br, color);
        }
    }
}


void ofApp::GUI_sidebar() {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoSavedSettings;
    
    
    ImGui::SetNextWindowSize(ImVec2(398, ofGetHeight()-20));
    ImGui::SetNextWindowPos(ImVec2(ImVec2(ofGetWidth()-408,10)));
    
    if( ImGui::Begin("sidebar", NULL, ImVec2(0.f,0.f), -1.0f, window_flags) ) {
    }
    
    ImGui::End();

}

void ofApp::GUI_entityMenu() {
    // show menu for creating Entities like
    // light-structures and effects
    
    static ImVec4 color = ImColor(0.8f, 0.5f, 1.0f, 1.0f);
    ImGui::ColorButton(color);
    
    if ( mToggleEntityMenu ) {
        mToggleEntityMenu = false;
        
        if( !ImGui::BeginPopup("entity context menu") ) {
            ImGui::OpenPopup("entity context menu");
            ImVec2 mPos = ImGui::GetIO().MousePos;
            ImGui::SetNextWindowPos(ImVec2( mPos.x-20.0, mPos.y-20.0 ));
        } else {
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
    }
    
    
    if (ImGui::BeginPopup("entity context menu"))
    {
        mEntityMenuIsOpen = true;
        if( mActivateFilter ) {
            ImGui::SetKeyboardFocusHere();
            mActivateFilter = false;
        }
        
        mEntityMenuFilter.Draw("");
        const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
        /*
        for (int i = 0; i < ARRAYSIZE(lines); i++)
            if (mEntityMenuFilter.PassFilter(lines[i]))
                ImGui::BulletText("%s", lines[i]);
        */
        

        ImGui::BeginChild("child", ImVec2(208.0f, 75.0f), false);
        ImGui::PushItemWidth(208);

        for (int line = 0; line < ARRAYSIZE(lines); line++)
        {
            if (mEntityMenuFilter.PassFilter(lines[line])) {
                ImGui::Button(lines[line]);
            
            }

        }
        ImGui::PopItemWidth();
        ImGui::EndChild();
        
        
        
        ImGui::EndPopup();
    } else {
        mEntityMenuIsOpen = false;
        if( mEntityMenuFilter.InputBuf[0] != 0 ) {
            mEntityMenuFilter.Clear();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if( mEntityMenuIsOpen ) {
        if( key == 27 ) { // ESC
            mToggleEntityMenu = true;
        }
        else if( !mEntityMenuFilter.IsActive() ) {
            if( key == ' ' ) {
                if( !mEntityMenuFilter.IsActive() ) {
                    mToggleEntityMenu = true;
                }
            }
        }
    } else {
        if( key == ' ' ) {
            if( !mEntityMenuFilter.IsActive() ) {
                mToggleEntityMenu = true;
                mActivateFilter = true;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}


void ofApp::mouseScrolled(float x, float y)
{
    ofLogVerbose(__FUNCTION__) << "x: " << x << " y: " << y;
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
