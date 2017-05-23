#include "metnum.h"

const double ERRO = 0.0001;

void arruma_linhas(double * * a, unsigned int n){
  int i;
  int j;
  int * maiores = (int *) malloc(sizeof(int)*n);

  for(i = 0; i < n; i++)
    maiores[i] = 0;
  for(j = 0; j < n ; j++){
    for(int i = 0; i < n; i++){
      if(a[i][j]>a[maiores[i]][j])
        maiores[i]=i;
    }
  }
  for(i = 0; i < n; i++)
    troca_linhas(a,NULL,i,maiores[i],n);
  return;
}

int criterio_linhas(double * * a, unsigned int n){
  // int i;
  // int j;
  // int s;
  // int falhou = FALSE;
  //
  // for(i = 0; (i < n) && (!falhou); i++){
  //   s = 0;
  //   for(j = 0; j < n; j++){
  //     if(j!=i){
  //       s += a[i][j];
  //     }
  //   }
  //   if(s>a[i][i])
  //     falhou = TRUE;
  //   }
  return TRUE;
}

double * gauss_jacobi(double * * a, double * b, double * x0, unsigned int n){
  return gauss_jacobi_seidel(a,b,x0,n,GAUSS_JACOBI);
}

double * gauss_seidel(double * * a, double * b, double * x0, unsigned int n){
  return gauss_jacobi_seidel(a,b,x0,n,GAUSS_SEIDEL);
}

double * gauss_jacobi_seidel(double * * a, double * b, double * x0, unsigned int n, int metodo){
  int i;
  int j;
  double * x = vetor_zerado(n);
  double * x_proximo = vetor_zerado(n);
  double c;
  int k = -1;

  copia_vetor(x0,x_proximo,n);
  do {
    k++;
    copia_vetor(x_proximo,x,n);
    for(i = 0; i < n; i++){
      x_proximo[i] = 1/a[i][i];
      c = b[i];
      for(j = 0; j < n; j++)
        if(j!=i){
          if((j>i)||(metodo == GAUSS_JACOBI))
            c -= a[i][j]*x[j];
          else
            c -= a[i][j]*x_proximo[j];
        }
      x_proximo[i] *= c;
    }
  } while(!convergiu(x,x_proximo,n));
  printf("Convergiu em %d iterações.\n",k);
  return x_proximo;
}

unsigned int convergiu(double * x1, double * x2, unsigned int n){
  int i;
  double dif;
  double maior = diferenca(x1[0],x2[0]);
  for(i = 1; i < n; i++){
    dif = diferenca(x1[i],x2[i]);
    if(dif>maior)
      maior=dif;
  }
  return maior<=ERRO;
}

double * vetor_zerado(unsigned int n){
  int i;
  double * v = (double *) malloc(sizeof(double)*n);

  for(i = 0; i < n; i++)
    v[i] = 0;
  return v;
}

double * * matriz_zerada(unsigned int n){
  int i;
  int j;
  double * * m = (double * *) malloc(sizeof(double)*n);

  for(i = 0; i < n; i++){
    m[i] = (double *) malloc(sizeof(double)*n);
    for(j = 0; j < n; j++)
      m[i][j] = 0;
    }
  return m;
}

void copia_vetor(double * v1, double * v2, unsigned int n){
  int i;

  for(i = 0; i < n; i++)
    v2[i] = v1[i];
}

void copia_matriz(double * * m1, double * * m2, unsigned int n){
  int i;
  int j;

  for(i = 0; i < n; i++)
    for(j = 0; j < n; j++)
      m2[i][j] = m1[i][j];
}

double * * transposta(double * * m, unsigned int n){
  int i;
  int j;
  double * * t = matriz_zerada(n);

  for(i = 0; i < n; i++)
    for(j = 0; j < n; j++)
      t[j][i] = m[i][j];
  return t;
}

