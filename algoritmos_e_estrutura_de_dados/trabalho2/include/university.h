#ifndef _UNIVERSITY_H_
#define _UNIVERSITY_H_
#define _POSIX_C_SOURCE 200809L //Necessário para uso da função getline da stdio
#include "common.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Constantes Gerais
#define NONE -1
#define INPUT_FILE_DELIMITERS ";\n"
#define INPUT_FILE_PATH "sample/input.txt"

#define NUMBER_FIELDS 10
#define NUMBER_OF_VARIABLE_SIZE_FIELDS 7 // Existem 6 campos de tamanho variável

// Campos do Arquivo de Inicialização

#define MATRICULA 0 // Tamanho Fixo, Obrigatório
#define NOME      1 // Tamanho Variável, Obrigatório // 0
#define SEXO      2 // Tamanho Fixo, Obrigatório
#define CPF       3 // Tamanho Fixo, Obrigatório // 1
#define RG        4 // Tamanho Variável, Obrigatório // 2
#define TELEFONE  5 // Tamanho Variável, Opcional // 3
#define CELULAR   6 // Tamanho Variável, Opcional // 4
#define EMAIL     7 // Tamanho Variável, Opcional // 5
#define ENDERECO  8 // Tamanho Variável, Obrigatório // 6
#define DATA_NASC 9 // Tamanho Fixo, Obrigatório

// Estrutura de Dados utilizada pela Árvore B
typedef struct data {
  unsigned short matricula;
  byte_pointer_t pointer;
} DATA;

// Estrutura de Cabeçalho do Arquivo de Dados Binário
typedef struct binary_data_header {
  byte_pointer_t free_head_register;  // Ponteiro para cabeça da lista de registros vazios
  small_t fragmentation_ratio; // Taxa de fragmentação externa do arquivo
} binary_data_header;

// Estrutura de Registros do Arquivo de Dados Binário
typedef struct binary_data_register {

  // Tamanhos
  data_register_size_t register_size; // Tamanho do registro, capaz de comportar o tamanho máximo teórico de 435 bytes
  small_t variable_field_size[NUMBER_OF_VARIABLE_SIZE_FIELDS];  // Tamanhos dos registros de tamanho variável

  // Campos Obrigatórios
  unsigned short matricula; // Valor máximo de matrícula é 65535
  char sexo;
  string_t cpf;
  unsigned int data_nascimento; //Armazeado como timestamp

  // Campos Obrigatórios de Tamanho Variável
  string_t endereco;
  string_t nome;
  string_t rg;

  // Campos Opcionais de Tamanho Variável
  string_t telefone;
  string_t celular;
  string_t email;

} binary_data_register;

//Função para mapear os índices de um vetor nos campos do registro do arquivo de dados binário
int mapping_function(small_t source);

//Função que realiza o mapeamento entre o vetor e os registros do arquivo de dados binário
boolean_t mapping(string_t ** str_fields, binary_data_register * data_register);

//Armazena um token lido do arquivo de entrada de acordo com o campo ao qual corresponde
boolean_t store(string_t token, small_t position, binary_data_register * data_register);

//Carrega o arquivo de entradas, gerando o arquivo de dados e de índice
boolean_t load_input_file(string_t path_input_file, FILE * data_file, FILE * index_file);

#endif
