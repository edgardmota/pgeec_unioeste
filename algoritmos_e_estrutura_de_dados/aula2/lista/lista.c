#include "lista.h"

no_estatico * cria_no_estatico(int chave){
  no_estatico * n = (no_estatico *) malloc(sizeof(no_estatico));
  n->chave = chave;
  return n;
}

lista_estatica * cria_lista_estatica(){
  lista_estatica * l = (lista_estatica *) malloc(sizeof(lista_estatica));
  l->primeiro = PRIMEIRO;
  l->ultimo = PRIMEIRO;
  return l;
}

no_estatico * busca_lista_estatica(int chave, lista_estatica * l){
  int i;
  no_estatico * no = NULL;

  i = l->primeiro;
  while((i < l->ultimo) && (!no)){
    if (l->elementos[i]->chave == chave)
      no = l->elementos[i];
    i = i + 1;
  }
  return no;
}

unsigned int checa_posicao_valida_lista_estatica(int pos, lista_estatica * l){
  return (((pos == PRIMEIRO) && checa_vazia_lista_estatica(l)) || (pos >= l->primeiro) && (pos < l->ultimo));
}

unsigned int checa_cheia_lista_estatica(lista_estatica * l){
  return (l->ultimo >= MAX_TAM);
}

unsigned int checa_vazia_lista_estatica(lista_estatica * l){
  return (l->ultimo == l->primeiro);
}

unsigned int desloca_elementos_lista_estatica(int pos, int direcao, lista_estatica * l){
  if(direcao == PARA_TRAS){
    for(int i=pos; i < l->ultimo; i++)
      l->elementos[i] = l->elementos[i+1];
    l->ultimo--;
    return TRUE;
  }
  else if(direcao == ADIANTE){
    for(int i=l->ultimo; i > pos; i--)
      l->elementos[i] = l->elementos[i-1];
    l->ultimo++;
    return TRUE;
  }
  else
    return FALSE;
}

unsigned int insere_lista_estatica(int chave, int pos, lista_estatica * l){
  if((!checa_cheia_lista_estatica(l)) && (checa_posicao_valida_lista_estatica(pos,l))){
    desloca_elementos_lista_estatica(pos,ADIANTE,l);
    l->elementos[pos] = cria_no_estatico(chave);
    return TRUE;
  }
  else
    return FALSE;
}

no_estatico * remove_lista_estatica(int pos, lista_estatica * l){
  no_estatico * removido;
  if((!checa_vazia_lista_estatica(l)) && (checa_posicao_valida_lista_estatica(pos,l))){
    removido = l->elementos[pos];
    desloca_elementos_lista_estatica(pos,PARA_TRAS,l);
    return removido;
  }
  else
    return NULL;
}

void imprime_lista_estatica(lista_estatica * l){
  if(checa_vazia_lista_estatica(l))
    printf("Lista vazia!\n");
  else{
    for(int i = l->primeiro; i < l->ultimo; i++)
      printf("%d ", l->elementos[i]->chave);
      printf("\n");
  }
}
