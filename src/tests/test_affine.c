#include "test_viewer.h"
#define M_PI 3.14159265358979323846

START_TEST(get_min_max_test) {
  data_t model;
  model.facets_count = 0;
  model.vertices_count = 3;
  my_create_matrix(3, 3, &model.coordinate_matrix);
  model.coordinate_matrix.matrix[0][0] = 1;
  model.coordinate_matrix.matrix[0][1] = 2;
  model.coordinate_matrix.matrix[0][2] = 3;
  model.coordinate_matrix.matrix[1][0] = 3;
  model.coordinate_matrix.matrix[1][1] = 1;
  model.coordinate_matrix.matrix[1][2] = 2;
  model.coordinate_matrix.matrix[2][0] = 2;
  model.coordinate_matrix.matrix[2][1] = 3;
  model.coordinate_matrix.matrix[2][2] = 1;
  get_minmax(&model);
  ck_assert_int_eq(model.min_max[0], 1);
  ck_assert_int_eq(model.min_max[1], 3);
  ck_assert_int_eq(model.min_max[2], 1);
  ck_assert_int_eq(model.min_max[3], 3);
  ck_assert_int_eq(model.min_max[4], 1);
  ck_assert_int_eq(model.min_max[5], 3);
  my_remove_matrix(&model.coordinate_matrix);
}
END_TEST

START_TEST(affine_rotate_test) {
  int status = 0;
  data_t model;
  double angles[3] = {0};
  model.facets_count = 0;
  model.vertices_count = 1;
  my_create_matrix(1, 3, &model.coordinate_matrix);
  model.coordinate_matrix.matrix[0][0] = 1;
  model.coordinate_matrix.matrix[0][1] = 1;
  model.coordinate_matrix.matrix[0][2] = 1;

  angles[0] = M_PI;
  status = affine_rotate(&model, angles, 0);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][0], 1., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][1], -1., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][2], -1., 1e-6);

  angles[0] = 0;
  angles[1] = M_PI;
  status = affine_rotate(&model, angles, 0);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][0], -1., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][1], -1., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][2], 1., 1e-6);

  angles[1] = 0;
  angles[2] = M_PI;
  status = affine_rotate(&model, angles, 0);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][0], 1., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][1], 1., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][2], 1., 1e-6);

  angles[0] = M_PI;
  angles[1] = M_PI;
  status = affine_rotate(&model, angles, 0);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][0], 1., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][1], 1., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][2], 1., 1e-6);

  angles[0] = INFINITY;
  status = affine_rotate(&model, angles, 0);
  ck_assert_int_eq(status, 1);

  my_remove_matrix(&model.coordinate_matrix);
}
END_TEST

START_TEST(affine_translate_test) {
  int status = 0;
  data_t model;
  double r_translation[3] = {1, 1, 1};
  model.facets_count = 0;
  model.vertices_count = 2;
  my_create_matrix(2, 3, &model.coordinate_matrix);
  model.coordinate_matrix.matrix[0][0] = -1;
  model.coordinate_matrix.matrix[0][1] = -1;
  model.coordinate_matrix.matrix[0][2] = -1;
  model.coordinate_matrix.matrix[1][0] = 1;
  model.coordinate_matrix.matrix[1][1] = 1;
  model.coordinate_matrix.matrix[1][2] = 1;

  status = affine_translate(&model, r_translation);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][0], 0., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][1], 0., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][2], 0., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[1][0], 2., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[1][1], 2., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[1][2], 2., 1e-6);

  r_translation[0] = INFINITY;
  status = affine_translate(&model, r_translation);
  ck_assert_int_eq(status, 1);

  my_remove_matrix(&model.coordinate_matrix);
}
END_TEST

START_TEST(affine_scale_test) {
  int status = 0;
  data_t model;
  model.facets_count = 0;
  model.vertices_count = 1;
  my_create_matrix(1, 3, &model.coordinate_matrix);
  model.coordinate_matrix.matrix[0][0] = -1;
  model.coordinate_matrix.matrix[0][1] = 2;
  model.coordinate_matrix.matrix[0][2] = 3;

  status = affine_scale(&model, 100);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][0], -100., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][1], 200., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][2], 300., 1e-6);

  status = affine_scale(&model, 0);
  ck_assert_int_eq(status, 1);

  my_remove_matrix(&model.coordinate_matrix);
}
END_TEST

START_TEST(affine_normalize_test) {
  int status = 0;
  data_t model;
  model.facets_count = 0;
  model.vertices_count = 1;
  my_create_matrix(1, 3, &model.coordinate_matrix);
  model.coordinate_matrix.matrix[0][0] = -300;
  model.coordinate_matrix.matrix[0][1] = 400;
  model.coordinate_matrix.matrix[0][2] = 0;
  get_minmax(&model);

  status = normalize_model_data(&model);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][0], -0.6, 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][1], 0.8, 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][2], 0., 1e-6);

  my_remove_matrix(&model.coordinate_matrix);

  model.vertices_count = 3;
  my_create_matrix(3, 3, &model.coordinate_matrix);
  model.coordinate_matrix.matrix[0][0] = -1;
  model.coordinate_matrix.matrix[0][1] = -1;
  model.coordinate_matrix.matrix[0][2] = 2;
  model.coordinate_matrix.matrix[1][0] = 0;
  model.coordinate_matrix.matrix[1][1] = 1;
  model.coordinate_matrix.matrix[1][2] = 2;
  model.coordinate_matrix.matrix[2][0] = 1;
  model.coordinate_matrix.matrix[2][1] = 1;
  model.coordinate_matrix.matrix[2][2] = 2;
  get_minmax(&model);

  status = normalize_model_data(&model);
  ck_assert_int_eq(status, 0);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][0], -1 / sqrt(2),
                          1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][1], -1 / sqrt(2),
                          1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[0][2], 0., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[1][0], 0, 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[1][1], 1 / sqrt(2),
                          1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[1][2], 0., 1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[2][0], 1 / sqrt(2),
                          1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[2][1], 1 / sqrt(2),
                          1e-6);
  ck_assert_double_eq_tol(model.coordinate_matrix.matrix[2][2], 0., 1e-6);

  my_remove_matrix(&model.coordinate_matrix);
}
END_TEST

// START_TEST (time_test){
//   data_t model = {0};;
//   my_parser("tests/test_data/cube.txt", &model);
//   clock_t start, end;
//   start = clock();
//   affine_rotate_x(&model, M_PI);
//   end = clock();
//   printf("TIME: %.8f\n", ((double)(end - start)) / CLOCKS_PER_SEC);
// }
// END_TEST

Suite *affine_suite(void) {
  Suite *s = suite_create("affine_suite");
  TCase *tc_affine = tcase_create("affine");

  // tcase_add_test(tc_affine, time_test);
  tcase_add_test(tc_affine, get_min_max_test);
  tcase_add_test(tc_affine, affine_rotate_test);
  tcase_add_test(tc_affine, affine_translate_test);
  tcase_add_test(tc_affine, affine_scale_test);
  tcase_add_test(tc_affine, affine_normalize_test);

  suite_add_tcase(s, tc_affine);
  return s;
}