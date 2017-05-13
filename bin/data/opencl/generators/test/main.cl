__kernel void generator(read_only image3d_t input, // float
						write_only image3d_t output, // float
                        const float seconds,
                        const float speed ) {

    const int4 inputDim = get_image_dim (input);
    const int4 outputDim = get_image_dim (output);

    const int4 outputCoords = (int4)( get_global_id(0), get_global_id(1), get_global_id(2), 0 );

    const float4 inputCoords = (float4)( (float)outputCoords.x/(float)outputDim.x, 
                                         (float)outputCoords.y/(float)outputDim.y,
                                         (float)outputCoords.z/(float)outputDim.z,
                                         0.0 );

    float4 pixel = read_imagef (input, CLK_NORMALIZED_COORDS_TRUE|CLK_FILTER_NEAREST|CLK_ADDRESS_NONE, inputCoords );

    pixel.r+=sin(seconds*speed+outputCoords.x);
    pixel.g+=cos(seconds*speed*0.1+outputCoords.y);
    pixel.b+=sin(seconds*speed*0.5+outputCoords.z);
    pixel.a=0.00;

    pixel.r*=0.5;
    pixel.g*=0.5;
    pixel.b*=0.5;


    //printf( "%d, %d, %d\n", outputCoords.x, outputCoords.y, outputCoords.z);

    write_imagef( output, outputCoords, pixel );

}