#ifndef _LEXER_H
#define _LEXER_H

#include <stdio.h>
#include <sys/types.h>

enum token_type {
  KEYWORD,
  ID,
  INT,
  FLOAT,
  STRING,
  CHAR,
  EOFTOKEN,
  LBRACE,
  RBRACE,
  LBRACKET,
  RBRACKET,
  LPAREN,
  RPAREN,
  COMMA,
  DOT,
  ELLIPSIS,
  MINUS,
  DECREMENT,
  ARROW,
  SUBASSIGN,
  PLUS,
  INCREMENT,
  ADDASSIGN,
  BITAND,
  LOGAND,
  ANDASSIGN,
  STAR,
  MULASSIGN,
  UNARY,
  NOT,
  NEQ,
  DIV,
  DIVASSIGN,
  MODULUS,
  MODASSIGN,
  RBRACEALT,
  HASHALT,
  DOUBLEHASHALT,
  LESSTHAN,
  LESSTHENEQUAL,
  LBRACEALT,
  LBRACKETALT,
};

typedef struct {
  char *filename;
  FILE *fp;
  off_t offset;
  char state;
  bool end_token;
  bool done;
} lexer_t;

typedef struct {
  token_type type;
  char *value;
} token_t;

#endif
