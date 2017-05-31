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
  //inserir(conteudo,0,fila);
  inserir(conteudo,fila->tamanho,fila);
}

void * out(Lista * fila){
  //return remover(fila->tamanho-1,fila);
  return remover(0,fila);
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

Elemento * get(int posicao, Lista * lista){
  Elemento * atual;
  Elemento * retornado = NULL;

  if((!vazia(lista)) && (posicao < lista->tamanho)){
    atual = lista->cabeca->proximo;
    while(posicao){
      atual = atual->proximo;
      posicao--;
    }
    retornado = atual;
  }
  return retornado;
}

Elemento * topo(Lista * lista){
  return get(0,lista);
}

Livro * criar_livro(Lista * buffer_linha){
  int i;
  char * campos[NUMERO_CAMPOS_TEXTO_LIVRO];
  Livro * livro = NULL;
  char * caracter;
  char * buffer;

  if(buffer_linha){
    Livro = (Livro *) malloc(sizeof(Livro));
    caracter = (char *) malloc(sizeof(char));
    buffer = (char *) malloc(sizeof(char));
    for(i = 0; i < NUMERO_CAMPOS_TEXTO_LIVRO; i++){
      strcpy(caracter,"");
      strcpy(buffer,"");
      do {
        strcat(buffer,caracter);
        caracter = out(buffer_linha);
      } while ((caracter != NULL) && (*caracter != ','));
      out(buffer_linha); // Descarta espaços
      if(i == 0)// Armazena o código
        livro->codigo = atoi(buffer);
      else{// Processa campos texto
        campos[i-1] = (char *) malloc(sizeof(char)*strlen(buffer));
        strcpy(campos[i-1],buffer);
      }
    }
    livro->titulo = campos[0];
    livro->autor = campos[1];
  }
  return livro;
}

char * duplicar_char(char * c){
  char * duplicado = (char *) malloc(sizeof(char));
  *duplicado = *c;
  return duplicado;
}

int * fila_para_numero(Lista * fila){
  int * numero = (int *) malloc(sizeof(int));
  char * buffer = (char *) malloc(sizeof(char)*fila->tamanho);
  char * caracter;

  caracter = out(fila);
  do{
    strcat(buffer,caracter);
    caracter = out(fila);
  } while (caracter != NULL);
  *numero = atoi(buffer);
  return numero;
}

void * formatar(Lista * buffer_linha){
  char * caracter;

  caracter = topo(buffer_linha)->conteudo;
  switch (*caracter){
    case 'E':
    case 'P':
      out(buffer_linha); // descarta prefixo E ou P
      return fila_para_numero(buffer_linha);
      break;
    default:
      return criar_livro(buffer_linha);
      break;
  }
}

int algo_pendente(Lista * pendentes){
  return (!vazio(pendentes[ESTANTE]) || !vazio(pendentes[PRATELEIRA]) || !vazio(pendentes[LIVROS]));
}

void processar(Lista * pendentes, int novo_lido, Lista * estantes){
  Lista * prateleira;
  Lista * prateleiras;
  int n_prateleira;
  int n_estante;

  if(algo_pendente(pendentes)){
    prateleiras = criar_lista();
    n_estante = *(int *)pop(abertos[ESTANTE]);
    switch (novo_lido) {
      case ESTANTE:
        if(!vazio(pendentes[LIVROS])){
          n_prateleira = *(int *)pop(pendentes[PRATELEIRA]);
          prateleira = criar_lista();
          while(!vazio(pendentes[LIVROS])){
              pop(pendentes[LIVROS]);
          }
        }
          if(!vazia(pendentes[PRATELEIRA])){ //Prateleiras pendentes de processamento
            prateleira = pop(pendentes[PRATELEIRA]);
            while(prateleira){
              push(prateleira,prateleiras);
              prateleira = pop(pendentes[PRATELEIRA]);
            }
          }
          prateleira = criar_lista();
          livro = criar_livro(pop(pendentes[LIVROS]));
          while(livro){
            push(livro,prateleira);
            livro = criar_livro(pop(pendentes[LIVROS]));
          }
        } else if(!vazio(pendentes[PRATELEIRA])){

        } else {
        }
        break;
      case PRATELEIRA:
        if(!vazio(pendentes[LIVROS])){
          n_estante = *(int *)pop(pendentes[ESTANTE]);
          n_prateleira = *(int *)pop(pendentes[PRATELEIRA]);

        } else if(!vazio(aberto[PRATELEIRA])){

        } else {
        }
        break;
    }
  }
  return;








  return;
}

Lista * inicializar(char * nome_arquivo){
  FILE * arquivo;
  Lista * buffer_linha;
  Lista * estantes = criar_lista();
  Lista * pendentes[3];
  Lista * sendo_lido;
  Livro * livro;

  char * caracter;
  int bytes_lidos;
  int i;

  for(i = ESTANTE; i < LIVROS; i++){
    pendentes[i] = criar_lista();
  }

  arquivo = fopen(nome_arquivo,MODO_ABERTURA);
  do{
    bytes_lidos = fread(caracter,TAMANHO_LEITURA,UNIDADES_LEITURA,arquivo);
    if(vazia(buffer_linha)){ // Começo da linha
      switch (*caracter){
        case 'E': // Leitura da Estante
          processar(pendentes,ESTANTE,estantes);
          sendo_lido = pendentes[ESTANTE];
        case 'P': // Leitura da Prateleira
          processar_abertos(pendentes,PRATELEIRA,estantes);
          sendo_lido = pendentes[PRATELEIRA];
        default: // Leitura do Livro
          sendo_lido = pendentes[LIVROS];
          break;
      }
      in(duplicar_char(caracter),buffer_linha);
    }
    else {// Meio da linha
      if((*caracter != MARCADOR_EOL) && (*caracter != EOF)){
        in(duplicar_char(caracter),buffer_linha);
      }
      else { // Fim de leitura da linha
        push(formatar(buffer_linha),sendo_lido);
        liberar(buffer_linha);
      }
    }
  } while (bytes_lidos != 0);
  return estantes;
}
