#include "rac.h"

int main(void){
  racional * r1 = inicializa(1,5);
  racional * r2 = inicializa(7,3);
  racional * m = multiplicacao(r1,r2);
  racional * a = adicao(r1,r2);

  printf("r1: %s\n",formata(r1));
  printf("r2: %s\n",formata(r2));
  printf("m: %s\n",formata(m));
  printf("a: %s\n",formata(a));

  return 0;
}
