//
//  GPUEffect.h
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#ifndef GPUEffect_h
#define GPUEffect_h

#include <stdio.h>
#include "Effect.h"
#include "MSAOpenCL.h"
#include "OCLKernelRegistry.h"
#include "boost/shared_ptr.hpp"

#define GPU_EFFECT_MIN_X 1
#define GPU_EFFECT_MIN_Y 1
#define GPU_EFFECT_MIN_Z 1

#define GPU_EFFECT_MAX_X 20
#define GPU_EFFECT_MAX_Y 20
#define GPU_EFFECT_MAX_Z 20

#define GPU_EFFECT_MIN_SPEED 0.01
#define GPU_EFFECT_MAX_SPEED 100.0

//typedef boost::shared_ptr<msa::OpenCLBufferManagedT<int>> BufferRef;
typedef boost::shared_ptr<msa::OpenCLImage> ImageRef;

class GPUEffect: public Effect {

protected:
    
    void setupImages();
    
    OCLKernelWrapperRef mKernelWrapper;
    OCLKernelWrapperParamList mKernelWrapperParams;
    
    ImageRef mEmptyInputImage;
    ImageRef mImage;
        
    ofFbo mDebugDrawFrameBuffer;
    ofEasyCam mCam;
    
    
    int mSizeX;
    int mSizeY;
    int mSizeZ;
    float mSpeed;
            
public:
    
    GPUEffect( const ImVec2 position );
    virtual int getTypeFlags();
    virtual void inspectorContent();
    virtual void update();

    virtual ImageRef& getImage();
    
    void debugDraw();
    
};

typedef boost::shared_ptr<GPUEffect> GPUEffectRef;

#endif /* GPUEffect_h */
