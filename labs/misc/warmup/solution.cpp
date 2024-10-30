
#include "solution.h"

#ifndef SOLUTION
int solution(int *arr, int N) {
  
  int res = 0;
  for (int i = 0; i < N; i++) {
    res += arr[i];
  }
  return res;
}
#endif

#ifdef SOLUTION
int solution(int *arr, int N) {
  return (1+N)*N/2;
}
#endif
