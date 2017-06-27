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
  small_t token_len;
  small_t count;

  if(*control == NONE)
    *control = 0;
  for(count = 0; (*control < strlen(string)) && (strchr(delimiters, string[*control]) == NULL); (*control)++){
    token_len = count == 0 ? sizeof(char) : strlen(token);
    token = (string_t) realloc(token,(int)token_len + sizeof(char));
    token[(int)count] = string[*control];
    token[strlen(token)] = '\0';
    count++;
  }
  (*control)++;
  return token;
}
