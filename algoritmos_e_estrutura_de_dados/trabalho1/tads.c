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
  pilha->topo = cabeca;
  pilha->tamanho = 0;
  return pilha;
}


int pilha_vazia(Pilha * pilha){
  return (pilha->topo == pilha->cabeca);
}

Elemento * pop(Pilha * pilha){
  void * conteudo;
  Elemento * liberar;
  if(!pilha_vazia(pilha)){
    conteudo = pilha->topo->conteudo;
    liberar = pilha->cabeca;
    pilha->cabeca = pilha->cabeca->proximo;
    pilha->topo = pilha->topo->proximo;
    pilha->tamanho--;
    free(liberar);
    return cria_elemento(conteudo);
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

int fila_vazia(Fila * fila){
  return ((fila->primeiro == fila->cabeca) && (fila->ultimo == fila->cabeca));
}

Fila * cria_fila(void){
  Fila * fila = (Fila *) malloc(sizeof(Fila));
  Elemento * cabeca = cria_elemento(NULL);
  fila->cabeca = cabeca;
  fila->primeiro = cabeca;
  fila->ultimo = cabeca;
  fila->tamanho = 0;
  return fila;
}

void in(void * conteudo, Fila * fila){
  Elemento * e = cria_elemento(conteudo);
  fila->tamanho++;
  if(fila_vazia(fila)){
    fila->primeiro = e;
    fila->ultimo = e;
    fila->cabeca->proximo = e;
  }
  else{
    fila->ultimo->proximo = e;
    fila->ultimo = e;
  }
}

Elemento * out(Fila * fila){
  void * conteudo;
  Elemento * liberar;
  if(!fila_vazia(fila)){
    fila->tamanho--;
    conteudo = fila->primeiro->conteudo;
    liberar = fila->cabeca;
    fila->cabeca = fila->cabeca->proximo;
    if(fila->tamanho == 0)
        fila->primeiro = fila->cabeca;
    else
        fila->primeiro = fila->primeiro->proximo;
    free(liberar);
    return cria_elemento(conteudo);
  }
  else
    return NULL;
}
