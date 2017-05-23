#include "metnum.h"
//Opções Menu Principal
#define MENU_RAIZES 1
#define MENU_SISTEMAS_LINEARES 2

//Opções do Menu de Raizes
#define BISSECCAO 1
#define ITERACAO_LINEAR 2
#define NEWTON_RAPHSON 3
#define SECANTE 4

//Opções do Menu de Sistemas Lineares
#define ELIMINACAO_GAUSS 1
#define ELIMINACAO_GAUSS_L_U 2
#define ELIMINACAO_GAUSS_CHOLESKY 3

//Opcoes Gerais
#define SAIR 10
#define INVALIDA 11

typedef struct entrada_iteracao_linear {
  polinomio * fi;
  double x0;
} entrada_iteracao_linear;

entrada_iteracao_linear * menu_iteracao_linear(polinomio * p){
  double x0;
  polinomio * fi = (polinomio *) malloc(sizeof(polinomio));
  entrada_iteracao_linear * retorno = (entrada_iteracao_linear *) malloc(sizeof(entrada_iteracao_linear));

  printf("[Método da Iteração Linear]\n\n");
  printf("Forneça x[0]: ");
  scanf("%lf",&x0);
  printf("\nEntre com o polinômio φ (formato ax²+bx+c):\n\n");
  printf("a: ");
  scanf("%f",&fi->a);
  printf("b: ");
  scanf("%f",&fi->b);
  printf("c: ");
  scanf("%f",&fi->c);
  printf("\n%s\n\n",formata_polinomio("φ",fi));
  retorno->fi = fi;
  retorno->x0 = x0;
  return retorno;
}

polinomio * entrada_polinomio(void){
  polinomio * p = (polinomio *) malloc(sizeof(polinomio));

  printf("\nEntre com o polinômio (formato ax²+bx+c):\n\n");
  printf("a: ");
  scanf("%f",&p->a);
  printf("b: ");
  scanf("%f",&p->b);
  printf("c: ");
  scanf("%f",&p->c);
  printf("\n%s\n\n",formata_polinomio("f",p));
  return p;
}

intervalo * menu_bisseccao(polinomio * p) {
  intervalo * i = (intervalo *) malloc(sizeof(intervalo));
  unsigned int primeira_vez = TRUE;
  printf("[Método da Bisseção]\n\n");
  do {
    if (!primeira_vez)
      printf("\nIntervalo inicial [%0.2lf,%0.2lf] inválido. Entre com um novo intervalo:\n\n",i->a,i->b);
    else
      printf("Entre com o intervalo inicial [a,b]:\n\n");
    printf("a: ");
    scanf("%lf",&i->a);
    printf("b: ");
    scanf("%lf",&i->b);
    primeira_vez = FALSE;
  } while (!bolzano(i,p));
  return i;
}

unsigned int menu_raizes(void){
  unsigned int opcao;

  printf("Escolha o método numérico para obtenção de raízes OU sair:\n\n");
  printf("(%d) Método da Bissecção;\n",BISSECCAO);
  printf("(%d) Método de Iteração Linear;\n",ITERACAO_LINEAR);
  printf("(%d) Método de Newton-Raphson;\n",NEWTON_RAPHSON);
  printf("(%d) Método da Secante;\n",SECANTE);
  printf("(%d) Sair.\n\n",SAIR);
  printf("Opção: ");
  scanf("%d",&opcao);
  printf("\n");
  return opcao;
}

double * menu_newton_raphson_secante(unsigned int opcao, polinomio * p){
  unsigned int i;
  unsigned int multiplicador;
  char * mensagens[2];

  mensagens[0] = "[Método de Newton-Raphson]\n\n\0";
  mensagens[1] = "[Método da Secante]\n\n\0";
  switch (opcao) {
    case NEWTON_RAPHSON:
      multiplicador = 1;
      break;
    case SECANTE:
      multiplicador = 2;
      break;
  }
  double * x = (double *) malloc(multiplicador*sizeof(double));
  printf("%s",mensagens[multiplicador-1]);
  for(i = 0; i < multiplicador; i++){
    printf("Forneça x[%d]: ",i);
    scanf("%lf",&x[i]);
  }
  return x;
}

