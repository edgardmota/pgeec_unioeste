#include "tads.h"

#define TAMANHO 5

int main(void){
  int i;
  int j;
  int * n = (int *) malloc(sizeof(int)*TAMANHO);
  Fila * fila = cria_fila();
  Pilha * pilha = cria_pilha();
  Elemento * removido;

  for(i = 0;i < TAMANHO; i++){
      n[i] = i;
  }

  //Pilha\:
  printf("\nPilhas:\n\n");
  for(j = 0; j < 10; j++){
    for(i = 0;i < TAMANHO; i++){
        printf("Inserindo elemento %d... \n", n[i]);
        push(&n[i],pilha);
    }
    for(i = 0;i < TAMANHO; i++){
        printf("Removendo elemento %d...\n",*(int*)pop(pilha)->conteudo);
    }
  }
  //Fila\:
  printf("\nFila:\n\n");
  for(j = 0; j < 10; j++){
    for(i = 0;i < TAMANHO; i++){
        printf("Inserindo elemento %d... \n", n[i]);
        in(&n[i],fila);
    }
    for(i = 0;i < TAMANHO; i++){
        printf("Removendo elemento %d...\n",*(int*)out(fila)->conteudo);
    }
  }
  return 0;
}
