#include "university.h"
#include "btree.h"

boolean_t data_file_close(data_file_t ** data_file){
  data_file_header_flush(data_file);
  data_file_register_flush(data_file);
  fclose((*data_file)->file);
  free((*data_file)->in_memory_header);
  free(*data_file);
  return TRUE;
}

boolean_t data_file_register_flush(data_file_t ** data_file){
  //Forçando o flush do registro
  data_file_register_push(NULL,data_file);
  return TRUE;
}

boolean_t data_file_register_freeing(binary_data_register * data_register){
  str_field_freeing(&data_register->cpf);
  str_field_freeing(&data_register->nome);
  str_field_freeing(&data_register->rg);
  str_field_freeing(&data_register->endereco);
  str_field_freeing(&data_register->telefone);
  str_field_freeing(&data_register->celular);
  str_field_freeing(&data_register->email);
  return TRUE;
}

data_file_t * create_data_file(string_t path, string_t creation_mode){
  data_file_t * data_file = (data_file_t *) malloc(sizeof(data_file_t));

  data_file->file = fopen(path,creation_mode);
  data_file->in_memory_header = (binary_data_header *) malloc(sizeof(binary_data_header));
  data_file_set_free_head_register(SEEK_SET,&data_file);
  data_file_set_fragmentation_ratio(0,&data_file);
  data_file_header_flush(&data_file);
  data_file->modification[DATA_FILE_REGISTER_MODIFICATION] = FALSE;
  data_file->flush_offset = (byte_offset_t) sizeof(data_file->in_memory_header);
  data_file->handled_register = (binary_data_register *) malloc(sizeof(binary_data_register));
  data_file->handled_register->cpf = NULL;
  data_file->handled_register->nome = NULL;
  data_file->handled_register->rg = NULL;
  data_file->handled_register->endereco = NULL;
  data_file->handled_register->telefone = NULL;
  data_file->handled_register->celular = NULL;
  data_file->handled_register->email = NULL;

  return data_file;
}

boolean_t data_file_set_fragmentation_ratio(small_t fragmentation_ratio, data_file_t ** data_file){
  (*data_file)->in_memory_header->fragmentation_ratio = fragmentation_ratio;
  (*data_file)->modification[DATA_FILE_HEADER_MODIFICATION] = TRUE;
  return TRUE;
}

boolean_t data_file_set_free_head_register(byte_offset_t free_head_register, data_file_t ** data_file){
  (*data_file)->in_memory_header->free_head_register = free_head_register;
  (*data_file)->modification[DATA_FILE_HEADER_MODIFICATION] = TRUE;
  return TRUE;
}

boolean_t data_file_header_flush(data_file_t ** data_file){
  //Houve modificação no header
  if((*data_file)->modification[DATA_FILE_HEADER_MODIFICATION]){
    fseek((*data_file)->file, 0, SEEK_SET);
    fwrite(&(*data_file)->in_memory_header,sizeof((*data_file)->in_memory_header),1,(*data_file)->file);
    (*data_file)->modification[DATA_FILE_HEADER_MODIFICATION] = FALSE;
  }
  return TRUE;
}

byte_offset_t data_file_register_write(data_file_t ** data_file){
  binary_data_register * data_register = (*data_file)->handled_register;
  byte_offset_t next_register_offset;

  fseek((*data_file)->file, (*data_file)->flush_offset, SEEK_SET);
  next_register_offset = ftell((*data_file)->file) + data_register->register_size;
  // Escrita dos campos de tamanho fixo
  fwrite(&data_register->register_size,sizeof(data_register->register_size),1,(*data_file)->file);
  fwrite(&data_register->variable_field_size,sizeof(data_register->variable_field_size),1,(*data_file)->file);
  fwrite(&data_register->matricula,sizeof(data_register->matricula),1,(*data_file)->file);
  fwrite(&data_register->sexo,sizeof(data_register->sexo),1,(*data_file)->file);
  fwrite(data_register->cpf,strlen(data_register->cpf),1,(*data_file)->file);
  fwrite(&data_register->data_nascimento,sizeof(data_register->data_nascimento),1,(*data_file)->file);

  // Escrita dos campos de tamanho variável
  fwrite(data_register->nome,data_register->variable_field_size[NOME + NOME_OFFSET],1,(*data_file)->file);
  fwrite(data_register->rg,data_register->variable_field_size[RG + OTHERS_OFFSET],1,(*data_file)->file);
  fwrite(data_register->endereco,data_register->variable_field_size[ENDERECO + OTHERS_OFFSET],1,(*data_file)->file);
  fwrite(data_register->telefone,data_register->variable_field_size[TELEFONE + OTHERS_OFFSET],1,(*data_file)->file);
  fwrite(data_register->celular,data_register->variable_field_size[CELULAR + OTHERS_OFFSET],1,(*data_file)->file);
  fwrite(data_register->email,data_register->variable_field_size[EMAIL + OTHERS_OFFSET],1,(*data_file)->file);

  (*data_file)->modification[DATA_FILE_REGISTER_MODIFICATION] = FALSE;

  return next_register_offset;
}

