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
    mOpenCL.setupFromOpenGL(); // TODO: find out correct device number
    setupFromDirectory("opencl/generators");
    setupCommonKernels("opencl/common.cl");

}

void OCLKernelRegistry::setupCommonKernels( const string file ) {
    ofFile common = ofFile( ofToDataPath(file) );
    if( common.isFile() ) {
        msa::OpenCLProgramPtr pr = mOpenCL.loadProgramFromFile(common.path(), false);
        mApplyPreviewColorKernel = pr->loadKernel("applyPreviewColor");
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
                        
                        const msa::OpenCLProgramPtr programPtr = mOpenCL.loadProgramFromFile(code.path(), false, "-I \"../../../data/opencl/include\"");
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


msa::OpenCLKernelPtr& OCLKernelRegistry::getBlendKernel(int inputs) {
    
    std::unordered_map<int, msa::OpenCLKernelPtr>::iterator result = mBlendKernels.find(inputs);
    
    if( result == mBlendKernels.end() ) {
        // not found: create new and insert
        
        string code =
        "__kernel void blend(__read_only int inputCount,\n";
        
        for( int i=0; i<inputs; i++ ) {
            code += "                    __read_only image3d_t input"+std::to_string(i)+",\n";
        }

        for( int i=0; i<inputs; i++ ) {
            code += "                    __read_only float blendAmount"+std::to_string(i)+",\n";
        }
        
        code +=
        "                    __write_only image3d_t output ) {\n"
        "    const int4 outputCoords = (int4)( get_global_id(0), get_global_id(1), get_global_id(2), 0 );\n"
        "    const int4 outputDim = get_image_dim (output);\n"
        "    const float4 inputCoords = (float4)((float)outputCoords.x/(float)outputDim.x,\n"
        "                                        (float)outputCoords.y/(float)outputDim.y,\n"
        "                                        (float)outputCoords.z/(float)outputDim.z,\n"
        "                                        0.0 );\n"
        "    float4 outputPixel = (float4)(0.0,0.0,0.0,0.0);\n";
        /*
        "    float blendAmountSum = 0;\n";
        
        for( int i=0; i<inputs; i++ ) {
            code +=
            "    blendAmountSum+=blendAmount"+std::to_string(i)+";\n";
        }
        */
        
        for( int i=0; i<inputs; i++ ) {
            const string inputIndex = std::to_string(i);
            code +=
            "    float4 inputPixel"+inputIndex+" = read_imagef (input"+inputIndex+", CLK_NORMALIZED_COORDS_TRUE|CLK_FILTER_NEAREST|CLK_ADDRESS_NONE, inputCoords );\n"
            //"    const float f"+inputIndex+" = blendAmount"+inputIndex+" / blendAmountSum;\n"
            "    const float f"+inputIndex+" = blendAmount"+inputIndex+";\n"
            "    outputPixel += (float4)(inputPixel"+inputIndex+".r * f"+inputIndex+", inputPixel"+inputIndex+".g * f"+inputIndex+", inputPixel"+inputIndex+".b * f"+inputIndex+", inputPixel"+inputIndex+".a * f"+inputIndex+" );\n";
        }
        code +=
        //"    outputPixel.r /= inputCount;\n"
        //"    outputPixel.g /= inputCount;\n"
        //"    outputPixel.b /= inputCount;\n"
        //"    outputPixel.a /= inputCount;\n"
        "    write_imagef( output, outputCoords, outputPixel );\n"
        "}\n";
        
        ofLog() << code;
        
        msa::OpenCLProgramPtr p = mOpenCL.loadProgramFromSource(code);
        msa::OpenCLKernelPtr k = p->loadKernel("blend");
        mBlendKernels[inputs] = k;
        return mBlendKernels[inputs];
    } else {
        return result->second;
    }
}

OCLKernelWrapperList& OCLKernelRegistry::getKernels() {
    return mKernels;
}

msa::OpenCLKernelPtr& OCLKernelRegistry::getApplyPreviewColorKernel() {
    return mApplyPreviewColorKernel;
}
