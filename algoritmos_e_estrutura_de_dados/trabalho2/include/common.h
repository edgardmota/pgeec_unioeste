#ifndef _COMMON_H_
#define _COMMON_H_
#define MALLOC_CHECK_ 3
#define _XOPEN_SOURCE //Para poder usar a função strptimeq:qq
#include <time.h>
#include <string.h>
#include <stdlib.h>

// Tipos definidos
typedef struct tm structured_time_t;
typedef char small_t;
typedef char * string_t;
typedef unsigned long long byte_pointer_t; // Tamanho do Maior Arquivo de Dados Possível: 28507725 (~ 3,9 MB)
typedef unsigned short data_register_size_t;
typedef unsigned char boolean_t;

//Constantes Gerais
#define NONE -1

// Constantes Booleanas
#define TRUE 1
#define FALSE 0

// Nó da lista de nós vazios
typedef struct empty_node {
  data_register_size_t size; // Tamanho do nó vazio
  byte_pointer_t next; // Ponteiro para próximo nó
} empty_node;

time_t str_to_timestamp(string_t date, string_t format);
string_t tokenizer(string_t string, string_t delimiters, int * control);

#endif
