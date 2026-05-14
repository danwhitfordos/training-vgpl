#include <stdio.h>

#include "vgpl_test.h"
#include "parser.h"

VGPL_TEST(empty) {
  struct Lexer l;
  struct Parser p;
  lexer_init(&l, "");
  parser_init(&p, &l);

  struct AST_Node *got = parser_next_node(&p);
  assert(got->type == EOF_NODE);

  free_ast_node(got);
}

VGPL_TEST(onenumber) {
  struct Lexer l;
  struct Parser p;
  lexer_init(&l, "1;");
  parser_init(&p, &l);

  struct AST_Node *got = parser_next_node(&p);
  assert(got->type == LIT_INT);
  assert(got->number == 1);

  free_ast_node(got);
}

VGPL_TEST(simpleadd) {
  struct Lexer l;
  struct Parser p;
  lexer_init(&l, "5 + 7;");
  parser_init(&p, &l);

  struct AST_Node *got = parser_next_node(&p);
  assert(got->type == EXPR_BIN);
  assert(got->binex.op == OP_PLUS);
  assert(got->binex.left->type == LIT_INT);
  assert(got->binex.left->number == 5);
  assert(got->binex.right->type == LIT_INT);
  assert(got->binex.right->number == 7);

  free_ast_node(got);
}

VGPL_TEST(multiadd) {
  char * input = "1+2+3;";
  struct Lexer l;
  struct Parser p;
  lexer_init(&l, input);
  parser_init(&p, &l);

  struct AST_Node *got = parser_next_node(&p);
  assert(got->type == EXPR_BIN);
  assert(got->binex.op == OP_PLUS);

  struct AST_Node *left = got->binex.left;
  assert(left->type == EXPR_BIN);
  assert(left->binex.op == OP_PLUS);  
  assert(left->binex.left->type == LIT_INT);
  assert(left->binex.left->number == 1);
  assert(left->binex.right->type == LIT_INT);
  assert(left->binex.right->number == 2);
  
  struct AST_Node *right = got->binex.right;
  assert(right->type == LIT_INT);
  assert(right->number == 3);

  free_ast_node(got);
}

VGPL_TEST(two_expressions) {
  char * input = "1+2; 2 + 3 ; ";
  struct Lexer l;
  struct Parser p;
  lexer_init(&l, input);
  parser_init(&p, &l);

  struct AST_Node *got;
  got = parser_next_node(&p);
  assert(got->type == EXPR_BIN);

  free_ast_node(got);
  
  got = parser_next_node(&p);
  assert(got->type == EXPR_BIN);

  free_ast_node(got);
}

VGPL_TEST(minus_mult) {
  char * input = "1-2*3;";
  struct Lexer l;
  struct Parser p;
  lexer_init(&l, input);
  parser_init(&p, &l);

  struct AST_Node *got;
  got = parser_next_node(&p);

  assert(got->type == EXPR_BIN);
  assert(got->binex.op == OP_MINUS);

  free_ast_node(got);
}

VGPL_TEST(mult_minus) {
  char * input = "1*2-3;";
  struct Lexer l;
  struct Parser p;
  lexer_init(&l, input);
  parser_init(&p, &l);

  struct AST_Node *got;
  got = parser_next_node(&p);

  assert(got->type == EXPR_BIN);
  assert(got->binex.op == OP_MINUS);

  free_ast_node(got);
}

VGPL_TEST(parens) {
  char * input = "(10/3);";
  struct Lexer l;
  struct Parser p;
  lexer_init(&l, input);
  parser_init(&p, &l);

  struct AST_Node *got;
  got = parser_next_node(&p);

  assert(got->binex.op == OP_DIV);
  assert(got->binex.left->number == 10);
  assert(got->binex.right->number == 3);

  free_ast_node(got);
}

VGPL_TEST(parens_add) {
  char * input = "(10+3);";
  struct Lexer l;
  struct Parser p;
  lexer_init(&l, input);
  parser_init(&p, &l);

  struct AST_Node *got;
  got = parser_next_node(&p);

  assert(got->binex.op == OP_PLUS);
  assert(got->binex.left->number == 10);
  assert(got->binex.right->number == 3);

  free_ast_node(got);
}

VGPL_TEST(parens_multi) {
  char * input = "(10/3) * 2;";
  struct Lexer l;
  struct Parser p;
  lexer_init(&l, input);
  parser_init(&p, &l);

  struct AST_Node *got;
  got = parser_next_node(&p);

  assert(got->binex.op == OP_MULT);
  assert(got->binex.left->binex.op == OP_DIV);
  assert(got->binex.left->binex.left->number == 10);
  assert(got->binex.left->binex.right->number == 3);
    
  assert(got->binex.right->number == 2);

  free_ast_node(got);
}

VGPL_TEST(parens_override_precedence) {
  char * input = "(10+3) * 2;";
  struct Lexer l;
  struct Parser p;
  lexer_init(&l, input);
  parser_init(&p, &l);

  struct AST_Node *got;
  got = parser_next_node(&p);

  assert(got->binex.op == OP_MULT);

  assert(got->binex.left->binex.op == OP_PLUS);
  assert(got->binex.left->binex.left->number == 10);
  assert(got->binex.left->binex.right->number == 3);

  assert(got->binex.right->number == 2);

  free_ast_node(got);
}

VGPL_TEST(print_stmt) {
  char * input = "print 2+3;";
  struct Lexer l;
  struct Parser p;
  lexer_init(&l, input);
  parser_init(&p, &l);

  struct AST_Node *got;
  got = parser_next_node(&p);

  assert(got->type == STMT_PRINT);
  assert(got->expr->type == EXPR_BIN);
  assert(got->expr->binex.op == OP_PLUS);
  assert(got->expr->binex.left->number ==  2);
  assert(got->expr->binex.right->number == 3);

  free_ast_node(got);
}

int main() {
  VGPL_RUN(empty);
  VGPL_RUN(onenumber);
  VGPL_RUN(simpleadd);
  VGPL_RUN(multiadd);
  VGPL_RUN(two_expressions);
  VGPL_RUN(minus_mult);
  VGPL_RUN(mult_minus);
  VGPL_RUN(parens);
  VGPL_RUN(parens_add);
  VGPL_RUN(parens_multi);
  VGPL_RUN(parens_override_precedence);
  VGPL_RUN(print_stmt);
}
