#ifndef _BTREE_H_
#define _BTREE_H_
#include "common.h"

#define B_TREE_ORDER 5
#define MIN_PAGE_SIZE  (int) ((B_TREE_ORDER) / 2)

// Índices de filhos
#define LEFT     0
#define RIGHT    1

// Índices do vetor de modificações de buffer
#define B_TREE_HEADER_MODIFICATION    0
#define B_TREE_NODE_MODIFICATION      1

// Estrutura de Dados utilizada pela Árvore B
typedef struct data {
  unsigned short matricula;
  byte_offset_t byte_offset;
} DATA;

//Estrutura do Cabeçalho do Arquivo de Índice (Árvore B)
typedef struct b_tree_header {

  //Ponteiros
  byte_offset_t free_head_register; // Cabeça da lista de registros vazios
  byte_offset_t root_register; // Raiz da árvore B

} b_tree_header;

//Estrutura de Registros do Arquivo de Índice (Árvore B)
typedef struct b_tree_node {
  small_t number_keys; //Número de chaves na página
  DATA page[B_TREE_ORDER]; //Página da árvore (overflow permitido p/ simplificação)
  byte_offset_t child[B_TREE_ORDER + 1]; //Ponteiros para filhos
} b_tree_node;

typedef struct b_tree_t {
  FILE * file;
  boolean_t modification[2];
  b_tree_header * in_memory_header;
  b_tree_node * handled_node;
  byte_offset_t flush_offset;
} b_tree_t;

//Busca na árvore binária
byte_offset_t b_tree_search(b_tree_t ** b_tree, unsigned short matricula);

// Faz flush do nó em memória para o disco
boolean_t b_tree_node_flush(b_tree_t ** b_tree);

//Realiza a cópia de um dado de origem para um de destino
boolean_t data_copy(DATA * destination, DATA * source);

//Seta valores iniciais para um nó recém-criado
boolean_t b_tree_make_node(b_tree_t ** b_tree, DATA * data, byte_offset_t * children);

//Escreve o nó da árvore binária em disco
byte_offset_t b_tree_node_write(b_tree_t ** b_tree_t);

//Cria um novo nó na árvore binária
byte_offset_t b_tree_new_node(DATA * data, byte_offset_t * children, b_tree_t ** b_tree);

//Busca binária na página com determinado byte-offset
int binary_search(b_tree_t ** b_tree, byte_offset_t offset, DATA * data);

//Inserção de dados na página com determinado byte-offset
boolean_t b_tree_insert_data(b_tree_t ** b_tree, byte_offset_t offset, DATA * data, byte_offset_t right_child);

//Obtém o número de chaves com o byte_offset informado da árvore binária
small_t b_tree_get_number_of_keys(b_tree_t ** b_tree, byte_offset_t offset);

//Obtém dado para o nó com o byte_offset informado da árvore binária
DATA * b_tree_get_data(b_tree_t ** b_tree, byte_offset_t offset, int i);

//Obtém filho para o nó com o byte_offset informado da árvore binária
byte_offset_t b_tree_get_child(b_tree_t ** b_tree, byte_offset_t offset, int i);

//Seta o número de chaves para o nó com o byte_offset informado da árvore binária
boolean_t b_tree_set_number_of_keys(b_tree_t ** b_tree, byte_offset_t offset, small_t number_keys);

//Seta um dado para o nó com o byte_offset informado da árvore binária
boolean_t b_tree_set_data(b_tree_t ** b_tree, byte_offset_t offset, int i, DATA * data);

//Seta um filho para o nó com o byte_offset informado da árvore binária
boolean_t b_tree_set_child(b_tree_t ** b_tree, byte_offset_t offset, int i, byte_offset_t child);

//Realiza a busca do nó para inserir a chave e faz as subdivisões quando necessárias
byte_offset_t b_tree_insert_aux(b_tree_t ** b_tree, byte_offset_t root, DATA * data, boolean_t * h, DATA * return_data);

//Lê um nó da árvore binária do disco
boolean_t b_tree_dereference(b_tree_t ** btree, byte_offset_t offset);

// Faz a liberação em memória da estrutura da árvore B
boolean_t b_tree_close(b_tree_t ** b_tree);

// Obtém o byte-offset para a raiz da árvore B
byte_offset_t b_tree_get_root(b_tree_t ** b_tree);

// Grava em disco o header da árvore B caso o mesmo tenha sido modificado em memória
boolean_t b_tree_header_flush(b_tree_t ** b_tree);

// Seta o byte-offset para o primeiro nó da árvore B (nó raiz)
boolean_t b_tree_set_root_register(byte_offset_t root_register, b_tree_t ** b_tree);

// Seta o byte-offset para o primeiro nó livre na lista de nós livres do arquivo
boolean_t b_tree_set_free_head_register(byte_offset_t free_head_register, b_tree_t ** b_tree);

// Criação da estrutura que representa a árvore B em memória
b_tree_t * create_b_tree(string_t path, string_t creation_mode);

// Obtém o byte-offset para o primeiro nó livre na lista de nós livres do arquivo
byte_offset_t b_tree_get_free_head_register(b_tree_t ** b_tree);

//Inserção na árvore B
boolean_t b_tree_insert(DATA * data, b_tree_t ** b_tree);

//Criação de um dado da árvore B
DATA * b_tree_create_data(unsigned short matricula, byte_offset_t byte_offset);

#endif
