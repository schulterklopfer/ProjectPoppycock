//
//  Observer.cpp
//  PoppyCock
//
//  Created by jash on 01.05.17.
//
//

#include "Observer.h"
#include "GPUEffect.h"

Observer::Observer( const ImVec2 position ) : Entity(position), mInputCount(0) {
    setupPreview();
};


int Observer::getTypeFlags() {
    return Observer::Type::OBSERVER|Observer::Type::ENTITY;
}

void Observer::update() {
    
    if( mInputs.size() > 0 ) {
        
        
        if( mInputs.size() != mInputCount ) {
            mInputCount = mInputs.size();
            mBlendKernel = KernelRegistryInstance->getBlendKernel(mInputCount);
            recalcBlendValues();
        }
        
        int argIndex=0;

        mBlendKernel->setArg(argIndex++, mInputCount);
        
        for( int i=0 ; i<mInputCount; i++ ) {
            if( mInputs[i]->getSource()->isOfType(Interactive::Type::GPU_EFFECT) ) {
                GPUEffectRef eRef = TO_GPU_EFFECT(mInputs[i]->getSource());
                mBlendKernel->setArg(argIndex++, *(eRef->getImage().get()) );
            }
        }
        
        for( int i=0; i<mInputCount; i++ ) {
            if( mInputs[i]->getSource()->isOfType(Interactive::Type::GPU_EFFECT) ) {
                GPUEffectRef eRef = TO_GPU_EFFECT(mInputs[i]->getSource());
                mBlendKernel->setArg(argIndex++, mBlendValues[i] );
            }
        }
        
        mBlendKernel->setArg(argIndex++, *mImage );
        mBlendKernel->run3D( mSizeX, mSizeY, mSizeZ );

    }
    

}


void Observer::draw( const ImVec2 offset, const float scale ) {
    Entity::draw(offset, scale);
    ImGui::GetWindowDrawList()->AddCircleFilled(getDrawPosition(mPosition, offset, scale), 18*scale, 0xffaaffaa, 64 );
}

void Observer::inspectorContent() {
    
    ImGui::PushID(this);
    if( ImGui::CollapsingHeader("Observer")) {

        if (ImGui::TreeNode("Preview"))
        {
            drawPreview();

            const ImVec2 offset = ImGui::GetCursorScreenPos();
            
            // moves CursorScreenPos so we grabbed it before drawing the image
            ImGui::Image( (ImTextureID)(uintptr_t)mPreviewFrameBuffer.getTexture().getTextureData().textureID , ImVec2(350,350) );
            mCam.update( offset );
            ImGui::TreePop();
            
        }
        
    }
    
    
    ImGui::PopID();

}

void Observer::setupPreview() {
    
    mPreviewMesh.clear();
    mPreviewMesh.enableColors();
    mPreviewMesh.disableNormals();
    mPreviewMesh.disableIndices();
    mPreviewMesh.ofMesh::disableTextures();
    
    mPreviewMesh.setMode(OF_PRIMITIVE_POINTS);
    
    for( int z = 0; z<sPreviewDivisions; z++ ) {
        for( int y = 0; y<sPreviewDivisions; y++ ) {
            for( int x = 0; x<sPreviewDivisions; x++ ) {
                const int index = sPreviewDivisions*sPreviewDivisions*z +
                sPreviewDivisions*y +
                x;
                mPreviewMesh.addVertex( ofVec3f((float)x/(float)(sPreviewDivisions-1),
                                                           (float)y/(float)(sPreviewDivisions-1),
                                                           (float)z/(float)(sPreviewDivisions-1) ) );
                
                mPreviewMesh.addColor( ofFloatColor((float)x/(float)(sPreviewDivisions-1),
                                                               (float)y/(float)(sPreviewDivisions-1),
                                                               (float)z/(float)(sPreviewDivisions-1),
                                                               1.0) );
            }
        }
    }
    
    mPreviewPositions.initFromGLObject(mPreviewMesh.getVbo().getVertexBuffer().getId(), sPreviewVertexCount);
    mPreviewColors.initFromGLObject(mPreviewMesh.getVbo().getColorBuffer().getId(), sPreviewVertexCount);
    
    
    
    // set shared vbo as argument
    
    mPreviewShader.load("shaders/effectPreviewVert.glsl", "shaders/effectPreviewFrag.glsl", "shaders/effectPreviewGeom.glsl");
    
}

void Observer::drawPreview() {
    
    /*
    if( (stateFlags & Interactive::State::SELECT) != Interactive::State::SELECT)
        return;
    */
    const msa::OpenCLKernelPtr applyPreviewColorKernel = KernelRegistryInstance->getApplyPreviewColorKernel();
    
    applyPreviewColorKernel->setArg(0, *mImage.get() );
    applyPreviewColorKernel->setArg(1, mPreviewPositions);
    applyPreviewColorKernel->setArg(2, mPreviewColors);
    applyPreviewColorKernel->run1D(sPreviewVertexCount);
    
    const float scale = 100.0f;
    
    mPreviewFrameBuffer.begin();
    
    //ofClear(1.0f,1.0f,1.0f);
    ofBackground(0, 0, 0);
    //ofBackgroundGradient( ofColor( 255 ), ofColor( 128 ), OF_GRADIENT_CIRCULAR );
    
    
    mCam.begin();
    
    Observer::mPreviewShader.begin();
    Observer::mPreviewShader.setUniform1f("time", ofGetElapsedTimef()*0.125 );
    Observer::mPreviewShader.setUniform1f("boxSize", (1.0f/(float)sPreviewDivisions)*0.25 );
    
    ofPushMatrix();
    ofScale(scale, scale, scale);
    ofTranslate(-0.5f,-0.5f,-0.5f);
    Observer::mPreviewMesh.draw();
    ofPopMatrix();
    
    Observer::mPreviewShader.end();
    
    ofPushMatrix();
    ofNoFill();
    ofDrawBox(0.0f, 0.0f, 0.0f, scale, scale, scale);
    ofFill();
    ofPopMatrix();
    
    mCam.end();
    
    mPreviewFrameBuffer.end();
    
}

void Observer::move(const float x, const float y) {
    Entity::move(x,y);
    recalcBlendValues();
}

void Observer::setPosition(const ImVec2 pos) {
    Entity::setPosition(pos);
    recalcBlendValues();
}

void Observer::recalcBlendValues() {
    mBlendValues.resize(mInputCount);
    for( int i=0 ; i<mInputCount; i++ ) {
        GPUEffectRef eRef = TO_GPU_EFFECT(mInputs[i]->getSource());
        
        if(eRef != NULL ) {
            const ImVec2 pos = eRef->getPosition();
            const float radius = eRef->getAOE()->getRadius();
            const ImVec2 delta = ImVec2( pos.x - mPosition.x, pos.y - mPosition.y );
            const float l = 1.0f/Entity::Q_rsqrt(delta.x * delta.x + delta.y * delta.y);
        
            if( radius > 0 ) {
                float bvi = l/radius;
                if( bvi > 1.0 ) bvi = 1.0;
                if( bvi < 0.0 ) bvi = 0.0;
                
                mBlendValues[i] = 1.0f - bvi;
            } else {
                mBlendValues[i] = 0.0f;
            }
        }
    }
    
}

// Observer has no outputs.
bool Observer::providesOutputTo( const EntityRef &target ) {
    return false;
}

bool Observer::providesOutput() {
    return false;
}

// accepts more than one input
bool Observer::acceptsInputFrom( const EntityRef &source ) {
    return this != source.get();
}
