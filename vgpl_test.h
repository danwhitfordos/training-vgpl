#ifndef vgpl_test_h
#define vgpl_test_h
#include <stdio.h>
#include <assert.h>

void print_test_start(char *name) {
  printf("  test '%s' running\n", name);
}

#define VGPL_TEST(name) static void test_##name()
#define VGPL_RUN(name) print_test_start(#name); test_##name();
#endif
