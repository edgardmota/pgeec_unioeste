#ifndef _LISTA_H_
#define _LISTA_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

//Lista com Estrutura Estática

#define ADIANTE 1
#define PARA_TRAS -1

#define PRIMEIRO 0
#define MAX_TAM 5

typedef struct no_estatico {
  int chave;
} no_estatico;

typedef struct lista_estatica {
  no_estatico * elementos[MAX_TAM];
  int primeiro;
  int ultimo;
} lista_estatica;

no_estatico * cria_no_estatico(int chave);
lista_estatica * cria_lista_estatica();
unsigned int checa_cheia_lista_estatica(lista_estatica * l);
unsigned int checa_vazia_lista_estatica(lista_estatica *);
unsigned int checa_posicao_valida_lista_estatica(int pos, lista_estatica * l);
no_estatico * busca_lista_estatica(int chave, lista_estatica * l);
void imprime_lista_estatica(lista_estatica * l);
unsigned int desloca_elementos_lista_estatica(int pos, int direcao, lista_estatica * l);
unsigned int insere_lista_estatica(int chave, int pos, lista_estatica * );
no_estatico * remove_lista_estatica(int pos, lista_estatica * );

//Lista Dinâmica

typedef struct no {
  int chave;
  struct no * proximo;
} no;

typedef struct lista {
  no * cabeca;
  no * cauda;
} lista;

no * cria_no(int chave);
lista * cria_lista();
unsigned int checa_vazia_lista(lista *);
unsigned int checa_posicao_valida_lista(int pos, lista * l);
no * busca_lista(int chave, lista * l);
void imprime_lista(lista * l);
unsigned int insere_lista(int chave, int pos, lista * );
no * proximo(no * n);
no * remove_lista(int pos, lista * );

#endif
