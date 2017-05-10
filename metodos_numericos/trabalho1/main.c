#include "metnum.h"

//Opções do Menu
#define BISSECCAO 1
#define ITERACAO_LINEAR 2
#define NEWTON_RAPHSON 3
#define SECANTE 4
#define SAIR 5
#define INVALIDA 6

#define TAMANHO_SISTEMA 2

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

unsigned int menu_principal(void){
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
    opcao = menu_principal();
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

double * * menu_leitura_matriz(char * nome, unsigned int n, unsigned int m){
  double * * matriz;
  unsigned int i;
  unsigned int j;
  matriz = (double **) malloc(sizeof(double)*m);

  printf("Forneça a Matriz '%s':\n\n", nome);
  for(i = 0; i < m; i++){
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

  printf("Forneça o Vetor '%s':\n\n", nome);
  for(i = 0; i < n; i++){
    printf("Entre com o elemento %s[%d]: ", nome, i);
    scanf("%lf", &vetor[i]);
  }
  printf("\n");
  return vetor;
}

void imprime_matriz(char * nome, double * * matriz, unsigned int n, unsigned int m){
  char tab;
  unsigned int i;
  unsigned int j;

  printf("%s =", nome);
  for (i = 0; i < n; i++) {
    printf("\t|\t");
    tab = '\t';
    for(j = 0; j < m; j++){
      if((m-j)==1)
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


void menu_sistemas_lineares(void){


}

int main(void){
  int i;
  double * * a;
  double * b;
  double * x;
  int n = TAMANHO_SISTEMA;
  int m = TAMANHO_SISTEMA;
  //menu_zeros();
  a = menu_leitura_matriz("A",n,m);
  b = menu_leitura_vetor("b",n);
  triangular_superior(a,b,n);
  x = gauss(a,b,n);
  imprime_matriz("A",a,n,m);
  imprime_vetor("b",b,n);
  imprime_vetor("x",x,n);
  return 0;
}
