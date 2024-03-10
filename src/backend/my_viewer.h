#ifndef my_VIEWER_H
#define my_VIEWER_H

#define _GNU_SOURCE
#include <getopt.h>
#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "my_matrix/my_matrix.h"

/**
 * @brief Facets structure, for working with facets
 * It stores:
 * pointer for array with vertices coordinates for facet
 * count of vertices in facet
 */
typedef struct facets {
  int *vertices;
  int count_of_vertices_in_facet;
} facets_t;

/**
 * @brief Data structure.
 * It stores:
 * int value for vertices and facets amount
 * double array for min_x, max_x, min_y, max_y, min_z, max_z
 * matrix structure for coordinates
 * facets structure for storing facets
 */
typedef struct data {
  int vertices_count;
  int facets_count;
  double min_max[6];
  matrix_t coordinate_matrix;
  facets_t *facets;
} data_t;

/**
 * @brief Enum for handling different errors types.
 * It stores:
 * OK flag if all ok
 * Error flag if we have some problems
 */
typedef enum ERR_flags {
  OK = 0,
  ERROR = 1,
  ERROR_FILE = 2,
  ERROR_DATA = 3,
  ERROR_MEMMORY = 4
} eflag;

// parser functions

/**
 * @brief                   Parsing file in type .obj
 *
 * @param file              Pointer for file
 * @param data              Pointer for data structure that we are going to fill
 * from file
 *
 * @return                  Error flag
 */
eflag my_parser(const char *file, data_t *data);

/**
 * @brief                   Set data with vertices coordinates
 *
 * @param A                 Matrix with coordinates
 * @param line              Line with vertices coordinates
 * @param min_max           Array with min/max coordinates of each vertices
 *
 * @return                  Error flag
 */
eflag set_vertices_data(matrix_t *A, char *line, double *min_max);

/**
 * @brief                   Increasing matrix shape
 *
 * @param A                 Matrix with coordinates
 *
 * @return                  Error flag
 */
eflag increasing_matrix(matrix_t *A);

/**
 * @brief                   Set data with facets
 *
 * @param line              Line with vertices in facet
 * @param facets            Structure with facets
 * @param count             Quantity of facets
 *
 * @return                  Error flag
 */
eflag set_facets_data(char *line, facets_t **facet, int count);

// help func

/**
 * @brief                   Init data that we will fill from parsing file
 *
 * @param data              Pointer for data structure of our model that we are
 * going to fill from file
 *
 * @return                  Nan void function
 */
void my_init_data(data_t *data);

/**
 * @brief                   Free all allocated memmory for facet structure
 *
 * @param facet             Pointer for facer structure that we are going to
 * free
 * @param count_of_facets   Quantity of facets to free
 *
 * @return                  Nan void function
 */
void my_remove_facet(facets_t **facet, int count_of_facets);

/**
 * @brief                   Free all allocated memmory
 *
 * @param data              Pointer for data structure of our model that we are
 * going to free
 *
 * @return                  Nan void function
 */
void my_remove_data(data_t *data);

//  affine functions

/**
 * @brief                   Help function to find min and max value of x, y, z
 * coordinates of vertexes in 3D model (data->coordinate_matrix.matrix) and puts
 * them into array data->min_max = [x_min, x_max, y_min, y_max, z_min, z_max]
 *
 * @param data              Pointer for data structure of our model in which we
 * ara going to find minmax of x,y,z
 *
 * @return                  Error flag
 */
eflag get_minmax(data_t *data);

/**
 * @brief                   Function performs rotation of model by angle
 * (x_angle; y_angle; z_angle)
 *
 * @param data              Pointer for data structure of our model that we are
 * going to rotate
 * @param angles            Array with angles for rotation
 * @param upd_min_max       Variable to update min max meaning
 *
 * @return                  Error flag
 */
eflag affine_rotate(data_t *data, double angles[3], int upd_min_max);

/**
 * @brief                   Function performs scaling of model by scale
 * coefficient
 *
 * @param data              Pointer for data structure of our model that we are
 * going to scale
 * @param angles            Scaling coefficient
 *
 * @return                  Error falg
 */
eflag affine_scale(data_t *data, double scale);

/**
 * @brief                   Function performs translating of model by
 * translating coefficient of x, y, z
 *
 * @param data              Pointer for data structure of our model that we are
 * going to translate
 * @param tranc_coef        Translating coefficient [x_c, y_c, z_c]
 *
 * @return                  Error falg
 */
eflag affine_translate(data_t *data, double tranc_coef[3]);

/**
 * @brief                   Help function to normalize model coordinates so we
 * can easily work
 *
 * @param data              Pointer for data structure of our model that we are
 * going to normalize
 *
 * @return                  Error flag
 */
eflag normalize_model_data(data_t *data);

#endif  //  my_VIEWER_H