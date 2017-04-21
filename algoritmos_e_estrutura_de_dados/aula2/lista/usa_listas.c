#include "lista.h"

#define BUSCADO 3

int main(void){
  no_estatico * removido;
  no_estatico * buscado;
  lista_estatica * lista = cria_lista_estatica();

  for(int i = PRIMEIRO; i< MAX_TAM; i++)
      insere_lista_estatica(i,lista->primeiro,lista);
  buscado = busca_lista_estatica(BUSCADO,lista);
  if (buscado)
    printf("O elemento %d foi encontrado\n", BUSCADO);
  else
    printf("O elemento %d não foi encontrado\n", BUSCADO);
  do {
      imprime_lista_estatica(lista);
      removido = remove_lista_estatica(lista->primeiro,lista);
      if(removido)
        printf("O elemento %d foi removido\n", removido->chave);
  } while(removido);
  return 0;
}
