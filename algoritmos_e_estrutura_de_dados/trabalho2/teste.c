#include <stdio.h>
#include <stdlib.h>

// Constantes Gerais
#define PROBABILIDADE_INTERRUPCAO 50
#define NUMERO_DE_INTERRUPCOES 6
#define TEMPO_ESPERA 1

// Tipos de Processo
#define USUÁRIO     0
#define INTERRUPCAO 1

// Interrupções
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

void executar(Processo * processo, Fila * fila_interrupcoes, int * interrupcoes_ativas, int * tempo_total, float prob_interrupcao){
  Processo * nova_interrupcao;
  Processo * primeira_interrupcao_fila;
  int tempo_total_inicio;

  // Guardar o tempo total
  tempo_total_inicio = *tempo_total;

  if(processo->tipo == USUARIO){
    printf("\nTempo de Início: %d\n", *tempo_total);
    printf("Duracão Esperada: %d\n\n", processo->duracao);
    printf("Fila de Interrupções:\n\n", );
    imprime(fila_interrupcoes);
    printf("\n\n", );
  }
  for(int i = 1; i <= processo->duracao; i++){
    nova_interrupcao = gera_processo(INTERRUPCAO,interrupcoes_ativas); // Gera, com certa probabilidade, um processo do tipo interrupção aleatório, com duração aleatória
    if(nova_interrupcao){ // Como existe chance de não gerar interrupção, já que é gerado com certa probabilidade, é necessário checar se gerou
      insere(nova_interrupcao,fila_interrupcoes);
      printf("[%d]",nova_interrupcao->prioridade);
    }
    primeira_interrupcao_fila = primeiro(fila_interrupcoes);
    if((primeira_interrupcao_fila) && (primeira_interrupcao_fila->prioridade < processo->prioridade)){ // A primeira parte do 'if' é porque a fila pode estar vazia
        executar(remove(fila_interrupcoes),fila_interrupcoes,tempo_total,PROBABILIDADE_INTERRUPCAO/2);
    }
    *tempo_total += 1;
    switch (processo->tipo) {
      case INTERRUPCAO:
        printf("%d",processo->prioridade);
        break;
      case USUARIO:
        printf("u");
        break;
    }
    fflush(stdout);
    sleep(TEMPO_ESPERA);
  }
  if(processo->tipo == USUARIO){
    printf("\n\nTempo Final: %d\n", *tempo_total);
    printf("Duracão Esperada/Duração Real: %d/%d\n", processo->duracao, *tempo_total - tempo_total_inicial);
  }
  free(processo);
  return;
}

int main(void){
  Processo * processo_usuario;
  Fila * fila_interrupcoes = cria_fila(); // Fila de prioridade com as interrupções esperando pra serem executadas

  int interrupcoes_ativas[NUMERO_DE_INTERRUPCOES]; //Matém quais interrupções estão ativas pra não gerar duas interrupções do mesmo tipo
  int tempo_total = 1; //Tempo total do sistema

  zerar_interrupcoes_ativas(&interrupcoes_ativas);
  while(1){
    processo_usuario = gera_processo(USUARIO,NULL);
    executar(processo_usuario,fila_interrupcoes,&interrupcoes_ativas,&tempo_total,PROBABILIDADE_INTERRUPCAO);
  }
  return 0;
}
