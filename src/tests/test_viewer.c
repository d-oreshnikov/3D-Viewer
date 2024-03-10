#include "test_viewer.h"

int main() {
  int fc = 1;
  runTest(affine_suite(), &fc);
  runTest(test_file(), &fc);
  runTest(test_model(), &fc);

  return fc;
}

void runTest(Suite *suit, int *fc) {
  SRunner *sr = srunner_create(suit);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_ENV);
  int f = srunner_ntests_failed(sr);
  if (*fc != 0) {
    *fc = f;
  }
  srunner_free(sr);
}