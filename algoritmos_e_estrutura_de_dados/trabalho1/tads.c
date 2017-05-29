#include "tads.h"

Elemento * criar_elemento(void * conteudo){
  Elemento * e = (Elemento *) malloc(sizeof(Elemento));
  e->conteudo = conteudo;
  e->proximo = NULL;
  return e;
}

int vazia(Lista * lista){
  return !lista->tamanho;
}

Lista * criar_lista(void){
  Lista * lista = (Lista *) malloc(sizeof(Lista));
  Elemento * cabeca = criar_elemento(NULL);
  lista->cabeca = cabeca;
  lista->tamanho = 0;
  return lista;
}

int inserir(void * conteudo, int posicao, Lista * lista){
  Elemento * e;
  Elemento * atual;

  if(posicao <= lista->tamanho){
      e = criar_elemento(conteudo);
      atual = lista->cabeca;
      while(posicao){
        atual = atual->proximo;
        posicao--;
      }
      e->proximo = atual->proximo;
      atual->proximo = e;
      lista->tamanho++;
      return TRUE;
  }
  else
    return FALSE;
}

void * remover(int posicao, Lista * lista){
  void * removido = (void*) malloc(sizeof(void));
  Elemento * anterior;
  Elemento * atual;

  if(!vazia(lista)){
    if(posicao < lista->tamanho){
      anterior = lista->cabeca;
      atual = anterior->proximo;
      while(posicao){
        anterior = anterior->proximo;
        atual = atual->proximo;
        posicao--;
      }
      anterior->proximo = atual->proximo;
      *(int*)removido = *(int*)atual->conteudo;
      free(atual);
      lista->tamanho--;
      return removido;
    }
    else
      return NULL;
  }
  else
    return NULL;
}


void in(void * conteudo, Lista * fila){
  inserir(conteudo,0,fila);
}

void * out(Lista * fila){
  return remover(fila->tamanho-1,fila);
}

void * pop(Lista * pilha){
  return remover(0,pilha);
}

void push(void * conteudo, Lista * pilha){
  inserir(conteudo,0,pilha);
}

void liberar(Lista * lista){
  while(pop(lista));
}

Livro * criar_livro(Lista * buffer_linha){
  char * campos[NUMERO_CAMPOS_TEXTO_LIVRO];
  Livro * livro = (Livro *) malloc(sizeof(Livro));
  int i;

  char * c = (char *) malloc(sizeof(char));
  char * buffer = (char *) malloc(sizeof(char));

  for(i = 0; i < NUMERO_CAMPOS_TEXTO_LIVRO; i++){
    strcpy(c,"");
    strcpy(buffer,"");
    do { // Leitura do Código do Livro
      strcat(buffer,c);
      c = out(buffer_linha);
    } while ((c != NULL) && (*c != ','));
    out(buffer_linha); // Descarta espaço
    if(i == 0)
      livro->codigo = atoi(buffer);
    else{
      campos[i-1] = (char *) malloc(sizeof(char)*strlen(buffer));
      strcpy(campos[i-1],buffer);
    }
  }
  livro->titulo = campos[0];
  livro->autor = campos[1];

  return livro;
}

char * duplicar_char(char * c){
  char * duplicado = (char *) malloc(sizeof(char));
  *duplicado = *c;
  return duplicado;
}

Lista * inicializar(char * nome_arquivo){
  FILE * arquivo;

  Lista * estantes = criar_lista();
  Lista * prateleiras = criar_lista();
  Lista * livros = criar_lista();
  Lista * buffer_linha = criar_lista();

  Lista * elemento_lido;
  int bytes_lidos;
  char * buffer_caracter = (char *) malloc(sizeof(char));
  Livro * livro;
  int modo_leitura;

  arquivo = fopen(nome_arquivo,MODO_ABERTURA);
  do {
    bytes_lidos = fread(buffer_caracter,TAMANHO_LEITURA,UNIDADES_LEITURA,arquivo);
    if(vazia(buffer_linha)){ // Começo da linha
      switch (*buffer_caracter) {
        case 'E': // Leitura da Estante
          modo_leitura = MODO_LEITURA_ESTANTE;
          break;
        case 'P': // Leitura da Prateleira
          modo_leitura = MODO_LEITURA_PRATELEIRA;
          prateleiras = criar_lista();
          push(estantes,prateleiras);
          break;
        default: // Leitura do Livro
          modo_leitura = MODO_LEITURA_LIVRO;
          livros = criar_lista();
          break;
      }
      in(duplicar_char(buffer_caracter),buffer_linha);
    }
    else {// Meio da linha
      if(*buffer_caracter != MARCADOR_EOL) {
          if(modo_leitura == MODO_LEITURA_LIVRO)
            in(duplicar_char(buffer_caracter),buffer_linha);
      }
      else{ // Fim de leitura da linha
        switch (modo_leitura) {
          case MODO_LEITURA_ESTANTE:
          case MODO_LEITURA_PRATELEIRA:
            break;
          case MODO_LEITURA_LIVRO:
            livro = criar_livro(buffer_linha);
            printf("%s\n",livro->titulo);
            push(livro,livros);
            break;
        }
        liberar(buffer_linha);
        buffer_linha = criar_lista();
      }
    }
  } while (bytes_lidos != 0);
  return estantes;
}
