#ifndef _UNIVERSITY_H_
#define _UNIVERSITY_H_
#define _POSIX_C_SOURCE 200809L //Necessário para uso da função getline da stdio
#include "common.h"
#include <stdio.h>
#include <math.h>

// Constantes Gerais
#define INPUT_FILE_DELIMITERS ";\n"
#define INPUT_FILE_PATH "sample/input.txt"
#define DATE_FORMAT "%d/%m/%Y"

// Constantes de mapeamento
#define DATA_MAPPING 0
#define SIZE_MAPPING 1

// Quantidade de campos do arquivo de inicialização
#define NUMBER_FIELDS 10
#define NUMBER_OF_VARIABLE_SIZE_FIELDS 6 // Existem 6 campos de tamanho variável

// Campos do Arquivo de Inicialização

#define MATRICULA 0 // Tamanho Fixo, Obrigatório
#define NOME      1 // Tamanho Variável, Obrigatório // dado 0, tamanho 0
#define SEXO      2 // Tamanho Fixo, Obrigatório
#define CPF       3 // Tamanho Fixo, Obrigatório // dado 1, tamanho -
#define RG        4 // Tamanho Variável, Obrigatório // dado 2, tamanho 1
#define TELEFONE  5 // Tamanho Variável, Opcional // dado 3, tamanho 2
#define CELULAR   6 // Tamanho Variável, Opcional // dado 4, tamanho 3
#define EMAIL     7 // Tamanho Variável, Opcional // dado 5, tamanho 4
#define ENDERECO  8 // Tamanho Variável, Obrigatório // dado 6, tamanho 5
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
  time_t data_nascimento; //Armazeado como timestamp

  // Campos Obrigatórios de Tamanho Variável
  string_t endereco;
  string_t nome;
  string_t rg;

  // Campos Opcionais de Tamanho Variável
  string_t telefone;
  string_t celular;
  string_t email;

} binary_data_register;

//Função para mapear os índices de um vetor nos campos do registro do arquivo de dados binário ou no vetor de tamanhos variáveis
int mapping_function(small_t source, small_t mapping_type);

//Função que realiza o mapeamento entre o vetor e os registros do arquivo de dados binário
boolean_t data_mapping(string_t ** str_fields, binary_data_register * data_register);

//Armazena um token lido do arquivo de entrada de acordo com o campo ao qual corresponde (mapeamento)
boolean_t store(string_t token, small_t position, string_t ** mapping, binary_data_register * data_register);

//Carrega o arquivo de entradas, gerando o arquivo de dados e de índice
boolean_t load_input_file(string_t path_input_file, FILE * data_file, FILE * index_file);

#endif
