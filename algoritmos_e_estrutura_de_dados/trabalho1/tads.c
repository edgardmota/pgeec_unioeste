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
  int inseriu = FALSE;

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
      inseriu = TRUE;
  }
  return inseriu;
}

void * remover(int posicao, Lista * lista){
  void * removido = NULL;
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
      removido = (void*) malloc(sizeof(void));
      *(int*)removido = *(int*)atual->conteudo;
      free(atual);
      lista->tamanho--;
    }
  }
  return removido;
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

Livro * criar_livro(Lista * buffer_linha, int n_estante, int n_prateleira){
  char * campos[NUMERO_CAMPOS_TEXTO_LIVRO];
  Livro * livro = (Livro *) malloc(sizeof(Livro));
  int i;

  char * caracter = (char *) malloc(sizeof(char));
  char * buffer = (char *) malloc(sizeof(char));

  for(i = 0; i < NUMERO_CAMPOS_TEXTO_LIVRO; i++){
    strcpy(caracter,"");
    strcpy(buffer,"");
    do { // Leitura do Código do Livro
      strcat(buffer,caracter);
      caracter = out(buffer_linha);
    } while ((caracter != NULL) && (*caracter != ','));
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
  livro->endereco.estante = n_estante;
  livro->endereco.prateleira = n_prateleira;

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
  Lista * estante_atual;
  int n_estante;

  Lista * prateleiras = criar_lista();
  Lista * prateleira_atual;
  int n_prateleira;

  Lista * livros = criar_lista();
  Lista * leitura_livros = criar_lista();
  Livro * livro;

  Lista * buffer_linha = criar_lista();
  Lista * elemento_lido;
  int bytes_lidos;
  char * caracter = (char *) malloc(sizeof(char));

  arquivo = fopen(nome_arquivo,MODO_ABERTURA);
  do{
    bytes_lidos = fread(caracter,TAMANHO_LEITURA,UNIDADES_LEITURA,arquivo);
    if(vazia(buffer_linha)){ // Começo da linha
      switch (*caracter){
        case 'E': // Leitura da Estante
          if(!vazia(leitura_livros)){ //Se houverem livros empilhados

            prateleira_atual = prateleiras->cabeca;
            n_prateleira = *((int)pop(prateleira_atual));

            estante_atual = estantes->cabeca;
            n_estante = *((int)pop(estante_atual))

            livro = pop(leitura_livros);
            while (livro){
                push(criar_livro(livro,n_estante,n_prateleira),prateleira_atual);
                livro = pop(leitura_livros);
            }
            push(prateleira_atual,estante_atual);
          }
          push(NULL,estantes);
          elemento_lido = leitura_estantes;
          break;
        case 'P': // Leitura da Prateleira
          if(!vazia(leitura_livros)){ //Se houverem livros empilhados

            prateleira_atual = prateleiras->cabeca;
            n_prateleira = *((int)pop(prateleira_atual));

            livro = pop(leitura_livros);
            while (livro){
                push(criar_livro(livro,n_estante,n_prateleira),prateleira_atual);
                livro = pop(leitura_livros);
            }
          }
          elemento_lido = leitura_prateleiras;
          break;
        default: // Leitura do Livro
          elemento_lido = leitura_livros;
          break;
      }
      in(duplicar_char(caracter),buffer_linha);
    }
    else {// Meio da linha
      if(*caracter != MARCADOR_EOL) {
        in(duplicar_char(caracter),buffer_linha);
      }
      else{ // Fim de leitura da linha
        liberar(buffer_linha);
        buffer_linha = criar_lista();
      }
    }
  } while (bytes_lidos != 0);
  return estantes;
}
