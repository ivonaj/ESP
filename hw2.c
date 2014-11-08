//-----------------------------------------------------------------------------
// hw2.c
//
// Turm rechnen!
//
// Group: 
//
// Authors: P
//
// Latest Changes: 
//-----------------------------------------------------------------------------
//

#include <stdio.h>
#include <limits.h> //needed for INTMAX
//#define INTMAX 1333758960

int main()
{
  int matrikel_nr = 1114658;
  
  int i;
  for(i = 2; i < 10; i++)
  {
    printf("%10d * %d\n", matrikel_nr, i);
    
    if(matrikel_nr >= INT_MAX / i) //matrikel_nr * i >= INT_MAX 
    {
      printf("An overflow has occurred!\n");
      return 0;
    }
    
    matrikel_nr = matrikel_nr * i;
  }
  
  for(i = 2; i < 10; i++)
  {
    printf("%10d : %d\n", matrikel_nr, i);
    matrikel_nr = matrikel_nr / i;
  }
  printf("%10d", matrikel_nr);
  
  return 0;
}





