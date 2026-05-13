#ifndef vgpl_lexer_h
#include <stddef.h>

struct Lexer {
  char *src;
  size_t src_length;
  size_t pos;
};

enum TokenType {
  TT_ERR,
  TT_EOF,
  TT_PLUS,
  TT_MINUS,
  TT_MULT,
  TT_DIV,
  TT_LB,
  TT_RB,
  TT_SC,
  TT_INT
};

#define LEXEME_BUF_SIZE 50

struct Token {
  enum TokenType type;
  char lexeme[LEXEME_BUF_SIZE];
};

void lexer_init(struct Lexer*, char *);
void lexer_next_token(struct Lexer*, struct Token*);

#endif
