
#include "blendModes.h"
#include "noise.h"

__kernel void generator(read_only image3d_t input, // float
                        write_only image3d_t output, // float
                        const int blendMode,
                        const float blendOpacity,
                        const float time,
                        const float speed,
                        /* custom params */
                        const float4 bias,
                        const float4 scale,
                        const float lacunarity,
                        const float increment,
                        const float octaves,
                        const float amplitude ) {
    
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
    
    float4 position = (float4)(((float)outputCoords.x + bias.x*time*speed)/ (float)outputDim.x,
                               ((float)outputCoords.y + bias.y*time*speed) / (float)outputDim.x,
                               ((float)outputCoords.z + bias.z*time*speed) / (float)outputDim.x,
                               1.0 );
    
    float value = multifractal3d(position, scale.x, lacunarity, increment, octaves);
    
    float4 outputPixel = (float4)(value, value, value, 1.0f) * amplitude;
    outputPixel.w = 1.0f;
    /* *************************** */
    
    /* do not change */
    write_imagef( output, outputCoords, blend4_with_opacity( blendMode, inputPixel, outputPixel, clamp( blendOpacity, 0.0f, 1.0f ) ) );
    /* end do not change */

}
