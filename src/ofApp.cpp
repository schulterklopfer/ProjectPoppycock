#include "ofApp.h"
#include "ImGui.h"
#include "OCLKernelRegistry.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetEscapeQuitsApp(false);
    ofHideCursor();
    //required call
    mGui.setup();
    
    ImGui::GetIO().MouseDrawCursor = true;
    
    //backgroundColor is stored as an ImVec4 type but can handle ofColor
    mBackgroundColor = ofColor::gray;
    mEntityAreaScale = 1.0;
    mEntityMenuSelectedOption = 0;
    mEntityMenuCreate = -1;
    mEntityMenuIsOpen = false;

    show_test_window = false;
    
    //mGui.setTheme(new ThemeTest());
        
    KernelRegistryInstance->setupFromDirectory("opencl/generators");
}

//--------------------------------------------------------------
void ofApp::update(){
    // update (for example time scaled millis counter)
    Globals::update();
    
    // call update() in entities
    EntityList* const entities = mEntityManager.getEntitiesToUpdate();

    for( EntityListIterator iter = entities->begin(); iter != entities->end(); ++iter ) {
        (*iter)->update();
    }
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    
    //backgroundColor is stored as an ImVec4 type but is converted to ofColor automatically
    
    ofSetBackgroundColor(mBackgroundColor);
    
    //required to call this at beginning
    mGui.begin();
    
    if( show_test_window ) {
        ImGui::ShowTestWindow();
    }
    
    GUI_sidebar();
    GUI_entityArea();
    
    //required to call this at end
    mGui.end();

}

