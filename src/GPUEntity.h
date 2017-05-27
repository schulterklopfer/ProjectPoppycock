//
//  GPUEntity.hpp
//  PoppyCock
//
//  Created by jash on 25.05.17.
//
//

#ifndef GPUEntity_h
#define GPUEntity_h

#include <stdio.h>
#include "Effect.h"
#include "MSAOpenCL.h"
#include "ImEasyCam.h"
#include "OCLKernelRegistry.h"
#include "boost/shared_ptr.hpp"

#define GPU_ENTITY_MIN_X 1
#define GPU_ENTITY_MIN_Y 1
#define GPU_ENTITY_MIN_Z 1

#define GPU_ENTITY_MAX_X 20
#define GPU_ENTITY_MAX_Y 20
#define GPU_ENTITY_MAX_Z 20

//typedef boost::shared_ptr<msa::OpenCLBufferManagedT<int>> BufferRef;
typedef boost::shared_ptr<msa::OpenCLImage> ImageRef;

class GPUEntity {
    
    typedef struct{
        float4 pos;
        float4 col;
    } PreviewVertex;
    
private:
    
protected:
    
    static msa::OpenCLKernelPtr sApplyPreviewColorKernel;
    
    virtual void setupImages();
    
    ImageRef mImage;
    
    ofFbo mPreviewFrameBuffer;
    ImEasyCam mCam;
    
    int mSizeX;
    int mSizeY;
    int mSizeZ;
    
public:
    
    GPUEntity();
    ~GPUEntity();
    
    virtual ImageRef& getImage();
    
    
};

#endif /* GPUEntity_h */
