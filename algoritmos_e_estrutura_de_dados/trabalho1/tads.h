#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define MODO_ABERTURA "r+b"
#define MARCADOR_EOL '\n'
#define TAMANHO_LEITURA 1
#define UNIDADES_LEITURA 1

#define MODO_LEITURA_PRATELEIRA 0
#define MODO_LEITURA_ESTANTE 1
#define MODO_LEITURA_LIVRO 2

typedef struct endereco_livro {
  int estante;
  int prateleira;
} Endereco_livro;

typedef struct livro {
  int codigo;
  char * titulo;
  char * autor;
  Endereco_livro endereco;
} Livro;

typedef struct elemento {
  void * conteudo;
  struct elemento * proximo;
} Elemento;

typedef struct lista {
  Elemento * cabeca;
  int tamanho;
} Lista;

typedef struct sala {
  int ra;
  Lista * pilha;
} Sala;

typedef struct Salas {
  Lista * salas_livres;
  Lista * salas_ocupadas;
  Lista * fila_espera;
} Salas;

//Funções de Apoio
Elemento * criar_elemento(void * conteudo);

//Lista
Lista * criar_lista(void);
int vazia(Lista * lista);
int inserir(void * conteudo, int posicao, Lista * lista);
void * remover(int posicao, Lista * lista);

//Pilha
void * pop(Lista * pilha);
void push(void * conteudo, Lista * pilha);

//Fila
void * out(Lista * fila);
void in(void * conteudo, Lista * fila);

//Funcionalidades da Biblioteca
char * monta_buffer_caracter(char caracter);

Lista * inicializar(char * nome_arquivo);
int inserir_livro(Livro * livro, Lista * estantes);
Livro * retirar_livro(int codigo, Lista * estantes, Salas * salas);
Endereco_livro * buscar_endereco_livro(int codigo, Lista * estantes);
int locar_sala(int ra, Sala * salas); //Falta o retorno
int emprestar_livro(int ra, int codigo, Lista * estantes, Sala * salas);
int liberar_sala(int ra, Sala * salas, Lista * estantes);
void imprimir_mapa_de_estantes(Lista * estantes);
void imprimir_mapa_de_salas(Sala * salas);
void imprimir_fila_de_espera_de_sala(Sala * salas);
