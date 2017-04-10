#include "metnum.h"

polinomio * derivada(polinomio * p){
  polinomio * d = (polinomio *) malloc(sizeof(polinomio));

  d->a = 0;
  d->b = p->a*2;
  d->c = p->b;

  return d;
}

float resolve_polinomio(float x, polinomio * p){
  float resultado;

  resultado = p->a*pow(x,2)+p->b*x+p->c;
  return resultado;
}

unsigned int bolzano(intervalo * i, polinomio * p){
  if ((resolve_polinomio(i->a,p)*resolve_polinomio(i->b,p))<0)
    return TRUE;
  else
    return FALSE;
}

double diferenca(double a, double b){
  return fabs(a-b);
}

char * formata_polinomio(char * nome, polinomio * p){
  char string_temporaria[40] = "";
  char * string_final;
  char buffer[10] = "";
  char a[10] = "";
  char b[10] = "";
  char c[10] = "";

  //Checando se a é diferente de 0
  if (diferenca(p->a,0)>=ERRO){
    //a é negativo
    if (p->a < 0)
      strcpy(a,"-");
    //Checando se a é inteiro
    if (diferenca(p->a,(int)p->a)<ERRO){
      if(((int)p->a == 1) || ((int)p->a == -1))
        sprintf(buffer,"x²");
      else
        sprintf(buffer,"%dx²",abs(p->a));
    }
    else
      sprintf(buffer,"%0.2fx²",fabs(p->a));
  }
  strcat(a,buffer);
  strcpy(buffer,"");
  //Checando se b é diferente de 0
  if (diferenca(p->b,0)>=ERRO){
    //b é negativo
    if (p->b < 0)
      strcpy(b,"-");
    else if (diferenca(p->a,0)>=ERRO)
      strcpy(b,"+");
    //Checando se b é inteiro
    if (diferenca(p->b,(int)p->b)<ERRO){
      if(((int)p->b == 1) || ((int)p->b == -1))
        sprintf(buffer,"x");
      else
        sprintf(buffer,"%dx",abs(p->b));
    }
    else
      sprintf(buffer,"%0.2fx",fabs(p->b));
  }
  strcat(b,buffer);
  strcpy(buffer,"");
  //Checando se c é diferente de 0
  if (diferenca(p->c,0)>=ERRO){
    //c é negativo
    if (p->c < 0)
      strcpy(c,"-");
    else if ((diferenca(p->a,0)>=ERRO) || (diferenca(p->b,0)>=ERRO))
      strcpy(c,"+");
    //Checando se c é inteiro
    if (diferenca(p->c,(int)p->c)<ERRO){
      sprintf(buffer,"%d",abs(p->c));
    }
    else
      sprintf(buffer,"%0.2f",fabs(p->c));
  } else if ((diferenca(p->a,0)<ERRO) && (diferenca(p->b,0)<ERRO))
      strcpy(c,"0");
  strcat(c,buffer);
  sprintf(string_temporaria,"%s(x)=%s%s%s",nome,a,b,c);
  string_final = (char *) malloc(strlen(string_temporaria)*sizeof(char));
  strcpy(string_final,string_temporaria);
  return string_final;
}

double bisseccao(intervalo * i, polinomio * p){
  double m;
  double aux;
  unsigned int k = 0;

  printf("\n----------------------------------\n");
  printf("k\tm\t\tDiferença\n");
  printf("----------------------------------\n");
  do {
    m = (i->a + i->b)/2;
    aux = i->b;
    i->b = m;
    if(!bolzano(i,p)){
      i->a = m;
      i->b = aux;
    }
    printf("%d\t%0.8lf\t%0.8lf\n",k,m,diferenca(i->a,i->b));
    k++;
  } while(diferenca(i->a,i->b)>=ERRO);
  printf("----------------------------------");
  return m;
}

double iteracao_linear(double x0, polinomio * fi, polinomio * p){
  double x;
  double x_proximo;
  unsigned int k=0;

  x = x0;
  x_proximo = x0;
  printf("----------------------------------\n");
  printf("k\tx[k]\t\tErro\n");
  printf("----------------------------------\n");
  do {
    x = x_proximo;
    x_proximo = resolve_polinomio(x,fi);
    printf("%d\t%0.8lf\t%0.8lf\n",k,x,diferenca(x,x_proximo));
    k++;
  } while(diferenca(x,x_proximo)>=ERRO);
  return x_proximo;
}

double newton_raphson(double x0, polinomio * p){
  double x;
  double x_proximo;
  polinomio * d;
  unsigned int k=0;

  x_proximo = x0;
  d = derivada(p);
  printf("\n----------------------------------\n");
  printf("k\tx[k]\t\tErro\n");
  printf("----------------------------------\n");
  do {
    x = x_proximo;
    x_proximo = x - (resolve_polinomio(x,p)/resolve_polinomio(x,d));
    printf("%d\t%0.8lf\t%0.8lf\n",k,x,diferenca(x,x_proximo));
    k++;
  } while(diferenca(x,x_proximo)>=ERRO);
  printf("----------------------------------");
  return x_proximo;
}

double secante(double * x_s, polinomio * p){
  double x;
  double x_anterior;
  double aux;
  unsigned int k = 2;

  x_anterior = x_s[0];
  x = x_s[1];
  printf("\n----------------------------------\n");
  printf("k\tx[k]\t\tErro\n");
  printf("----------------------------------\n");
  do {
    aux = x;
    x = (x_anterior*resolve_polinomio(x,p)-x*resolve_polinomio(x_anterior,p))/(resolve_polinomio(x,p)-resolve_polinomio(x_anterior,p));
    printf("%d\t%0.8lf\t%0.8lf\n",k,x,diferenca(x,x_anterior));
    x_anterior = aux;
    k++;
  } while(diferenca(x_anterior,x)>=ERRO);
  printf("----------------------------------");
  return x;
}
