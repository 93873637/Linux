#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define BLOCK_DIM 128

const int size_x = 128;
const int size_y = 1;

__global__ static void threadDemo(unsigned int * ret)
{
    unsigned int xIndex = blockDim.x *blockIdx.x + threadIdx.x;
    unsigned int yIndex = blockDim.y * blockIdx.y + threadIdx.y;

    printf("block Dim (%d, %d) \n", blockDim.x, blockDim.y);
    printf("block id (%d, %d)\n", blockIdx.x, blockIdx.y);
    printf("thread id (%d, %d) \n", threadIdx.x, threadIdx.y);
    
    if (xIndex < size_x && yIndex < size_y) {
        unsigned int index = xIndex + size_x * yIndex;

        ret[index] = xIndex;
	ret[index + size_x * size_y] = yIndex;
    }

}

void ThreadDemo(void)
{
    unsigned int * ret = 0;
    unsigned int  host_ret[size_x*size_y*2] = {0};
    int i = 0;

    cudaMalloc((void**) &ret, sizeof(unsigned int)*(size_x*size_y*2));

    dim3 grid(size_x / BLOCK_DIM, 1);
    dim3 block(BLOCK_DIM, 1, 1);


    
    threadDemo<<<grid,block>>>(ret);

    cudaMemcpy(&host_ret, ret, sizeof(unsigned int)*(size_x*size_y*2), cudaMemcpyDeviceToHost);


    for (i = 0; i < size_x*size_y; i++) {
        printf("(%u,%u)", host_ret[i], host_ret[size_x*size_y+i]);

    }

    cudaFree(ret);

}