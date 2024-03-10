#include "my_viewer.h"

eflag get_minmax(data_t *data) {
  /*
Description:
  Help function to find min and max value of x, y, z coordinates
  of vertexes in 3D model (data->coordinate_matrix.matrix) and puts them
  into array data->min_max = [x_min, x_max, y_min, y_max, z_min, z_max]

Args:
  (data_t*) data    : Pointer for data structure that we are going to find
minmax of x,y,z

Returns:
  Error flag
*/

  eflag status = OK;

  if (data == NULL || data->vertices_count <= 0 ||
      !(my_matrix_correct(&data->coordinate_matrix))) {
    // check this place
    status = ERROR;
  } else {
    // matrix_t matrix[n_rows][n_cols];
    // n_rows = (number of vertexes - 1);
    // n_cols = 3: x, y, z coordinates.

    // x_min x_max
    data->min_max[0] = data->coordinate_matrix.matrix[0][0];
    data->min_max[1] = data->coordinate_matrix.matrix[0][0];
    // y_min y_max
    data->min_max[2] = data->coordinate_matrix.matrix[0][1];
    data->min_max[3] = data->coordinate_matrix.matrix[0][1];
    // z_min z_max
    data->min_max[4] = data->coordinate_matrix.matrix[0][2];
    data->min_max[5] = data->coordinate_matrix.matrix[0][2];

    for (int vertice = 0; vertice < data->vertices_count; vertice++) {
      for (int coord = 0; coord < 3; coord++) {
        // x_min; y_min; z_min;
        if (data->min_max[coord * 2] >
            data->coordinate_matrix.matrix[vertice][coord]) {
          data->min_max[coord * 2] =
              data->coordinate_matrix.matrix[vertice][coord];
        }
        // x_max; y_max; z_max;
        if (data->min_max[coord * 2 + 1] <
            data->coordinate_matrix.matrix[vertice][coord]) {
          data->min_max[coord * 2 + 1] =
              data->coordinate_matrix.matrix[vertice][coord];
        }
      }
    }
  }
  return status;
}

eflag affine_rotate(data_t *data, double angles[3], int upd_min_max) {
  /*
Description:
  Function performs rotation of model by angle (x_angle; y_angle; z_angle)

Args:
  (data_t*) data    : Pointer for data structure that we are going to find
minmax of x,y,z double angles[3]  : Array with angles for rotation (x_angle;
y_angle; z_angle) int upd_min_max   : Variable to update min max meaning
Returns:
  Error flag
*/

  eflag status = OK;

  for (int coord = 0; coord < 3; coord++) {
    if (isnan(angles[coord]) || isinf(angles[coord])) {
      status = ERROR;
    }
  }

  if (fabs(angles[0]) < 1e-6 && fabs(angles[1]) < 1e-6 &&
      fabs(angles[2]) < 1e-6) {
    return OK;
  }

  if (status == OK) {
    matrix_t M_rotation, r_vertex, r_result;

    my_create_matrix(3, 3, &M_rotation);
    my_create_matrix(3, 1, &r_vertex);

    M_rotation.matrix[0][0] = cos(angles[1]) * cos(angles[2]);
    M_rotation.matrix[0][1] = -sin(angles[2]) * cos(angles[1]);
    M_rotation.matrix[0][2] = sin(angles[1]);
    M_rotation.matrix[1][0] = sin(angles[0]) * sin(angles[1]) * cos(angles[2]) +
                              sin(angles[2]) * cos(angles[0]);
    M_rotation.matrix[1][1] =
        -sin(angles[0]) * sin(angles[1]) * sin(angles[2]) +
        cos(angles[0]) * cos(angles[2]);
    M_rotation.matrix[1][2] = -sin(angles[0]) * cos(angles[1]);
    M_rotation.matrix[2][0] = sin(angles[0]) * sin(angles[2]) -
                              sin(angles[1]) * cos(angles[0]) * cos(angles[2]);
    M_rotation.matrix[2][1] = sin(angles[0]) * cos(angles[2]) +
                              sin(angles[1]) * cos(angles[0]) * sin(angles[2]);
    M_rotation.matrix[2][2] = cos(angles[0]) * cos(angles[1]);

    for (int vertice = 0; vertice < data->vertices_count; vertice++) {
      for (int coord = 0; coord < 3; coord++) {
        r_vertex.matrix[coord][0] =
            data->coordinate_matrix.matrix[vertice][coord];
      }
      status = my_mult_matrix(&M_rotation, &r_vertex, &r_result);

      for (int coord = 0; coord < 3; coord++) {
        (data->coordinate_matrix.matrix[vertice][coord]) =
            r_result.matrix[coord][0];
      }
    }
    if (upd_min_max == 1) {
      get_minmax(data);
    }

    my_remove_matrix(&M_rotation);
    my_remove_matrix(&r_vertex);
    my_remove_matrix(&r_result);
  }
  return status;
}

