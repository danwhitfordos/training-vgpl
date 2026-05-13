#ifndef vgpl_test_h
#include <assert.h>

#define VGPL_TEST(name) static void test_##name()
#define VGPL_RUN(name) test_##name();
#endif
