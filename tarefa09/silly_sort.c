/*
 * Tempo medido no ambiente:
 * - Sequencial: 1.00s
 * - Paralela sem política de escalonamento: 6.29s
 * - Paralela com melhor política (guided): 5.91s
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void)
{
    int i, j, n = 30000;

    // Allocate input, output and position arrays
    int *in = (int *)calloc(n, sizeof(int));
    int *pos = (int *)calloc(n, sizeof(int));
    int *out = (int *)calloc(n, sizeof(int));

    if (in == NULL || pos == NULL || out == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        free(in);
        free(pos);
        free(out);
        return 1;
    }

    // Initialize input array in the reverse order
    for (i = 0; i < n; i++)
        in[i] = n - i;

    // Parallel version with exactly two threads.
    // Guided scheduling was the fastest among the tested policies.
    omp_set_num_threads(2);
#pragma omp parallel for schedule(guided)
    for (i = 0; i < n; i++) {
        int count = 0;
        for (int j = 0; j < n; j++)
            if (in[i] > in[j])
                count++;
        pos[i] = count;
    }

    // Move elements to final position
    for (i = 0; i < n; i++)
        out[pos[i]] = in[i];

    // Check if answer is correct
    for (i = 0; i < n; i++)
        if (i + 1 != out[i]) {
            printf("test failed\n");
            free(in);
            free(pos);
            free(out);
            return 1;
        }

    printf("test passed\n");

    free(in);
    free(pos);
    free(out);
    return 0;
}  
