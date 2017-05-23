#include "tads.h"

Elemento * cria_elemento(void * conteudo){
  Elemento * e = (Elemento *) malloc(sizeof(Elemento));
  e->conteudo = conteudo;
  e->proximo = NULL;
  return e;
}

Pilha * cria_pilha(void){
  Pilha * pilha = (Pilha *) malloc(sizeof(Pilha));
  Elemento * cabeca = cria_elemento(NULL);
  pilha->cabeca = cabeca;
  pilha->topo = NULL;
  pilha->tamanho = 0;
  return pilha;
}


int pilha_vazia(Pilha * pilha){
  return (pilha->topo == NULL);
}

Elemento * pop(Pilha * pilha){
  Elemento * liberar;
  if(!pilha_vazia(pilha)){
    liberar = pilha->cabeca;
    pilha->cabeca = pilha->cabeca->proximo;
    pilha->topo = pilha->topo->proximo;
    pilha->tamanho--;
    free(liberar);
    return cria_elemento(pilha->cabeca->conteudo);
  }
  else
    return NULL;
}

void push(void * conteudo, Pilha * pilha){
   Elemento * e = cria_elemento(conteudo);
   pilha->topo = e;
   e->proximo = pilha->cabeca->proximo;
   pilha->cabeca->proximo = e;
   pilha->tamanho++;
}

Fila * cria_fila(void){
  Fila * fila = (Fila *) malloc(sizeof(Fila));
  Elemento * cabeca = cria_elemento(NULL);
  fila->cabeca = cabeca;
  fila->primeiro = NULL;
  fila->ultimo = NULL;
  fila->tamanho = 0;
  return fila;
}

// Elemento * out(Fila * fila);
// void in(void * conteudo, Fila * fila);
// int fila_vazia(Fila * fila);
