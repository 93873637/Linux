#include <stdio.h>
#include "./cuda/threadDemo.cuh"
#include "./cuda/census_test.cuh"
#include "./cuda/deviceCap.cuh"

int main()
{
  //ThreadDemo();
  //censusDemo();
  deviceCap();

  
  return 0;
}
