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
  Elemento * elemento;
  Lista * prateleira = NULL;

  codigo = livro->codigo;
  n_prateleira = livro->endereco.prateleira;
  n_estante = livro->endereco.estante;
  elemento = get(n_estante,estantes);
  if(elemento){
    elemento = get(n_prateleira,(Lista *) elemento->conteudo);
    if(elemento){
      prateleira = (Lista *) elemento->conteudo;
    }
  }
  return prateleira;
}

Livro * buscar_livro(int codigo, Lista * livros, Lista * estantes){
  Livro * livro = NULL;
  Lista * prateleira;

  Elemento * elemento = get(encontrar_posicao(codigo,livros),livros);
  if(elemento){
    livro = (Livro *) elemento->conteudo;
    if(codigo == livro->codigo){
      prateleira = buscar_prateleira(livro,estantes);
      elemento = get(encontrar_posicao(codigo,prateleira),prateleira);
      if((elemento == NULL) || (((Livro *)elemento->conteudo)->codigo != livro->codigo))
        livro = NULL;
    }
    else {
      livro = NULL;
    }
  }
  return livro;
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
  int acumulado;

  if(buffer_linha){
    for(i = 0; i < NUMERO_CAMPOS_LIVRO; i++){
      buffer = (char *) malloc(sizeof(char));
      *buffer = '\0';
      caracter = (char *) out(buffer_linha);
      while ((caracter != NULL) && (*caracter != ',')){
        acumulado = strlen(buffer) + 1;
        buffer = realloc(buffer, acumulado);
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
      free(buffer);
    }
  }
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
  Livro * livro = buscar_livro(codigo,livros,estantes);

  if(livro)
    return &(livro->endereco);
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

void gravar_livros(Lista * livros,char * path_livros){
  int codigo;
  char * titulo;
  int tamanho_titulo;
  char * autor;
  int tamanho_autor;
  Endereco_livro * endereco;
  Elemento * cursor;
  FILE * arquivo;
  Livro * livro;

  arquivo = fopen(path_livros,MODO_ESCRITA_BINARIOS);
  cursor = topo(livros);
  while(cursor != NULL){
    livro = (Livro *) cursor->conteudo;

    codigo = livro->codigo;
    fwrite(&codigo,sizeof(codigo),1,arquivo);

    titulo = livro->titulo;
    tamanho_titulo = strlen(titulo);
    fwrite(&tamanho_titulo,sizeof(tamanho_titulo),1,arquivo);
    fwrite(titulo,sizeof(*titulo),tamanho_titulo,arquivo);

    autor = livro->autor;
    tamanho_autor = strlen(autor);
    fwrite(&tamanho_autor,sizeof(tamanho_autor),1,arquivo);
    fwrite(autor,sizeof(*autor),tamanho_autor,arquivo);

    endereco = &livro->endereco;
    fwrite(endereco,sizeof(*endereco),1,arquivo);

    cursor = cursor->proximo;
  }
  fclose(arquivo);
  return;
}

void gravar_estantes(Lista * estantes,char * path_estantes){
  Elemento * cursor;
  int tamanho;
  FILE * arquivo;

  arquivo = fopen(path_estantes,MODO_ESCRITA_BINARIOS);
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
  gravar_estantes(estantes,path_estantes);
  gravar_livros(livros,path_livros);
  return;
}

void carregar_arquivos(char * path_estantes, Lista * estantes, char * path_livros, Lista * livros){
  carregar_estantes(path_estantes,estantes);
  carregar_livros(path_livros,livros,estantes);
  return;
}

void carregar_livros(char * path_livros, Lista * livros, Lista * estantes){
  FILE * arquivo;
  Livro * livro;
  char * titulo;
  char * autor;
  int codigo;
  int tamanho_titulo;
  int tamanho_autor;
  Endereco_livro * endereco = (Endereco_livro*) malloc(sizeof(Endereco_livro));
  int bytes_lidos;

  arquivo = fopen(path_livros,MODO_LEITURA_BINARIOS);
  bytes_lidos = fread(&codigo,sizeof(codigo),1,arquivo);
  while (bytes_lidos != 0){

    fread(&tamanho_titulo,sizeof(tamanho_titulo),1,arquivo);
    titulo = (char *) malloc(sizeof(char)*tamanho_titulo);
    fread(titulo,sizeof(*titulo),tamanho_titulo,arquivo);
    titulo[tamanho_titulo] = '\0';

    fread(&tamanho_autor,sizeof(tamanho_autor),1,arquivo);
    autor = (char *) malloc(sizeof(char)*tamanho_autor);
    fread(autor,sizeof(*autor),tamanho_autor,arquivo);
    autor[tamanho_autor] = '\0';

    fread(endereco,sizeof(*endereco),1,arquivo);

    livro = criar_livro(codigo,titulo,autor,endereco->estante,endereco->prateleira);
    inserir_livro(livro,buscar_prateleira(livro,estantes),livros);

    bytes_lidos = fread(&codigo,sizeof(codigo),1,arquivo);

  }
  fclose(arquivo);
  return;
}

void carregar_estantes(char * path_estantes, Lista * estantes){
  FILE * arquivo;
  int bytes_lidos;
  Lista * prateleira;
  int tamanho;
  int i;

  arquivo = fopen(path_estantes,MODO_LEITURA_BINARIOS);
  bytes_lidos = fread(&tamanho,sizeof(tamanho),1,arquivo);
  while (bytes_lidos != 0){
    prateleira = criar_lista();
    for(i = 0; i < tamanho; i++){
      in(criar_lista(),prateleira);
    }
    in(prateleira,estantes);
    bytes_lidos = fread(&tamanho,sizeof(tamanho),1,arquivo);
  }
  fclose(arquivo);
  return;
}

Sala * criar_sala(){
  Sala * sala = (Sala *) malloc(sizeof(Sala));
  sala->ra = RA_NULO;
  sala->pilha = criar_lista();
  return sala;
}

Salas * criar_salas(int n_salas){
  Salas * salas = (Salas *) malloc(sizeof(Salas));
  int i;

  salas->livres = criar_lista();
  salas->ocupadas = criar_lista();
  salas->fila = criar_lista();
  for(i = 0; i < N_SALAS; i++){
    in(criar_sala(),salas->livres);
  }
  return salas;
}

Emprestimo * criar_emprestimo(int ra, Livro * livro){
  Emprestimo * emprestimo = (Emprestimo *) malloc(sizeof(Emprestimo));
  emprestimo->ra = ra;
  emprestimo->livro = livro;
  return emprestimo;
}

int remover_prateleira(Livro * livro, Lista * estantes){
  Lista * prateleira = buscar_prateleira(livro,estantes);

  if (remover(encontrar_posicao(livro->codigo,prateleira),prateleira))
    return TRUE;
  else
    return FALSE;
}

int inserir_prateleira(Livro * livro, Lista * estantes){
  Lista * prateleira = buscar_prateleira(livro,estantes);

  if (inserir(livro,encontrar_posicao(livro->codigo,prateleira),prateleira))
    return TRUE;
  else
    return FALSE;
}

int liberar_sala(int ra, Salas * salas, Lista * estantes){
  Sala * sala = sala_usada(ra,salas->ocupadas,TRUE);
  Emprestimo * emprestimo = NULL;
  Livro * livro;
  int liberou = FALSE;

  if(sala){
    while(!vazia(sala->pilha)){
      livro = pop(sala->pilha);
      inserir_prateleira(livro,estantes);
    }
    sala->ra = RA_NULO;
    push(sala,salas->livres);
    emprestimo = out(salas->fila);
    if(emprestimo){
      locar_sala(emprestimo,salas);
    }
    liberou = TRUE;
  }
  return liberou;
}

Sala * sala_usada(int ra, Lista * ocupadas, int liberar){
  Elemento * atual;
  int posicao = 0;
  Sala * sala = NULL;

  atual = topo(ocupadas);
  while((atual != NULL) && (((Sala *)(atual->conteudo))->ra != ra)){
    atual = atual->proximo;
    posicao++;
  }
  if (atual != NULL){
    if(liberar)
      return (Sala *) remover(posicao,ocupadas);
    else
      return (Sala *)(atual->conteudo);
  }
  else
    return NULL;
}

int locar_sala(Emprestimo * emprestimo, Salas * salas){
    Sala * sala;

    sala = sala_usada(emprestimo->ra,salas->ocupadas,FALSE);
    if(sala){
      push(emprestimo->livro,sala->pilha);
    }
    else{
      if(!vazia(salas->livres)){
        sala = pop(salas->livres);
        sala->ra = emprestimo->ra;
        push(emprestimo->livro,sala->pilha);
        push(sala,salas->ocupadas);
      }
      else
        in(emprestimo,salas->fila);
    }
    return TRUE;
}

int emprestar_livro(int ra, int codigo, Lista * estantes, Lista * livros, Salas * salas){
  Emprestimo * emprestimo;
  Endereco_livro * endereco;
  int emprestado = FALSE;
  Livro * livro;

  livro = buscar_livro(codigo,livros,estantes);
  if(livro){
    remover_prateleira(livro,estantes);
    emprestimo = criar_emprestimo(ra,livro);
    locar_sala(emprestimo,salas);
    emprestado = TRUE;
  }
  return emprestado;
}

void imprimir_mapa_de_estantes(Lista * estantes){
  int estante = 0;
  int prateleira = 0;
  Livro * livro;
  Elemento * cursor_prateleiras;
  Elemento * cursor_livros;
  Elemento * cursor_estantes = get(estante,estantes);

  printf("\n------------------------------------\n");
  while(cursor_estantes){
    printf("[E%d]:\n",estante);
    cursor_prateleiras = topo(cursor_estantes->conteudo);
    while(cursor_prateleiras){
      printf("\t[P%d]:\n",prateleira);
      cursor_livros = topo(cursor_prateleiras->conteudo);
      while(cursor_livros){
        livro = (Livro *) cursor_livros->conteudo;
        printf("\t\t[%d/%s/%s]\n",livro->codigo,livro->titulo,livro->autor);
        cursor_livros = cursor_livros->proximo;
      }
      prateleira++;
      cursor_prateleiras = cursor_prateleiras->proximo;
    }
    estante++;
    prateleira=0;
    cursor_estantes = cursor_estantes->proximo;
  }
  printf("\n------------------------------------\n");
  return;
}

void imprimir_fila_de_espera_de_sala(Salas * salas){
  Elemento * cursor_emprestimos;
  Emprestimo * emprestimo;
  Livro * livro;

  printf("\n------------------------------------\nFila (%d): \n\n",salas->fila->tamanho);
  cursor_emprestimos = topo(salas->fila);
  while(cursor_emprestimos){
    emprestimo = (Emprestimo *) cursor_emprestimos->conteudo;
    livro = emprestimo->livro;
    printf("\tRA: %d\n",emprestimo->ra);
    printf("\t\t\t[%d/%s/%s]\n",livro->codigo,livro->titulo,livro->autor);
    cursor_emprestimos = cursor_emprestimos->proximo;
  }
  printf("------------------------------------\n");
  return;
}

void imprimir_mapa_de_salas(Salas * salas){
  Elemento * cursor_salas;
  Sala * sala;
  Elemento * cursor_livros;
  Livro * livro;
  printf("\n------------------------------------\nSalas livres: %d\n",salas->livres->tamanho);
  printf("Salas ocupadas (%d):\n\n",salas->ocupadas->tamanho);
  int n_sala = 0;

  cursor_salas = topo(salas->ocupadas);
  while(cursor_salas){
    printf("\t[%d]:\n",n_sala);
    sala = (Sala *) cursor_salas->conteudo;
    printf("\t\tRA: %d\n",sala->ra);
    cursor_livros = topo(sala->pilha);
    while(cursor_livros){
      livro = (Livro *) cursor_livros->conteudo;
      printf("\t\t\t[%d/%s/%s]\n",livro->codigo,livro->titulo,livro->autor);
      cursor_livros = cursor_livros->proximo;
    }
    n_sala++;
    cursor_salas = cursor_salas->proximo;
  }
  printf("------------------------------------\n");
  return;
}

void inicializar(char * nome_arquivo, Lista * estantes, Lista * livros){
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

  arquivo_inicializacao = fopen(nome_arquivo,MODO_LEITURA_INICIALIZACAO);
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
