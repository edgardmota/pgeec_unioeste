#include <stdio.h>
#include <stdlib.h>

int potencia(int a, int n){
  if (n == 0)
      return 1;
  else
    return a*potencia(a,n-1);
}

int main(void){
  int a = 2;
  int n = 6;
  printf("%d^%d=%d\n",a,n,potencia(a,n));
  return 0;
}
