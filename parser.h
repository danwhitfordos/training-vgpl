#ifndef vgpl_parser_h
#define vgpl_parser_h
#include "lexer.h"

enum AST_NODE_TYPE {
  EOF_NODE,
  ERR_NODE,
  LIT_INT,
  EXPR_BIN,
  STMT_PRINT
};

enum BinOp {
  OP_PLUS,
  OP_MINUS,
  OP_MULT,
  OP_DIV
};

struct BinaryExpr {
  enum BinOp op;
  struct AST_Node *left;
  struct AST_Node *right;
};

struct AST_Node {
  enum AST_NODE_TYPE type;
  union {
    int number;
    struct BinaryExpr binex;
    struct AST_Node *expr;
  };
};

struct Parser {
  struct Lexer *l;
  struct Token previous;
  struct Token current;
};

void parser_init(struct Parser *, struct Lexer *);
struct AST_Node *parser_next_node(struct Parser *);
void free_ast_node(struct AST_Node *);

#endif
