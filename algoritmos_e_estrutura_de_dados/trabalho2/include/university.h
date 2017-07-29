#ifndef _UNIVERSITY_H_
#define _UNIVERSITY_H_
#define _POSIX_C_SOURCE 200809L //Necessário para uso da função getline da stdio
#include "common.h"
#include "btree.h"
#include <math.h>

// Constantes Gerais
#define INPUT_FILE_DELIMITERS ";\n"
#define INPUT_FILE_PATH "sample/input.txt"
#define DATA_FILE "data.db"
#define INDEX_FILE "index.db"
#define DATE_FORMAT "%d/%m/%Y"
#define BYTE_OFFSET_0 0 // deslocamento de 0 bytes, inválido e não-negativo

// Modos de Abertura de Arquivos
#define INPUT_FILE_OPEN_MODE "r"
#define BINARY_DATA_CREATION_MODE "w"

// Índices do vetor de modificações de buffer
#define DATA_FILE_HEADER_MODIFICATION    0
#define DATA_FILE_REGISTER_MODIFICATION  1

// Deslocamento entre posição dos campos no arquivo de entrada e vetor de tamanhos
#define NOME_OFFSET -1
#define OTHERS_OFFSET -3

// Quantidade de campos do arquivo de inicialização
#define NUMBER_FIELDS 10
#define NUMBER_OF_VARIABLE_SIZE_FIELDS 6 // Existem 6 campos de tamanho variável
#define NUMBER_OF_STRING_FIELDS 7 // Existem 7 campos do tipo string

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

// Estrutura de Cabeçalho do Arquivo de Dados Binário
typedef struct binary_data_header {
  byte_offset_t free_head_register;  // Ponteiro para cabeça da lista de registros vazios
  small_t fragmentation_ratio; // Taxa de fragmentação externa do arquivo
} binary_data_header;

// Estrutura de Registros do Arquivo de Dados Binário
typedef struct binary_data_register {

  //************************************* TAMANHO FIXO ************************************************************
  // Tamanhos
  data_register_size_t register_size; // Tamanho do registro, capaz de comportar o tamanho máximo teórico de 435 bytes
  small_t variable_field_size[NUMBER_OF_VARIABLE_SIZE_FIELDS];  // Tamanhos dos registros de tamanho variável

  // Campos Obrigatórios
  unsigned short matricula; // Valor máximo de matrícula é 65535
  char sexo;
  string_t cpf;
  time_t data_nascimento; //Armazeado como timestamp

  // ************************************* TAMANHO VARIÁVEL *******************************************************
  // Campos Obrigatórios de Tamanho Variável
  string_t nome;
  string_t rg;
  string_t endereco;

  // Campos Opcionais de Tamanho Variável
  string_t telefone;
  string_t celular;
  string_t email;

} binary_data_register;

typedef struct data_file_t {
  FILE * file;
  boolean_t modification[2];
  binary_data_header * in_memory_header;
  binary_data_register * handled_register;
  byte_offset_t flush_offset;
} data_file_t;

// Copia um registro do arquivo de dados para outro em memória
boolean_t data_file_copy_register(binary_data_register * destination, binary_data_register * source);

// Libera a estrutura que representa o arquivo de dados em memória
boolean_t data_file_close(data_file_t ** data_file);

// Faz o flush de registros que tenham sido modificados apenas em memória para o arquivo de dados
boolean_t data_file_register_flush(data_file_t ** data_file);

// Cria a estrutura em memória que representa um arquivo de dados
data_file_t * create_data_file(string_t path, string_t creation_mode);

// Altera no header em memória o ponteiro a taxa de fragmentação do arquivo de dados
boolean_t data_file_set_fragmentation_ratio(small_t fragmentation_ratio, data_file_t ** data_file);

// Altera no header em memória o ponteiro para a lista de registros livres do arquivo de dados
boolean_t data_file_set_free_head_register(byte_offset_t free_head_register, data_file_t ** data_file);

// Faz o flush pro disco de um header de arquivo de dados modificado em memória mas ainda não persistido
boolean_t data_file_header_flush(data_file_t ** data_file);

// Escrita no arquivo de dados propriamente dita
byte_offset_t data_file_register_write(binary_data_register * data_register, data_file_t ** data_file);

// Empurra registros que ainda estejam em memória para o disco caso os mesmo ainda não tenham sido escritos
byte_offset_t data_file_register_push(binary_data_register * data_register, data_file_t ** data_file);

// Insere um registro no arquivo de dados e no arquivo de índice
boolean_t data_file_insert(binary_data_register * data_register, data_file_t ** data_file, b_tree_t ** index_file);

// Atribui o tamanho total do registro
boolean_t total_register_size_filling(binary_data_register * data_register);

// 'Freeing' dos campos do tipos string
boolean_t str_field_freeing(string_t * field);

// Preenchimento de campos do tipos string
boolean_t str_field_filling(string_t token, string_t * field, small_t * size);

// Preenche adequadamente o campo do registro de acordo com o token lido
boolean_t register_filling(string_t token, small_t position, binary_data_register * data_register);

// Carrega o arquivo de entradas, gerando o arquivo de dados e de índice
boolean_t load_input_file(string_t path_input_file, data_file_t ** data_file, b_tree_t ** index_file);

#endif
