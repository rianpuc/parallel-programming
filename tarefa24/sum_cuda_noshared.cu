#include <stdio.h>
#include <stdlib.h>

__global__ void sum_cuda(double* a, double *s, int width) {
  int tid = blockIdx.x * blockDim.x + threadIdx.x;
  int stride = blockDim.x * gridDim.x;
  double local_sum = 0.0;

  for (int i = tid; i < width; i += stride)
    local_sum += a[i];

  atomicAdd(s, local_sum);
}

int main() {
  int width = 40000000;
  int size = width * sizeof(double);

  int block_size = 1024;
  int num_blocks = (width - 1) / block_size + 1;

  double *a = (double*) malloc(size);
  double *s = (double*) malloc(sizeof(double));

  for (int i = 0; i < width; i++)
    a[i] = i;
  s[0] = 0.0;

  double *d_a, *d_s;

  cudaMalloc((void **) &d_a, size);
  cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
  cudaMalloc((void **) &d_s, sizeof(double));
  cudaMemcpy(d_s, s, sizeof(double), cudaMemcpyHostToDevice);

  dim3 dimGrid(num_blocks, 1, 1);
  dim3 dimBlock(block_size, 1, 1);

  sum_cuda<<<dimGrid, dimBlock>>>(d_a, d_s, width);

  cudaMemcpy(s, d_s, sizeof(double), cudaMemcpyDeviceToHost);

  printf("\nSum = %f\n", s[0]);

  free(a);
  free(s);
  cudaFree(d_a);
  cudaFree(d_s);
}
