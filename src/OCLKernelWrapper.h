//
//  OCLKernelWrapper.h
//  PoppyCock
//
//  Created by jash on 08.05.17.
//
//

#ifndef OCLKernelWrapper_h
#define OCLKernelWrapper_h

#include <stdio.h>
#include "MSAOpenCL.h"

class OCLKernelWrapper {

public:
    
    typedef enum KernelType {
        D2,
        D3
    } KernelType;
    
    typedef struct Param {
        string name;
        float value;
        float defaultValue;
        float minValue;
        float maxValue;
    } Param;
    
    OCLKernelWrapper( const msa::OpenCLKernelPtr kernel, const string name, const string description, const string version,
                      const KernelType type, const vector<Param> params ) :
        mKernel(kernel),
        mName(name), mDescription(description), mVersion(version),
        mType(type), mParams(params) {};
    
    const string getName();
    const string getDescription();
    const string getVersion();
    const KernelType getType();
    const msa::OpenCLKernelPtr& getKernel();
    const vector<Param>& getParams();
    
private:
    
    string mName;
    string mVersion;
    string mDescription;
    KernelType mType;
    msa::OpenCLKernelPtr mKernel;
    vector<Param> mParams;

};


typedef boost::shared_ptr<OCLKernelWrapper> OCLKernelWrapperRef;
typedef std::vector<OCLKernelWrapperRef> OCLKernelWrapperList;
typedef OCLKernelWrapperList::iterator OCLKernelWrapperListIterator;


#endif /* OCLKernelWrapper_h */