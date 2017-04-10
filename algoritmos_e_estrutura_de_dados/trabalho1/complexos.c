#include "compl.h"

int main(void){
  complexo * c1 = inicializa(1,5);
  complexo * c2 = inicializa(7,3);
  complexo * m = multiplicacao(c1,c2);
  complexo * a = adicao(c1,c2);

  printf("c1: %s\n",formata(c1));
  printf("c2: %s\n",formata(c2));
  printf("m: %s\n",formata(m));
  printf("a: %s\n",formata(a));

  return 0;
}
