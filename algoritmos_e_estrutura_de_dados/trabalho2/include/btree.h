#ifndef _BTREE_H_
#define _BTREE_H_
#include "common.h"

#define B_TREE_DEGREE 4

//Estrutura do Cabeçalho do Arquivo de Índice (Árvore B)
typedef struct binary_index_header {

  //Ponteiros
  byte_pointer_t free_head_register; // Cabeça da lista de registros vazios
  byte_pointer_t root_register; // Raiz da árvore B

  small_t fragmentation_ratio; //Taxa de fragmentação externa do arquivo

} binary_index_header;

//Estrutura de Registros do Arquivo de Índice (Árvore B)
typedef struct binary_index_register {
  DATA page[B_TREE_DEGREE]; //Página da árvore
  byte_pointer_t [B_TREE_DEGREE]; //Ponteiros para filhos
} binary_index_register;


#endif
