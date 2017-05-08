__kernel void generator(__global int *input,
                        int sizeX,
                        int sizeY,
                        int sizeZ,
                        float seconds,
                        float speed,
                        __global int *output) {
    
    int globalIndex = get_global_id(0);
  
    if( globalIndex == 0 ) {
        output[globalIndex] = sizeX;
    } else if( globalIndex == 1 ) {
        output[globalIndex] = sizeY;
    } else if( globalIndex == 2 ) {
        output[globalIndex] = sizeZ;
    } else if( globalIndex == 3 ) {
        output[globalIndex] = (int)seconds;
    } else {
        output[globalIndex] = globalIndex;
    }
    
}