eflag affine_scale(data_t *data, double scale) {
  /*
Description:
  Function performs scaling of model by scaling coefficient

Args:
  (data_t*) data    : Pointer for data structure that we are going to scale
  double scale      : Scaling coefficient

Returns:
  Error flag
*/

  eflag status = OK;

  if (fabs(scale) < 1e-6 || isnan(scale) || isinf(scale)) {  // epsilon???
    return ERROR;
  }

  for (int vertice = 0; vertice < data->vertices_count; vertice++) {
    for (int coord = 0; coord < 3; coord++) {
      data->coordinate_matrix.matrix[vertice][coord] *= scale;
    }
  }

  for (int i = 0; i < 6; i++) {
    data->min_max[i] *= scale;
  }

  return status;
}

eflag affine_translate(data_t *data, double tranc_coef[3]) {
  /*
Description:
 Function performs translating of model by translating coefficient of x, y, z

Args:
  (data_t*) data    : Pointer for data structure of our model that we are going
to translate double tranc_coef : Translating coefficient [x_c, y_c, z_c]

Returns:
  Error flag
*/

  eflag status = OK;

  for (int coord = 0; coord < 3; coord++) {
    if (isnan(tranc_coef[coord]) || isinf(tranc_coef[coord])) {
      status = ERROR;
    }
  }

  // check if this needs
  if (fabs(tranc_coef[0]) < 1e-6 && fabs(tranc_coef[1]) < 1e-6 &&
      fabs(tranc_coef[2]) < 1e-6) {
    return OK;
  }

  if (status == OK) {
    for (int vertice = 0; vertice < data->vertices_count; vertice++) {
      for (int coord = 0; coord < 3; coord++) {
        data->coordinate_matrix.matrix[vertice][coord] += tranc_coef[coord];
      }
    }

    for (int coord = 0; coord < 3; coord++) {
      data->min_max[coord * 2] += tranc_coef[coord];
      data->min_max[coord * 2 + 1] += tranc_coef[coord];
    }
  }

  return status;
}

eflag normalize_model_data(data_t *data) {
  /*
Description:
  Help function to normalize model coordinates so we can easily work

Args:
  (data_t*) data    : Pointer for data structure of our model that we are going
to normalize

Returns:
  Error flag
*/

  eflag status = OK;
  double r_center[3] = {0};
  double max = 0;

  if (data == NULL || data->vertices_count <= 0 ||
      !(my_matrix_correct(&data->coordinate_matrix))) {
    status = ERROR;
  }

  if (status == OK) {
    if (data->vertices_count != 1) {
      r_center[0] = -(data->min_max[1] + data->min_max[0]) / 2;
      r_center[1] = -(data->min_max[3] + data->min_max[2]) / 2;
      r_center[2] = -(data->min_max[5] + data->min_max[4]) / 2;
      status = affine_translate(data, r_center);
    }

    max = 1.0 /
          sqrt(pow(fmax(fabs(data->min_max[0]), fabs(data->min_max[1])), 2) +
               pow(fmax(fabs(data->min_max[2]), fabs(data->min_max[3])), 2) +
               pow(fmax(fabs(data->min_max[4]), fabs(data->min_max[5])), 2));

    if (status == OK && fabs(max) > 1e-6) {
      status = affine_scale(data, max);
    }
  }

  return status;
}
