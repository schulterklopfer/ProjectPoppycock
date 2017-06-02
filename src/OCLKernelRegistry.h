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

#include <map>

class OCLKernelRegistry : public ofxSingleton::UnmanagedSingleton<OCLKernelRegistry> {

private:
    msa::OpenCL	mOpenCL;
    OCLKernelWrapperList mKernels;
    std::vector<const char*> mKernelNames;
    
    msa::OpenCLKernelPtr mApplyPreviewColorKernel;
    msa::OpenCLKernelPtr mBlendToObserverKernel;
    
    std::unordered_map<int, msa::OpenCLKernelPtr> mBlendKernels;
    
public:
    OCLKernelRegistry();
    ~OCLKernelRegistry();
    void setup();
    void setupFromDirectory( const string directory );
    void setupCommonKernels( const string file );
    OCLKernelWrapperList& getKernels();
    std::vector<const char*>& getKernelNames();
    
    msa::OpenCLKernelPtr& getApplyPreviewColorKernel();
    msa::OpenCLKernelPtr& getBlendKernel( int inputs );
    
};

typedef std::shared_ptr<OCLKernelRegistry> OCLKernelRegistryRef;

#define KernelRegistryInstance (OCLKernelRegistry::getInstance())

#endif /* OCLKernelRegistry_h */
