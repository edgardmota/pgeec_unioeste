#include "tads.h"

#define TAMANHO 5

int main(void){
  Lista * linha = criar_lista();
  char * caracter = (char *) malloc(sizeof(char));
  *caracter = 'P';
  in(duplicar_char(caracter),linha);
  *caracter = '1';
  in(duplicar_char(caracter),linha);
  *caracter = '7';
  in(duplicar_char(caracter),linha);
  printf("Prateleira %d.\n",*((int *)processar_linha(linha)));
  return 0;
}
