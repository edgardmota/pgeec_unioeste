#include "btree.h"

boolean_t b_tree_close(b_tree_t ** b_tree){
  b_tree_header_flush(b_tree);
  b_tree_node_flush(b_tree);
  fclose((*b_tree)->file);
  free((*b_tree)->in_memory_header);
  free(*b_tree);
  return TRUE;
}

boolean_t b_tree_node_flush(b_tree_t ** b_tree){
  //Forçando o flush do registro
  b_tree_new_node(NULL,NULL,b_tree);
  return TRUE;
}

byte_offset_t b_tree_get_root(b_tree_t ** b_tree){
  return (*b_tree)->in_memory_header->root_register;
}

boolean_t b_tree_header_flush(b_tree_t ** b_tree){
  //Houve modificação no header
  if((*b_tree)->modification[B_TREE_HEADER_MODIFICATION]){
    fseek((*b_tree)->file, 0, SEEK_SET);
    fwrite(&(*b_tree)->in_memory_header,sizeof((*b_tree)->in_memory_header),1,(*b_tree)->file);
    (*b_tree)->modification[B_TREE_HEADER_MODIFICATION] = FALSE;
  }
  return TRUE;
}

boolean_t b_tree_set_root_register(byte_offset_t root_register, b_tree_t ** b_tree){
  (*b_tree)->in_memory_header->root_register = root_register;
  (*b_tree)->modification[B_TREE_HEADER_MODIFICATION] = TRUE;
  return TRUE;
}

boolean_t b_tree_set_free_head_register(byte_offset_t free_head_register, b_tree_t ** b_tree){
  (*b_tree)->in_memory_header->free_head_register = free_head_register;
  (*b_tree)->modification[B_TREE_HEADER_MODIFICATION] = TRUE;
  return TRUE;
}

b_tree_t * create_b_tree(string_t path, string_t creation_mode){
  b_tree_t * b_tree = (b_tree_t *) malloc(sizeof(b_tree_t));

  b_tree->file = fopen(path,creation_mode);
  b_tree->in_memory_header = (b_tree_header *) malloc(sizeof(b_tree_header));
  b_tree->flush_offset = sizeof(b_tree_header);

  b_tree_set_free_head_register(SEEK_SET,&b_tree);
  b_tree_set_root_register(SEEK_SET,&b_tree);
  b_tree_header_flush(&b_tree);
  b_tree->modification[B_TREE_NODE_MODIFICATION] = FALSE;
  b_tree->handled_node = (b_tree_node *) malloc(sizeof(b_tree_node));

  return b_tree;
}

byte_offset_t b_tree_get_free_head_register(b_tree_t ** b_tree){
  return (*b_tree)->in_memory_header->free_head_register;
}

boolean_t data_copy(DATA * destination, DATA * source){
  if(source){
    destination->matricula = source->matricula;
    destination->byte_offset = source->byte_offset;
  }
  return TRUE;
}

boolean_t b_tree_make_node(b_tree_t ** b_tree, DATA * data, byte_offset_t * children){
    b_tree_node * node = (*b_tree)->handled_node;

    node->number_keys = 1;
    data_copy(&node->page[0],data);
    node->child[LEFT] = children[LEFT];
    node->child[RIGHT] = children[RIGHT];
    for (int i = RIGHT + 1; i <= B_TREE_ORDER; i++)
      node->child[i] = SEEK_SET;

    (*b_tree)->modification[B_TREE_NODE_MODIFICATION] = TRUE;

    return TRUE;
}

byte_offset_t b_tree_node_write(b_tree_t ** b_tree){
  b_tree_node * node = (*b_tree)->handled_node;
  int node_size = sizeof(b_tree_node);
  byte_offset_t next_node_offset;

  fseek((*b_tree)->file, (*b_tree)->flush_offset, SEEK_SET);
  next_node_offset = ftell((*b_tree)->file) + node_size;

  fwrite(node,node_size,1,(*b_tree)->file);

  (*b_tree)->modification[B_TREE_NODE_MODIFICATION] = FALSE;

  return next_node_offset;
}

byte_offset_t b_tree_new_node(DATA * data, byte_offset_t * children, b_tree_t ** b_tree){

  //Caso haja dados e os mesmos tenham sido modificados
  if((*b_tree)->modification[B_TREE_NODE_MODIFICATION])
    b_tree_node_write(b_tree);

  //Quando se força o flush, o data_register vem NULL
  if(data){
    b_tree_make_node(b_tree,data,children);
    (*b_tree)->flush_offset += sizeof(b_tree_node);
  }

  return (*b_tree)->flush_offset;
}


