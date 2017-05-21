__kernel void applyPreviewColor( __read_only image3d_t input,
                                __global __write_only float* packedPositions,
                                __global __write_only float4* colors ) {
    
    // 1D, packed positions
    
    const int vertexIndex = get_global_id(0);
    float4 p;
    
    // load position with vload3, because of the packed input vector
    p.xyz = vload3( vertexIndex, packedPositions );
    p.w = 1.0;
    
    float4 pixelColor = read_imagef (input, CLK_NORMALIZED_COORDS_TRUE|CLK_FILTER_NEAREST|CLK_ADDRESS_CLAMP_TO_EDGE, p );
    
    colors[vertexIndex].r = pixelColor.r;
    colors[vertexIndex].g = pixelColor.g;
    colors[vertexIndex].b = pixelColor.b;
    colors[vertexIndex].a = 1.0;
    
}