boolean_t data_file_copy_register(data_file_t ** data_file, binary_data_register * source){
  binary_data_register * destination = (*data_file)->handled_register;

  //Quando se força o flush, o data_register vem NULL
  if(source){
    data_file_register_freeing(destination);
    //Strings
    destination->nome = (string_t) malloc(sizeof(char)*strlen(source->nome) + 1);
    str_field_filling(source->nome,&destination->nome,&destination->variable_field_size[NOME + NOME_OFFSET]);
    destination->cpf = (string_t) malloc(sizeof(char)*strlen(source->cpf) + 1);
    str_field_filling(source->cpf,&destination->cpf,NULL);
    destination->rg = (string_t) malloc(sizeof(char)*strlen(source->rg) + 1);
    str_field_filling(source->rg,&destination->rg,&destination->variable_field_size[RG + OTHERS_OFFSET]);
    destination->telefone = source->telefone ? (string_t) malloc(sizeof(char)*strlen(source->telefone) + 1) : NULL;
    str_field_filling(source->telefone,&destination->telefone,&destination->variable_field_size[TELEFONE + OTHERS_OFFSET]);
    destination->celular = source->celular ? (string_t) malloc(sizeof(char)*strlen(source->celular) + 1) : NULL;
    str_field_filling(source->celular,&destination->celular,&destination->variable_field_size[CELULAR + OTHERS_OFFSET]);
    destination->email = source->email ? (string_t) malloc(sizeof(char)*strlen(source->email) + 1) : NULL;
    str_field_filling(source->email,&destination->email,&destination->variable_field_size[EMAIL + OTHERS_OFFSET]);
    destination->endereco = source->endereco ? (string_t) malloc(sizeof(char)*strlen(source->endereco) + 1) : NULL;
    str_field_filling(source->endereco,&destination->endereco,&destination->variable_field_size[ENDERECO + OTHERS_OFFSET]);

    //Demais campos
    destination->matricula = source->matricula;
    destination->sexo = source->sexo;
    destination->data_nascimento = source->data_nascimento;
    total_register_size_filling(destination);

    (*data_file)->modification[DATA_FILE_REGISTER_MODIFICATION] = TRUE;
  }

  return TRUE;
}

byte_offset_t data_file_register_push(binary_data_register * data_register, data_file_t ** data_file){
  byte_offset_t register_offset = (*data_file)->flush_offset;

  //Caso haja dados e os mesmos tenham sido modificados
  if((*data_file)->modification[DATA_FILE_REGISTER_MODIFICATION])
    (*data_file)->flush_offset = data_file_register_write(data_file);

  data_file_copy_register(data_file,data_register);

  return register_offset;
}

