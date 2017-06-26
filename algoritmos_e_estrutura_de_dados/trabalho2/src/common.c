#include "common.h"

time_t str_to_timestamp(string_t date, string_t format){
  structured_time_t s_date;

  if (strptime(date, format, &s_date) != NULL)
    return mktime(&s_date);
  else
    return NONE;
}

string_t tokenizer(string_t string, string_t delimiters, int * control){
  string_t token = NULL;
  small_t token_size;
  small_t count;

  if(*control == NONE)
    *control = 0;
  for(count = 0; (*control < strlen(string)) && (strchr(delimiters,string[(int)*control]) == NULL); (*control)++){
    if(count == 0)
      token =(string_t) malloc(sizeof(char));
    token_size = strlen(token);
    token[(int)count] = string[(int)*control];
    if(realloc(token,token_size + 1)){
      token[count + 1] = '\0';
      count++;
    }
  }
  (*control)++;
  return token;
}
