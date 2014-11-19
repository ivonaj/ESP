//------------------------------------------------------------------------------
// hw5.c
//
// File reading and reversing to the output.
//
// Group: - 
//
// Authors: Peter Lorenz
//
// Latest Changes:
//------------------------------------------------------------------------------
//
// STR + D EOF erzeugen
#include <stdio.h>
#include <stdlib.h> //for malloc
#define BUFFER 2
#define ERR_OUT_OF_MEM 1


//------------------------------------------------------------------------------
// readInput reads all characters from a file until EOF
// @length: must be declared in order to use it out of scope
//
// @return: 0 for success and 2 for error
//
int readInput(char **output, int *length)
{
  int input, len;
  char *new_file = NULL;
  *output = malloc(sizeof(char) * BUFFER);
  if(*output == NULL)
  {
    printf("Out of memory!\n");
    return ERR_OUT_OF_MEM;
  }

  for(len = 0; (input = getchar()) != EOF; len++)
  {
    (*output)[len] = input;
    new_file = (char *) realloc(*output, len + BUFFER);
    
    if(new_file != NULL)
    {
      *output = new_file;
    }
    else
    {
      printf("Out of memory!\n");
      free(*output);
      return ERR_OUT_OF_MEM;
    }
  }

  (*output)[len] = '\0';
  printf("%s", *output);
  *length = len; //needed to flee from scope...
  return 0;
}


//------------------------------------------------------------------------------
// reverseInput takes the already stored string and give it out to the stdout
// in the reverse order.
// @output: is already filled with chars
//
void reverseInput(char *output, int len_rev)
{
  for(; (len_rev > 0); len_rev--)
  {
    printf("%c", output[(len_rev) - 1]);
  }
  printf("\n");
}


int main()
{
  int len_rev;
  char *output = NULL;
  if(readInput(&output, &len_rev) != 1)
  {
    reverseInput(output, len_rev);
    free(output);
    return 0;
  }
  else
  {
    return 1;
  }
}
