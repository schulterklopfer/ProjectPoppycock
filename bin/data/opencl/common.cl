__kernel void slicer(__write_only image2d_t output, // float
                     __read_only image3d_t input, // float
                     const int axis,
                     const float position ) {

    
    const int2 outputDim = get_image_dim (output);
    const int2 outputCoords = (int2)( get_global_id(0), get_global_id(1) ); 


    // for starters move through z-Axis
    float4 inputCoords;

    if( axis == 1 ) {
        // y-axis
        inputCoords = (float4)( (float)outputCoords.x/(float)outputDim.x, 
                                clamp(position,0.0f,1.0f),
                                (float)outputCoords.y/(float)outputDim.y,
                                 0.0 );
    } else if( axis == 2 ) {
        // x-axis
        inputCoords = (float4)( clamp(position,0.0f,1.0f),
                                (float)outputCoords.y/(float)outputDim.y, 
                                (float)outputCoords.x/(float)outputDim.x,
                                 0.0 );
    }  else {
        // z-axis
        inputCoords = (float4)( (float)outputCoords.x/(float)outputDim.x, 
                                (float)outputCoords.y/(float)outputDim.y,
                                 clamp(position,0.0f,1.0f),
                                 0.0 );
    }
    

    float4 pixel = read_imagef (input, CLK_NORMALIZED_COORDS_TRUE|CLK_FILTER_NEAREST|CLK_ADDRESS_CLAMP_TO_EDGE, inputCoords );
    
    pixel.a = 1.0;

    // will run in 2D
    write_imagef( output, outputCoords, pixel );
}