#include "tads.h"

#define TAMANHO_PILHA 5

int main(void){
  int i;
  int * n = (int *) malloc(sizeof(int)*TAMANHO_PILHA);
  Pilha * pilha = cria_pilha();
  Elemento * removido;

  for(i = 0;i < TAMANHO_PILHA; i++){
      n[i] = i;
  }
  for(i = 0;i < TAMANHO_PILHA; i++){
      printf("Inserindo elemento %d... \n", n[i]);
      push(&n[i],pilha);
  }
  for(i = 0;i < TAMANHO_PILHA; i++){
      printf("Removendo elemento %d...\n",*(int*)pop(pilha)->conteudo);
  }
  return 0;
}
