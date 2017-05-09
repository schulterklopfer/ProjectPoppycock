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
    
    mSizeX = mSizeY = mSizeZ = 1;
    mTotalSize = mSizeX*mSizeY*mSizeZ;
    __sizes__[0] = mTotalSize;
    int data[1] = { (int)ofRandom(100) };

    mBuffer.initBuffer(mTotalSize);
    
}

void GPUEffect::update() {
    
    if( mInputs.size() == 0 ) {
        // no inputs! generate empty input buffer
        if( mEmptyInputBuffer == NULL ) {
            ofLogVerbose(__FUNCTION__) << "creating empty input buffer";
            mEmptyInputBuffer = BufferRef(new msa::OpenCLBufferManagedT<int>() );
            int data[1] = { 1 };
            mEmptyInputBuffer->initBuffer( mTotalSize );
        }
        mKernelWrapper->getKernel()->setArg(0, *(mEmptyInputBuffer.get()) );
    } else {
        // destroy empty input buffer
        if( mEmptyInputBuffer != NULL ) {
            ofLogVerbose(__FUNCTION__) << "removing empty input buffer";
            mEmptyInputBuffer.reset();
            mEmptyInputBuffer = NULL;
        }
        if( mInputs[0]->getSource()->isOfType(Interactive::Type::GPU_EFFECT) ) {
            GPUEffectRef eRef = TO_GPU_EFFECT(mInputs[0]->getSource());
            mKernelWrapper->getKernel()->setArg(0, *(eRef->getBuffer()) );
        }
    }
    
    mKernelWrapper->getKernel()->setArg(1, mSizeX);
    mKernelWrapper->getKernel()->setArg(2, mSizeY);
    mKernelWrapper->getKernel()->setArg(3, mSizeZ);
    mKernelWrapper->getKernel()->setArg(4, Globals::getElapsedTimef() );
    mKernelWrapper->getKernel()->setArg(5, 1.0f );
    mKernelWrapper->getKernel()->setArg(6, mBuffer );
    mKernelWrapper->getKernel()->run(1, __sizes__ );

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

}

msa::OpenCLBufferManagedT<int>* GPUEffect::getBuffer() {
    return &mBuffer;
}

