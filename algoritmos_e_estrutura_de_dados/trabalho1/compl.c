#include "compl.h"

typedef struct complexo {
  double a;
  double b;
} complexo;

complexo * inicializa(double a, double b){
  complexo * c = (complexo *) malloc(sizeof(complexo));

  c->a = a;
  c->b = b;

  return c;
}

complexo * adicao(complexo * c1, complexo * c2){
  complexo * c3 = (complexo *) malloc(sizeof(complexo));

  c3->a = c1->a + c2->a;
  c3->b = c1->b + c2->b;
  return c3;
}

complexo * multiplicacao(complexo * c1, complexo * c2){
  complexo * c3 = (complexo *) malloc(sizeof(complexo));

  c3->a = (c1->a*c2->a) - (c1->b*c2->b);
  c3->b = (c1->a*c2->b) + (c2->a*c1->b);
  return c3;
}

char * formata(complexo * c){
  char * str;
  char temp_str[40];

  sprintf(temp_str,"(%0.2lf)+(%0.2lf)i",c->a,c->b);
  str = (char *) malloc(strlen(temp_str)*sizeof(char));
  strcpy(str,temp_str);
  return str;
}
