#include <stdio.h>
#include <stdlib.h>

int fibonacci(int n){
  if(n <= 1)
    return n;
  else
    return (fibonacci(n-1) + fibonacci(n-2));
}

int main(void){
  int n = 10;
  int i;

  for(i = 0; i < n; i++)
    printf("%d, ",fibonacci(i));
  printf("\n");
  return 0;
}
