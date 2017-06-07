
#include "blendModes.h"
#include "noise.h"

#define pi 3.141592

static float4 cpos(float t, float p){
    return (float4)(cos(t)*cos(p),cos(t)*sin(p),sin(t), 1.0);
}

static float cos2(float t,float s){
    float co = cos(t);
    return pow(co*co,s);
}

static float lenp(float4 dir){
    float led = length(dir);
    float ang = asin(led/2.0)*2.0;
    return ang;
}

__kernel void generator(read_only image3d_t input, // float
                        write_only image3d_t output, // float
                        const int blendMode,
                        const float blendOpacity,
                        const float time,
                        const float speed,
                        /* custom params: see package.json */
                        const float4 resolution ) {
    
    /* do not change */
    const int4 outputDim = get_image_dim (output);
    const int4 outputCoords = (int4)( get_global_id(0), get_global_id(1), get_global_id(2), 0 );
    const int4 inputDim  = get_image_dim (input);
    const float4 inputCoords = (float4)((float)outputCoords.x*(float)inputDim.x/(float)outputDim.x,
                                        (float)outputCoords.y*(float)inputDim.y/(float)outputDim.y,
                                        (float)outputCoords.z*(float)inputDim.z/(float)outputDim.z,
                                        0.0 );
    float4 inputPixel = read_imagef (input, CLK_NORMALIZED_COORDS_FALSE|CLK_FILTER_NEAREST|CLK_ADDRESS_NONE, inputCoords );
    /* end do not change */

    /* *************************** */
    /* generate pixel colors here: */
    
    float4 ocn = (float4)(
                          ((float)outputCoords.x)/(((float)outputDim.x)),
                          ((float)outputCoords.y)/(((float)outputDim.y)),
                          ((float)outputCoords.z)/(((float)outputDim.z)),1.0);
    
    float4 p = ocn/resolution;
    
    //p.x*= resolution.x / resolution.y;
    //p.y*= resolution.y / resolution.z; // new
    //p = (p-0.5f)*2.0f;
    
    float4 outputPixel = float4(0.0f);
    //outputPixel.r = cos2(p.x*pi*2.0f,20.0f)*0.5f;
    //outputPixel.g = cos2(p.y*pi*2.0f,20.0f)*0.5f;

    float4 org = cpos(time,time*0.887f);
    //float4 now = cpos((p.y+1.0f)/2.0f*pi,p.x*pi/2.0f);
    float dist = lenp(p-org);
    outputPixel.b = cos2(dist*100.0f,5.0f)/(dist);
    /* *************************** */
    
    /* do not change */
    write_imagef( output, outputCoords, blend4_with_opacity( blendMode, inputPixel, outputPixel, clamp( blendOpacity, 0.0f, 1.0f ) ) );
    /* end do not change */

}