double * menu_newton_raphson(polinomio * p) {
  return menu_newton_raphson_secante(NEWTON_RAPHSON,p);
}

double * menu_secante(polinomio * p) {
  return menu_newton_raphson_secante(SECANTE,p);
}

void menu_zeros(void){
  polinomio * p;
  intervalo * i;
  entrada_iteracao_linear * e;
  double raiz;
  double * x;
  unsigned int opcao;

  p = entrada_polinomio();
  do {
    opcao = menu_raizes();
    switch(opcao){
      case BISSECCAO:
        i = menu_bisseccao(p);
        raiz = bisseccao(i,p);
        break;
      case ITERACAO_LINEAR:
        e = menu_iteracao_linear(p);
        raiz = iteracao_linear(e->x0,e->fi,p);
        break;
      case NEWTON_RAPHSON:
        x = menu_newton_raphson(p);
        raiz = newton_raphson(x[0],p);
        break;
      case SECANTE:
        x = menu_secante(p);
        raiz = secante(x,p);
        break;
      case SAIR:
        break;
      default:
        opcao = INVALIDA;
        printf("Método inválido\n\n");
        break;
    }
    if ((opcao != SAIR) && (opcao != INVALIDA)){
      if (diferenca(raiz,(int)raiz)<ERRO)
        printf("\nA raiz da equação %s é %d.\n\n",formata_polinomio("f",p),(int)raiz);
      else
        printf("\nA raiz da equação %s é %0.8lf.\n\n",formata_polinomio("f",p),raiz);
    }
  } while(opcao != SAIR);
}

double * * menu_leitura_matriz(char * nome, unsigned int n){
  double * * matriz;
  unsigned int i;
  unsigned int j;
  matriz = (double **) malloc(sizeof(double)*n);

  printf("Forneça a Matriz '%s':\n\n", nome);
  for(i = 0; i < n; i++){
    matriz[i] = (double *) malloc(sizeof(double)*n);
    for(j = 0; j < n; j++){
      printf("Entre com o elemento %s[%d][%d]: ", nome, i, j);
      scanf("%lf", &matriz[i][j]);
    }
  }
  printf("\n");
  return matriz;
}

double * menu_leitura_vetor(char * nome, unsigned int n){
  double * vetor;
  unsigned int i;
  vetor = (double *) malloc(sizeof(double)*n);

  printf("Forneça a Matriz '%s':\n\n", nome);
  for(i = 0; i < n; i++){
    printf("Entre com o elemento %s[%d]: ", nome, i);
    scanf("%lf", &vetor[i]);
  }
  printf("\n");
  return vetor;
}

void imprime_matriz(char * nome, double * * matriz, unsigned int n){
  char tab;
  unsigned int i;
  unsigned int j;

  printf("%s =", nome);
  for (i = 0; i < n; i++) {
    printf("\t|\t");
    tab = '\t';
    for(j = 0; j < n; j++){
      if((n-j)==1)
        tab = '\0';
      printf("%0.2lf%c", matriz[i][j],tab);
      }
    printf("\t|\n");
  }
  printf("\n");
}

void imprime_vetor(char * nome, double * vetor, unsigned int n){
  unsigned int i;

  printf("%s =", nome);
  for (i = 0; i < n; i++) {
    printf("\t|\t%0.2lf\t|\n", vetor[i]);
  }
  printf("\n");
}