void ofApp::GUI_entityArea() {

    ImGuiWindowFlags window_flags = 0;
    //window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoSavedSettings;
    
    float margin = 10;
    float width = ofGetWidth()-(400+2*margin);
    float height = ofGetHeight()-2*margin;
    
    ImGui::SetNextWindowSize(ImVec2(width, height));
    ImGui::SetNextWindowPos(ImVec2(margin, margin));
    
    ImGui::SetMouseCursor(0);
    
    ImGuiIO io = ImGui::GetIO();
    
    const bool shortcutKeyDown = io.ShortcutsUseSuperKey?io.KeySuper:io.KeyCtrl;
    
    if( ImGui::Begin("Patch", NULL, window_flags) ) {

        ImGui::BeginChild("draw area");
        
        if( ImGui::IsRootWindowOrAnyChildHovered() ) {
            ImGui::SetWindowFocus();
        }



        
        ImVec2 drawAreaSize = ImGui::GetWindowContentRegionMax();
        mEntityAreaViewRect.setSize(drawAreaSize.x/mEntityAreaScale,
                                    drawAreaSize.y/mEntityAreaScale);
        const ImVec2 cPos = ImGui::GetCursorScreenPos();
        ImVec2 relativeOffset = ImVec2( mEntityAreaViewRect.getX() + cPos.x,
                                        mEntityAreaViewRect.getY() + cPos.y );

        const ImVec2 relMousePosition = ImVec2( io.MousePos.x - cPos.x,
                                               io.MousePos.y - cPos.y );

        EntityList* const entities = mEntityManager.getEntities();
        ConnectorList* const connectors = mEntityManager.getConnectors();

        mEntityManager.hotInteractive = NULL;

        // check for keyboard things if entity menu is not open
        if( !GUI_entityMenu() ) {
            const bool backspacePressed = ImGui::IsKeyPressed( ImGui::GetKeyIndex(ImGuiKey_Backspace));
            if( backspacePressed ) {
                ofLogVerbose( __FUNCTION__ ) << "entity area: backspace pressed inside entities window";
                // delete selected interactive from EntityManager
                if( mEntityManager.selectedInteractives.size() != 0 ) {
                    for( SelectionIterator iter = mEntityManager.selectedInteractives.begin(); iter != mEntityManager.selectedInteractives.end(); ++iter ) {
                        mEntityManager.deleteInteractive(*iter);
                    }
                    mEntityManager.selectedInteractives.clear();
                }
            }
            
        }
        bool overAnyInteractive = false;
        
        GUI_entityArea_backgroundGrid();
        
        // draw connectors
        for( ConnectorListIterator iter = connectors->begin(); iter != connectors->end(); ++iter ) {
            (*iter)->draw( relativeOffset, mEntityAreaScale );
        }
        
        // draw entities
        for( EntityListIterator iter = entities->begin(); iter != entities->end(); ++iter ) {
            (*iter)->draw( relativeOffset, mEntityAreaScale );
            if( (*iter)->isOfType(Interactive::Type::GPU_EFFECT) ) {
                GPUEffectRef e = TO_GPU_EFFECT((*iter));
                e->debugDraw();
            }
        }
        
        if (ImGui::IsWindowFocused() && ImGui::IsMouseHoveringWindow() ) {
            // check some commands here:
            //if( io.ShortcutsUseSuperKey?io.KeySuper:io.KeyCtrl ) {
            //    ofLogVerbose( __FUNCTION__ ) << "duplicating selection " << (int)'d';
                
            //}

            // target mode is entered when entity is dragged and shit key is held down
            // this is used to draw connections from draggingEntity to hotEntity
            const bool targetMode = (shortcutKeyDown && mEntityManager.draggingInteractive != NULL);
            
            // check if we hover an entity in reverse so
            // top entities are selected first
            if( targetMode || (mEntityManager.draggingInteractive == NULL && !ImGui::IsMouseDragging() ) ) {
                //EntityList* list = mEntityManager.getEntities();
                InteractiveList* const list = mEntityManager.getInteractives();
                for (unsigned i = list->size(); i-- > 0; ) {
                    // hover
                    InteractiveRef eRef = list->at(i);
                    eRef->stateFlags &= ~(Entity::State::DOWN|Entity::State::OVER|Entity::State::TARGET|Entity::State::SOURCE);

                    if( eRef->hitTest((relMousePosition.x - mEntityAreaViewRect.getX())/mEntityAreaScale,
                                      (relMousePosition.y - mEntityAreaViewRect.getY())/mEntityAreaScale) ) {
                        
                        overAnyInteractive = true;
                        
                        // mark entity under mouse as hot
                        if( mEntityManager.hotInteractive == NULL ) {
                            mEntityManager.hotInteractive = eRef;
                            if( !io.MouseDown[0] ) {
                                // classic mouse over is without pressed button
                                mEntityManager.hotInteractive->stateFlags |= Entity::State::OVER;
                            }
                            if( targetMode && mEntityManager.hotInteractive != mEntityManager.draggingInteractive ) {
                                if(mEntityManager.hotInteractive->isOfType(Interactive::Type::ENTITY) &&
                                   mEntityManager.draggingInteractive->isOfType(Interactive::Type::ENTITY) ) {
                                    // only accepted as target when entity and acceptsInput
                                    const EntityRef hotEntity = TO_ENTITY(mEntityManager.hotInteractive);
                                    const EntityRef draggingEntity = TO_ENTITY(mEntityManager.draggingInteractive);
                                    if( hotEntity->acceptsInputFrom(draggingEntity) ) {
                                        hotEntity->stateFlags |= Entity::State::TARGET;
                                    }
                                }
                            }
                        }
                        
                        // mark entity under mouse when left button is down as active
                        if( io.MouseDown[0] && mEntityManager.hotInteractive == eRef && mEntityManager.draggingInteractive == NULL ) {
                            // only active when not dragging
                            mEntityManager.activeInteractive = eRef;
                            mEntityManager.activeInteractive->stateFlags |= Entity::State::DOWN;
                        }
                        // select entity under mouse when button was pressed and released over the same entity
                        // but only when not in target mode
                        else if( io.MouseReleased[0] && mEntityManager.activeInteractive == eRef && !targetMode ) {
                            
                            
                            //  multiselect
                            if( !(io.MultiSelectUsesSuperKey?io.KeySuper:io.KeyCtrl) ) {
                                for( SelectionIterator iter = mEntityManager.selectedInteractives.begin(); iter != mEntityManager.selectedInteractives.end(); ++iter ) {
                                    if( (*iter) == eRef )
                                        continue;
                                    (*iter)->stateFlags &= ~Entity::State::SELECT;
                                }
                                mEntityManager.selectedInteractives.clear();
                            }
                            if( !mEntityManager.isInSelection(eRef) ) {
                                eRef->stateFlags |= Entity::State::SELECT;
                                mEntityManager.selectedInteractives.insert(eRef);
                            }

                        }
                    }
                }
                
            }

            // reset drag mode when mouse is release or shortcut key is released
            if( io.MouseReleased[0] || mShortcutKeyDown != shortcutKeyDown ) {
                if( mEntityManager.draggingInteractive != NULL ) {
                    mEntityManager.draggingInteractive->stateFlags &= ~(Entity::State::DRAG|Entity::State::SOURCE);
                }
                if( mEntityManager.activeInteractive != NULL ) {
                    mEntityManager.activeInteractive->stateFlags &= ~(Entity::State::DOWN);
                }
                if( mEntityManager.hotInteractive != NULL ) {
                    mEntityManager.hotInteractive->stateFlags &= ~(Entity::State::TARGET);

                }
                
                if( mEntityManager.draggingInteractive != NULL && mEntityManager.hotInteractive != NULL && shortcutKeyDown ) {
                    
                    // check if hot and dragging interactive are Entities
                    if(mEntityManager.draggingInteractive->isOfType(Interactive::Type::ENTITY) &&
                       mEntityManager.hotInteractive->isOfType(Interactive::Type::ENTITY) ) {
                        const EntityRef draggingEntity = TO_ENTITY(mEntityManager.draggingInteractive);
                        const EntityRef hotEntity = TO_ENTITY(mEntityManager.hotInteractive);
                        
                        // we have a connection, add connection object to data structs
                        // connections have a direction. there can be multiple connections out
                        // but only one connection in. Only add a connection if entities say
                        // its ok
                        if( draggingEntity->providesOutputTo(hotEntity) && hotEntity->acceptsInputFrom(draggingEntity) ) {
                            ofLogVerbose(__FUNCTION__) << "connection from " << draggingEntity << " to " << hotEntity;
                            mEntityManager.createConnector(draggingEntity, hotEntity);
                        }
                    }
                    
                    
                }
                
                mEntityManager.activeInteractive = mEntityManager.draggingInteractive = NULL;
                
                // reset selection rectangle when mouse is released
                mMouseSelectionActive = !io.MouseReleased[0];
            }
            
            if( ImGui::IsMouseDragging() ) {
                
                //EntityRef draggingEntity= NULL;
                //EntityRef hotEntity = NULL;
                //EntityRef activeEntity = NULL;
                //AOERef aoe = NULL;
                
                /*
                if(mEntityManager.draggingInteractive != NULL ) {
                   if( mEntityManager.draggingInteractive->isOfType(Interactive::Type::ENTITY) ) {
                       draggingEntity = TO_ENTITY(mEntityManager.draggingInteractive);
                   } else if( mEntityManager.draggingInteractive->isOfType(Interactive::Type::AOE) ) {
                       aoe = TO_AOE(mEntityManager.draggingInteractive);
                   }
                }
                
                if(mEntityManager.hotInteractive != NULL &&
                   mEntityManager.hotInteractive->isOfType(Interactive::Type::ENTITY) ) {
                    hotEntity = TO_ENTITY(mEntityManager.hotInteractive);
                }
                
                if(mEntityManager.activeInteractive != NULL &&
                   mEntityManager.activeInteractive->isOfType(Interactive::Type::ENTITY) ) {
                    activeEntity = TO_ENTITY(mEntityManager.activeInteractive);
                }
                 */
                
                if( mEntityManager.activeInteractive != NULL && mEntityManager.draggingInteractive == NULL ) {
                    // mark and set dragging object, if its not a connector
                    if( !mEntityManager.activeInteractive->isOfType(Interactive::Type::CONNECTOR) ) {
                        
                        mEntityManager.draggingInteractive = mEntityManager.activeInteractive;
                        mEntityManager.draggingInteractive->stateFlags |= Entity::State::DRAG;

                        if( mEntityManager.activeInteractive->isOfType(Interactive::Type::ENTITY) ) {
                            const EntityRef activeEntity = TO_ENTITY(mEntityManager.activeInteractive);
                            if( shortcutKeyDown ) {
                                if( activeEntity->providesOutput() ) {
                                    mEntityManager.draggingInteractive->stateFlags |= Entity::State::SOURCE;
                                }
                            }
                        }
                    }
                }

                
                if( mEntityManager.draggingInteractive != NULL ) {
                    
                    // drag connection to another entity
                    if( mEntityManager.draggingInteractive->isOfType(Interactive::Type::ENTITY) ) {
                        const EntityRef draggingEntity = TO_ENTITY(mEntityManager.draggingInteractive);

                        if( shortcutKeyDown ) {
                            
                            const EntityRef hotEntity = TO_ENTITY(mEntityManager.hotInteractive);
                            
                            if( hotEntity != NULL &&
                               hotEntity->acceptsInputFrom(draggingEntity) &&
                               draggingEntity->providesOutputTo(hotEntity) ) {
                                // ... to center of target
                                
                                Connector::draw((ofPoint)Entity::getDrawPosition(draggingEntity->getPosition(), relativeOffset,mEntityAreaScale),
                                                (ofPoint)Entity::getDrawPosition(hotEntity->getPosition(), relativeOffset,mEntityAreaScale),
                                                mEntityAreaScale,
                                                20.f, 20.f,
                                                Interactive::State::GHOST );
                                
                            } else {
                                // ... to mouse pointer
                                if( draggingEntity->providesOutput() && draggingEntity != hotEntity ) {
                                    Connector::draw((ofPoint)Entity::getDrawPosition(draggingEntity->getPosition(), relativeOffset,mEntityAreaScale),
                                                    (ofPoint)io.MousePos,
                                                    mEntityAreaScale,
                                                    20.f, 0.f,
                                                    Interactive::State::GHOST );
                                    
                                }
                                
                            }
                        } else {

                            if( mEntityManager.isInSelection(draggingEntity) ) {
                                // drag entity around
                                // move the whole selection
                                for( SelectionIterator iter = mEntityManager.selectedInteractives.begin(); iter != mEntityManager.selectedInteractives.end(); ++iter ) {
                                    // only move entities, not connectors
                                    if( (*iter)->isOfType(Interactive::Type::CONNECTOR) )
                                        continue;
                                    const EntityRef selectedEntity = TO_ENTITY((*iter));
                                    
                                    if( selectedEntity != NULL ) {
                                        selectedEntity->move(
                                                             io.MouseDelta.x/mEntityAreaScale,
                                                             io.MouseDelta.y/mEntityAreaScale
                                                             );
                                        
                                    }
                                }
                                
                            } else {
                                draggingEntity->move(
                                                     io.MouseDelta.x/mEntityAreaScale,
                                                     io.MouseDelta.y/mEntityAreaScale
                                                     );
                            }
                        }
                    } else if( mEntityManager.draggingInteractive->isOfType(Interactive::Type::AOE) ) {
                        const AOERef aoe = TO_AOE(mEntityManager.draggingInteractive);
                        aoe->move( io.MouseDelta.x/mEntityAreaScale, io.MouseDelta.y/mEntityAreaScale );
                    }
                } else if( shortcutKeyDown ) {
                    ImGui::SetMouseCursor(2);
                    // drag view around with shortcut mouse drag
                    // TODO: Limit to bounds of EntityManager
                    mEntityAreaViewRect.position.x += io.MouseDelta.x;
                    mEntityAreaViewRect.position.y += io.MouseDelta.y;
                } else {
                    if( mMouseSelectionActive ) {
                        // draw rectangle for mouse selection and select interactives inside it.
                        ofRectangle drawRect = ofRectangle((mMouseSelectionStartPoint*mEntityAreaScale +
                                                        mEntityAreaViewRect.position +
                                                        (ofPoint)cPos),
                                                       (ofPoint)io.MousePos);
                        
                        ofRectangle rect = ofRectangle(mMouseSelectionStartPoint,
                                                       ((ofPoint)io.MousePos - (ofPoint)cPos - mEntityAreaViewRect.position )/mEntityAreaScale);

                        ImGui::GetWindowDrawList()->AddRectFilled((ImVec2)drawRect.getMin(), (ImVec2)drawRect.getMax(), 0x11ffffff);

                        InteractiveList* const list = mEntityManager.getInteractives();
                        
                        // TODO: selection based on shape, not on bounds
                        for( InteractiveListIterator iter = list->begin(); iter != list->end(); ++iter ) {
                            // do not select connectors
                            if( (*iter)->isOfType(Interactive::Type::CONNECTOR ) )
                                continue;
                            if( (*iter)->getBounds()->intersects(rect) ) {
                                (*iter)->stateFlags |= Entity::State::SELECT;
                                mEntityManager.selectedInteractives.insert(*iter);
                            } else {
                                mEntityManager.deselectInteractive(*iter);
                            }
                        }
                        
                        
                    } else {
                        
                        mMouseSelectionStartPoint.set((io.MousePos.x - cPos.x - mEntityAreaViewRect.position.x)/mEntityAreaScale,
                                                      (io.MousePos.y - cPos.y - mEntityAreaViewRect.position.y)/mEntityAreaScale );
                        mMouseSelectionActive = true;
                    }
                    
                }
                
            }
            
            if( io.MouseClicked[0]  && !overAnyInteractive && mEntityManager.selectedInteractives.size() != 0 ) {
                for( SelectionIterator iter = mEntityManager.selectedInteractives.begin(); iter != mEntityManager.selectedInteractives.end(); ++iter ) {
                    (*iter)->stateFlags &= ~Entity::State::SELECT;
                }
                mEntityManager.selectedInteractives.clear();
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

        }
                
        mEntityManager.checkDirtyBounds();
        //mEntityManager.drawBoundingBox(relativeOffset, mEntityAreaScale);
        
        ImGui::EndChild();
        mShortcutKeyDown = shortcutKeyDown;
        
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
    //window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoSavedSettings;
    
    
    ImGui::SetNextWindowSize(ImVec2(398, ofGetHeight()-20));
    ImGui::SetNextWindowPos(ImVec2(ImVec2(ofGetWidth()-408,10)));
    
    if( ImGui::Begin("Inspector", NULL, ImVec2(0.f,0.f), -1.0f, window_flags) ) {
        if( mEntityManager.selectedInteractives.size() > 0 ) {
            for( SelectionIterator iter = mEntityManager.selectedInteractives.begin(); iter != mEntityManager.selectedInteractives.end(); ++iter ) {
                (*iter)->inspectorContent();
            }
        } else {
            ImGui::Text("Nothing selected");
        }
    }
    
    ImGui::End();

}

bool ofApp::GUI_entityMenu() {
    // show menu for creating Entities like
    // light-structures and effects
  
    // consider only space pressed if window is focused and mouse is over it
    bool initialFilterFocus = false;
    bool arrowKeysUsed = false;
    const ofPoint cPos = (ofPoint)ImGui::GetCursorScreenPos();
    
    if( !mEntityMenuIsOpen &&
        (ImGui::IsKeyPressed(' ', false) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter), false)) &&
        ImGui::IsWindowFocused() &&
        ImGui::IsRootWindowOrAnyChildHovered() ) {
        
        ImGui::OpenPopup("entity context menu");
        ImVec2 mPos = ImGui::GetIO().MousePos;
        ImGui::SetNextWindowPos(ImVec2( mPos.x-20.0, mPos.y-20.0 ));
        mEntityMenuSelectedOption = 0;
        initialFilterFocus = true;
    }
    
    if (ImGui::BeginPopup("entity context menu"))
    {
        std::vector<EntityMenuListEntry> filteredLines;
        
        for( std::vector<EntityMenuListEntry>::iterator iter=mEntityMenuEntries.begin(); iter!=mEntityMenuEntries.end(); ++iter ) {
            if( mEntityMenuFilter.PassFilter(iter->label.c_str()) ) {
                filteredLines.push_back(*iter);
            }
        }

        const int optionCount = filteredLines.size();

        if (ImGui::IsWindowFocused() && mEntityMenuIsOpen ) {
            bool closeMenu = false;
            
            if( ImGui::IsKeyPressed( ImGui::GetKeyIndex(ImGuiKey_Escape)) ||
               (ImGui::IsKeyPressed(' ', false) && !mEntityMenuFilter.IsActive())) {
                closeMenu=true;
            } else if( ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter), false) ){
                mEntityMenuCreate = mEntityMenuSelectedOption;
            } else if( mEntityMenuCreate != -1 ) {
                ofLogVerbose( __FUNCTION__ ) << "selecting: " << mEntityMenuCreate;
                
                Interactive::Type entityToCreate = filteredLines[mEntityMenuCreate].type;
                
                ofPoint position = ((ofPoint)ImGui::GetIO().MousePos - cPos - mEntityAreaViewRect.position)/mEntityAreaScale;
                
                mEntityManager.createEntity(entityToCreate, position);
                
                mEntityMenuCreate = -1;
                closeMenu=true;
            } else if( ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow)) ){
                mEntityMenuSelectedOption--;
                arrowKeysUsed = true;
            } else if( ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow)) ){
                mEntityMenuSelectedOption++;
                arrowKeysUsed = true;
            }
            
            if( closeMenu ) {
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
                mEntityMenuIsOpen = false;
                if( mEntityMenuFilter.InputBuf[0] != 0 ) {
                    mEntityMenuFilter.Clear();
                }
                return false;
            }
        }
        
        if( mEntityMenuSelectedOption >= optionCount ) {
            mEntityMenuSelectedOption=0;
        } else if( mEntityMenuSelectedOption < 0 ) {
            mEntityMenuSelectedOption=optionCount-1;
        }
        
        
        mEntityMenuIsOpen = true;

        ImGui::CaptureKeyboardFromApp();
        
        // sets focus to filter either when mouse was release over other element
        // or menu was opened. This ensures, that the focus is always
        // set where it needs to be
        
        
        ImGui::Text("CREATE");
        
        if( initialFilterFocus || ImGui::GetIO().MouseReleased[0] ) {
            ImGui::SetKeyboardFocusHere();
        }
        
        mEntityMenuFilter.Draw("");
        
        ImGui::BeginChild("child", ImVec2(208.0f, 75.0f), false);
        ImGui::PushItemWidth(208);

        for (int line = 0; line < optionCount; line++) {
            const bool selected = line==mEntityMenuSelectedOption;
            if( selected && arrowKeysUsed ) {
                ImGui::SetScrollHere(0.5f);
            }

            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.f,1.f,1.f,0.2f));
            if( ImGui::Selectable(filteredLines[line].label.c_str(), selected ) ) {
                mEntityMenuCreate = line;
            }
            ImGui::PopStyleColor(1);
        }
        
        ImGui::PopItemWidth();
        ImGui::EndChild();
        ImGui::EndPopup();
        return true;
    } else {
        mEntityMenuIsOpen = false;
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
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
