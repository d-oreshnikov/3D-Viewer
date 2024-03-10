#include <check.h>

#include "../backend/my_viewer.h"

Suite *affine_suite();
Suite *test_file();
Suite *test_model();

void runTest(Suite *suit, int *fc);