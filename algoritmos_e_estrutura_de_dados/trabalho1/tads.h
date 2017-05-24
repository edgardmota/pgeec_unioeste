#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

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

typedef struct estante {
  int numero;
  Livro * prateleira;
} Estante;

typedef struct elemento {
  void * conteudo;
  struct elemento * proximo;
} Elemento;

typedef struct pilha {
  Elemento * cabeca;
  Elemento * topo;
  int tamanho;
} Pilha;

typedef struct fila {
  Elemento * cabeca;
  Elemento * primeiro;
  Elemento * ultimo;
  int tamanho;
} Fila;

typedef struct sala {
  int ra;
  Pilha * pilha;
} Sala;

typedef struct Salas {
  Pilha * salas_livres;
  Pilha * salas_ocupadas;
  Fila * fila;
} Salas;

//Funções de Apoio
Elemento * cria_elemento(void * conteudo);

//Pilha
Pilha * cria_pilha(void);
Elemento * pop(Pilha * pilha);
void push(void * conteudo, Pilha * pilha);
int pilha_vazia(Pilha * pilha);

//Fila
Fila * cria_fila(void);
Elemento * out(Fila * fila);
void in(void * conteudo, Fila * fila);
int fila_vazia(Fila * fila);

//Funcionalidades da Bliblioteca
void inicializa(char * nome_arquivo, Estante * estantes);
int inserir_livro(Livro * livro, Estante * estantes);
Livro * retirar_livro(int codigo, Estante * estantes, Salas * salas);
Endereco_livro * buscar_endereco_livro(int codigo, Estante * estantes);
int locar_sala(int ra, Sala * salas); //Falta o retorno
int emprestar_livro(int ra, int codigo, Estante * estantes, Sala * salas);
int liberar_sala(int ra, Sala * salas, Estante * estantes);
void imprimir_mapa_de_estantes(Estante * estantes);
void imprimir_mapa_de_salas(Sala * salas);
void imprimir_fila_de_espera_de_sala(Sala * salas);
