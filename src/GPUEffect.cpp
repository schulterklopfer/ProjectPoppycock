//
//  GPUEffect.cpp
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#include "GPUEffect.h"
#include "Globals.h"

GPUEffect::GPUEffect( ImVec2 position ) : Effect(position), mSizeX(10), mSizeY(10), mSizeZ(10), mSpeed(1.0) {
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
    
    mDebugDrawFrameBuffer.allocate(fboSettings);
    
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
    
    if( mMaxEdgeDistanceFromObserver == -1 ) return;
    
    float pixels[4*mSizeX*mSizeY*mSizeZ];
    
    mImage->read(&pixels );
    
    mDebugDrawFrameBuffer.begin();
    
    ofBackground(0, 0, 0);
    

    const float maxSize = 150.f;

    const float size = min( min(maxSize/(float)(mSizeX+1),maxSize/(float)(mSizeY+1)), maxSize/(float)(mSizeZ+1) );
    
    float movementSpeed = .1;
    float t = (ofGetElapsedTimef()) * movementSpeed;

    
    const ofVec3f offset = ofVec3f( mSizeX-1, mSizeY-1, mSizeZ-1 )*-size;
    
    float r,g,b;
    int index;
    
    mCam.begin();
    
    for( int z=0; z<mSizeZ; z++ ) {
        for( int y=0; y<mSizeY; y++ ) {
            for( int x=0; x<mSizeX; x++ ) {
                index = (mSizeX*mSizeY*z + mSizeX*y + x)*4;
                r = pixels[index+0];
                g = pixels[index+1];
                b = pixels[index+2];
                
                ofPushMatrix();
                
                ofNoFill();
                ofSetColor(r*0xff,g*0xff,b*0xff);
                ofDrawBox(ofVec3f(x,y,z)*2.f*size + offset,size*0.5f);
                ofPopMatrix();

            }
        }
    }
    
    mCam.end();
    
    mDebugDrawFrameBuffer.end();
}

