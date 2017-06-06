#include "bib.h"

//Opções Menu

#define SAIR 0
#define INICIALIZAR 1
#define INSERIR_LIVRO 2
#define REMOVER_LIVRO 3
#define BUSCAR_ENDERECO_LIVRO 4
#define EMPRESTIMO_LIVRO 5
#define LIBERAR_SALA 6
#define IMPRIMIR_ESTANTES 7
#define IMPRIMIR_SALAS 8
#define IMPRIMIR_FILA 9

#define MAX_STRING 40
#define clear() printf("\033[H\033[J")

int menu_inserir_livro(Lista * estantes, Lista * livros){
  Livro * livro;
  char * titulo = (char * ) malloc(sizeof(char)*MAX_STRING);
  char * autor = (char * ) malloc(sizeof(char)*MAX_STRING);
  int codigo;
  int n_estante;
  int n_prateleira;
  Lista * prateleira;

  printf("\nDigite o código do livro: ");
  scanf("%d",&codigo);
  getchar();
  printf("Digite o título do livro: ");
  fgets(titulo,MAX_STRING,stdin);
  strtok(titulo,"\n");
  printf("Digite o autor do livro: ");
  fgets(autor,MAX_STRING,stdin);
  strtok(autor,"\n");
  printf("Digite o nº da estante: ");
  scanf("%d",&n_estante);
  printf("Digite o nº da prateleira: ");
  scanf("%d",&n_prateleira);
  livro = criar_livro(codigo,titulo,autor,n_estante,n_prateleira);
  prateleira = buscar_prateleira(livro,estantes);
  if (prateleira)
    return inserir_livro(livro,prateleira,livros);
  else
    return FALSE;
}

int menu_remover_livro(Lista * estantes, Lista * livros){
  int codigo;
  Livro * livro;

  printf("\nDigite o código do livro: ");
  scanf("%d",&codigo);
  livro = buscar_livro(codigo,livros,estantes);
  if(livro)
    return (remover_livro(livro,estantes,livros) != NULL);
  else
    return FALSE;
}

int menu_endereco_livro(Lista * estantes, Lista * livros){
  int codigo;
  Endereco_livro * endereco;

  printf("\nDigite o código do livro: ");
  scanf("%d",&codigo);
  endereco = buscar_endereco_livro(codigo,estantes,livros);
  if(endereco)
    printf("\n------------------------------------\nEstante: %d\nPrateleira: %d\n------------------------------------\n",endereco->estante,endereco->prateleira);
  else
    return FALSE;
}

int menu_emprestimo_livro(Lista * estantes, Lista * livros, Salas * salas){
  int codigo;
  int ra;

  printf("\nDigite o código do livro: ");
  scanf("%d",&codigo);
  printf("Digite o RA: ");
  scanf("%d",&ra);
  return emprestar_livro(ra,codigo,estantes,livros,salas);
}

int menu_liberar_sala(Salas * salas, Lista * estantes){
  int ra;

  printf("\nDigite o RA: ");
  scanf("%d",&ra);
  return liberar_sala(ra,salas,estantes);
}

int invalida(int opcao){
  return (opcao < INICIALIZAR) && (opcao > IMPRIMIR_FILA);
}

void espera_enter(void){
  getchar();
  getchar();
  return;
}

int main(void){
  Lista * estantes = criar_lista();
  Lista * livros = criar_lista();
  Salas * salas = criar_salas(N_SALAS);
  int opcao;
  int sair = FALSE;

  if(( access( ARQUIVO_ESTANTES, F_OK ) != -1 ) && ( access( ARQUIVO_CATALOGO, F_OK ) != -1 ))
    carregar_arquivos(ARQUIVO_ESTANTES,estantes,ARQUIVO_CATALOGO,livros);
  clear();
  while(!sair){
    printf("\nEscolha uma opção:\n\n");
    printf("(%d) Carga Inicial\n",INICIALIZAR);
    printf("(%d) Inserir Livro\n",INSERIR_LIVRO);
    printf("(%d) Remover Livro\n",REMOVER_LIVRO);
    printf("(%d) Buscar Endereco Livro\n",BUSCAR_ENDERECO_LIVRO);
    printf("(%d) Emprestar Livro\n",EMPRESTIMO_LIVRO);
    printf("(%d) Liberar Sala\n",LIBERAR_SALA);
    printf("(%d) Imprimir Estantes\n",IMPRIMIR_ESTANTES);
    printf("(%d) Imprimir Salas\n",IMPRIMIR_SALAS);
    printf("(%d) Imprimir Fila\n",IMPRIMIR_FILA);
    printf("\n(%d) Sair\n",SAIR);
    printf("\nSua escolha: ");
    scanf("%d",&opcao);
    clear();
    if(((estantes->tamanho != 0) && (livros->tamanho!=0)) || (opcao == SAIR) || (opcao == INICIALIZAR) || invalida(opcao) ){
      switch (opcao) {
        case INICIALIZAR:
          if((estantes->tamanho == 0) && (livros->tamanho==0)){
            inicializar(ARQUIVO_INICIALIZACAO,estantes,livros);
            printf("\n------------------------------------\nBanco de dados inicial carregado!\n------------------------------------\n");
          }
          else
            printf("\n------------------------------------\nBanco de dados já carregado!\n------------------------------------\n");
          break;
        case INSERIR_LIVRO:
          if (menu_inserir_livro(estantes,livros))
            printf("\n------------------------------------\nLivro inserido!\n------------------------------------\n");
          else
            printf("\n------------------------------------\nErro: Não foi possível inserir o livro!\n------------------------------------\n");
          break;
        case REMOVER_LIVRO:
          if (menu_remover_livro(estantes,livros))
            printf("\n------------------------------------\nLivro removido!\n------------------------------------\n");
          else
            printf("\n------------------------------------\nErro: Não foi possível remover o livro!\n------------------------------------\n");
          break;
        case BUSCAR_ENDERECO_LIVRO:
          if (!menu_endereco_livro(estantes,livros))
            printf("\n------------------------------------\nErro: Livro indisponível!\n------------------------------------\n");
          break;
        case EMPRESTIMO_LIVRO:
          if (menu_emprestimo_livro(estantes,livros,salas))
            printf("\n------------------------------------\nEmpréstimo feito com sucesso!\n------------------------------------\n");
          else
            printf("\n------------------------------------\nErro: Livro indisponível!\n------------------------------------\n");
          break;
        case LIBERAR_SALA:
          if (menu_liberar_sala(salas,estantes))
            printf("\n------------------------------------\nSala liberada!\n------------------------------------\n");
          else
            printf("\n------------------------------------\nErro: Não foi possível liberar sala!\n------------------------------------\n");
          break;
        case IMPRIMIR_ESTANTES:
          imprimir_mapa_de_estantes(estantes);
          break;
        case IMPRIMIR_SALAS:
          imprimir_mapa_de_salas(salas);
          break;
        case IMPRIMIR_FILA:
          imprimir_fila_de_espera_de_sala(salas);
          break;
        case SAIR:
          sair = TRUE;
          printf("\n");
          break;
        default:
          printf("\n------------------------------------\nOpção inválida!\n------------------------------------\n");
          break;
      }
    }
    else
      printf("\n------------------------------------\nErro: Necessário fazer carga inicial!\n------------------------------------\n");
    if((opcao != SAIR) && !invalida(opcao)){
      printf("\nPressione ENTER para continuar...");
      espera_enter();
      clear();
    }
  }
  persistir_em_arquivo(estantes,ARQUIVO_ESTANTES,livros,ARQUIVO_CATALOGO);
  return 0;
}
