#include "vgpl_test.h"
#include "lexer.h"
#include "string.h"

VGPL_TEST(empty) {
  struct Lexer l;
  struct Token t;
  lexer_init(&l, "");
  lexer_next_token(&l, &t);

  assert(t.type == TT_EOF);
}

VGPL_TEST(operands) {
  struct Lexer l;
  lexer_init(&l, "+ - * /");

  struct Token got;
  lexer_next_token(&l, &got);
  assert(got.type == TT_PLUS);
  lexer_next_token(&l, &got);
  assert(got.type == TT_MINUS);
  lexer_next_token(&l, &got);
  assert(got.type == TT_MULT);
  lexer_next_token(&l, &got);
  assert(got.type == TT_DIV);
}

VGPL_TEST(expression) {
  struct Lexer l;
  lexer_init(&l, "(1 + 2) * 3;");

  struct Token got;
  lexer_next_token(&l, &got);
  assert(got.type == TT_LB);

  lexer_next_token(&l, &got);
  assert(got.type == TT_INT);
  assert(strcmp(got.lexeme, "1") == 0);

  lexer_next_token(&l, &got);
  assert(got.type == TT_PLUS);

  lexer_next_token(&l, &got);
  assert(got.type == TT_INT);
  assert(strcmp(got.lexeme, "2") == 0);

  lexer_next_token(&l, &got);
  assert(got.type == TT_RB);

  lexer_next_token(&l, &got);
  assert(got.type == TT_MULT);

  lexer_next_token(&l, &got);
  assert(got.type == TT_INT);
  assert(strcmp(got.lexeme, "3") == 0);

  lexer_next_token(&l, &got);
  assert(got.type == TT_SC);

  lexer_next_token(&l, &got);
  assert(got.type == TT_EOF);
}

VGPL_TEST(manydigits) {
  struct Lexer l;
  lexer_init(&l, "1234;");

  struct Token got;
  lexer_next_token(&l, &got);
  assert(got.type == TT_INT);
  assert(strcmp(got.lexeme, "1234") == 0);

  lexer_next_token(&l, &got);
  assert(got.type == TT_SC);

  lexer_next_token(&l, &got);
  assert(got.type == TT_EOF);
}

int main() {
  VGPL_RUN(empty);
  VGPL_RUN(operands);
  VGPL_RUN(expression);
  VGPL_RUN(manydigits);
}
