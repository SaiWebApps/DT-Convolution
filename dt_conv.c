#include <stdio.h>
#include <string.h>
#include <pmmintrin.h>
#include <time.h>

//Constants
#define LEN1 5
#define LEN2 5
#define RESULT_LEN ((LEN1)+(LEN2)-1)

//Load given arrays with values.
void loadArrays(int *arr1, int *arr2) {
    int i;
    for (i = 0; i<LEN1; i++) {
       arr1[i] = i+1;
    }
    for (i = 0; i<LEN2; i++) {
       arr2[i] = i+5;
    }
}

//Print out contents of specified array, w/ length len.
void print_array(int* a, int len) {
   printf("[ ");
   int k = 0;
   for (k = 0; k < len; k++) {
      printf("%d ", a[k]);
   }
   printf("]\n");
}

//Purely sequential DT convolution
void seq_convolve(int* a1, int len1, int* a2, int len2, int* result) {
   int i, j;
   for (i = 0; i<len2; i++) {
      int n = a2[i];
      for (j = 0; j<len1; j++) {
	        result[i+j] += a1[j] * n;
      }
   }
}

//Sequential DT convolution using Intel SSE intrinsics
void sse_convolve(int* a1, int len1, int* a2, int len2, int* result) {
   int i, j;
   for (i = 0; i<len2; i++) {
      int n = a2[i];
      __m128 res, x = _mm_set_ps(n,n,n,n);

      for (j = 0; j <= len1-4; j+=4) {
	       __m128 tempInput = _mm_loadu_ps(a1+j);
	       __m128 origOutput = _mm_loadu_ps(result+i+j);
        __m128 product = _mm_mul_ps(tempInput, x);
       	res = _mm_add_ps(origOutput, product);
        _mm_storeu_ps(&result[i+j], res);
      }

      for (; j < len1; j++) {
       	result[i+j] += a1[j] * n;
      }
   }
}

int main(int argc, char *argv[]) {
  int a1[LEN1];
  int a2[LEN2];
  int result[RESULT_LEN]; //Need to clear before use b/c contains junk right now
  int k;
  time_t t1, t2;

  loadArrays(a1,a2); //load arrays with values
  time(&t1); //start time

  //Clear array.
  memset(result, 0, RESULT_LEN * sizeof(int));
  
  //If user specifies "seq" as cmdline arg, then execute purely sequential version.
  //Otherwise, execute sse version.
  if (argc == 2 && strcmp(argv[1], "seq") == 0) 
     seq_convolve(a1, LEN1, a2, LEN2, result);
  else
     sse_convolve(a1, LEN1, a2, LEN2, result);

  print_array(result, RESULT_LEN); //Print result array.
  time(&t2); //end time
  printf("Execution time = %f seconds\n", difftime(t2,t2));
  return 0;
}
