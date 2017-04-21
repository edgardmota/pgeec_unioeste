#include "rac.h"

typedef struct racional {
  int n;
  int d;
} racional;

racional * inicializa(int n, int d){
  racional * r = (racional *) malloc(sizeof(racional));

  r->n = n;
  r->d = d;

  return r;
}

racional * adicao(racional * r1, racional * r2){
  racional * r3 = (racional *) malloc(sizeof(racional));

  r3->n = (r2->d * r1->n) + (r1->d * r2->n);
  r3->d = r1->d * r2->d;
  return r3;
}

racional * multiplicacao(racional * r1, racional * r2){
  racional * r3 = (racional *) malloc(sizeof(racional));

  r3->n = r1->n * r2->n;
  r3->d = r1->d * r2->d;
  return r3;
}

char * formata(racional * r){
  char * str;
  char temp_str[40];

  sprintf(temp_str,"%d/%d",r->n,r->d);
  str = (char *) malloc(strlen(temp_str)*sizeof(char));
  strcpy(str,temp_str);
  return str;
}
