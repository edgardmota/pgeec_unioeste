#include "btree.h"

boolean_t b_tree_insert(DATA * data, FILE ** index_file){
    printf("%hu: %llu\n",data->matricula,data->byte_offset);
    return TRUE;
}

DATA * create_btree_data(unsigned short matricula, byte_offset_t byte_offset){
  DATA * data = (DATA *) malloc(sizeof(data));

  data->matricula = matricula;
  data->byte_offset = byte_offset;
  return data;
}
