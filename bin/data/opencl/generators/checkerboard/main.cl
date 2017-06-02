
#include "blendModes.h"

__kernel void generator(read_only image3d_t input, // float
                        write_only image3d_t output, // float
                        const int blendMode,
                        const float blendOpacity,
                        const float time,
                        const float speed,
                        /* custom params: see package.json */
                        const int size,
                        const float4 offset,
                        const float4 moveDir,
                        const float4 color1,
                        const float4 color2 ) {

    
    
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
    const float4 move = (float4)( (float)(outputDim.x) * time * speed * moveDir.x,
                                  (float)(outputDim.y) * time * speed * moveDir.y,
                                  (float)(outputDim.z) * time * speed * moveDir.z, 0.0f );
    const float4 outputPixel =
        ((int)((outputCoords.x+offset.x+move.x)/(float)size) +
         (int)((outputCoords.y+offset.y+move.y)/(float)size) +
         (int)((outputCoords.z+offset.z+move.z)/(float)size) ) % 2 == 0 ? color1 : color2;
    outputPixel.a = 1.0;
    /* *************************** */
    
    /* do not change */
    write_imagef( output, outputCoords, blend4_with_opacity( blendMode, inputPixel, outputPixel, clamp( blendOpacity, 0.0f, 1.0f ) ) );
    /* end do not change */
}
