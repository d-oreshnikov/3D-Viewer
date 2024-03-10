#include "my_viewer.h"

void my_init_data(data_t *data) {
  /*
Description:
  Init data that we will fill from parsing file

Args:
  (data_t*) data    : Pointer for data structure of our model that we are going
to fill from file

Returns:
  Nan void function
*/

  data->facets_count = 0;
  data->vertices_count = 0;
  for (int i = 0; i < 6; i++) {
    data->min_max[i] = 0;
  }

  data->coordinate_matrix.matrix = NULL;
  data->coordinate_matrix.columns = 0;
  data->coordinate_matrix.rows = 0;
  data->facets = NULL;
}

void my_remove_facet(facets_t **facet, int count_of_facets) { /*
Description:
 Free all allocated memmory for facet structure

Args:
 (facets_t*) facet    : Pointer for facer structure that we are going to free
 int count_of_facets  : Quantity of facets to free

Returns:
 Nan void function
*/

  if ((*facet) != NULL) {
    for (int i = 0; i < count_of_facets; i++) {
      if ((*facet)[i].vertices != NULL) {
        free((*facet)[i].vertices);
        (*facet)[i].vertices = NULL;
        (*facet)[i].count_of_vertices_in_facet = 0;
      }
    }
    free(*facet);
    (*facet) = NULL;
  }
}

void my_remove_data(data_t *data) {
  /*
Description:
  Free all allocated memmory

Args:
  (data_t*) data    : Pointer for data structure of our model that we are going
to free

Returns:
  Nan void function
*/

  my_remove_matrix(&(data->coordinate_matrix));

  data->facets_count = 0;
  data->vertices_count = 0;
  for (int i = 0; i < 6; i++) {
    data->min_max[i] = 0;
  }
}

eflag set_facets_data(char *line, facets_t **facet, int count) {
  /*
Description:
  Set data with facets

Args:
  (char*) line          : Line with vertices in facet
  (facets_t**) facets   : Structure with facets
  (int) count           : Count of facets

Returns:
  error flag
*/

  eflag status = OK;
  size_t n_vertices = 0;
  facets_t *tmp_facet_pointer = NULL;
  int *tmp_vertices_ptr = NULL;

  if (count == 0) {
    tmp_facet_pointer = (facets_t *)calloc(1, sizeof(facets_t));
  } else {
    tmp_facet_pointer =
        (facets_t *)realloc((*facet), (sizeof(facets_t) * (count + 1)));
  }

  if (tmp_facet_pointer == NULL) {
    status = ERROR_MEMMORY;
  } else {
    *facet = tmp_facet_pointer;
    (*facet)[count].count_of_vertices_in_facet = 0;
    (*facet)[count].vertices = NULL;
  }

  line += 2;
  char *iline = strtok(line, " ");

  while (status == OK && iline != NULL && atoi(iline) > 0) {
    if (n_vertices == 0) {
      tmp_vertices_ptr = (int *)calloc(1, sizeof(int));
    } else {
      tmp_vertices_ptr = (int *)realloc((*facet)[count].vertices,
                                        sizeof(int) * (n_vertices + 1));
    }

    if (tmp_vertices_ptr == NULL) {
      status = ERROR_MEMMORY;
    } else {
      (*facet)[count].vertices = tmp_vertices_ptr;
    }

    (*facet)[count].vertices[n_vertices] = atoi(iline);
    iline = strtok(NULL, " ");
    n_vertices++;
  }

  if (status == OK) {
    (*facet)[count].count_of_vertices_in_facet = n_vertices;
  }
  return status;
}

