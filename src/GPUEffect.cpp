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
    
    mSizeX = mSizeY = mSizeZ = 100;
    mTotalSize = mSizeX*mSizeY*mSizeZ;
    __sizes__[0] = mTotalSize;
    
    mInputBuffer.initBuffer(mTotalSize);
    mOutputBuffer.initBuffer(mTotalSize);
    
}

void GPUEffect::update() {
    mKernelWrapper->getKernel()->setArg(0, mInputBuffer);
    mKernelWrapper->getKernel()->setArg(1, mSizeX);
    mKernelWrapper->getKernel()->setArg(2, mSizeY);
    mKernelWrapper->getKernel()->setArg(3, mSizeZ);
    mKernelWrapper->getKernel()->setArg(4, Globals::getElapsedTimef() );
    mKernelWrapper->getKernel()->setArg(5, 1.0f );
    mKernelWrapper->getKernel()->setArg(6, mOutputBuffer );
    mKernelWrapper->getKernel()->run(1, __sizes__ );
    
    // this is just a test, only use on observer to read from
    // hardware
    //mOutputBuffer.readFromDevice();
    //ofLogVerbose(__FUNCTION__) << mOutputBuffer[3];
    

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

msa::OpenCLBufferManagedT<int>* GPUEffect::getInputBuffer() {
    return &mInputBuffer;
}

msa::OpenCLBufferManagedT<int>* GPUEffect::getOutputBuffer() {
    return &mOutputBuffer;
}

