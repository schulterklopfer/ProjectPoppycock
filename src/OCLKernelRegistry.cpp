//
//  OpenCLKernelRegistry.cpp
//  PoppyCock
//
//  Created by jash on 07.05.17.
//
//

#include "OCLKernelRegistry.h"
#include "ofFileUtils.h"
#include "ofxJSON.h"

OFXSINGLETON_DEFINE_UNMANAGED(OCLKernelRegistry)

OCLKernelRegistry::OCLKernelRegistry() {}

void OCLKernelRegistry::setup() {
    mOpenCL.setupFromOpenGL(1); // TODO: find out correct device number
    setupFromDirectory("opencl/generators");
    setupCommonKernels("opencl/common.cl");

}

void OCLKernelRegistry::setupCommonKernels( const string file ) {
    ofFile common = ofFile( ofToDataPath(file) );
    if( common.isFile() ) {
        mCommonKernels = mOpenCL.loadProgramFromFile(common.path(), false);
    }
}

void OCLKernelRegistry::setupFromDirectory( const string directory ) {
    
    ofDirectory dir(directory);
    if( dir.isDirectory() ) {
        dir.listDir();
        
        //go through and print out all the paths
        for(int i = 0; i < dir.size(); i++){
            
            ofFile subDir(dir.getPath(i));
            
            if( subDir.isDirectory() ) {
                ofFile package = ofFile(subDir.path() + "/package.json" );
                ofFile code = ofFile(subDir.path() + "/main.cl" );
                
                if( package.isFile() && code.isFile() ) {
                    ofxJSONElement JSON;
                    const bool parsingSuccessful = JSON.open(package.path());
                    if( parsingSuccessful ) {
                        
                        const string name = JSON["name"].asString();
                        const string description = JSON["description"].asString();
                        const string type = JSON["type"].asString();
                        const string version = JSON["version"].asString();
                        
                        OCLKernelWrapper::KernelType kType;
                        
                        if ( type == "D3" || type == "d3" ) {
                            kType = OCLKernelWrapper::KernelType::D3;
                        } else {
                            kType = OCLKernelWrapper::KernelType::D2;
                        }
                        
                        std::vector<OCLKernelWrapper::Param> params;
                        
                        for( Json::Value::iterator iter = JSON["params"].begin(); iter != JSON["params"].end(); ++iter ) {
                            const string name = (*iter)["name"].asString();
                            const float defaultValue = (*iter)["default"].asFloat();
                            const float minValue = (*iter)["min"].asFloat();
                            const float maxValue = (*iter)["max"].asFloat();
                            
                            if( name != "" ) {
                                params.push_back({ name, defaultValue, defaultValue, minValue, maxValue });
                            }
                        }
                        
                        const msa::OpenCLProgramPtr programPtr = mOpenCL.loadProgramFromFile(code.path(), false);
                        const msa::OpenCLKernelPtr kernel = programPtr->loadKernel("generator");
                        
                        if( kernel->getCLKernel() != NULL && name != "" ) {
                            // everything ok
                            mKernels.push_back( OCLKernelWrapperRef( new OCLKernelWrapper(kernel,name,description,version,kType,params) ) );
                        }
                        
                    }
                }
            }
        }
    }
}

OCLKernelWrapperList& OCLKernelRegistry::getKernels() {
    return mKernels;
}

msa::OpenCLKernelPtr OCLKernelRegistry::getSlicerKernel() {
    //return mSlicerKernel;
    ofFile common = ofFile( ofToDataPath("opencl/common.cl") );
    if( mCommonKernels != NULL ) {
        return mCommonKernels->loadKernel("slicer"); // slicer for volume rendering
    }
    return NULL;
}
