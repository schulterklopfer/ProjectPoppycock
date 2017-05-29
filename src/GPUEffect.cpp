//
//  GPUEffect.cpp
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#include "GPUEffect.h"
#include "Globals.h"

ofVboMesh GPUEffect::sPreviewMesh;
msa::OpenCLBufferManagedT<float3> GPUEffect::sPreviewPositions;
msa::OpenCLBufferManagedT<float4> GPUEffect::sPreviewColors;
ofShader GPUEffect::sPreviewShader;

GPUEffect::GPUEffect( ImVec2 position ) : Effect(position), GPUEntity(), mSpeed(1.0) {
    // for testing, use first kernel wrapper
    // TODO: make this configurable:
    mKernelWrapper = KernelRegistryInstance->getKernels().at(0);
    mKernelWrapperParams = mKernelWrapper->getParams(); // create a copy of params for this generator
}

GPUEffect::~GPUEffect() {
    if( mEmptyInputImage != NULL ) {
        mEmptyInputImage.reset();
        mEmptyInputImage = NULL;
    }
}

void GPUEffect::setupImages() {
    // reset empty image buffer, will be regenerated in next update()
    mEmptyInputImage.reset();
    mEmptyInputImage = NULL;
    GPUEntity::setupImages();
}

void GPUEffect::setupPreview() {
    
    GPUEffect::sPreviewMesh.clear();
    GPUEffect::sPreviewMesh.enableColors();
    GPUEffect::sPreviewMesh.disableNormals();
    GPUEffect::sPreviewMesh.disableIndices();
    GPUEffect::sPreviewMesh.ofMesh::disableTextures();
    
    GPUEffect::sPreviewMesh.setMode(OF_PRIMITIVE_POINTS);
    
    for( int z = 0; z<sPreviewDivisions; z++ ) {
        for( int y = 0; y<sPreviewDivisions; y++ ) {
            for( int x = 0; x<sPreviewDivisions; x++ ) {
                const int index = sPreviewDivisions*sPreviewDivisions*z +
                sPreviewDivisions*y +
                x;
                GPUEffect::sPreviewMesh.addVertex( ofVec3f((float)x/(float)(sPreviewDivisions-1),
                                                           (float)y/(float)(sPreviewDivisions-1),
                                                           (float)z/(float)(sPreviewDivisions-1) ) );
                
                GPUEffect::sPreviewMesh.addColor( ofFloatColor((float)x/(float)(sPreviewDivisions-1),
                                                               (float)y/(float)(sPreviewDivisions-1),
                                                               (float)z/(float)(sPreviewDivisions-1),
                                                               1.0) );
            }
        }
    }
    
    GPUEffect::sPreviewPositions.initFromGLObject(GPUEffect::sPreviewMesh.getVbo().getVertexBuffer().getId(), sPreviewVertexCount);
    GPUEffect::sPreviewColors.initFromGLObject(GPUEffect::sPreviewMesh.getVbo().getColorBuffer().getId(), sPreviewVertexCount);
    
    
    GPUEffect::sApplyPreviewColorKernel = KernelRegistryInstance->getApplyPreviewColorKernel();
    
    GPUEffect::sPreviewShader.load("shaders/effectPreviewVert.glsl", "shaders/effectPreviewFrag.glsl", "shaders/effectPreviewGeom.glsl");
    
}

void GPUEffect::drawPreview() {

    /*
    if( mMaxEdgeDistanceFromObserver == -1 ||
       (stateFlags & Interactive::State::SELECT) != Interactive::State::SELECT)
        return;
     */
    GPUEffect::sApplyPreviewColorKernel->setArg(0, *mImage.get() );
    GPUEffect::sApplyPreviewColorKernel->setArg(1, GPUEffect::sPreviewPositions);
    GPUEffect::sApplyPreviewColorKernel->setArg(2, GPUEffect::sPreviewColors);
    GPUEffect::sApplyPreviewColorKernel->run1D(sPreviewVertexCount);
    
    const float scale = 100.0f;
    
    mPreviewFrameBuffer.begin();
    
    //ofClear(1.0f,1.0f,1.0f);
    ofBackground(0, 0, 0);
    //ofBackgroundGradient( ofColor( 255 ), ofColor( 128 ), OF_GRADIENT_CIRCULAR );
    
    
    mCam.begin();
    
    GPUEffect::sPreviewShader.begin();
    GPUEffect::sPreviewShader.setUniform1f("time", ofGetElapsedTimef()*0.125 );
    GPUEffect::sPreviewShader.setUniform1f("boxSize", (1.0f/(float)sPreviewDivisions)*0.25 );
    
    ofPushMatrix();
    ofScale(scale, scale, scale);
    ofTranslate(-0.5f,-0.5f,-0.5f);
    GPUEffect::sPreviewMesh.draw();
    ofPopMatrix();
    
    GPUEffect::sPreviewShader.end();
    
    ofPushMatrix();
    ofNoFill();
    ofDrawBox(0.0f, 0.0f, 0.0f, scale, scale, scale);
    ofFill();
    ofPopMatrix();
    
    mCam.end();
    
    mPreviewFrameBuffer.end();
    
}

