#ifndef _BTREE_H_
#define _BTREE_H_
#include "common.h"

#define B_TREE_ORDER 5

// Índices do vetor de modificações de buffer
#define B_TREE_HEADER_MODIFICATION    0
#define B_TREE_REGISTER_MODIFICATION  1

// Estrutura de Dados utilizada pela Árvore B
typedef struct data {
  unsigned short matricula;
  byte_offset_t byte_offset;
} DATA;

//Estrutura do Cabeçalho do Arquivo de Índice (Árvore B)
typedef struct binary_index_header {

  //Ponteiros
  byte_offset_t free_head_register; // Cabeça da lista de registros vazios
  byte_offset_t root_register; // Raiz da árvore B

} binary_index_header;

//Estrutura de Registros do Arquivo de Índice (Árvore B)
typedef struct binary_index_register {
  small_t number_keys; //Número de chaves na página
  DATA page[B_TREE_ORDER]; //Página da árvore (overflow permitido p/ simplificação)
  byte_offset_t child[B_TREE_ORDER + 1]; //Ponteiros para filhos
} binary_index_register;

typedef struct b_tree_t {
  FILE * file;
  boolean_t modification[2];
  binary_index_header * in_memory_header;
  binary_index_register * handled_register;
  byte_offset_t flush_offset;
} b_tree_t;

//Inserção na árvore B
boolean_t b_tree_insert(DATA * data, b_tree_t ** index_file);

//Criação de um dado da árvore B
DATA * create_btree_data(unsigned short matricula, byte_offset_t byte_offset);

#endif