int binary_search(b_tree_t ** b_tree, byte_offset_t offset, DATA * data){
  int half, i, f;
  i = 0;
  f = b_tree_get_number_of_keys(b_tree,offset) - 1;

  while (i <= f) {
    half = (i + f)/2;
    if (b_tree_get_data(b_tree,offset,half)->matricula == data->matricula)
       return half; //Encontrou. Retorna a posição em que a chave está.
    else {
      if (b_tree_get_data(b_tree,offset,half)->matricula > data->matricula)
        f = half - 1;
      else
        i = half + 1;
      }
  }
  return i; //Não encontrou. Retorna a posição do ponteiro para o filho.
}

byte_offset_t b_tree_search(b_tree_t ** b_tree, unsigned short matricula){
  byte_offset_t node;
  int pos; //posição retornada pelo busca binária.

  node = b_tree_get_root(b_tree);
  while (node) {
    pos = binary_search(b_tree,node,b_tree_create_data(matricula,NONE));
    if ((pos < b_tree_get_number_of_keys(b_tree,node)) && (b_tree_get_data(b_tree,node,pos)->matricula == matricula))
      return b_tree_get_data(b_tree,node,pos)->byte_offset;
    else
      node = b_tree_get_child(b_tree,node,pos);
   }
  return SEEK_SET;
}

DATA * b_tree_create_data(unsigned short matricula, byte_offset_t byte_offset){
  DATA * data = (DATA *) malloc(sizeof(data));

  data->matricula = matricula;
  data->byte_offset = byte_offset;
  return data;
}

boolean_t b_tree_insert_data(b_tree_t ** b_tree, byte_offset_t offset, DATA * data, byte_offset_t right_child){
  int k, pos, n_keys;
  unsigned short info = data->matricula;

  //busca para obter a posição ideal para inserir a nova chave
  pos = binary_search(b_tree,offset,data);
  n_keys = b_tree_get_number_of_keys(b_tree,offset);

  k = n_keys;
  //realiza o remanejamento para manter as chaves ordenadas
  while ((k > pos) && (info < (b_tree_get_data(b_tree,offset,k-1))->matricula)) {
    b_tree_set_data(b_tree,offset,k,b_tree_get_data(b_tree,offset,k-1));
    b_tree_set_child(b_tree,offset,k+1,b_tree_get_child(b_tree,offset,k));
    k--;
  }
  //insere a chave na posição ideal
  b_tree_set_data(b_tree,offset,pos,data);
  b_tree_set_child(b_tree,offset,pos + 1,right_child);
  b_tree_set_number_of_keys(b_tree,offset,n_keys + 1);
  return TRUE;
}

small_t b_tree_get_number_of_keys(b_tree_t ** b_tree, byte_offset_t offset){
  byte_offset_t preserved_byte_offset = (*b_tree)->flush_offset;
  small_t return_value;

  b_tree_dereference(b_tree,offset);
  return_value = (*b_tree)->handled_node->number_keys;
  b_tree_dereference(b_tree,preserved_byte_offset);

  return return_value;
}

DATA * b_tree_get_data(b_tree_t ** b_tree, byte_offset_t offset, int i){
  byte_offset_t preserved_byte_offset = (*b_tree)->flush_offset;
  DATA * return_value = b_tree_create_data(NONE,NONE);

  b_tree_dereference(b_tree,offset);
  data_copy(return_value,&(*b_tree)->handled_node->page[i]);
  b_tree_dereference(b_tree,preserved_byte_offset);

  return return_value;
}

byte_offset_t b_tree_get_child(b_tree_t ** b_tree, byte_offset_t offset, int i){
  byte_offset_t preserved_byte_offset = (*b_tree)->flush_offset;
  byte_offset_t return_value;

  b_tree_dereference(b_tree,offset);
  return_value = (*b_tree)->handled_node->child[i];
  b_tree_dereference(b_tree,preserved_byte_offset);

  return return_value;
}

boolean_t b_tree_set_number_of_keys(b_tree_t ** b_tree, byte_offset_t offset, small_t number_keys){
  byte_offset_t preserved_byte_offset = (*b_tree)->flush_offset;

  b_tree_dereference(b_tree,offset);
  (*b_tree)->handled_node->number_keys = number_keys;
  (*b_tree)->modification[B_TREE_NODE_MODIFICATION] = TRUE;
  b_tree_dereference(b_tree,preserved_byte_offset);

  return TRUE;
}

boolean_t b_tree_set_data(b_tree_t ** b_tree, byte_offset_t offset, int i, DATA * data){
  byte_offset_t preserved_byte_offset = (*b_tree)->flush_offset;

  b_tree_dereference(b_tree,offset);
  data_copy(&(*b_tree)->handled_node->page[i],data);
  (*b_tree)->modification[B_TREE_NODE_MODIFICATION] = TRUE;
  b_tree_dereference(b_tree,preserved_byte_offset);

  return TRUE;
}

