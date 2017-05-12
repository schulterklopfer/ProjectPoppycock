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

//typedef boost::shared_ptr<msa::OpenCLBufferManagedT<int>> BufferRef;
typedef boost::shared_ptr<msa::OpenCLImage> ImageRef;

class GPUEffect: public Effect {

protected:
    
    OCLKernelWrapperRef mKernelWrapper;
    ImageRef mEmptyInputImage;
    
    msa::OpenCLBufferManagedT<int> mBuffer;
    msa::OpenCLImage mImage;
    
    ofFbo mDebugDrawFrameBuffer;
    ofEasyCam mCam;
    
    
    int mSizeX;
    int mSizeY;
    int mSizeZ;
    
    int mTotalSize;
        
public:
    
    GPUEffect( const ImVec2 position );
    virtual int getTypeFlags();
    virtual void inspectorContent();
    virtual void update();

    virtual msa::OpenCLImage* getOpenCLImage();
    
    void debugDraw();
    
};

typedef boost::shared_ptr<GPUEffect> GPUEffectRef;

#endif /* GPUEffect_h */
