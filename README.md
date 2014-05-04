DT-Convolution
==============

Optimizing purely sequential DT Convolution through SSE; parallelizing through OpenMP

Lessons Learned about Intel SSE intrinsics
- Better to use floating-point intrinsics since they are truly element-wise; integer intrinsics (especially fo multiplication) are not necessarily so.
- Basic intrinsics operations (from highest-addressed int to lowest-addressed int)
  * __m128 _mm_setzero_ps() -> (0,0,0,0)
  * __m128 _mm_set1_ps(float f) -> (f,f,f,f)
  * __m128 _mm_set_ps(float w, float x, float y, float z) -> (w, x, y, z)
  * __m128 _mm_setr_ps(float w, float x, float y, float z) -> (z, y, x, w)
  * __m128 _mm_loadu_ps(float *addr) -> Given that float addr[4] = {w,x,y,z}, load will operate as follows: (z,y,x,w)
  * __m128 _mm_loadr_ps(float *addr) -> (w,x,y,z)
  * __m128 _mm_storeu_ps(float *addr, __m128) -> Store the value in the given register into specified array/address.

Lessons Learned about timing in C:
- clock() and clock_t -> for processor time
- time_t and time(&time_t) -> for calendar time and real seconds
