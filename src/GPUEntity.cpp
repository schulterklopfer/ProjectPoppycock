//
//  GPUEntity.cpp
//  PoppyCock
//
//  Created by jash on 25.05.17.
//
//

#include "GPUEntity.h"
#include "Globals.h"

msa::OpenCLKernelPtr GPUEntity::sApplyPreviewColorKernel;

GPUEntity::GPUEntity() : Serializable(), mSizeX(10), mSizeY(10), mSizeZ(10) {
    // for testing, use first kernel wrapper
    
    // TODO: make this configurable:
    
    setupImages();
    
    ofFbo::Settings fboSettings;
    fboSettings.width = 350;
    fboSettings.height = 350;
    fboSettings.internalformat = GL_RGBA;
    fboSettings.textureTarget = GL_TEXTURE_2D;
    fboSettings.useDepth = true;
    
    mPreviewFrameBuffer.allocate(fboSettings);
    
    mPreviewFrameBuffer.begin();
    ofEnableDepthTest();
    mPreviewFrameBuffer.end();
}

GPUEntity::~GPUEntity() {
        
    if( mImage != NULL ) {
        mImage.reset();
        mImage = NULL;
    }
    
    mPreviewFrameBuffer.clear();
    
}

void GPUEntity::setupImages() {
    // reset empty image buffer, will be regenerated in next update()
    
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

ImageRef& GPUEntity::getImage() {
    return mImage;
}

void GPUEntity::serialize( Json::Value* outJSON ) {
    (*outJSON)["size"]["x"] = mSizeX;
    (*outJSON)["size"]["y"] = mSizeY;
    (*outJSON)["size"]["z"] = mSizeZ;
}

void GPUEntity::deserialize( Json::Value* inJSON ) {
    
}

