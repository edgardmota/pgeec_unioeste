#include <stdio.h>
#include <stdlib.h>

#define TAMANHO 10

int maior_vetor(int * n, int a, int b){
  int maior;

  if(a == b)
    return n[a];
  else
    maior = maior_vetor(n, a+1, b);
    if(n[a]>maior)
      return n[a];
    else
      return maior;
}

int main(void){
  int * n = (int *) malloc(sizeof(int)*TAMANHO);
  n[0] = 90;
  n[1] = 1;
  n[2] = 7;
  n[3] = 1;
  n[4] = 3;
  n[5] = 2;
  n[6] = 3;
  n[7] = 2;
  n[8] = 0;
  n[9] = -3;
  printf("Maior elemento é %d\n",maior_vetor(n,0,TAMANHO-1));
  return 0;
}
