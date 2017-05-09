__kernel void generator(__global int *input,
                        int sizeX,
                        int sizeY,
                        int sizeZ,
                        float seconds,
                        float speed,
                        __global int *output) {
    
    int globalIndex = get_global_id(0);
    output[globalIndex] = input[globalIndex]+1;
}
