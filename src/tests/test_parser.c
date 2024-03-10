#include "test_viewer.h"

START_TEST(error_check_1) {
  data_t data = {0};
  my_init_data(&data);
  int status = my_parser("tests/test_data/non_exist.obj", &data);
  ck_assert_int_eq(status, ERROR_FILE);
}
END_TEST

START_TEST(error_check_2) {
  int status = my_parser("tests/test_data/cube.obj", NULL);
  ck_assert_int_eq(status, ERROR_DATA);
}
END_TEST

START_TEST(models_1) {
  data_t data = {0};
  int status = my_parser("tests/test_data/cube.obj", &data);
  ck_assert_int_eq(status, OK);
  matrix_t model;
  my_create_matrix(8, 3, &model);
  model.matrix[0][0] = 1.0;
  model.matrix[0][1] = -1.0;
  model.matrix[0][2] = -1.0;
  model.matrix[1][0] = 1.0;
  model.matrix[1][1] = -1.0;
  model.matrix[1][2] = 1.0;
  model.matrix[2][0] = -1.0;
  model.matrix[2][1] = -1.0;
  model.matrix[2][2] = 1.0;
  model.matrix[3][0] = -1.0;
  model.matrix[3][1] = -1.0;
  model.matrix[3][2] = -1.0;
  model.matrix[4][0] = 1.0;
  model.matrix[4][1] = 1.0;
  model.matrix[4][2] = -1.0;
  model.matrix[5][0] = 1.0;
  model.matrix[5][1] = 1.0;
  model.matrix[5][2] = 1.0;
  model.matrix[6][0] = -1.0;
  model.matrix[6][1] = 1.0;
  model.matrix[6][2] = 1.0;
  model.matrix[7][0] = -1.0;
  model.matrix[7][1] = 1.0;
  model.matrix[7][2] = -1.0;

  for (int r = 0; r < model.rows; r++) {
    for (int c = 0; c < model.columns; c++) {
      ck_assert_double_eq_tol(model.matrix[r][c],
                              data.coordinate_matrix.matrix[r][c], 1.0e-6);
    }
  }
  facets_t *temp = (facets_t *)calloc(12, sizeof(facets_t));
  for (int i = 0; i < 12; i++) {
    temp[i].vertices = (int *)calloc(3, sizeof(int));
    temp[i].count_of_vertices_in_facet = 3;
  }

  temp[0].vertices[0] = 2;
  temp[0].vertices[1] = 3;
  temp[0].vertices[2] = 4;
  temp[0].count_of_vertices_in_facet = 3;

  temp[1].vertices[0] = 8;
  temp[1].vertices[1] = 7;
  temp[1].vertices[2] = 6;
  temp[1].count_of_vertices_in_facet = 3;

  temp[2].vertices[0] = 5;
  temp[2].vertices[1] = 6;
  temp[2].vertices[2] = 2;
  temp[2].count_of_vertices_in_facet = 3;

  temp[3].vertices[0] = 6;
  temp[3].vertices[1] = 7;
  temp[3].vertices[2] = 3;
  temp[3].count_of_vertices_in_facet = 3;

  temp[4].vertices[0] = 3;
  temp[4].vertices[1] = 7;
  temp[4].vertices[2] = 8;
  temp[4].count_of_vertices_in_facet = 3;

  temp[5].vertices[0] = 1;
  temp[5].vertices[1] = 4;
  temp[5].vertices[2] = 8;
  temp[5].count_of_vertices_in_facet = 3;

  temp[6].vertices[0] = 1;
  temp[6].vertices[1] = 2;
  temp[6].vertices[2] = 4;
  temp[6].count_of_vertices_in_facet = 3;

  temp[7].vertices[0] = 5;
  temp[7].vertices[1] = 8;
  temp[7].vertices[2] = 6;
  temp[7].count_of_vertices_in_facet = 3;

  temp[8].vertices[0] = 1;
  temp[8].vertices[1] = 5;
  temp[8].vertices[2] = 2;
  temp[8].count_of_vertices_in_facet = 3;

  temp[9].vertices[0] = 2;
  temp[9].vertices[1] = 6;
  temp[9].vertices[2] = 3;
  temp[9].count_of_vertices_in_facet = 3;

  temp[10].vertices[0] = 4;
  temp[10].vertices[1] = 3;
  temp[10].vertices[2] = 8;
  temp[10].count_of_vertices_in_facet = 3;

  temp[11].vertices[0] = 5;
  temp[11].vertices[1] = 1;
  temp[11].vertices[2] = 8;
  temp[11].count_of_vertices_in_facet = 3;

  for (int n = 0; n < data.facets_count; n++) {
    ck_assert_int_eq(temp[n].count_of_vertices_in_facet,
                     data.facets[n].count_of_vertices_in_facet);
    for (int i = 0; i < data.facets[n].count_of_vertices_in_facet; i++) {
      ck_assert_int_eq(temp[n].vertices[i], data.facets[n].vertices[i]);
    }
  }

  ck_assert_double_eq(data.min_max[0], -1.0);
  ck_assert_double_eq(data.min_max[1], 1.0);
  ck_assert_double_eq(data.min_max[2], -1.0);
  ck_assert_double_eq(data.min_max[3], 1.0);
  ck_assert_double_eq(data.min_max[4], -1.0);
  ck_assert_double_eq(data.min_max[5], 1.0);

  ck_assert_int_eq(data.vertices_count, 8);
  ck_assert_int_eq(data.facets_count, 12);

  my_remove_matrix(&model);
  my_remove_facet(&temp, 12);
  my_remove_data(&data);
}
END_TEST

Suite *test_file(void) {
  Suite *s = suite_create("ERRORS_CHECK");
  TCase *test_case = tcase_create("ERRORS_CHECK");

  tcase_add_test(test_case, error_check_1);
  tcase_add_test(test_case, error_check_2);

  suite_add_tcase(s, test_case);
  return s;
}

Suite *test_model(void) {
  Suite *s = suite_create("MODEL");
  TCase *test_case = tcase_create("MODEL");

  tcase_add_test(test_case, models_1);

  suite_add_tcase(s, test_case);
  return s;
}
