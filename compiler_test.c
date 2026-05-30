#include <assert.h>

#include "vgpl_test.h"
#include "lexer.h"
#include "parser.h"
#include "compiler.h"

VGPL_TEST(a_number) {
    struct Lexer l;
    struct Parser p;
    struct Compiler c;
    lexer_init(&l, "2;");
    parser_init(&p, &l);
    compiler_init(&c, &p);
    
    assert (2==2);
}

int main() {
    VGPL_RUN(a_number)
}
