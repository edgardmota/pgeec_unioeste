#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Macro de limpar a tela
#define clear() printf("\033[H\033[J")

// Constantes Gerais
#define PROBABILIDADE_INTERRUPCAO 30
#define NUMERO_DE_INTERRUPCOES    6
#define TEMPO_ESPERA              1
#define DURACAO_PROCESSOS_USUARIO 20
#define DURACAO_ITERRUPCOES       10
#define INDIFERENTE               100

// Tipos de Processo
#define TIPO_USUARIO     0
#define TIPO_INTERRUPCAO 1

// Prioridades
#define MOUSE       0
#define TECLADO     1
#define DISCO       2
#define IMPRESSORA  3
#define REDE        4
#define JOYSTICK    5

typedef struct processo {
  int tipo;
  int prioridade;
  int duracao;
} Processo;

typedef struct no {
  Processo * interrupcao;
  struct no * proximo;
} No;

typedef struct fila {
  No * cabeca;
  int ativas[NUMERO_DE_INTERRUPCOES];
  int tamanho;
} Fila;

No * cria_no(Processo * interrupcao, No * proximo){
  No * no = (No *) malloc(sizeof(no));

  no->interrupcao = interrupcao;
  no->proximo = proximo;
  return no;
}

Fila * cria_fila(){
  Fila * fila = (Fila *) malloc(sizeof(Fila));
  int i;

  fila->cabeca = cria_no(NULL,NULL);
  for(i = 0; i < NUMERO_DE_INTERRUPCOES; i++){
    fila->ativas[i] = 0;
  }
  fila->tamanho = 0;
  return fila;
}

void insere(Processo * interrupcao, Fila * fila_interrupcoes){
  No * anterior = fila_interrupcoes->cabeca;
  No * atual = fila_interrupcoes->cabeca->proximo;
  No * novo_no;

  while((atual) && (interrupcao->prioridade > atual->interrupcao->prioridade)){
    anterior = atual;
    atual = atual->proximo;
  }
  novo_no = cria_no(interrupcao,atual);
  anterior->proximo = novo_no;
  fila_interrupcoes->tamanho++;
  fila_interrupcoes->ativas[interrupcao->prioridade] = 1;
  return;
}

Processo * remover(Fila * fila_interrupcoes){
  No * removido = fila_interrupcoes->cabeca->proximo;
  Processo * interrupcao;

  if(removido)
    fila_interrupcoes->cabeca->proximo = removido->proximo;
  interrupcao = removido->interrupcao;
  fila_interrupcoes->ativas[interrupcao->prioridade] = 0;
  fila_interrupcoes->tamanho--;
  free(removido);
  return interrupcao;
}

Processo * primeiro(Fila * fila_interrupcoes){
  No * primeiro = fila_interrupcoes->cabeca->proximo;

  if(primeiro)
    return primeiro->interrupcao;
  else
    return NULL;
}

int numero_randomico(int intervalo){
  return (rand() % intervalo) + 1;
}

int prioridade_randomica(Fila * fila_interrupcoes, int prioridade){
  int i = 0;

  while(1){
    if((i!=prioridade)&&(!fila_interrupcoes->ativas[i])){
      if(numero_randomico(100) <= (PROBABILIDADE_INTERRUPCAO - 1))
        return i;
    }
    i = (i + 1) % NUMERO_DE_INTERRUPCOES;
  }
}

void imprime(Fila * fila_interrupcoes){
  No * anterior = fila_interrupcoes->cabeca;
  No * atual = fila_interrupcoes->cabeca->proximo;

  while(atual){
    printf("[%d] ",atual->interrupcao->prioridade);
    anterior = atual;
    atual = atual->proximo;
  }
  return;
}

Processo * gera_processo(int tipo, int prioridade, Fila * fila_interrupcoes, float prob_interrupcao){
  Processo * processo = NULL;

  if( ((prioridade == NUMERO_DE_INTERRUPCOES) || (fila_interrupcoes->tamanho != 1)) && ((tipo == TIPO_USUARIO) || ((tipo == TIPO_INTERRUPCAO) && (fila_interrupcoes->tamanho < NUMERO_DE_INTERRUPCOES) && (numero_randomico(100) <= (prob_interrupcao - 1)))))
    processo = (Processo *) malloc(sizeof(Processo));
  if(processo){
    processo->tipo = tipo;
    switch (tipo) {
      case TIPO_INTERRUPCAO:
        processo->duracao = numero_randomico(DURACAO_ITERRUPCOES);
        processo->prioridade = prioridade_randomica(fila_interrupcoes,prioridade);
        break;
      case TIPO_USUARIO:
        processo->duracao = numero_randomico(DURACAO_PROCESSOS_USUARIO);
        processo->prioridade = prioridade;
        break;
    }
  }
  return processo;
}

void executar(Processo * processo, Fila * fila_interrupcoes, int * tempo_total, float prob_interrupcao){
  Processo * nova_interrupcao;
  Processo * primeira_interrupcao_fila;
  int tempo_total_inicio;
  int i;

  // Guardar o tempo total
  tempo_total_inicio = *tempo_total;

  if(processo->tipo == TIPO_USUARIO){
    printf("\nTempo de Início: %d\n", *tempo_total);
  }
  for(i = 1; i <= processo->duracao; i++){
    if((i==1) && (processo->tipo == TIPO_USUARIO))
      printf("\n[t=%d]: ",processo->duracao);
    nova_interrupcao = gera_processo(TIPO_INTERRUPCAO,processo->prioridade,fila_interrupcoes,prob_interrupcao); // Gera, com certa probabilidade, um processo do tipo interrupção aleatório, com duração aleatória
    if(nova_interrupcao){ // Como existe chance de não gerar interrupção, já que é gerado com certa probabilidade, é necessário checar se gerou
      insere(nova_interrupcao,fila_interrupcoes);
      printf("[i=%d,t=%d]",nova_interrupcao->prioridade,nova_interrupcao->duracao);
    }
    primeira_interrupcao_fila = primeiro(fila_interrupcoes);
    while((primeira_interrupcao_fila) && (primeira_interrupcao_fila->prioridade < processo->prioridade)){ // A primeira parte do 'if' é porque a fila pode estar vazia
        executar(remover(fila_interrupcoes),fila_interrupcoes,tempo_total,prob_interrupcao/2);
        prob_interrupcao = prob_interrupcao/2;
        primeira_interrupcao_fila = primeiro(fila_interrupcoes);
    }
    switch (processo->tipo) {
      case TIPO_INTERRUPCAO:
        printf("%d",processo->prioridade);
        break;
      case TIPO_USUARIO:
        printf("u");
        break;
    }
    fflush(stdout);
    sleep(TEMPO_ESPERA);
    *tempo_total += 1;
  }
  if(processo->tipo == TIPO_USUARIO){
    printf("\n\nTempo Fim/Duração: %d/%d\n", *tempo_total, *tempo_total - tempo_total_inicio);
  }
  free(processo);
  return;
}

int main(void){
  Processo * processo_usuario;
  Fila * fila_interrupcoes = cria_fila(); // Fila de prioridade com as interrupções esperando pra serem executadas
  int tempo_total = 1; //Tempo total do sistema
  srand(time(NULL));

  while(1){
    processo_usuario = gera_processo(TIPO_USUARIO,NUMERO_DE_INTERRUPCOES,NULL,INDIFERENTE);
    executar(processo_usuario,fila_interrupcoes,&tempo_total,PROBABILIDADE_INTERRUPCAO);
  }
  return 0;
}
