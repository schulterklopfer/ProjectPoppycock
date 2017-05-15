//
//  OpenCLKernelRegistry.h
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#ifndef OCLKernelRegistry_h
#define OCLKernelRegistry_h

#include <stdio.h>
#include "ofxSingleton.h"
#include "MSAOpenCL.h"
#include "OCLKernelWrapper.h"


class OCLKernelRegistry : public ofxSingleton::UnmanagedSingleton<OCLKernelRegistry> {

private:
    msa::OpenCL	mOpenCL;
    OCLKernelWrapperList mKernels;
    
    //msa::OpenCLKernelPtr mSlicerKernel;
    msa::OpenCLProgramPtr mCommonKernels;
    
public:
    OCLKernelRegistry();
    
    void setup();
    void setupFromDirectory( const string directory );
    void setupCommonKernels( const string file );
    OCLKernelWrapperList& getKernels();
    
    msa::OpenCLKernelPtr getSlicerKernel();
    
};

typedef std::shared_ptr<OCLKernelRegistry> OCLKernelRegistryRef;

#define KernelRegistryInstance (OCLKernelRegistry::getInstance())

#endif /* OCLKernelRegistry_h */
