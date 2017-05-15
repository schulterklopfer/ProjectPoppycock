//
//  GPUEffect.cpp
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#include "GPUEffect.h"
#include "Globals.h"

GPUEffect::GPUEffect( ImVec2 position ) : Effect(position), mSizeX(2), mSizeY(2), mSizeZ(2), mSpeed(1.0) {
    // for testing, use first kernel wrapper
    
    // TODO: make this configurable:
    mKernelWrapper = KernelRegistryInstance->getKernels().at(0);
    mKernelWrapperParams = mKernelWrapper->getParams(); // create a copy of params for this generator

    
    setupImages();
    
    ofFbo::Settings fboSettings;
    fboSettings.width = 380;
    fboSettings.height = 380;
    fboSettings.internalformat = GL_RGBA;
    fboSettings.textureTarget = GL_TEXTURE_2D;
    fboSettings.useDepth = true;
    
    mDebugDrawFrameBuffer.allocate(fboSettings);
    
    mDebugDrawFrameBuffer.begin();
    ofEnablePointSprites();
    glPointSize(4.0f);
    glLineWidth(4.0f);
    ofEnableDepthTest();
    mDebugDrawFrameBuffer.end();
    
    mSlicerImage = ImageRef(new msa::OpenCLImage());
    
    // jash: no idea why, but i couldnt get sharing to work
    // without setting pixels first. Texture allocation
    // from pixels probably does some magic I dont
    // understand yet
    ofFloatPixels pixels;
    pixels.allocate(128, 128,OF_PIXELS_RGBA);
    mSlicerTex.allocate(pixels);
    mSlicerImage->initFromTexture(mSlicerTex);
 
    
    plane.set(100,100);
    plane.setResolution(PREVIEW_DIVISIONS+1,PREVIEW_DIVISIONS+1);
    plane.mapTexCoordsFromTexture(mSlicerTex);
    
    mSlicerKernel = KernelRegistryInstance->getSlicerKernel();
}

GPUEffect::~GPUEffect() {
    
    if( mEmptyInputImage != NULL ) {
        mEmptyInputImage.reset();
        mEmptyInputImage = NULL;
    }
    
    if( mImage != NULL ) {
        mImage.reset();
        mImage = NULL;
    }
    
    if( mSlicerImage != NULL ) {
        mSlicerImage.reset();
        mSlicerImage = NULL;
    }
    
    if( mSlicerKernel != NULL ) {
        mSlicerKernel.reset();
        mSlicerKernel = NULL;
    }
    
    mSlicerTex.clear();
    
    mDebugDrawFrameBuffer.clear();

}

void GPUEffect::setupImages() {
    // reset empty image buffer, will be regenerated in next update()
    mEmptyInputImage.reset();
    mEmptyInputImage = NULL;
    
    if( mImage != NULL ) {
        mImage.reset();
        mImage = NULL;
    }
    
    mImage = ImageRef( new msa::OpenCLImage() );
    
    mImage->initWithoutTexture(mSizeX,
                               mSizeY,
                               mSizeZ, // if 1, then we have a 2d image
                               CL_RGBA, // default
                               CL_FLOAT, // default
                               CL_MEM_READ_WRITE, // default
                               NULL, // default
                               CL_FALSE);  // default
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
    mKernelWrapper->getKernel()->setArg(2, Globals::getElapsedTimef() );
    mKernelWrapper->getKernel()->setArg(3, mSpeed );
    
    mKernelWrapper->getKernel()->run3D( mSizeX, mSizeY, mSizeZ );

    
}

int GPUEffect::getTypeFlags() {
    return Interactive::Type::GPU_EFFECT|Interactive::Type::EFFECT|Interactive::Type::ENTITY;
}


void GPUEffect::inspectorContent() {
    
    ImGui::PushID(this);
    int index=0;

    if (ImGui::CollapsingHeader("Resolution"))
    {
        
        ImGui::Columns(2);
        ImGui::Separator();
        
        const char* dimFieldNames[3] = { "x", "y", "z" };
        int* dimPtrs[3] = { &mSizeX, &mSizeY, &mSizeZ };
        const int dimMin[3] = { GPU_EFFECT_MIN_X, GPU_EFFECT_MIN_Y, GPU_EFFECT_MIN_Z };
        const int dimMax[3] = { GPU_EFFECT_MAX_X, GPU_EFFECT_MAX_Y, GPU_EFFECT_MAX_Z };
        
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
        ImGui::Separator();

    }
    ImGui::Separator();
    if (ImGui::CollapsingHeader("Generator"))
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
        ImGui::Separator();

    }
    ImGui::Separator();
    if (ImGui::CollapsingHeader("Debug view"))
    {
        
        ImGui::Columns(1);
        ImGui::Image( (ImTextureID)(uintptr_t)mDebugDrawFrameBuffer.getTexture().getTextureData().textureID , ImVec2(375,375) );
        
        
    }
    ImGui::PopID();


}

ImageRef& GPUEffect::getImage() {
    return mImage;
}

void GPUEffect::debugDraw() {
    
    if( mMaxEdgeDistanceFromObserver == -1 ||
       (stateFlags & Interactive::State::SELECT) != Interactive::State::SELECT)
        return;

    const msa::OpenCLKernelPtr slicerKernel = mSlicerKernel;
    
    if( slicerKernel != NULL ) {
        slicerKernel->setArg(0,*(mSlicerImage.get()));
        slicerKernel->setArg(1,*(mImage.get()));
        
        const float halfStep = 0.5f/(float)PREVIEW_DIVISIONS;
        
        mDebugDrawFrameBuffer.begin();
        
        //ofClear(1.0f,1.0f,1.0f);
        ofBackground(0, 0, 0);
        
        mCam.begin();
        
        // draws a lot of double verts but its better than
        // downloading data from the gfx card.
        
        for( int i=0; i<=PREVIEW_DIVISIONS; i++ ) {
            // get center position in every division
            //const float position = (float)i/(float)(PREVIEW_DIVISIONS) + halfStep;
            const float position = (float)i/(float)(PREVIEW_DIVISIONS);
            
            // slice x,y on z
            slicerKernel->setArg(2,0); // axis 0 -> z
            slicerKernel->setArg(3,position);
            slicerKernel->run2D(128, 128);
            
            ofPushMatrix();
            __drawSlice( (1.0f-position) );
            ofPopMatrix();
            
            // slice x,z on y
            slicerKernel->setArg(2,1); // axis 1 -> y
            slicerKernel->setArg(3,position);
            slicerKernel->run2D(128, 128);
            
            ofPushMatrix();
            ofRotateX(-90.0f);
            __drawSlice( position );
            ofPopMatrix();
            
            // slice y,z on x
            slicerKernel->setArg(2,2); // axis 2 -> x
            slicerKernel->setArg(3,position);
            slicerKernel->run2D(128, 128);
            
            ofPushMatrix();
            ofRotateY(90.0f);
            __drawSlice( position );
            ofPopMatrix();
            
        }
        
        ofPushMatrix();
        ofNoFill();
        ofDrawBox(0.0f, 0.0f, 0.0f, 100.0f, 100.0f, 100.0f);
        ofFill();
        ofPopMatrix();
        
        mCam.end();
        
        
        mDebugDrawFrameBuffer.end();
    }
    


}

void GPUEffect::__drawSlice( float position ) {
    plane.setPosition( 0.0f, 0.0f, 100.0f * position - 50.f);
    mSlicerTex.bind();
    plane.drawVertices();
    mSlicerTex.unbind();
}

