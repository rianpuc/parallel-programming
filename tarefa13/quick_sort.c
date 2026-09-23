/*
 * Tempos de execucao (baseline sequencial anterior e gcc -fopenmp, sem -O2):
 * Sequencial (baseline): 1,59 s
 * Paralelo com tasks (2 threads): 5,11 s
 * Speedup: 0,31
 */

/* C implementation QuickSort from  http://w...content-available-to-author-only...s.org/quick-sort/ */
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
 
// A utility function to swap two elements
void swap(int* a, int* b)
{
  int t = *a;
  *a = *b;
  *b = t;
}
 
/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (int arr[], int low, int high)
{
  int pivot = arr[high];    // pivot
  int i = (low - 1);  // Index of smaller element
 
  for (int j = low; j <= high- 1; j++)
    {
      // If current element is smaller than or
      // equal to pivot
      if (arr[j] <= pivot)
        {
	  i++;    // increment index of smaller element
	  swap(&arr[i], &arr[j]);
        }
    }
  swap(&arr[i + 1], &arr[high]);
  return (i + 1);
}
 
void quickSortParallel(int arr[], int low, int high)
{
  if (low < high)
    {
      int pi = partition(arr, low, high);

      #pragma omp task shared(arr)
      quickSortParallel(arr, low, pi - 1);

      #pragma omp task shared(arr)
      quickSortParallel(arr, pi + 1, high);

      #pragma omp taskwait
    }
}

/* Function to print an array */
void printArray(int arr[], int size)
{
  int i;
  for (i=0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
}
 
// Driver program to test above functions
int main()
{
  int i,n = 10000000;
  int *arr = (int*) malloc(n*sizeof(int));

  for(i=0; i < n; i++)
    arr[i] = rand()%n;

  omp_set_dynamic(0);
  omp_set_num_threads(2);

  #pragma omp parallel
  {
    #pragma omp single
    quickSortParallel(arr, 0, n-1);
  }
  printf("Sorted array: \n");
  printArray(arr, n);
  return 0;
}