#include "../includes/tetris.h"

void printMatrix(int **matrix, int rows, int cols) {
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}

int **createMatrix(int rows, int cols) {
  int **matrix = malloc(rows * sizeof(int *) + rows * cols * sizeof(int));
  if (matrix == NULL) {
    return NULL;
  }
  int *ptr = (int *)(matrix + rows);
  for (int i = 0; i < rows; i++) {
    matrix[i] = ptr + cols * i;
  }

  int count = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix[i][j] = count;
    }
  }

  return matrix;
}

void freeMatrix(int **matrix) { free(matrix); }
