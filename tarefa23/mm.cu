// matriz 2000x2000
// sequencial: 23.776s
// openmp: 3.657s
// RTX 3060: kernel 141.917ms, total 152.130ms
// NCU: warps_launched=125000, smsp__thread_inst_executed_per_inst_executed.ratio=32.
// tempo do kernel NCU: 379.322 ms.

#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>

#define TILE_SIZE 16

static int check_cuda(cudaError_t status, const char *operation)
{
  if (status != cudaSuccess) {
    fprintf(stderr, "%s: %s\n", operation, cudaGetErrorString(status));
    return 0;
  }
  return 1;
}

__global__ void mm_kernel(const double *a, const double *b, double *c,
                          int width)
{
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;

  if (row < width && col < width) {
    double sum = 0.0;
    for (int k = 0; k < width; k++) {
      sum += a[row * width + k] * b[k * width + col];
    }
    c[row * width + col] = sum;
  }
}

struct CudaResources {
  double *a = NULL;
  double *b = NULL;
  double *c = NULL;
  double *device_a = NULL;
  double *device_b = NULL;
  double *device_c = NULL;
  cudaEvent_t total_start = NULL;
  cudaEvent_t kernel_start = NULL;
  cudaEvent_t kernel_stop = NULL;
  cudaEvent_t total_stop = NULL;

  ~CudaResources()
  {
    if (total_start != NULL) cudaEventDestroy(total_start);
    if (kernel_start != NULL) cudaEventDestroy(kernel_start);
    if (kernel_stop != NULL) cudaEventDestroy(kernel_stop);
    if (total_stop != NULL) cudaEventDestroy(total_stop);
    if (device_a != NULL) cudaFree(device_a);
    if (device_b != NULL) cudaFree(device_b);
    if (device_c != NULL) cudaFree(device_c);
    free(a);
    free(b);
    free(c);
  }
};

int main(int argc, char **argv)
{
  int width = 2000;
  if (argc > 1) {
    char *end = NULL;
    long parsed_width = strtol(argv[1], &end, 10);
    if (*argv[1] == '\0' || *end != '\0' || parsed_width <= 0 ||
        parsed_width > 46340) {
      fprintf(stderr, "Uso: %s [largura positiva (maximo 46340)]\n", argv[0]);
      return EXIT_FAILURE;
    }
    width = (int)parsed_width;
  }

  size_t elements = (size_t)width * (size_t)width;
  size_t bytes = elements * sizeof(double);
  CudaResources resources;
  resources.a = (double *)malloc(bytes);
  resources.b = (double *)malloc(bytes);
  resources.c = (double *)malloc(bytes);
  if (resources.a == NULL || resources.b == NULL || resources.c == NULL) {
    fprintf(stderr, "Falha ao alocar memoria para matrizes de largura %d\n", width);
    return EXIT_FAILURE;
  }

  for (int row = 0; row < width; row++) {
    for (int col = 0; col < width; col++) {
      resources.a[(size_t)row * width + col] = (double)row;
      resources.b[(size_t)row * width + col] = (double)col;
    }
  }

  dim3 block(TILE_SIZE, TILE_SIZE);
  dim3 grid((width + TILE_SIZE - 1) / TILE_SIZE,
            (width + TILE_SIZE - 1) / TILE_SIZE);
  float kernel_ms = 0.0f;
  float total_ms = 0.0f;

  if (!check_cuda(cudaMalloc((void **)&resources.device_a, bytes), "cudaMalloc(a)") ||
      !check_cuda(cudaMalloc((void **)&resources.device_b, bytes), "cudaMalloc(b)") ||
      !check_cuda(cudaMalloc((void **)&resources.device_c, bytes), "cudaMalloc(c)")) {
    return EXIT_FAILURE;
  }
  if (!check_cuda(cudaEventCreate(&resources.total_start), "cudaEventCreate(total_start)") ||
      !check_cuda(cudaEventCreate(&resources.kernel_start), "cudaEventCreate(kernel_start)") ||
      !check_cuda(cudaEventCreate(&resources.kernel_stop), "cudaEventCreate(kernel_stop)") ||
      !check_cuda(cudaEventCreate(&resources.total_stop), "cudaEventCreate(total_stop)")) {
    return EXIT_FAILURE;
  }

  if (!check_cuda(cudaEventRecord(resources.total_start), "cudaEventRecord(total_start)") ||
      !check_cuda(cudaMemcpy(resources.device_a, resources.a, bytes, cudaMemcpyHostToDevice), "copia de A") ||
      !check_cuda(cudaMemcpy(resources.device_b, resources.b, bytes, cudaMemcpyHostToDevice), "copia de B") ||
      !check_cuda(cudaEventRecord(resources.kernel_start), "cudaEventRecord(kernel_start)")) {
    return EXIT_FAILURE;
  }

  mm_kernel<<<grid, block>>>(resources.device_a, resources.device_b,
                             resources.device_c, width);
  if (!check_cuda(cudaGetLastError(), "lancamento do kernel") ||
      !check_cuda(cudaEventRecord(resources.kernel_stop), "cudaEventRecord(kernel_stop)") ||
      !check_cuda(cudaMemcpy(resources.c, resources.device_c, bytes, cudaMemcpyDeviceToHost), "copia de C") ||
      !check_cuda(cudaEventRecord(resources.total_stop), "cudaEventRecord(total_stop)") ||
      !check_cuda(cudaEventSynchronize(resources.total_stop), "sincronizacao CUDA")) {
    return EXIT_FAILURE;
  }

  if (!check_cuda(cudaEventElapsedTime(&kernel_ms, resources.kernel_start,
                                       resources.kernel_stop),
                  "tempo do kernel") ||
      !check_cuda(cudaEventElapsedTime(&total_ms, resources.total_start,
                                       resources.total_stop),
                  "tempo total")) {
    return EXIT_FAILURE;
  }

  for (int row = 0; row < width; row++) {
    for (int col = 0; col < width; col++) {
      double expected = (double)width * row * col;
      double actual = resources.c[(size_t)row * width + col];
      if (actual != expected) {
        fprintf(stderr, "Resultado incorreto em C[%d][%d]: %.17g (esperado %.17g)\n",
                row, col, actual, expected);
        return EXIT_FAILURE;
      }
    }
  }

  printf("Largura: %d\n", width);
  printf("Bloco CUDA: %dx%d; grid: %dx%d\n",
         TILE_SIZE, TILE_SIZE, grid.x, grid.y);
  printf("Tempo do kernel: %.3f ms\n", kernel_ms);
  printf("Tempo total (copias + kernel): %.3f ms\n", total_ms);
  printf("Validacao: OK\n");
  return EXIT_SUCCESS;
}