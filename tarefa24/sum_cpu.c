#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void) {
  const int width = 40000000;
  double *a = malloc(width * sizeof(*a));
  if (!a) return 1;

  for (int i = 0; i < width; i++) a[i] = i;

  double sum_seq = 0.0;
  double start = omp_get_wtime();
  for (int i = 0; i < width; i++) sum_seq += a[i];
  double time_seq = omp_get_wtime() - start;

  double sum_omp = 0.0;
  start = omp_get_wtime();
  #pragma omp parallel for reduction(+:sum_omp)
  for (int i = 0; i < width; i++) sum_omp += a[i];
  double time_omp = omp_get_wtime() - start;

  printf("Sum sequential = %.0f, time = %.6f s\n", sum_seq, time_seq);
  printf("Sum OpenMP = %.0f, time = %.6f s\n", sum_omp, time_omp);

  free(a);
  return 0;
}
