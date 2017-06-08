//
//  Observer.hpp
//  PoppyCock
//
//  Created by jash on 01.05.17.
//
//

#ifndef Observer_h
#define Observer_h

#include <stdio.h>
#include "Entity.h"
#include "GPUEntity.h"
#include "MSAOpenCL.h"

class Observer: public Entity, public GPUEntity {
    
private:
    
    void setupPreview();
    void drawPreview();

    msa::OpenCLKernelPtr mBlendKernel;
    vector<float> mBlendValues;
    
    ofVboMesh mPreviewMesh;
    msa::OpenCLBufferManagedT<float3> mPreviewPositions;
    msa::OpenCLBufferManagedT<float4> mPreviewColors;
    
    ofShader mPreviewShader;
    
    int mInputCount;

    
public:
    static const int sPreviewDivisions = 30;
    static const int sPreviewVertexCount = sPreviewDivisions*sPreviewDivisions*sPreviewDivisions;

    Observer( const ImVec2 position );
    
    virtual void draw( const ImVec2 offset, const float scale );
    virtual int getTypeFlags();
    virtual bool acceptsInputFrom(const boost::shared_ptr<Entity> &source );
    virtual bool providesOutputTo(const boost::shared_ptr<Entity> &source );
    virtual bool providesOutput();

    virtual void update();
    virtual void inspectorContent();
    
    virtual void setPosition( const ImVec2 pos );
    virtual void move( const float x, const float y );
    virtual void serialize( Json::Value* outJSON);
    virtual void deserialize( Json::Value* inJSON );

    void recalcBlendValues();

};
#endif /* Observer_h */
