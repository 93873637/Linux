#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

#define WINDOW_WIDTH 9
#define WINDOW_HEIGHT 7
#define IMAGE_WIDTH 1280
#define IMAGE_HEIGHT 720

#define BLOCK_SIZE 128
#define LINES_PER_BLOCK 16

__global__ static void censusTransform(int width, int height, int * ret)
{
    int half_kw = WINDOW_WIDTH /2;
    int half_kh = WINDOW_HEIGHT /2 ;

    int tid = threadIdx.x;
    int x0 = blockIdx.x * (BLOCK_SIZE - WINDOW_WIDTH + 1) - half_kw;
    int y0 = blockIdx.y * LINES_PER_BLOCK;

    printf("block Dim (%d %d) (%d %d) \n", blockDim.x, blockDim.y, blockIdx.x, blockIdx.y);
    //printf("block id (%d %d) \n", blockIdx.x, blockIdx.y);
    for (int i = 0; i < WINDOW_HEIGHT; ++i) {
        int x = x0 + tid;
	int y = y0 - half_kh + i;
        int value = 0;
	
        if (0 <= x && x < IMAGE_WIDTH && 0 <= y && y < IMAGE_HEIGHT) {
	    value = x+y*IMAGE_WIDTH;
	    ret[i] = value / IMAGE_WIDTH;
	    ret[WINDOW_HEIGHT + i] = value % IMAGE_WIDTH;
            //printf("row %d  col %d \n", value / IMAGE_WIDTH, value % IMAGE_WIDTH);
        }



    }
    __syncthreads();
}

void censusDemo(void)
{
    printf("censusDemo +++++\n");
    int * ret = 0;
    int host_ret[WINDOW_HEIGHT*2] = {0};
    
    cudaMalloc((void**) &ret, sizeof(int)*WINDOW_HEIGHT*2);
    
    int width_per_block = BLOCK_SIZE - WINDOW_WIDTH + 1;
    int height_per_block = LINES_PER_BLOCK;
    dim3 grid((IMAGE_WIDTH + width_per_block - 1) / width_per_block, (IMAGE_HEIGHT + height_per_block - 1) / height_per_block);
    //dim3 grid(1,1);
    dim3 block(BLOCK_SIZE);

    censusTransform<<<grid,block>>>(IMAGE_WIDTH, IMAGE_HEIGHT, ret);

    cudaMemcpy(host_ret, ret, sizeof(int)*WINDOW_HEIGHT*2, cudaMemcpyDeviceToHost);
    for (int i = 0; i < WINDOW_HEIGHT; i++) {
        printf("(%d  %d)", host_ret[i], host_ret[WINDOW_HEIGHT+i]);

    }

    cudaFree(ret);

}
