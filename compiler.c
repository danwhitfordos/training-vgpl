#include "compiler.h"

void compiler_init(struct Compiler *c, struct Parser *p) {
    c->p = p;
}