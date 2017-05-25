#include "tads.h"

Elemento * criar_elemento(void * conteudo){
  Elemento * e = (Elemento *) malloc(sizeof(Elemento));
  e->conteudo = conteudo;
  e->proximo = NULL;
  return e;
}


int vazia(Lista * lista){
  return !lista->tamanho;
}

Lista * criar_lista(void){
  Lista * lista = (Lista *) malloc(sizeof(Lista));
  Elemento * cabeca = criar_elemento(NULL);
  lista->cabeca = cabeca;
  lista->tamanho = 0;
  return lista;
}

int inserir(void * conteudo, int posicao, Lista * lista){
  Elemento * e;
  Elemento * atual;

  if(posicao <= lista->tamanho){
      e = criar_elemento(conteudo);
      atual = lista->cabeca;
      while(posicao){
        atual = atual->proximo;
        posicao--;
      }
      e->proximo = atual->proximo;
      atual->proximo = e;
      lista->tamanho++;
      return TRUE;
  }
  else
    return FALSE;
}

void * remover(int posicao, Lista * lista){
  void * removido = (void*) malloc(sizeof(void));
  Elemento * anterior;
  Elemento * atual;

  if(!vazia(lista)){
    if(posicao < lista->tamanho){
      anterior = lista->cabeca;
      atual = anterior->proximo;
      while(posicao){
        anterior = anterior->proximo;
        atual = atual->proximo;
        posicao--;
      }
      anterior->proximo = atual->proximo;
      *(int*)removido = *(int*)atual->conteudo;
      free(atual);
      lista->tamanho--;
      return removido;
    }
    else
      return NULL;
  }
  else
    return NULL;
}


void in(void * conteudo, Lista * fila){
  inserir(conteudo,0,fila);
}

void * out(Lista * fila){
  return remover(fila->tamanho-1,fila);
}

void * pop(Lista * pilha){
  return remover(0,pilha);
}

void push(void * conteudo, Lista * pilha){
  inserir(conteudo,0,pilha);
}
