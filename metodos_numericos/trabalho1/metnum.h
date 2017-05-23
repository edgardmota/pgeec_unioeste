#ifndef _METNUM_H_
#define _METNUM_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern const double ERRO;
#define GAUSS_JACOBI 4
#define GAUSS_SEIDEL 5

//Constantes Booleanas
#define TRUE 1
#define FALSE 0

typedef struct polinomio {
  float a;
  float b;
  float c;
} polinomio;

typedef struct termo_nao_linear {
  float constante;
  float * expo_x;
} termo_nao_linear;

typedef struct intervalo {
  double a;
  double b;
} intervalo;

termo_nao_linear * * * jacobiana(termo_nao_linear * * * snl, unsigned int n);

void arruma_linhas(double * * a, unsigned int n);

int criterio_linhas(double * * a, unsigned int n);

double * gauss_jacobi(double * * a, double * b, double * x0, unsigned int n);

double * gauss_seidel(double * * a, double * b, double * x0, unsigned int n);

double * gauss_jacobi_seidel(double * * a, double * b, double * x0, unsigned int n, int metodo);

unsigned int convergiu(double * x1, double * x2, unsigned int n);

double * vetor_zerado(unsigned int n);

double * gauss_jacobi(double * * a, double * b, double * x0, unsigned int n);

double * * transposta(double * * m, unsigned int n);

double * eliminacao_gauss_cholesky(double * * a, double * b, unsigned int n);

double * * cholesky (double * * a, unsigned int n);

void copia_vetor(double * v1, double * v2, unsigned int n);

double * * matriz_zerada(unsigned int n);

void copia_matriz(double * * m1, double * * m2, unsigned int n);

void L_U (double * * a, double * b, double * * l, double * * u, unsigned int n);

int max_modulo_col(double * * a, unsigned int l, unsigned int c, unsigned int n);

void troca_linhas(double * * a, double * b, unsigned int l1, unsigned int l2, unsigned int n);

void triangular_superior(double * * a, double * b, unsigned int n, double * * l);

double * eliminacao_gauss_L(double * * a, double * b, unsigned int n);

double * eliminacao_gauss(double * * a, double * b, unsigned int n);

double * eliminacao_gauss_L_U(double * * a, double * b, unsigned int n);

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
