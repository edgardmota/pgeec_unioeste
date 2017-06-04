#include "bib.h"

int main(void){
  Lista * prateleira;
  Lista * estantes = criar_lista();
  Lista * livros = criar_lista();
  Livro * livro;
  int codigo;
  int posicao;
  Endereco_livro * e;

  inicializar(ARQUIVO_INICIALIZACAO,estantes,livros);
  // livro = buscar_livro(13,livros);
  // prateleira = buscar_prateleira(livro,estantes);
  // posicao = encontrar_posicao(13,prateleira);
  // remover(posicao,prateleira);
  printf("Qual livro deseja buscar? ");
  scanf("%d",&codigo);
  e = buscar_endereco_livro(codigo,estantes,livros);
  if(e)
    printf("\nEstante: %d\nPrateleira:%d\n\n",e->estante,e->prateleira);
  else
    printf("\nLivro indisponível!\n\n");
  persistir_em_arquivo(estantes,ARQUIVO_ESTANTES,livros,ARQUIVO_CATALOGO);
  return 0;
}
