#ifndef _RACIONAIS_H_
#define _RACIONAIS_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct racional racional;

racional * inicializa(int n, int d);

racional * adicao(racional * r1, racional * r2);

racional * multiplicacao(racional * r1, racional * r2);

char * formata(racional * r);

#endif