boolean_t b_tree_set_child(b_tree_t ** b_tree, byte_offset_t offset, int i, byte_offset_t child){
  byte_offset_t preserved_byte_offset = (*b_tree)->flush_offset;

  b_tree_dereference(b_tree,offset);
  (*b_tree)->handled_node->child[i] = child;
  (*b_tree)->modification[B_TREE_NODE_MODIFICATION] = TRUE;
  b_tree_dereference(b_tree,preserved_byte_offset);

  return TRUE;
}

byte_offset_t b_tree_insert_aux(b_tree_t ** b_tree, byte_offset_t root, DATA * data, boolean_t * h, DATA * return_data){
  DATA * promoted;
  byte_offset_t right_child;
  byte_offset_t children[2];
  int pos;
  byte_offset_t temp;


  if(!root){
    *h = TRUE;
    data_copy(return_data,data);
    return SEEK_SET;
  }
  else {
         pos = binary_search(b_tree,root,data);
         if ((b_tree_get_number_of_keys(b_tree,root) > pos) && (b_tree_get_data(b_tree,root,pos)->matricula == data->matricula)){
            //Matrícula já inserida
           *h = FALSE;
         }
         else {
               //desce na árvore até encontrar o nó folha para inserir a chave.
               right_child = b_tree_insert_aux(b_tree,b_tree_get_child(b_tree,root,pos),data,h,return_data);
               if (*h){ //Se true deve inserir a return_data no nó.
                     if (b_tree_get_number_of_keys(b_tree,root) < B_TREE_ORDER){ //Tem espaço na página
                       b_tree_insert_data(b_tree,root,return_data,right_child);
                       *h = FALSE;
                     }
                     else {
                        promoted = b_tree_get_data(b_tree,root,MIN_PAGE_SIZE);
                        children[LEFT] = b_tree_get_child(b_tree,root,MIN_PAGE_SIZE + 1);
                        children[RIGHT] = b_tree_get_child(b_tree,root,MIN_PAGE_SIZE + 2);
                        temp = b_tree_new_node(b_tree_get_data(b_tree,root,MIN_PAGE_SIZE + 1),children,b_tree);
                        for (int i = MIN_PAGE_SIZE + 2; i < B_TREE_ORDER; i++)
                          b_tree_insert_data(b_tree,temp,b_tree_get_data(b_tree,root,i),b_tree_get_child(b_tree,root,i + 1));
                         //atualiza nó raiz.
                        for (int i = MIN_PAGE_SIZE; i < B_TREE_ORDER; i++)
                          b_tree_set_child(b_tree,root,i + 1,SEEK_SET);
                        b_tree_set_number_of_keys(b_tree,root,MIN_PAGE_SIZE);

                         //Verifica em qual nó será inserida a nova chave
                        if (pos <= MIN_PAGE_SIZE)
                          b_tree_insert_data(b_tree,root,return_data,right_child);
                        else
                          b_tree_insert_data(b_tree,temp,return_data,right_child);

                       //retorna o mediano para inserí-lo no nó pai e o temp como filho direito do mediano.
                       data_copy(return_data,promoted);
                       return temp;
                       }
                }
            }
     }
  return SEEK_SET;
}

boolean_t b_tree_dereference(b_tree_t ** b_tree, byte_offset_t offset){
  int node_size = sizeof(b_tree_node);

  if(offset != (*b_tree)->flush_offset){
    if((*b_tree)->modification[B_TREE_NODE_MODIFICATION])
      b_tree_node_write(b_tree);
    fseek((*b_tree)->file,offset, SEEK_SET);
    fread((*b_tree)->handled_node,node_size,1,(*b_tree)->file);
    (*b_tree)->flush_offset = offset;
    (*b_tree)->modification[B_TREE_NODE_MODIFICATION] = FALSE;
  }

  return TRUE;
}

boolean_t b_tree_insert(DATA * data, b_tree_t ** b_tree){
  boolean_t h;
  DATA * return_data = b_tree_create_data(NONE,NONE);
  byte_offset_t children[2];
  byte_offset_t root;

  if(!*b_tree){
    *b_tree = create_b_tree(B_TREE_FILE,BINARY_DATA_CREATION_MODE);
  }
  root = b_tree_get_root(b_tree);
  children[RIGHT] = b_tree_insert_aux(b_tree,root,data,&h,return_data);
  if(h) { //Aumentar a altura da árvore
     children[LEFT] = root;
     b_tree_set_root_register(b_tree_new_node(return_data,children,b_tree),b_tree);
   }
  return TRUE;
}
