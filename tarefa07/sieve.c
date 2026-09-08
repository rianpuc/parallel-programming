/*
 * Tempo medido no ambiente:
 * - Sequencial: 0.128754 s
 * - static: 0.027217 s
 * - dynamic: 0.026407 s
 * - guided: 0.027235 s
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <time.h>

static double elapsed_seconds(const struct timespec *start,
                              const struct timespec *end)
{
   return (double)(end->tv_sec - start->tv_sec) +
          (double)(end->tv_nsec - start->tv_nsec) / 1e9;
}

static bool *sieve_of_eratosthenes(int n)
{
   bool *prime = malloc((size_t)(n + 1) * sizeof(*prime));

   if (prime == NULL)
      return NULL;

   memset(prime, true, (size_t)(n + 1) * sizeof(*prime));
   prime[0] = false;
   prime[1] = false;

   for (int p = 2; p <= n / p; p++)
   {
      if (prime[p])
      {
         for (int i = p * p; i <= n; i += p)
            prime[i] = false;
      }
   }

   return prime;
}

static long long count_primes_sequential(const bool *prime, int n)
{
   long long primes = 0;

   for (int value = 2; value <= n; value++)
      if (prime[value])
         primes++;

   return primes;
}

static long long count_primes_parallel(const bool *prime, int n,
                                       omp_sched_t schedule_kind,
                                       int chunk_size)
{
   long long primes = 0;

   omp_set_schedule(schedule_kind, chunk_size);
#pragma omp parallel for schedule(runtime) reduction(+:primes)
   for (int value = 2; value <= n; value++)
      if (prime[value])
         primes++;

   return primes;
}

static double benchmark_parallel(const bool *prime, int n,
                                 omp_sched_t schedule_kind,
                                 int chunk_size, long long *result)
{
   struct timespec start;
   struct timespec end;

   clock_gettime(CLOCK_MONOTONIC, &start);
   *result = count_primes_parallel(prime, n, schedule_kind, chunk_size);
   clock_gettime(CLOCK_MONOTONIC, &end);

   return elapsed_seconds(&start, &end);
}

int main(void)
{
   int n;
   int threads;
   bool *prime;
   long long sequential_result;
   long long parallel_result;
   struct timespec start;
   struct timespec end;
   double sequential_time;
   double static_time;
   double dynamic_time;
   double guided_time;

   if (scanf("%d", &n) != 1 || n < 2 || n > 1000000000)
   {
      fprintf(stderr, "Informe um inteiro entre 2 e 1000000000.\n");
      return EXIT_FAILURE;
   }

   prime = sieve_of_eratosthenes(n);
   if (prime == NULL)
   {
      fprintf(stderr, "Nao foi possivel alocar memoria para o crivo.\n");
      return EXIT_FAILURE;
   }

   clock_gettime(CLOCK_MONOTONIC, &start);
   sequential_result = count_primes_sequential(prime, n);
   clock_gettime(CLOCK_MONOTONIC, &end);
   sequential_time = elapsed_seconds(&start, &end);

   threads = omp_get_max_threads();
   static_time = benchmark_parallel(prime, n, omp_sched_static, 0,
                                    &parallel_result);
   dynamic_time = benchmark_parallel(prime, n, omp_sched_dynamic, 1000,
                                     &parallel_result);
   guided_time = benchmark_parallel(prime, n, omp_sched_guided, 1000,
                                    &parallel_result);

   printf("Quantidade de primos: %lld\n", sequential_result);
   printf("Threads: %d\n", threads);
   printf("Sequencial: %.6f s\n", sequential_time);
   printf("Paralelo (static):  %.6f s\n", static_time);
   printf("Paralelo (dynamic): %.6f s\n", dynamic_time);
   printf("Paralelo (guided):  %.6f s\n", guided_time);

   if (parallel_result != sequential_result)
   {
      fprintf(stderr, "Erro: as contagens nao coincidem.\n");
      free(prime);
      return EXIT_FAILURE;
   }

   free(prime);
   return EXIT_SUCCESS;
}