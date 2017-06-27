void processo(int meu_tipo, int minha_prioridade, int minha_duracao, Fila * fila_interrupcoes, int * interrupcoes_ativas, int * tempo_total){
  Processo * nova_interrupcao;
  Processo * primeiro_fila_interrupcoes;
  int tempo_total_inicial;

  tempo_total_inicial = *tempo_total;
  printf("\nTempo de início do Processo: %d\n", *tempo_total);
  printf("Duracão Esperada do Processo: %d\n", minha_duracao);
  for(int i = 1; i <= minha_duracao; i++){
    nova_interrupcao = gera_processo(TIPO_INTERRUPCAO,interrupcoes_ativas); // Gera, com certa probabilidade, um processo do tipo interrupção aleatório, com duração aleatória
    if(nova_interrupcao) // Como existe chance de não gerar interrupção, já que é gerado com certa probabilidade, é necessário checar se gerou
      insere_por_prioridade(fila_interrupcoes,nova_interrupcao);
    primeiro_fila_interrupcoes = primeiro(fila_interrupcoes);
    if((primeiro_fila_interrupcoes) && (primeiro_fila_interrupcoes->prioridade < minha_prioridade)){ // A primeira parte do 'if' é porque a fila pode estar vazia
        remove(fila_interrupcoes);
        processo(primeiro_fila_interrupcoes->tipo,primeiro_fila_interrupcoes->prioridade,primeiro_fila_interrupcoes->duracao,fila_interrupcoes,tempo_total);
        free(primeiro_fila_interrupcoes);
    }
    *tempo_total += 1;
    switch (meu_tipo) {
      case TIPO_INTERRUPCAO:
        printf("%d\n",minha_prioridade);
        break;
      case TIPO_USUARIO:
        printf("U\n");
        break;
    }
    sleep(TEMPO_ESPERA);
  }
  printf("\nTempo Final do Processo: %d\n", *tempo_total);
  printf("Duracão Real do Processo: %d\n", *tempo_total - tempo_total_inicial);
  return;
}

int main(void){
  Processo * processo_usuario;
  Fila * fila_interrupcoes = cria_fila(); // Fila de prioridade com as interrupções esperando pra serem executadas
  int interrupcoes_ativas[NUMERO_DE_INTERRUPCOES]; //Matém quais interrupções estão ativas pra não gerar duas interrupções do mesmo tipo
  int tempo_total = 1; //Tempo total do sistema
  while(1){
    processo_usuario = gera_processo(TIPO_USUARIO,NULL);
    processo(processo_usuario->tipo,processo_usuario->prioridade,processo_usuario->duracao,fila_interrupcoes,&interrupcoes_ativas,&tempo_total);
    free(processo_usuario);
  }
  return 0;
}