eflag increasing_matrix(matrix_t *A) {
  /*
Description:
  Increasing matrix shape

Args:
  (matrix_t*) A         : Pointer to Matrix with coordinates

Returns:
  error flag
*/

  double **temp_matrix;
  double *temp_row;

  eflag status = OK;
  A->rows++;
  temp_matrix = (double **)realloc(A->matrix, (sizeof(double *) * (A->rows)));

  if (temp_matrix != NULL) {
    A->matrix = temp_matrix;
    temp_row = (double *)calloc(3, sizeof(double));
    if (temp_row != NULL) {
      A->matrix[A->rows - 1] = temp_row;
    } else {
      status = ERROR_MEMMORY;
    }
  } else {
    status = ERROR_MEMMORY;
  }
  // maybe need to free temp matrix
  return status;
}

eflag set_vertices_data(matrix_t *A, char *line, double *min_max) {
  /*
Description:
  Set data with vertices coordinates

Args:
  (matrix_t*) A         : Pointer to Matrix with coordinates
  (char*) line          : Pointer to line with vertices coordinates
  (double*) min_max     : Pointer to array with min/max coordinates of each
vertices

Returns:
  error flag
*/

  double value = 0.0;
  eflag status = OK;

  if (A && A->rows == 0 && A->columns == 0) {
    status = my_create_matrix(1, 3, A);
  } else {
    status = increasing_matrix(A);
  }

  if (status == OK) {
    line += 2;
    char *iline = strtok(line, " ");

    for (int i = 0; i < 3; i++) {
      value = atof(iline);
      A->matrix[A->rows - 1][i] = value;

      if (min_max[2 * i] > value) {
        min_max[2 * i] = value;
      } else if (min_max[2 * i + 1] < value) {
        min_max[2 * i + 1] = value;
      }
      // make some tests why like this
      iline = strtok(NULL, " ");
    }
  }
  return status;
}

eflag my_parser(const char *file, data_t *data) {
  /*
Description:
  parsing given file

Args:
  (const char*) file    : Pointer for file .obj for parsing
  (data_t*) data        : Pointer for data structure of our model that we are
going to fill from file

Returns:
  error flag
*/

  eflag status = OK;
  int regflag = REG_EXTENDED;
  FILE *fp = NULL;
  char *line = NULL;
  size_t len = 0;
  int regcomp_v = 0;
  int regcomp_f = 0;
  regex_t reg_v = {0};
  regex_t reg_f = {0};

  char v_pattern[] =
      "^[v]([ ]+[+-]?[0-9]+[.]?[0-9]*){3}((([ ]+[0-9]+[.]?[0-9]*)?)|([ "
      "]+[0-9]+[.]?[0-9]*([ "
      "]+([0]|[1]|(0.[0-9]*))){3}))[\n]?$";

  char f_pattern[] =
      "^[f]([ "
      "][1-9][0-9]*([/]([+-]?[0-9]+[.]?[0-9]*)?([/"
      "][+-]?[0-9]+[.]?[0-9]*)?)?){3,}[ ]?[\n]?$";

  fp = fopen(file, "r");

  regcomp_v = regcomp(&reg_v, v_pattern, regflag);
  regcomp_f = regcomp(&reg_f, f_pattern, regflag);

  if (fp == NULL) {
    status = ERROR_FILE;
  } else if (regcomp_f != 0 || regcomp_v != 0) {
    status = ERROR;
  } else if (data == NULL) {
    status = ERROR_DATA;
  } else {
    while ((status == OK) && (getline(&line, &len, fp) != -1)) {
      // working with verteces
      if (line[0] == 'v' && line[1] == ' ') {
        if (regexec(&reg_v, line, 0, NULL, 0) == 0) {
          status =
              set_vertices_data(&data->coordinate_matrix, line, data->min_max);
        }
      }
      // working with facets
      else if (line[0] == 'f') {
        if (regexec(&reg_f, line, 0, NULL, 0) == 0) {
          status = set_facets_data(line, &(data->facets), data->facets_count);
          data->facets_count++;
        }
      }
    }
    if (line) free(line);
    fclose(fp);
    data->vertices_count = data->coordinate_matrix.rows;
  }
  regfree(&reg_v);
  regfree(&reg_f);
  return status;
}