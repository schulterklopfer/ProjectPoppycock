__kernel void generator(read_only image3d_t input, // float
                        write_only image3d_t output, // float
                        const float time,
                        const float speed ) {

    float3 resolution = (float3)(20.f,20.f,20.f);
    
    const int4 outputDim = get_image_dim (output);
    
    const int4 outputCoords = (int4)( get_global_id(0), get_global_id(1), get_global_id(2), 0 );
    
    float3 position = (float3)((float)outputCoords.x/resolution.x,
                               (float)outputCoords.y/resolution.y,
                               (float)outputCoords.z/resolution.z );
    
    
    float color = 0.0;
    
    color += sin( position.x * cos( time * speed / 15.0 ) * 80.0 ) + cos( position.y * cos( time * speed / 15.0 ) * 10.0 );
    color += sin( position.y * sin( time * speed / 10.0 ) * 40.0 ) + cos( position.z * sin( time * speed / 25.0 ) * 40.0 );
    color += sin( position.z * sin( time * speed / 5.0 ) * 10.0 ) + sin( position.x * sin( time * speed / 35.0 ) * 80.0 );
    color *= sin( time * speed / 10.0 ) * 0.5;
    
    float4 outputPixel = (float4)( color, color * 0.5, sin( color + time * speed / 3.0 ) * 0.75, 1.0f );
    
    
    const float4 inputCoords = (float4)((float)outputCoords.x/(float)outputDim.x,
                                        (float)outputCoords.y/(float)outputDim.y,
                                        (float)outputCoords.z/(float)outputDim.z,
                                        0.0 );
    
    float4 inputPixel = read_imagef (input, CLK_NORMALIZED_COORDS_TRUE|CLK_FILTER_NEAREST|CLK_ADDRESS_NONE, inputCoords );
    
    inputPixel.a = 1.0;
    
    outputPixel+=inputPixel;
    outputPixel*=0.5f;
    
    write_imagef( output, outputCoords, outputPixel );

}
