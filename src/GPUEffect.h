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
#include "GPUEntity.h"
#include "MSAOpenCL.h"
#include "ImEasyCam.h"
#include "OCLKernelRegistry.h"
#include "boost/shared_ptr.hpp"

#define GPU_EFFECT_MIN_SPEED 0.01
#define GPU_EFFECT_MAX_SPEED 100.0

//typedef boost::shared_ptr<msa::OpenCLBufferManagedT<int>> BufferRef;
typedef boost::shared_ptr<msa::OpenCLImage> ImageRef;

class GPUEffect: public Effect, public GPUEntity {

    typedef struct{
        float4 pos;
        float4 col;
    } PreviewVertex;
    
private:

protected:

    virtual void setupImages();

    static ofVboMesh sPreviewMesh;
    static msa::OpenCLBufferManagedT<float3> sPreviewPositions;
    static msa::OpenCLBufferManagedT<float4> sPreviewColors;
    static ofShader sPreviewShader;

    
    OCLKernelWrapperRef mKernelWrapper;
    OCLKernelWrapperParamList mKernelWrapperParams;
    
    ImageRef mEmptyInputImage;
    
    float mSpeed;
    int mKernelIndex;
            
public:
    
    static const int sPreviewDivisions = 30;
    static const int sPreviewVertexCount = sPreviewDivisions*sPreviewDivisions*sPreviewDivisions;
    
    GPUEffect( const ImVec2 position );
    ~GPUEffect();
    static void setupPreview();
    void drawPreview();

    virtual int getTypeFlags();
    virtual void inspectorContent();
    virtual void update();
    virtual void draw( const ImVec2 offset, const float scale );
    virtual void serialize( Json::Value* outJSON);
    virtual void deserialize( Json::Value* inJSON );


};

typedef boost::shared_ptr<GPUEffect> GPUEffectRef;

#endif /* GPUEffect_h */