boolean_t data_file_insert(binary_data_register * data_register, data_file_t ** data_file, b_tree_t ** index_file){
  byte_offset_t register_offset;

  if(!*data_file)
    *data_file = create_data_file(DATA_FILE,BINARY_DATA_CREATION_MODE);
  register_offset = data_file_register_push(data_register,data_file);
  data_file_register_flush(data_file);
  // printf("%d: %d\n", data_register->matricula, register_offset);
  b_tree_insert(b_tree_create_data(data_register->matricula,register_offset),index_file);

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

boolean_t data_file_load_register(unsigned short matricula, data_file_t ** data_file, b_tree_t ** index_file){
  byte_offset_t register_offset = b_tree_search(index_file,matricula);
  binary_data_register * data_register = (*data_file)->handled_register;

  data_file_register_flush(data_file);
  (*data_file)->flush_offset = register_offset;
  fseek((*data_file)->file, register_offset, SEEK_SET);

  // Leitura dos campos de tamanho fixo
  fread(&data_register->register_size,sizeof(data_register->register_size),1,(*data_file)->file);
  fread(&data_register->variable_field_size,sizeof(data_register->variable_field_size),1,(*data_file)->file);
  fread(&data_register->matricula,sizeof(data_register->matricula),1,(*data_file)->file);
  fread(&data_register->sexo,sizeof(data_register->sexo),1,(*data_file)->file);
  fread(data_register->cpf,CPF_SIZE,1,(*data_file)->file);
  fread(&data_register->data_nascimento,sizeof(data_register->data_nascimento),1,(*data_file)->file);

  // Leitura dos campos de tamanho variável + colocação de terminador de string
  data_register->nome = (string_t) malloc(sizeof(char)*(int)data_register->variable_field_size[NOME + NOME_OFFSET] + sizeof(char));
  fread(data_register->nome,data_register->variable_field_size[NOME + NOME_OFFSET],1,(*data_file)->file);
  data_register->nome[(int)data_register->variable_field_size[NOME + NOME_OFFSET]] = '\0';

  data_register->rg = (string_t) malloc(sizeof(char)*(int)data_register->variable_field_size[RG + OTHERS_OFFSET] + sizeof(char));
  fread(data_register->rg,data_register->variable_field_size[RG + OTHERS_OFFSET],1,(*data_file)->file);
  data_register->rg[(int)data_register->variable_field_size[RG + OTHERS_OFFSET]] = '\0';

  data_register->endereco = (string_t) malloc(sizeof(char)*(int)data_register->variable_field_size[ENDERECO + OTHERS_OFFSET] + sizeof(char));
  fread(data_register->endereco,data_register->variable_field_size[ENDERECO + OTHERS_OFFSET],1,(*data_file)->file);
  data_register->endereco[(int)data_register->variable_field_size[ENDERECO + OTHERS_OFFSET]] = '\0';

  if(data_register->variable_field_size[TELEFONE + OTHERS_OFFSET]){
    data_register->telefone = (string_t) malloc(sizeof(char)*(int)data_register->variable_field_size[TELEFONE + OTHERS_OFFSET] + sizeof(char));
    fread(data_register->telefone,data_register->variable_field_size[TELEFONE + OTHERS_OFFSET],1,(*data_file)->file);
    data_register->telefone[(int)data_register->variable_field_size[TELEFONE + OTHERS_OFFSET]] = '\0';
  }
  else
    data_register->telefone[0] = '\0';

  if(data_register->variable_field_size[CELULAR + OTHERS_OFFSET]){
    data_register->celular = (string_t) malloc(sizeof(char)*(int)data_register->variable_field_size[CELULAR + OTHERS_OFFSET] + sizeof(char));
    fread(data_register->celular,data_register->variable_field_size[CELULAR + OTHERS_OFFSET],1,(*data_file)->file);
    data_register->celular[(int)data_register->variable_field_size[CELULAR + OTHERS_OFFSET]] = '\0';
  }
  else
    data_register->celular[0] = '\0';

  if(data_register->variable_field_size[EMAIL + OTHERS_OFFSET]){
    data_register->email = (string_t) malloc(sizeof(char)*(int)data_register->variable_field_size[EMAIL + OTHERS_OFFSET] + sizeof(char));
    fread(data_register->email,data_register->variable_field_size[EMAIL + OTHERS_OFFSET],1,(*data_file)->file);
    data_register->email[(int)data_register->variable_field_size[EMAIL + OTHERS_OFFSET]] = '\0';
  }
  else
    data_register->email[0] = '\0';

  (*data_file)->modification[DATA_FILE_REGISTER_MODIFICATION] = FALSE;

  return TRUE;

}

boolean_t load_input_file(string_t path_input_file, data_file_t ** data_file, b_tree_t ** index_file){
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
        data_file_insert(&data_register,data_file,index_file);
      }
      free(line);
      fclose(input_file);
      return TRUE;
    }
    else
      return FALSE;
}

boolean_t data_file_show_register(data_file_t ** data_file){
  binary_data_register * data_register = (*data_file)->handled_register;

  printf("\n-------------------------------\n");
  printf("Matrícula: %hu\n",data_register->matricula);
  printf("Nome: %s\n",data_register->nome);
  printf("Sexo: %c\n",data_register->sexo);
  printf("CPF: %s\n",data_register->cpf);
  printf("RG: %s\n",data_register->rg);
  printf("Telefone: %s\n",data_register->telefone);
  printf("Celular: %s\n",data_register->celular);
  printf("E-mail: %s\n",data_register->email);
  printf("Endereço: %s\n",data_register->endereco);
  printf("Data de Nascimento: XX/XX/XXXX\n");
  printf("-------------------------------\n\n");

  return TRUE;
}

int main(void){
  data_file_t * data_file = NULL;
  b_tree_t * index_file = NULL;
  unsigned short matricula = NONE;
  boolean_t first = TRUE;

  load_input_file(INPUT_FILE_PATH,&data_file,&index_file);

  while(matricula != 0) {
    if(!first){
      data_file_load_register(matricula,&data_file,&index_file);
      data_file_show_register(&data_file);
    }
    else
      first = FALSE;
    printf("Matricula: ");
    scanf("%hu",&matricula);

  }

  data_file_close(&data_file);
  b_tree_close(&index_file);
  return 0;
}
