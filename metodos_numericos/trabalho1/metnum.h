#ifndef _METNUM_H_
#define _METNUM_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern const double ERRO;

//Constantes Booleanas
#define TRUE 1
#define FALSE 0

typedef struct polinomio {
  float a;
  float b;
  float c;
} polinomio;

typedef struct intervalo {
  double a;
  double b;
} intervalo;

polinomio * derivada(polinomio * p);

double resolve_polinomio(double x, polinomio * p);

unsigned int bolzano(intervalo * i, polinomio * p);

double diferenca(double a, double b);

char * formata_polinomio(char * nome, polinomio * p);

double bisseccao(intervalo * i, polinomio * p);

double iteracao_linear(double x0, polinomio * fi, polinomio * p);

double newton_raphson(double x0, polinomio * p);

double secante(double * x_s, polinomio * p);

#endif
