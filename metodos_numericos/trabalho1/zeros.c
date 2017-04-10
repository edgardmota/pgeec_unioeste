#include "metnum.h"

polinomio * entrada_polinomio(void){
  polinomio * p = (polinomio *) malloc(sizeof(polinomio));

  printf("\nEntre com o polinômio (formato ax²+bx+c):\n\n");
  printf("a: ");
  scanf("%f",&p->a);
  printf("b: ");
  scanf("%f",&p->b);
  printf("c: ");
  scanf("%f",&p->c);
  printf("\n%s\n\n",formata_polinomio(p));
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

int main(void){
  polinomio * p;
  intervalo * i;
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
        printf("\nA raiz da equação %s é %d.\n\n",formata_polinomio(p),(int)raiz);
      else
        printf("\nA raiz da equação %s é %0.8lf.\n\n",formata_polinomio(p),raiz);
    }
  } while(opcao != SAIR);
  return 0;
}