void GPUEffect::update() {
    if( mInputs.size() == 0 ) {
        // no inputs! generate empty input image
        if( mEmptyInputImage == NULL ) {
            ofLogVerbose(__FUNCTION__) << "creating empty image";
            mEmptyInputImage = ImageRef(new msa::OpenCLImage() );
            
            mEmptyInputImage->initWithoutTexture(mSizeX,
                                                 mSizeY,
                                                 mSizeZ, // if 1, then we have a 2d image
                                                 CL_RGBA, // default
                                                 CL_FLOAT, // default
                                                 CL_MEM_READ_WRITE, // default
                                                 NULL, // default
                                                 CL_FALSE);  // default
            
            
        }
        mKernelWrapper->getKernel()->setArg(0, *(mEmptyInputImage.get()) );
    } else {
        // destroy empty input image
        if( mEmptyInputImage != NULL ) {
            ofLogVerbose(__FUNCTION__) << "removing empty image";
            mEmptyInputImage.reset();
            mEmptyInputImage = NULL;
        }
        if( mInputs[0]->getSource()->isOfType(Interactive::Type::GPU_EFFECT) ) {
            GPUEffectRef eRef = TO_GPU_EFFECT(mInputs[0]->getSource());
            mKernelWrapper->getKernel()->setArg(0, *(eRef->getImage().get()) );
        }
    }

    mKernelWrapper->getKernel()->setArg(1, *mImage.get() );
    mKernelWrapper->getKernel()->setArg(2, Connector::BlendMode::ADD );
    mKernelWrapper->getKernel()->setArg(3, 0.5f );
    mKernelWrapper->getKernel()->setArg(4, Globals::getElapsedTimef() );
    mKernelWrapper->getKernel()->setArg(5, mSpeed );
    
    mKernelWrapper->getKernel()->run3D( mSizeX, mSizeY, mSizeZ );

}

int GPUEffect::getTypeFlags() {
    return Interactive::Type::GPU_EFFECT|Interactive::Type::EFFECT|Interactive::Type::ENTITY;
}

void GPUEffect::draw(const ImVec2 offset, const float scale) {
    Effect::draw(offset, scale);
}

void GPUEffect::inspectorContent() {
    
    ImGui::PushID(this);
    if( ImGui::CollapsingHeader("GPU Effect")) {
        int index=0;
        
        if (ImGui::TreeNode("Resolution"))
        {
            
            ImGui::Columns(2);
            ImGui::Separator();
            
            const char* dimFieldNames[3] = { "x", "y", "z" };
            int* dimPtrs[3] = { &mSizeX, &mSizeY, &mSizeZ };
            const int dimMin[3] = { GPU_ENTITY_MIN_X, GPU_ENTITY_MIN_Y, GPU_ENTITY_MIN_Z };
            const int dimMax[3] = { GPU_ENTITY_MAX_X, GPU_ENTITY_MAX_Y, GPU_ENTITY_MAX_Z };
            
            for( int i=0; i<3; i++ ) {
                ImGui::PushID( index++ ); // Use field index as identifier.
                
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::Bullet();
                ImGui::Selectable(dimFieldNames[i]);
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                if( ImGui::DragInt( "##value", dimPtrs[i], 1, dimMin[i], dimMax[i]) ) {
                    ofLogVerbose(__FUNCTION__) << "dimension " << i << " changed";
                    setupImages();
                }
                ImGui::PopItemWidth();
                ImGui::NextColumn();
                
                ImGui::PopID();
            }
            
            
            ImGui::Columns(1);
            ImGui::TreePop();
            
        }

        if (ImGui::TreeNode("Generator"))
        {
            
            ImGui::Columns(2);
            ImGui::Separator();
            
            ImGui::PushID( index++ ); // Use field index as identifier.
            
            ImGui::AlignFirstTextHeightToWidgets();
            ImGui::Bullet();
            ImGui::Selectable("Speed");
            ImGui::NextColumn();
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat("##value", &mSpeed, 0.01f, GPU_EFFECT_MIN_SPEED, GPU_EFFECT_MAX_SPEED);
            ImGui::PopItemWidth();
            ImGui::NextColumn();
            
            ImGui::PopID();
            
            
            for( OCLKernelWrapperParamListIterator iter = mKernelWrapperParams.begin(); iter != mKernelWrapperParams.end(); ++iter ) {
                ImGui::PushID( index++ ); // Use field index as identifier.
                
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::Bullet();
                ImGui::Selectable(iter->name.c_str());
                ImGui::NextColumn();
                ImGui::PushItemWidth(-1);
                ImGui::DragFloat("##value", &(iter->value), 0.1f, iter->minValue, iter->maxValue);
                ImGui::PopItemWidth();
                ImGui::NextColumn();
                
                ImGui::PopID();
            }
            
            ImGui::Columns(1);
            ImGui::TreePop();
            
        }
        if (ImGui::TreeNode("Preview"))
        {
            GPUEffect::drawPreview();
            const ImVec2 offset = ImGui::GetCursorScreenPos();
            
            // moves CursorScreenPos so we grabbed it before drawing the image
            ImGui::Image( (ImTextureID)(uintptr_t)mPreviewFrameBuffer.getTexture().getTextureData().textureID , ImVec2(350,350) );
            mCam.update( offset );
            ImGui::TreePop();
            
        }

    }
    ImGui::PopID();
}

