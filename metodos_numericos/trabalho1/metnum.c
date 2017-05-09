#include "metnum.h"

const double ERRO = 0.0001;

int max_modulo_col(double * * a, unsigned int l, unsigned int c, unsigned int n){
  unsigned int max = l;

  for(int i = l+1; i < n; i++){
    if(fabs(a[i][c]) > fabs(a[max][c]))
      max = i;
  }
  return max;
}

void troca_linhas(double * * a, double * b, unsigned int l1, unsigned int l2, unsigned int n){
  double aux;
  if(l1 != l2){ //Troca apenas se as linhas forem diferentes
    for(int i = 0; i < n; i++){
      aux = a[l1][i];
      a[l1][i] = a[l2][i];
      a[l2][i] = aux;
      if(b){
        aux = b[l1];
        b[l1] = b[l2];
        b[l2] = aux;
      }
    }
  }
}

void triangular_superior(double * * a, double * b, unsigned int n){
  unsigned int k;
  unsigned int i;
  unsigned int j;
  unsigned int l2;
  double pivo;
  double m;

  for(k = 0; k < n - 1; k++)
    for(i = k + 1; i < n; i++){
      l2 = max_modulo_col(a,i,j,n);
      printf("Maior linha: %d\n", l2 );
      troca_linhas(a,b,l2,i,n);
      m = a[i][k]/a[k][k];
      a[i][k] = 0;
      for(j = k + 1; j < n; j++)
        a[i][j] = a[i][j] - m*a[k][j];
      if (b)
        b[i] = b[i] - m*b[k];
    }
}

double * gauss(double * * a, double * b, unsigned int n){
  unsigned int k;
  unsigned int j;
  double s;
  double * x = (double *) malloc(n*sizeof(double));

  x[n-1] = b[n-1]/a[n-1][n-1];
  for(k = n-2; k <= 0; k--){
    s = 0;
    for(j = k + 1; k < n; k++)
      s = s + a[k][j]*x[j];
    x[k] = (b[k] - s)/a[k][k];
  }
  return x;
}

polinomio * derivada(polinomio * p){
  polinomio * d = (polinomio *) malloc(sizeof(polinomio));

  d->a = 0;
  d->b = p->a*2;
  d->c = p->b;

  return d;
}

double resolve_polinomio(double x, polinomio * p){
  double resultado;

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

  printf("\n----------------------------------------------------------------------------------\n");
  printf("k\ta\t\tb\t\tm\t\tf(m)\t\tDiferença\n");
  printf("----------------------------------------------------------------------------------\n");
  m = (i->a + i->b)/2.0;
  while (fabs(resolve_polinomio(m,p))>=ERRO){
    printf("%d\t%0.8lf\t%0.8lf\t%0.8lf\t%0.8lf\t%0.8lf\n",k,i->a,i->b,m,resolve_polinomio(m,p),diferenca(i->a,i->b));
    aux = i->b;
    i->b = m;
    if(!bolzano(i,p)){
      i->a = m;
      i->b = aux;
    }
    m = (i->a + i->b)/2.0;
    k++;
  }
  printf("----------------------------------------------------------------------------------");
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
  while (fabs(resolve_polinomio(x,p))>=ERRO){
    x_proximo = resolve_polinomio(x,fi);
    printf("%d\t%0.8lf\t%0.8lf\n",k,x,diferenca(x,x_proximo));
    x = x_proximo;
    k++;
  }
  printf("----------------------------------\n");
  return x_proximo;
}

double newton_raphson(double x0, polinomio * p){
  double x;
  double x_proximo;
  polinomio * d;
  unsigned int k=0;

  x = x0;
  d = derivada(p);
  printf("\n----------------------------------\n");
  printf("k\tx[k]\t\tErro\n");
  printf("----------------------------------\n");
  while (fabs(resolve_polinomio(x,p))>=ERRO){
    x_proximo = x - (resolve_polinomio(x,p)/resolve_polinomio(x,d));
    printf("%d\t%0.8lf\t%0.8lf\n",k,x,diferenca(x,x_proximo));
    x = x_proximo;
    k++;
  }
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
  while (fabs(resolve_polinomio(x,p))>=ERRO){
    aux = x;
    x = (x_anterior*resolve_polinomio(x,p)-x*resolve_polinomio(x_anterior,p))/(resolve_polinomio(x,p)-resolve_polinomio(x_anterior,p));
    printf("%d\t%0.8lf\t%0.8lf\n",k,x,diferenca(x,x_anterior));
    x_anterior = aux;
    k++;
  };
  printf("----------------------------------");
  return x;
}
