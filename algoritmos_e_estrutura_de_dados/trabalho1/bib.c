#include "bib.h"

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

int encontrar_posicao(int codigo, Lista * livros){
  Elemento * atual;
  Elemento * anterior;
  int posicao = 0;
  int max_posicao_valida = livros->tamanho - 1;

  anterior = livros->cabeca;
  atual = anterior->proximo;
  while((atual != NULL) && (((Livro *)(atual->conteudo))->codigo < codigo)){
    anterior = atual;
    atual = atual->proximo;
    posicao++;
  }
  return posicao;
}

Lista * buscar_prateleira(Livro * livro, Lista * estantes){
  int n_prateleira;
  int n_estante;
  int codigo;

  codigo = livro->codigo;
  n_prateleira = livro->endereco.prateleira;
  n_estante = livro->endereco.estante;
  return (Lista *) get(n_prateleira,(Lista *) get(n_estante,estantes)->conteudo)->conteudo;
}

Livro * buscar_livro(int codigo, Lista * livros){
  Elemento * elemento = get(encontrar_posicao(codigo,livros),livros);
  if(elemento)
    return (Livro *) elemento->conteudo;
  else
    return NULL;
}

int inserir_livro(Livro * livro, Lista * prateleira, Lista * livros){
  inserir(livro,encontrar_posicao(livro->codigo,prateleira),prateleira);
  inserir(livro,encontrar_posicao(livro->codigo,livros),livros);
  return TRUE;
}

Livro * remover_livro(Livro * livro, Lista * estantes, Lista * livros){
  Lista * prateleira = buscar_prateleira(livro,estantes);

  remover(encontrar_posicao(livro->codigo,prateleira),prateleira);
  return (Livro *) remover(encontrar_posicao(livro->codigo,livros),livros);
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
      removido = atual->conteudo;
      free(atual);
      lista->tamanho--;
    }
  }
  return removido;
}


void in(void * conteudo, Lista * fila){
  inserir(conteudo,fila->tamanho,fila);
}

void * out(Lista * fila){
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

Livro * criar_livro(int codigo, char * titulo, char * autor, int n_estante, int n_prateleira){
  Livro * livro = (Livro *) malloc(sizeof(Livro));

  livro->codigo = codigo;
  livro->titulo = titulo;
  livro->autor = autor;
  livro->endereco.estante = n_estante;
  livro->endereco.prateleira = n_prateleira;
  return livro;
}

Livro * parsear_livro(Lista * buffer_linha){
  int i;
  int codigo;
  char * campos[NUMERO_CAMPOS_LIVRO-1];
  Livro * livro = NULL;
  char * caracter = NULL;
  char * buffer;
  int maior_string = 0;
  int acumulado;

  if(buffer_linha){
    buffer = (char *) malloc(sizeof(char));
    for(i = 0; i < NUMERO_CAMPOS_LIVRO; i++){
      *buffer = '\0';
      caracter = (char *) out(buffer_linha);
      while ((caracter != NULL) && (*caracter != ',')){
        acumulado = strlen(buffer) + 1;
        if(acumulado > maior_string){
          maior_string = acumulado;
          buffer = realloc(buffer, acumulado);
        }
        strncat(buffer,caracter,1);
        caracter = (char *) out(buffer_linha);
      }
      out(buffer_linha); // Descarta espaços
      if(i == 0)// Armazena o código
        codigo = atoi(buffer);
      else{// Processa campos texto
        campos[i-1] = (char *) malloc(sizeof(char)*strlen(buffer));
        strcpy(campos[i-1],buffer);
      }
    }
  }
  free(buffer);
  return criar_livro(codigo,campos[CAMPO_TITULO],campos[CAMPO_AUTOR],-1,-1);
}

char * duplicar_char(char * c){
  char * duplicado = (char *) malloc(sizeof(char));
  *duplicado = *c;
  return duplicado;
}

int * fila_para_numero(Lista * fila){
  int * numero = (int *) malloc(sizeof(int));
  char * buffer = (char *) malloc(sizeof(char)*fila->tamanho);
  char * caracter = NULL;

  strcpy(buffer,"");
  caracter = (char *) out(fila);
  do{
    strncat(buffer,caracter,1);
    caracter = (char *) out(fila);
  } while (caracter != NULL);
  *numero = atoi(buffer);
  return numero;
}

Endereco_livro * buscar_endereco_livro(int codigo, Lista * estantes, Lista * livros){
  Livro * livro = buscar_livro(codigo,livros);
  Lista * prateleira;

  if((livro != NULL) && (codigo == livro->codigo)){
    prateleira = buscar_prateleira(livro,estantes);
    livro = buscar_livro(codigo,prateleira);
    if((livro != NULL) && (codigo == livro->codigo))
      return &livro->endereco;
    else
      return NULL;
  }
  else
    return NULL;
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
      return parsear_livro(buffer_linha);
      break;
  }
}

int precisa_processar(int novo_lido, Lista * * pendentes){
  int retorno = FALSE;
  if((novo_lido == ESTANTE) && (!vazia(pendentes[ESTANTE]))){
    retorno = TRUE;
  } else if((novo_lido == PRATELEIRA) && (!vazia(pendentes[PRATELEIRA]))){
    retorno = TRUE;
  }
  return retorno;
}

