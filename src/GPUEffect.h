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

class GPUEffect: public Effect {

protected:
    
    OCLKernelWrapperRef mKernelWrapper;
    msa::OpenCLBufferManagedT<int> mInputBuffer;
    msa::OpenCLBufferManagedT<int> mOutputBuffer;
    
    int mSizeX;
    int mSizeY;
    int mSizeZ;
    
    int mTotalSize;
    
    size_t __sizes__[1];
    
public:
    
    GPUEffect( const ImVec2 position );
    virtual int getTypeFlags();
    virtual void inspectorContent();
    virtual void update();

    virtual msa::OpenCLBufferManagedT<int>* getInputBuffer();
    virtual msa::OpenCLBufferManagedT<int>* getOutputBuffer();
    
};

typedef boost::shared_ptr<GPUEffect> GPUEffectRef;

#endif /* GPUEffect_h */
