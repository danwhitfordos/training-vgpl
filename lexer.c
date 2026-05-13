#include <stdio.h>
#include <assert.h>

#include "lexer.h"

void lexer_init(struct Lexer *l, char *src) {
  l->src = src;
  l->pos = 0;
}

void lexer_int_token(struct Lexer *l, struct Token *t) {
  t->type = TT_INT;
  
  char *p = t->lexeme;
  
  char c = l->src[l->pos];
  while(c >= '0' && c <= '9') {
    assert(p - t->lexeme < 50);
    *p++ = c;
    c = l->src[++l->pos];
  }
  *p = '\0';
}

void lexer_next_token(struct Lexer *l, struct Token *t) {
  while (l->src[l->pos] < '!' && l->src[l->pos] != '\0') {
    l->pos++;
  }

  if (l->src[l->pos] >= '0' && l->src[l->pos] <= '9') {
    lexer_int_token(l, t);
    return;
  }
  
  switch (l->src[l->pos++]) {
  case '\0': t->type = TT_EOF; break;
  case '+': t->type = TT_PLUS; break;
  case '-': t->type = TT_MINUS; break;
  case '*': t->type = TT_MULT; break;
  case '/': t->type = TT_DIV; break;
  case '(': t->type = TT_LB; break;
  case ';': t->type = TT_SC; break;
  case ')': t->type = TT_RB; break;
  default: {
    fprintf(stderr, "Can not handle '%c' (%d)\n",
	    l->src[l->pos-1], l->src[l->pos-1]);
    t->type = TT_ERR;
    break;
  }
  }

}