void processar(Lista * * pendentes, int novo_lido, Lista * processadas, Lista * estantes, Lista * livros){
  Lista * prateleira;
  Lista * prateleiras;
  Livro * livro;
  int n_estante;
  int n_prateleira;

  n_estante = *(int *)topo(pendentes[ESTANTE])->conteudo;
  if(!vazia(pendentes[LIVROS])){
    n_prateleira = *(int *)pop(pendentes[PRATELEIRA]);
    prateleira = criar_lista();
    while(!vazia(pendentes[LIVROS])){
        livro = (Livro *)pop(pendentes[LIVROS]);
        livro->endereco.estante = n_estante;
        livro->endereco.prateleira = n_prateleira;
        inserir_livro(livro,prateleira,livros);
    }
    in(prateleira,processadas);
  }
  if(!vazia(pendentes[PRATELEIRA])){
    pop(pendentes[PRATELEIRA]);
    in(criar_lista(),processadas);
  }
  if(novo_lido == ESTANTE){
      prateleiras = criar_lista();
      while(!vazia(processadas)){
        in(out(processadas),prateleiras);
      }
      pop(pendentes[ESTANTE]);
      in(prateleiras,estantes);
  }
  return;
}

void gravar_livros(Lista * livros,FILE * arquivo){
  int codigo;
  char * titulo;
  int tamanho_titulo;
  char * autor;
  int tamanho_autor;
  Endereco_livro * endereco;
  Elemento * cursor;

  cursor = topo(livros);
  while(cursor != NULL){
    codigo = ((Livro *)(cursor->conteudo))->codigo;

    fwrite(&codigo,sizeof(codigo),1,arquivo);

    titulo = ((Livro *)(cursor->conteudo))->titulo;
    tamanho_titulo = strlen(titulo);
    fwrite(&tamanho_titulo,sizeof(tamanho_titulo),1,arquivo);
    fwrite(titulo,sizeof(*titulo),tamanho_titulo,arquivo);

    autor = ((Livro *)(cursor->conteudo))->autor;
    tamanho_autor = strlen(autor);
    fwrite(&tamanho_autor,sizeof(tamanho_autor),1,arquivo);
    fwrite(autor,sizeof(*autor),tamanho_autor,arquivo);

    endereco = &((Livro *)(cursor->conteudo))->endereco;
    fwrite(endereco,sizeof(*endereco),1,arquivo);

    cursor = cursor->proximo;
  }
  fclose(arquivo);
  return;
}

void gravar_estantes(Lista * estantes,FILE * arquivo){
  Elemento * cursor;
  int tamanho;

  cursor = topo(estantes);
  while(cursor != NULL){
    tamanho = ((Lista*)cursor->conteudo)->tamanho;
    fwrite(&tamanho,sizeof(tamanho),1,arquivo);
    cursor = cursor->proximo;
  }
  fclose(arquivo);
  return;
}

void persistir_em_arquivo(Lista * estantes, char * path_estantes, Lista * livros, char * path_livros){
  FILE * arquivo_estantes_db;
  FILE * arquivo_catalogo_db;

  arquivo_estantes_db = fopen(path_estantes,MODO_ABERTURA_BINARIOS);
  gravar_estantes(estantes,arquivo_estantes_db);
  arquivo_catalogo_db = fopen(path_livros,MODO_ABERTURA_BINARIOS);
  gravar_livros(livros,arquivo_catalogo_db);
  return;
}

void inicializar(char * nome_arquivo, Lista * estantes, Lista * livros){
  FILE * arquivo_catalogo_db;
  FILE * arquivo_estantes_db;
  FILE * arquivo_inicializacao;
  Lista * buffer_linha = criar_lista();
  Lista * processadas = criar_lista();
  Lista * pendentes[3];
  Lista * sendo_lido;
  Livro * livro;
  Elemento * cursor;

  char * caracter = (char *) malloc(sizeof(char));
  int bytes_lidos;
  int i;

  for(i = ESTANTE; i <= LIVROS; i++){
    pendentes[i] = criar_lista();
  }

  arquivo_inicializacao = fopen(nome_arquivo,MODO_ABERTURA_INICIALIZACAO);
  bytes_lidos = fread(caracter,TAMANHO_LEITURA,UNIDADES_LEITURA,arquivo_inicializacao);
  while (bytes_lidos != 0){
    if(vazia(buffer_linha)){ // Começo da linha
      switch (*caracter){
        case 'E': // Leitura da Estante
          if(precisa_processar(ESTANTE,pendentes))
            processar(pendentes,ESTANTE,processadas,estantes,livros);
          sendo_lido = pendentes[ESTANTE];
          break;
        case 'P': // Leitura da Prateleira
          if(precisa_processar(PRATELEIRA,pendentes))
            processar(pendentes,PRATELEIRA,processadas,estantes,livros);
          sendo_lido = pendentes[PRATELEIRA];
          break;
        default: // Leitura do Livro
          sendo_lido = pendentes[LIVROS];
          break;
      }
      in(duplicar_char(caracter),buffer_linha);
    }
    else {// Meio da linha
      if(*caracter != MARCADOR_EOL){
        in(duplicar_char(caracter),buffer_linha);
      }
      else {
        push(formatar(buffer_linha),sendo_lido);
        liberar(buffer_linha);
      }
    }
    bytes_lidos = fread(caracter,TAMANHO_LEITURA,UNIDADES_LEITURA,arquivo_inicializacao);
  }
  fclose(arquivo_inicializacao);
  processar(pendentes,ESTANTE,processadas,estantes,livros);
  return;
}