double * * cholesky (double * * a, unsigned int n){
  int k;
  int i;
  int j;
  double soma;
  double * * g = matriz_zerada(n);

  copia_matriz(a,g,n);
  for(k = 0; k < n; k++){
    for(i = 0; i < k; i++){
        soma = 0;
        for(j = 0; j < i; j++)
          soma += g[i][j] * g[k][j];
        g[k][i] = (g[k][i] - soma)/g[i][i];
    }
    soma = 0;
    for(j = 0; j < k; j++)
      soma += pow(g[i][j],2);
    g[k][k] = sqrt(g[k][k] - soma);
  }
  for(k = 0; k < n; k++)
      for(i = k+1; i < n; i++)
        g[k][i] = 0;
  return g;
}

void L_U (double * * a, double * b, double * * l, double * * u, unsigned int n){
  int i;
  int j;
  double * copia_b = vetor_zerado(n);

  for(i = 0; i < n; i++){
    l[i][i] = 1;
    for(j = i+1; j < n; j++)
      l[i][j] = 0;
  }
  copia_matriz(a,u,n);
  copia_vetor(b,copia_b,n);
  triangular_superior(u,copia_b,n,l);
}


int max_modulo_col(double * * a, unsigned int l, unsigned int c, unsigned int n){
  unsigned int max = l;
  int i;

  for(i = l+1; i < n; i++){
    if(fabs(a[i][c]) > fabs(a[max][c]))
      max = i;
  }
  return max;
}

void troca_linhas(double * * a, double * b, unsigned int l1, unsigned int l2, unsigned int n){
  double aux;
  int i;

    for(i = 0; i < n; i++){
      aux = a[l1][i];
      a[l1][i] = a[l2][i];
      a[l2][i] = aux;
    }
    if(b){
      aux = b[l1];
      b[l1] = b[l2];
      b[l2] = aux;
    }
}

void triangular_superior(double * * a, double * b, unsigned int n, double * * l){
  unsigned int k;
  unsigned int i;
  unsigned int j;
  unsigned int l2;
  double pivo;
  double m;

  for(k = 0; k < n - 1; k++)
    for(i = k + 1; i < n; i++){
      l2 = max_modulo_col(a,i,k,n);
      if (i != l2)
        troca_linhas(a,b,l2,i,n);
      m = a[i][k]/a[k][k];
      if(l)
        l[i][k] = m;
      a[i][k] = 0;
      for(j = k + 1; j < n; j++)
        a[i][j] = a[i][j] - m*a[k][j];
      if ((b) && (!l))
        b[i] = b[i] - m*b[k];
    }
}

double * eliminacao_gauss_L(double * * a, double * b, unsigned int n){
  int i;
  int j;
  double s;
  double * x = vetor_zerado(n);

  for(i = 0; i < n; i++){
    s = 0;
    for(j = 0; j <= i - 1; j++)
      s = s + a[i][j]*x[j];
    x[i] = (b[i] - s)/a[i][i];
  }
  return x;
}

double * eliminacao_gauss(double * * a, double * b, unsigned int n){
  int i;
  int j;
  double s;
  double * x = vetor_zerado(n);

  for(i = n-1; i >=0; i--){
    s = 0;
    for(j = i + 1; j < n; j++)
      s = s + a[i][j]*x[j];
    x[i] = (b[i] - s)/a[i][i];
  }
  return x;
}

double * eliminacao_gauss_cholesky(double * * a, double * b, unsigned int n){
    double * * g;
    double * * gt;
    double * y;
    g = cholesky(a,n);
    gt = transposta(g,n);
    y = eliminacao_gauss_L(g,b,n);
    return eliminacao_gauss(gt,y,n);
}

double * eliminacao_gauss_L_U(double * * a, double * b, unsigned int n){
    double * * l = matriz_zerada(n);
    double * * u = matriz_zerada(n);
    double * y;
    L_U(a,b,l,u,n);
    y = eliminacao_gauss_L(l,b,n);
    return eliminacao_gauss(u,y,n);
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
