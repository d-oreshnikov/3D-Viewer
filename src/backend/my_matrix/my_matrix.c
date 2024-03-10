#include "my_matrix.h"


int my_create_matrix(int rows, int columns, matrix_t *result) {
        /*
    Description:
        Create matrix with input demensions

    Args:
        int rows              : Quantity of rows
        int columns           : Quantity of columns
        (matrix_t*) result    : Pointer to result matrix 

    Returns:
         Bool 0 if OK / 1 if not
    */

    eflag_m error = OK_M;
    if (rows < 1 || columns < 1) {
        error = INCORRECT_MATRIX;
    } else {
        result->rows = rows;
        result->columns = columns;
        result->matrix = (double **)calloc(rows, sizeof(double *));
    }
    if (result->matrix != NULL) {
        for (int i = 0; i < rows; i++) {
        result->matrix[i] = (double *)calloc(columns, sizeof(double));
        if (!result->matrix[i]) {
            for (int j = 0; j < i; j++) free(result->matrix[j]);
            free(result->matrix);
        }
        }
    }
    return error;
}


void my_remove_matrix(matrix_t *A) {
        /*
    Description:
        Remove all allocated memmory for matrix

    Args:
        (matrix_t *) A              : Pointer to matrix that we whant to free

    Returns:
        Nan void function
    */

    if (A != NULL) {
        for (int i = 0; i < A->rows; i++) free(A->matrix[i]);
        free(A->matrix);
    }
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
}



int my_matrix_correct(matrix_t *matrix) {
        /*
    Description:
        Check if matrix correct

    Args:
        (matrix_t *) matrix          : Pointer to matrix that we whant to check

    Returns:
        Bool 1 if OK / 0 if not
    */

    return matrix && (matrix->rows > 0) && (matrix->columns > 0) &&
         matrix->matrix;
}


int my_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
        /*
    Description:
        Function that multiplicate two Matrix

    Args:
        (matrix_t *) A          : Pointer to first matrix that we whant to mult
        (matrix_t *) B          : Pointer to second matrix that we whant to mult
        (matrix_t *) result     : Pointer to result matrix

    Returns:
        Bool 0 if OK / 1 if not
    */

  eflag_m err = INCORRECT_MATRIX;
  if (my_matrix_correct(A) && my_matrix_correct(B)) {
    if (A->columns == B->rows) {
      err = my_create_matrix(A->rows, B->columns, result);
      if (err == OK_M) {
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < B->columns; j++) {
            for (int k = 0; k < B->rows; k++) {
              result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
            }
          }
        }
      }
    } else {
      err = CALC_ERROR;
    }
  }
  return err;
}
