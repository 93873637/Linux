#include "./common.h"

void listDevice(void)
{
    int deviceCount;
    cudaGetDeviceCount(&deviceCount);

    printf("device count %d \n", deviceCount);

    for (int i = 0; i < deviceCount; i++) {
        cudaDeviceProp prop;
	cudaGetDeviceProperties(&prop, i);


        printf("device name : %s\n", prop.name);
	printf("device major %d minor %d\n", prop.major, prop.minor);
	printf("warp size %d\n", prop.warpSize);
	printf("multi processor count %d\n", prop.multiProcessorCount);
	printf("max grid size %d %d %d\n", prop.maxGridSize[0], prop.maxGridSize[1], prop.maxGridSize[2]);
        printf("max thread dims %d %d %d\n", prop.maxThreadsDim[0], prop.maxThreadsDim[1], prop.maxThreadsDim[2]);
	printf("max thread count per block %d\n", prop.maxThreadsPerBlock);
	printf("max thread per multiprocessor %d\n", prop.maxThreadsPerMultiProcessor);
	printf("clock rate %d\n", prop.clockRate);

    }

}

void deviceCap(void)
{
    listDevice();

}