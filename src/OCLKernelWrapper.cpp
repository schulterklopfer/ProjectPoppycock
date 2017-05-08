//
//  OCLKernelWrapper.cpp
//  PoppyCock
//
//  Created by jash on 08.05.17.
//
//

#include "OCLKernelWrapper.h"

const string OCLKernelWrapper::getName() {
    return mName;
}

const string OCLKernelWrapper::getDescription() {
    return mDescription;
}

const string OCLKernelWrapper::getVersion() {
    return mVersion;
}

const OCLKernelWrapper::KernelType OCLKernelWrapper::getType() {
    return mType;
}

const msa::OpenCLKernelPtr& OCLKernelWrapper::getKernel() {
    return mKernel;
}

const vector<OCLKernelWrapper::Param>& OCLKernelWrapper::getParams() {
    return mParams;
}