unsigned int menu_escolha_metodo_lineares(void){
  unsigned int opcao;

  printf("Escolha o método resolução de sistema linear OU sair:\n\n");
  printf("(%d) Método da Eliminação de Gauss;\n",ELIMINACAO_GAUSS);
  printf("(%d) Método da Eliminação de Gauss (LU);\n",ELIMINACAO_GAUSS_L_U);
  printf("(%d) Método da Eliminação de Gauss (Cholesky);\n",ELIMINACAO_GAUSS_CHOLESKY);
  printf("(%d) Método da Gauss-Jacobi;\n",GAUSS_JACOBI);
  printf("(%d) Método da Gauss-Seidel;\n",GAUSS_SEIDEL);
  printf("(%d) Sair.\n\n",SAIR);
  printf("Opção: ");
  scanf("%d",&opcao);
  printf("\n");
  return opcao;
}

void menu_sistemas_lineares(void){
  int i;
  double * * a;
  double * * copia_a;
  double * * l;
  double * * u;
  double * * g;
  double * * gt;
  double * b;
  double * copia_b;
  double * x;
  double * x0;
  int n;
  unsigned int opcao;
  int falhou;

  printf("Entre com o tamanho do sistema: ");
  scanf("%d", &n);
  printf("\n");

  a = menu_leitura_matriz("A",n);
  b = menu_leitura_vetor("b",n);
  do {
    falhou = FALSE;
    imprime_matriz("A",a,n);
    imprime_vetor("b",b,n);
    opcao = menu_escolha_metodo_lineares();
    switch(opcao){
      case ELIMINACAO_GAUSS:
        copia_a = matriz_zerada(n);
        copia_b = vetor_zerado(n);
        copia_matriz(a,copia_a,n);
        copia_vetor(b,copia_b,n);
        triangular_superior(copia_a,copia_b,n,NULL);
        x = eliminacao_gauss(copia_a,copia_b,n);
        free(copia_a);
        free(copia_b);
        break;
      case ELIMINACAO_GAUSS_L_U:
        x = eliminacao_gauss_L_U(a,b,n);
        break;
      case ELIMINACAO_GAUSS_CHOLESKY:
        x = eliminacao_gauss_cholesky(a,b,n);
        break;
      case GAUSS_SEIDEL:
      case GAUSS_JACOBI:
        copia_a = matriz_zerada(n);
        copia_matriz(a,copia_a,n);
        if(!criterio_linhas(copia_a,n)){
          arruma_linhas(copia_a,n);
        }
        if(!criterio_linhas(copia_a,n)){
          printf("Problema de convergência identificado pelo método das linhas!\n\n");
          falhou = TRUE;
        }
        else {
          x0 = menu_leitura_vetor("x0",n);
          switch (opcao) {
            case GAUSS_SEIDEL:
              x = gauss_seidel(copia_a,b,x0,n);
              break;
            default:
              x = gauss_jacobi(copia_a,b,x0,n);
              break;
          }
          free(x0);
        }
        free(copia_a);
        break;
      case SAIR:
        break;
      default:
        opcao = INVALIDA;
        printf("Método inválido\n\n");
        break;
    }
    if ((opcao != SAIR) && (opcao != INVALIDA)&& (!falhou)){
      printf("Solução:\n\n");
      imprime_vetor("x",x,n);
      printf("Sistema Original:\n\n");
      free(x);
    }
  } while(opcao != SAIR);
}

int main(void){
  unsigned int opcao;

  do {
    printf("\nEscolha uma opção:\n\n");
    printf("(%d) Calculo de raízes;\n", MENU_RAIZES);
    printf("(%d) Sistemas Lineares;\n", MENU_SISTEMAS_LINEARES);
    printf("(%d) Sair.\n\n",SAIR);
    printf("Opção: ");
    scanf("%d", &opcao);
    printf("\n");
    switch(opcao){
      case MENU_RAIZES:
        menu_zeros();
        break;
      case MENU_SISTEMAS_LINEARES:
        menu_sistemas_lineares();
        break;
      case SAIR:
        break;
      default:
        opcao = INVALIDA;
        printf("Opção inválida\n\n");
        break;
    }
  } while(opcao != SAIR);
  return 0;
}
