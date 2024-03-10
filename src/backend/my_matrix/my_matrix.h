#ifndef my_MATRIX_H
#define my_MATRIX_H

#include <math.h>
#include <stdlib.h>

/**
 * @brief Matrix structure, for storing the coordinates
 * It stores:
 * pointer for matrix
 * count of rows
 * count of columns
*/
typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;


/**
 * @brief Enum for handling different errors types.
 * It stores:
 * OK flag if all ok
 * INCORRECT_MATRIX if we have sone problems with matrix
 * CALC_ERROR if we have some calculation problems
 */
typedef enum {
  OK_M = 0,
  INCORRECT_MATRIX = 1,
  CALC_ERROR = 2,
} eflag_m;

/**
 * @brief            Create matrix with input demensions
 * 
 * @param rows       Quantity of rows
 * @param columns    Quantity of columns
 * @param result     Pointer to created matrix
 * 
 * @return           Bool 0 if OK / 1 if not
*/
int my_create_matrix(int rows, int columns, matrix_t *result);

/**
 * @brief            Remove all allocated memmory for matrix
 * 
 * @param A          Pointer to matrix that we whant to free
 * 
 * @return           Nan void function
*/
void my_remove_matrix(matrix_t *A);



/**
 * @brief            Check if matrix correct
 * 
 * @param matrix     Pointer to matrix that we whant to check
 * 
 * @return           Bool 1 if OK / 0 if not
*/
int my_matrix_correct(matrix_t *matrix);


/**
 * @brief           Function that multiplicate two Matrix
 * 
 * @param A         Pointer to first matrix that we whant to mult
 * @param B         Pointer to second matrix that we whant to mult
 * @param result    Pointer to result matrix
 * 
 * @return          Bool 0 if OK / 1 if not
*/
int my_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

#endif //  my_MATRIX_H

