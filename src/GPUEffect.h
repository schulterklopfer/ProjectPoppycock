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

typedef boost::shared_ptr<msa::OpenCLBufferManagedT<int>> BufferRef;

class GPUEffect: public Effect {

protected:
    
    OCLKernelWrapperRef mKernelWrapper;
    BufferRef mEmptyInputBuffer;
    msa::OpenCLBufferManagedT<int> mBuffer;
    
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

    virtual msa::OpenCLBufferManagedT<int>* getBuffer();
    
};

typedef boost::shared_ptr<GPUEffect> GPUEffectRef;

#endif /* GPUEffect_h */
