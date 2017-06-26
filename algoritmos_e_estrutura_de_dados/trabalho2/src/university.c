#include "university.h"

int mapping_function(small_t source){
  int result =  floor(2  - 3.8714 * source + 2.4115 * pow(source,2) - 0.62202 * pow(source,3) + 0.088294 * pow(source,4) - 0.0065476 * pow(source,5) + 0.00019841 * pow(source,6));
  return result;
}

boolean_t mapping(string_t ** str_fields, binary_data_register * data_register){
  str_fields[mapping_function(NOME)] = &data_register->nome;
  str_fields[mapping_function(CPF)] = &data_register->cpf;
  str_fields[mapping_function(RG)] = &data_register->rg;
  str_fields[mapping_function(TELEFONE)] = &data_register->telefone;
  str_fields[mapping_function(CELULAR)] = &data_register->celular;
  str_fields[mapping_function(EMAIL)] = &data_register->email;
  str_fields[mapping_function(ENDERECO)] = &data_register->endereco;
  return TRUE;
}

boolean_t store(string_t token, small_t position, binary_data_register * data_register){
  string_t * str_fields[NUMBER_OF_VARIABLE_SIZE_FIELDS];
  string_t * str_field;

  mapping(str_fields,data_register);
  switch (position) {
    // Strings
    case NOME:
    case CPF:
    case RG:
    case TELEFONE:
    case CELULAR:
    case EMAIL:
    case ENDERECO:
      str_field = str_fields[mapping_function(position)];
      *str_field = (string_t) malloc(sizeof(string_t)*strlen(token) + 1);
      strcpy(*str_field,token);
      break;
    case MATRICULA:
      data_register->matricula = atoi(token);
      break;
    case DATA_NASC:
      data_register->data_nascimento = strtoul(token,NULL,10);
      break;
    case SEXO:
      data_register->sexo = token[0];
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
    binary_data_register data_register;

    input_file = fopen(path_input_file,"r");
    if(input_file){
      while ((bytes_read = getline(&line, &n, input_file)) != NONE) {
        // Extração da Matricula
        position = 0;
        token = strtok(line,INPUT_FILE_DELIMITERS);
        store(token,position,&data_register);
        // Extração dos demais campos
        for(; position < NUMBER_FIELDS-1; position++){
          store(token,position,&data_register);
          token = strtok(NULL,INPUT_FILE_DELIMITERS);
        }
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
