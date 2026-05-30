#ifndef vgpl_compiler_h
#define vgpl_compiler_h

#include <stdint.h>
#include "parser.h"

struct Compiler {
  struct Parser *p;
};

struct Chunk {
    uint8_t *code;
};

void compiler_init(struct Compiler *c, struct Parser *p);
void compile_node(struct Compiler *c, struct Chunk *chunk);

#endif
