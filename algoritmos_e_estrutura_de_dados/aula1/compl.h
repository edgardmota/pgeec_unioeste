#ifndef _COMPLEXOS_H_
#define _COMPLEXOS_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct complexo complexo;

complexo * inicializa(double a, double b);

complexo * adicao(complexo * c1, complexo * c2);

complexo * multiplicacao(complexo * c1, complexo * c2);

char * formata(complexo * c);

#endif
