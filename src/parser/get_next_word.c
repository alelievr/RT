#include "parser.h"

int   get_next_word(char **str, char *res)
{
  char *b = *str;

  if (!**str)
    return (0);
  while(ft_isspace(**str) && **str)
    (*str)++;
  if (!**str)
    return (0);
  while(!ft_isspace(**str) && **str)
    *res++ = *(*str)++;
  *res = 0;
  return (*str - b);
}
