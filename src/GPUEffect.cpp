//
//  GPUEffect.cpp
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#include "GPUEffect.h"
#include "Globals.h"

GPUEffect::GPUEffect( ImVec2 position ) : Effect(position) {
    // for testing, use first kernel wrapper
    
    // TODO: make this configurable:
    mKernelWrapper = KernelRegistryInstance->getKernels().at(0);
    
    mSizeX = mSizeY = mSizeZ = 2+(int)ofRandom(3);
    mTotalSize = mSizeX*mSizeY*mSizeZ;
    
    mImage.initWithoutTexture(mSizeX,
                              mSizeY,
                              mSizeZ, // if 1, then we have a 2d image
                              CL_RGBA, // default
                              CL_FLOAT, // default
                              CL_MEM_READ_WRITE, // default
                              NULL, // default
                              CL_FALSE);  // default
 
    
    ofFbo::Settings fboSettings;
    fboSettings.width = 380;
    fboSettings.height = 380;
    fboSettings.internalformat = GL_RGBA;
    fboSettings.textureTarget = GL_TEXTURE_2D;
    fboSettings.numSamples = 4;
    
    mDebugDrawFrameBuffer.allocate(fboSettings);
    
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
            mKernelWrapper->getKernel()->setArg(0, *(eRef->getOpenCLImage()) );
        }
    }

    mKernelWrapper->getKernel()->setArg(1, mImage );
    mKernelWrapper->getKernel()->setArg(2, Globals::getElapsedTimef() );
    mKernelWrapper->getKernel()->setArg(3, 1.0f );
    
    mKernelWrapper->getKernel()->run3D( mSizeX, mSizeY, mSizeZ );

}

int GPUEffect::getTypeFlags() {
    return Interactive::Type::GPU_EFFECT|Interactive::Type::EFFECT|Interactive::Type::ENTITY;
}


void GPUEffect::inspectorContent() {
    ImGui::Text("Inspector content");
    
    vector<OCLKernelWrapper::Param> params = mKernelWrapper->getParams();
    
    for( vector<OCLKernelWrapper::Param>::iterator iter = params.begin(); iter != params.end(); ++iter ) {
        ImGui::Text( "Parameter: %s", (*iter).name.c_str() );
    }
    
    ImGui::Text("Max distance from observer: %d", mMaxEdgeDistanceFromObserver );

    ImGui::Image( (ImTextureID)(uintptr_t)mDebugDrawFrameBuffer.getTexture().getTextureData().textureID , ImVec2(380,380) );
}

/*
msa::OpenCLBufferManagedT<int>* GPUEffect::getBuffer() {
    return &mBuffer;
}
*/

msa::OpenCLImage* GPUEffect::getOpenCLImage() {
    return &mImage;
}

void GPUEffect::debugDraw() {
    
    if( mMaxEdgeDistanceFromObserver == -1 ) return;
    
    float pixels[4*mSizeX*mSizeY*mSizeZ];
    
    mImage.read(&pixels );
    
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
                
                //ofNoFill();
                ofSetColor(r*0xff,g*0xff,b*0xff);
                ofDrawBox(ofVec3f(x,y,z)*2.f*size + offset,size*0.5f);
                ofPopMatrix();

            }
        }
    }
    
    mCam.end();
    
    mDebugDrawFrameBuffer.end();
}

