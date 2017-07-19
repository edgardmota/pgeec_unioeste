#include "university.h"
#include "btree.h"

boolean_t write_to_disk(binary_data_register * data_register, FILE ** data_file, FILE ** index_file){
  binary_data_header header;
  byte_offset_t register_offset;

  if(!*data_file){
    *data_file = fopen(DATA_FILE,BINARY_DATA_CREATION_MODE);
    header.free_head_register = BYTE_OFFSET_0;
    header.fragmentation_ratio = 0;
    fwrite(&header,sizeof(header),1,*data_file);
  }
  register_offset = ftell(*data_file);

  // Escrita dos campos de tamanho fixo
  fwrite(&data_register->register_size,sizeof(data_register->register_size),1,*data_file);
  fwrite(&data_register->variable_field_size,sizeof(data_register->variable_field_size),1,*data_file);
  fwrite(&data_register->matricula,sizeof(data_register->matricula),1,*data_file);
  fwrite(&data_register->sexo,sizeof(data_register->sexo),1,*data_file);
  fwrite(data_register->cpf,strlen(data_register->cpf),1,*data_file);
  fwrite(&data_register->data_nascimento,sizeof(data_register->data_nascimento),1,*data_file);

  // Escrita dos campos de tamanho variável
  fwrite(data_register->nome,data_register->variable_field_size[NOME + NOME_OFFSET],1,*data_file);
  fwrite(data_register->rg,data_register->variable_field_size[RG + OTHERS_OFFSET],1,*data_file);
  fwrite(data_register->endereco,data_register->variable_field_size[ENDERECO + OTHERS_OFFSET],1,*data_file);
  fwrite(data_register->telefone,data_register->variable_field_size[TELEFONE + OTHERS_OFFSET],1,*data_file);
  fwrite(data_register->celular,data_register->variable_field_size[CELULAR + OTHERS_OFFSET],1,*data_file);
  fwrite(data_register->email,data_register->variable_field_size[EMAIL + OTHERS_OFFSET],1,*data_file);

  b_tree_insert(create_btree_data(data_register->matricula,register_offset),index_file);

  return TRUE;
}

boolean_t str_field_filling(string_t token, string_t * field, small_t * size){
  if(size)
    *size = token ? strlen(token): 0;
  *field = token;
  return TRUE;
}

boolean_t str_field_freeing(string_t * field){
  if (*field){
    free(*field);
    *field = NULL;
  }
  return TRUE;
}

boolean_t total_register_size_filling(binary_data_register * data_register){
  data_register_size_t register_size = 0;

  // Tamanho dos campos de tamanho fixo
  register_size += sizeof(data_register->register_size);
  register_size += sizeof(data_register->variable_field_size);
  register_size += sizeof(data_register->matricula);
  register_size += sizeof(data_register->sexo);
  register_size += strlen(data_register->cpf);
  register_size += sizeof(data_register->data_nascimento);

  // Campos de Tamanho variável
  for(int i; i < NUMBER_OF_VARIABLE_SIZE_FIELDS; i++){
    register_size += data_register->variable_field_size[i];
  }
  data_register->register_size = register_size;
  return TRUE;
}

boolean_t register_filling(string_t token, small_t position, binary_data_register * data_register){
  switch (position) {
    // Strings
    case NOME:
      str_field_filling(token,&data_register->nome,&data_register->variable_field_size[NOME + NOME_OFFSET]);
      break;
    case CPF:
      str_field_filling(token,&data_register->cpf,NULL);
      break;
    case RG:
      str_field_filling(token,&data_register->rg,&data_register->variable_field_size[RG + OTHERS_OFFSET]);
      break;
    case TELEFONE:
      str_field_filling(token,&data_register->telefone,&data_register->variable_field_size[TELEFONE + OTHERS_OFFSET]);
      break;
    case CELULAR:
      str_field_filling(token,&data_register->celular,&data_register->variable_field_size[CELULAR + OTHERS_OFFSET]);
      break;
    case EMAIL:
      str_field_filling(token,&data_register->email,&data_register->variable_field_size[EMAIL + OTHERS_OFFSET]);
      break;
    case ENDERECO:
      str_field_filling(token,&data_register->endereco,&data_register->variable_field_size[ENDERECO + OTHERS_OFFSET]);
      break;
    case MATRICULA:
      data_register->matricula = atoi(token);
      free(token);
      break;
    case DATA_NASC:
      data_register->data_nascimento = str_to_timestamp(token,DATE_FORMAT);
      free(token);
      break;
    case SEXO:
      data_register->sexo = token[0];
      free(token);
      break;
  }
  return TRUE;
}

boolean_t load_input_file(string_t path_input_file, FILE ** data_file, FILE ** index_file){
    FILE * input_file;
    small_t position;
    string_t line;
    string_t token;
    size_t n = 0;
    int bytes_read;
    int tokenizer_control; //Índice da posição que permite a tokenização por meio de sucessivas chamadas da função tokenizer
    binary_data_register data_register;

    input_file = fopen(path_input_file,INPUT_FILE_OPEN_MODE);

    if(input_file){
      while ((bytes_read = getline(&line, &n, input_file)) != NONE) {
        tokenizer_control = NONE;
        for(position = 0; position <= NUMBER_FIELDS-1; position++){
          token = tokenizer(line,INPUT_FILE_DELIMITERS,&tokenizer_control);
          register_filling(token,position,&data_register);
        }
        total_register_size_filling(&data_register);
        write_to_disk(&data_register,data_file,index_file);
        str_field_freeing(&data_register.nome);
        str_field_freeing(&data_register.cpf);
        str_field_freeing(&data_register.rg);
        str_field_freeing(&data_register.telefone);
        str_field_freeing(&data_register.celular);
        str_field_freeing(&data_register.email);
        str_field_freeing(&data_register.endereco);
      }
      free(line);
      fclose(input_file);
      return TRUE;
    }
    else
      return FALSE;
}

int main(void){
  FILE * data_file = NULL;
  FILE * index_file = NULL;

  load_input_file(INPUT_FILE_PATH,&data_file,&index_file);
  return 0;
}
