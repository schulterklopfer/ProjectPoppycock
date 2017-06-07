
#include "blendModes.h"
#include "noise.h"

static float wave1( float x, float z, float time ) {
    return sin( x * z * 5.f + time * 10.f);
}

static float wave2( float x, float z, float time ) {
    return sin( x * z * 5.f + time );
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
    
    
    float4 p1 = p * 2.f + (float4)(0.f,1.f,0.f,0.f);
    float4 p2 = p * 2.f - (float4)(0.f,1.f,0.f,0.f);
    
    float w1 = wave1(p.x,p.z,time)*.25f;
    float w2 = wave2(p.x,p.z,time)*.25f;
    
    float d = clamp( sin(time*.5f) * 2.f, 0.f, 1.f);
    
    float l1 = mix( w1,w1+w2,1.f-d) - p.y - .5f * d;
    float l2 = mix( w2,w2+w1,1.f-d) - p.y + .5f * d;
    float red = smoothstep(.02f,.0f, fabs(w1 - p.y - .5f * d));
    float blu = smoothstep(.02f,.0f, fabs(w2 - p.y + .5f * d));
    
    float black = 1.f-smoothstep( 0.f, .02f, min(fabs(l1),fabs(l2)));
    float4 outputPixel =  (float4)( red, 0.f, blu, 1.f ) + black;
    
    /* *************************** */
    
    /* do not change */
    write_imagef( output, outputCoords, blend4_with_opacity( blendMode, inputPixel, outputPixel, clamp( blendOpacity, 0.0f, 1.0f ) ) );
    /* end do not change */

}




