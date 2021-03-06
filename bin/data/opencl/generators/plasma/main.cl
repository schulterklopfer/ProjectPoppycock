
#include "blendModes.h"
#include "noise.h"

__kernel void generator(read_only image3d_t input, // float
                        write_only image3d_t output, // float
                        const int blendMode,
                        const float blendOpacity,
                        const float time,
                        const float speed,
                        /* custom params: see package.json */
                        const float resolution ) {
    
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
    float color = 0.0;
    
    color += sin( outputCoords.x * cos( time * speed / 15.0 ) * 80.0 ) + cos( outputCoords.y * cos( time * speed / 15.0 ) * 10.0 );
    color += sin( outputCoords.y * sin( time * speed / 10.0 ) * 40.0 ) + cos( outputCoords.z * sin( time * speed / 25.0 ) * 40.0 );
    color += sin( outputCoords.z * sin( time * speed / 5.0 ) * 10.0 ) + sin( outputCoords.x * sin( time * speed / 35.0 ) * 80.0 );
    color *= sin( time * speed / 10.0 ) * 0.5;
    
    float4 outputPixel = (float4)( color, color * 0.5, sin( color + time * speed / 3.0 ) * 0.75, 1.0f );
    /* *************************** */
    
    /* do not change */
    write_imagef( output, outputCoords, blend4_with_opacity( blendMode, inputPixel, outputPixel, clamp( blendOpacity, 0.0f, 1.0f ) ) );
    /* end do not change */

}
