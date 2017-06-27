#include "university.h"

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

boolean_t load_input_file(string_t path_input_file, FILE * data_file, FILE * index_file){
    FILE * input_file;
    small_t position;
    string_t line;
    string_t token;
    size_t n = 0;
    int bytes_read;
    int tokenizer_control;
    binary_data_register data_register;

    input_file = fopen(path_input_file,INPUT_FILE_OPEN_MODE);

    if(input_file){
      while ((bytes_read = getline(&line, &n, input_file)) != NONE) {

        // Extração da Matricula
        tokenizer_control = NONE;
        position = 0;
        token = tokenizer(line,INPUT_FILE_DELIMITERS,&tokenizer_control);
        register_filling(token,position,&data_register);

        // Extração dos demais campos
        for(position++ ; position <= NUMBER_FIELDS-1; position++){
          token = tokenizer(line,INPUT_FILE_DELIMITERS,&tokenizer_control);
          register_filling(token,position,&data_register);
        }
        total_register_size_filling(&data_register);
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
  load_input_file(INPUT_FILE_PATH,NULL,NULL);
  return 0;
}
