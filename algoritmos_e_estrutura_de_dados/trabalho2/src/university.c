#include "university.h"

int mapping_function(small_t source, small_t mapping_type){
  int result = NONE;
  switch (mapping_type) {
    case DATA_MAPPING:
      // Interpolação Polinomial obtida pela resolução de Sistema Linear com Matriz de Vandermonde
      result = round(2  - 3.8714 * source + 2.4115 * pow(source,2) - 0.62202 * pow(source,3) + 0.088294 * pow(source,4) - 0.0065476 * pow(source,5) + 0.00019841 * pow(source,6));
      break;
    case SIZE_MAPPING:
      // Interpolação Polinomial obtida pela forma de Newton
      result = round((15873 * pow(source,5) - 476191 * pow(source,4) + 5634925 * pow(source,3) - 32857209 * pow(source,2) + 74349562 * source - 46666960)/20000000);
      break;
  }
  return result;
}

boolean_t data_mapping(string_t ** str_fields, binary_data_register * data_register){
  str_fields[mapping_function(NOME,DATA_MAPPING)] = &data_register->nome;
  str_fields[mapping_function(CPF,DATA_MAPPING)] = &data_register->cpf;
  str_fields[mapping_function(RG,DATA_MAPPING)] = &data_register->rg;
  str_fields[mapping_function(TELEFONE,DATA_MAPPING)] = &data_register->telefone;
  str_fields[mapping_function(CELULAR,DATA_MAPPING)] = &data_register->celular;
  str_fields[mapping_function(EMAIL,DATA_MAPPING)] = &data_register->email;
  str_fields[mapping_function(ENDERECO,DATA_MAPPING)] = &data_register->endereco;
  return TRUE;
}

boolean_t store(string_t token, small_t position, string_t ** mapping, binary_data_register * data_register){
  string_t * str_field;

  switch (position) {
    // Strings
    case NOME:
    case CPF:
    case RG:
    case TELEFONE:
    case CELULAR:
    case EMAIL:
    case ENDERECO:
      str_field = mapping[mapping_function(position,DATA_MAPPING)];
      if(token){
        *str_field = token;
        if(position != CPF)
          data_register->variable_field_size[mapping_function(position,SIZE_MAPPING)] = strlen(*str_field);
        }
      else{
        *str_field = NULL;
        data_register->variable_field_size[mapping_function(position,SIZE_MAPPING)] = 0;
      }
      break;
    case MATRICULA:
      data_register->matricula = atoi(token);
      break;
    case DATA_NASC:
      data_register->data_nascimento = str_to_timestamp(token,DATE_FORMAT);
      break;
    case SEXO:
      data_register->sexo = token[0];
      break;
  }
  return TRUE;
}

boolean_t load_input_file(string_t path_input_file, FILE * data_file, FILE * index_file){
    string_t * str_fields[NUMBER_OF_VARIABLE_SIZE_FIELDS + 1]; //CPF é fixo mas é string, por isso + 1
    FILE * input_file;
    small_t position;
    string_t line;
    string_t token;
    size_t n = 0;
    int bytes_read;
    int tokenizer_control;
    binary_data_register data_register;

    input_file = fopen(path_input_file,"r");
    data_mapping(str_fields,&data_register);
    if(input_file){
      while ((bytes_read = getline(&line, &n, input_file)) != NONE) {
        printf("%s",line);
        // Extração da Matricula
        tokenizer_control = NONE;
        position = 0;
        token = tokenizer(line,INPUT_FILE_DELIMITERS,&tokenizer_control);
        store(token,position,str_fields,&data_register);
        // Extração dos demais campos
        for(position++ ; position <= NUMBER_FIELDS-1; position++){
          token = tokenizer(line,INPUT_FILE_DELIMITERS,&tokenizer_control);
          store(token,position,str_fields,&data_register);
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
