#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "parser.h"

struct AST_Node *parser_get_expression(struct Parser *);

void parser_init(struct Parser *p, struct Lexer *l) {
  p->l = l;
}

void parser_advance(struct Parser *p) {
  p->previous = p->current;
  lexer_next_token(p->l, &p->current);
}

struct AST_Node *parser_get_atom(struct Parser *p) {
  if (p->current.type == TT_LB) {
    parser_advance(p);
    struct AST_Node *expr = parser_get_expression(p);
    assert(p->current.type == TT_RB);
    parser_advance(p);
    return expr;
  }

  parser_advance(p);
  assert(p->previous.type == TT_INT);
  struct AST_Node *node = malloc(sizeof(struct AST_Node));
  node->type = LIT_INT;
  node->number = atoi(p->previous.lexeme);
  return node;
}

struct AST_Node *parser_get_factor(struct Parser *p) {
  struct AST_Node *l = parser_get_atom(p);

  while (p->current.type == TT_MULT || p->current.type == TT_DIV) {
    parser_advance(p);
    enum BinOp op = p->previous.type == TT_MULT ? OP_MULT : OP_DIV;
    struct AST_Node *r = parser_get_atom(p);
    struct AST_Node *n = malloc(sizeof(struct AST_Node));
    n->type = EXPR_BIN;
    n->binex.op = op;
    n->binex.left = l;
    n->binex.right = r;
    l = n;
  }
  return l;
}

struct AST_Node *parser_get_term(struct Parser *p) {
  struct AST_Node *l = parser_get_factor(p);

  while (p->current.type == TT_PLUS || p->current.type == TT_MINUS) {
    parser_advance(p);
    enum BinOp op = p->previous.type == TT_PLUS ? OP_PLUS : OP_MINUS;
    struct AST_Node *r = parser_get_factor(p);
    struct AST_Node *n = malloc(sizeof(struct AST_Node));
    n->type = EXPR_BIN;
    n->binex.op = op;
    n->binex.left = l;
    n->binex.right = r;
    l = n;
  }
  return l;
}

struct AST_Node *parser_get_expression(struct Parser *p) {
  return parser_get_term(p);
}

struct AST_Node *parser_next_node(struct Parser *p) {
  parser_advance(p);

  switch (p->current.type) {
  case TT_EOF: {
    struct AST_Node *node = malloc(sizeof(struct AST_Node));
    node->type = EOF_NODE;
    return node;
  }
  case TT_ERR: {
    struct AST_Node *node = malloc(sizeof(struct AST_Node));
    node->type = ERR_NODE;
    return node;
  }
  case TT_WORD: {
    struct AST_Node *node = malloc(sizeof(struct AST_Node));
    if (strcmp("print", p->current.lexeme) == 0) {
      parser_advance(p);
      node->type = STMT_PRINT;
      node->expr = parser_get_expression(p);
      assert(p->current.type == TT_SC);
      return node;
    } else {
      fprintf(stderr, "unknown keyword: %s\n", p->current.lexeme);
      struct AST_Node *node = malloc(sizeof(struct AST_Node));
      node->type = ERR_NODE;
      return node;
    }
  }
  default: {
      struct AST_Node *node = parser_get_expression(p);
      assert(p->current.type == TT_SC);
      return node;
  }
  }
}

void free_ast_node(struct AST_Node *node) {
  switch (node->type) {
  case EXPR_BIN: {
    free_ast_node(node->binex.left);
    free_ast_node(node->binex.right);
    break;
  }
  case STMT_PRINT: {
    free_ast_node(node->expr);
    break;
  }
  default: break;;
  }

  free(node);
}
