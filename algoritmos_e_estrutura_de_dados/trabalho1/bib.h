#ifndef BIB_H__
#define BIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Constantes Gerais

#define TRUE 1
#define FALSE 0

//Constantes Pendentes

#define ESTANTE 0
#define PRATELEIRA 1
#define LIVROS 2

//Constantes Arquivos

#define MODO_LEITURA_INICIALIZACAO "r"
#define MODO_ESCRITA_BINARIOS "w+b"
#define MODO_LEITURA_BINARIOS "r+b"

#define MARCADOR_EOL '\n'
#define TAMANHO_LEITURA 1
#define UNIDADES_LEITURA 1

#define ARQUIVO_INICIALIZACAO "inicializa.txt"
#define ARQUIVO_CATALOGO "catalogo.db"
#define ARQUIVO_ESTANTES "estantes.db"

#define NUMERO_CAMPOS_LIVRO 3

#define CAMPO_TITULO 0
#define CAMPO_AUTOR 1

//Constantes Biblioteca

#define RA_NULO -1
#define N_SALAS 3

typedef struct elemento {
  void * conteudo;
  struct elemento * proximo;
} Elemento;

typedef struct lista {
  Elemento * cabeca;
  int tamanho;
} Lista;

//Estruturas Biblioteca

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

typedef struct Emprestimo {
  int ra;
  Livro * livro;
} Emprestimo;

typedef struct sala {
  int ra;
  Lista * pilha;
} Sala;

typedef struct Salas {
  Lista * livres;
  Lista * ocupadas;
  Lista * fila;
} Salas;

//Funções Listas

Elemento * criar_elemento(void * conteudo);
int vazia(Lista * lista);
Lista * criar_lista(void);
int inserir(void * conteudo, int posicao, Lista * lista);
void * remover(int posicao, Lista * lista);
void in(void * conteudo, Lista * fila);
void * out(Lista * fila);
void * pop(Lista * pilha);
void push(void * conteudo, Lista * pilha);
void liberar(Lista * lista);
Elemento * get(int posicao, Lista * lista);
Elemento * topo(Lista * lista);

//Funções Biblioteca

Livro * parsear_livro(Lista * buffer_linha);
char * duplicar_char(char * c);
int * fila_para_numero(Lista * fila);
void * formatar(Lista * buffer_linha);
int encontrar_posicao(int codigo, Lista * livros);
int precisa_processar(int novo_lido, Lista * * pendentes);
void processar(Lista * * pendentes, int novo_lido, Lista * processadas, Lista * estantes, Lista * livros);
void persistir_em_arquivo(Lista * estantes, char * path_estantes, Lista * livros, char * path_livros);
void gravar_estantes(Lista * estantes,char * path_estantes);
void gravar_livros(Lista * livros,char * path_livros);
Livro * buscar_livro(int codigo, Lista * livros, Lista * estantes);
Lista * buscar_prateleira(Livro * livro, Lista * estantes);
Livro * remover_livro(Livro * livro, Lista * estantes, Lista * livros);
Livro * criar_livro(int codigo, char * titulo, char * autor, int n_estante, int n_prateleira);
void carregar_arquivos(char * path_estantes, Lista * estantes, char * path_livros, Lista * livros);
void carregar_livros(char * path_livros, Lista * livros, Lista * estantes);
void carregar_estantes(char * path_estantes, Lista * estantes);
Sala * criar_sala();
Salas * criar_salas(int n_salas);
Emprestimo * criar_emprestimo(int ra, Livro * livro);
int remover_prateleira(Livro * livro, Lista * estantes);
int inserir_prateleira(Livro * livro, Lista * estantes);
Sala * sala_usada(int ra, Lista * ocupadas, int liberar);

void inicializar(char * nome_arquivo, Lista * estantes, Lista * livros);
int inserir_livro(Livro * livro, Lista * prateleira, Lista * livros);
Livro * retirar_livro(int codigo, Lista * estantes, Salas * salas);
Endereco_livro * buscar_endereco_livro(int codigo, Lista * estantes, Lista * livros);
int locar_sala(Emprestimo * emprestimo, Salas * salas);
int emprestar_livro(int ra, int codigo, Lista * estantes, Lista * livros, Salas * salas);
int liberar_sala(int ra, Salas * salas, Lista * estantes);
void imprimir_mapa_de_estantes(Lista * estantes);
void imprimir_mapa_de_salas(Salas * salas);
void imprimir_fila_de_espera_de_sala(Salas * salas);

#endif
