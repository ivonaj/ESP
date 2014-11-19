#include <stdio.h>
#define SIZEBLOCK 4
#define NUMBERMATRIX 4


//------------------------------------------------------------------------------
// print3DMatrix prints all matrixs side by side to the standard output
// @matrix: must be declared to be printed
void print3DMatrix(char matrix[NUMBERMATRIX][SIZEBLOCK][SIZEBLOCK])
{
  int i, j, number_matrix;
  for(i = 0; i < SIZEBLOCK; i++)
  {
    for(number_matrix = 0; number_matrix < NUMBERMATRIX; number_matrix++)
    {
      for(j = 0; j < SIZEBLOCK; j++)
      {
        printf("%c", matrix[number_matrix][i][j]);
        printf(j + 1 < SIZEBLOCK ? " " : "");
      }
      if(number_matrix < 3)
        printf(i < NUMBERMATRIX ? "  " : "");
    }
    printf("\n");
  }
}

/*
//------------------------------------------------------------------------------
// printMatrix prints a 2D matrix to the stdoutput
void printMatrix(char print_matrix[][COL]) //compiler needs to know 
{                                          //the size of the array
  int i;
  int j;
  for(i = 0; i < ROW; i++)
  {
    for(j = 0; j < COL; j++)
    {
      printf("%c ", print_matrix[i][j]);
    }
    printf("\n");
  }
  
}
*/


//------------------------------------------------------------------------------
// rotate3DMatrixClockwise
void rotate3DMatrixClockwise(char rotate_matrix[][SIZEBLOCK])
{
  char matrix[SIZEBLOCK][SIZEBLOCK];
  int i, j;
  
  for(i = 0; i < SIZEBLOCK; i++)
  {
    for(j = 0; j < SIZEBLOCK; j++)
    {
      matrix[i][j] = rotate_matrix[i][j];
    }
  }
  
  for(i = 0; i < SIZEBLOCK; i++)
  {
    for(j = 0; j < SIZEBLOCK; j++)
    {
      rotate_matrix[i][j] = matrix[SIZEBLOCK - j - 1][i];
    }
  }
}
int main()
{
  char matrix_ultimate[NUMBERMATRIX][SIZEBLOCK][SIZEBLOCK] = 
  { 
    {
      {'-','-','-','-'},
      {'-','o','-','-'},
      {'o','o','o','-'},
      {'-','-','-','-'}
    },
    
    {
      {'-','-','-','-'},
      {'-','o','o','-'},
      {'o','o','-','-'},
      {'-','-','-','-'}
    },
      
    {
      {'-','-','-','-'},
      {'-','o','-','-'},
      {'-','o','-','-'},
      {'-','o','o','-'}
    },
      
    {
      {'-','-','o','-'},
      {'-','-','o','-'},
      {'-','-','o','-'},
      {'-','-','o','-'}
    }
  };

  
  
  
  int rotations, block;
  for (rotations = 0; rotations <= 4; rotations++)
  {
    print3DMatrix(matrix_ultimate);
    if(rotations < 4)
      printf("\n");
    for (block = 0; block < NUMBERMATRIX; block++)
      rotate3DMatrixClockwise(matrix_ultimate[block]);
  }
  
  return 0;
}
