#include "lexer.h"

lexer_t init_lexer(char *filename) {
  lexer_t lex;  

  lex.filename = filename;
  lex.offset = 0;
  lex.fp = fopen(filename, "r");
  lex.state = 0;
  lex.end_token = false;
  lex.done = false;

  return lex;
}

token_t next_token(lexer_t *self) {
  token_t token;

  char c = getc(self->fp);
  token.value = strcat(token.value, c);
  offset += 1;
  switch (c) {
    case EOF: token.type = EOFTOKEN; return token;
    case '[': token.type = LBRACKET; return token;
    case ']': token.type = RBRACKET; return token;
    case '(': token.type = LPAREN; return token;
    case ')': token.type = RPAREN; return token;
    case '{': token.type = LBRACE; return token;
    case '}': token.type = RBRACE; return token;
    case '.':
              char c1 = getc(self->fp);
              char c2 = getc(self->fp);
              if (c2 == '.' && c1 == '.') {
                offset += 2;
                token.type = ELLIPSIS;
                token.value = strcat(token.value, c1);
                token.value = strcat(token.value, c2);
                fseek(self->fp, -2, SEEK_CUR);
                return token;
              } else {
                token.type = DOT;
                return token;
              }
    case '-':
              char c1 = getc(self->fp);
              offset += 1;
              token.value = strcat(token.value, c1);
              switch(c1) {
                case '-': token.type = DECREMENT; return token;
                case '>': token.type = ARROW; return token;
                case '=': token.type = SUBASSIGN; return token;
                default: token.value = token.value[0];
                         fseek(self->fp, -1, SEEK_CUR);
                         offset -= 1;
                         token.type = MINUS;
                         return token;
              }
    case '+':
              char c1 = getc(self->fp);
              offset += 1;
              token.value = strcat(token.value, c1);
              switch(c1) {
                case '+': token.type = INCREMENT; return token;
                case '=': token.type = ADDASSIGN; return token;
                default: token.value = token.value[0];
                         fseek(self->fp, -1, SEEK_CUR);
                         offset -= 1;
                         token.type = PLUS;
                         return token;
              }
    case '&':
              char c1 = getc(self->fp);
              offset += 1;
              token.value = strcat(token.value, c1);
              switch(c1) {
                case '&': token.type = LOGAND; return token;
                case '=': token.type = ANDASSIGN; return token;
                default: token.value = token.value[0];
                         fseek(self->fp, -1, SEEK_CUR);
                         offset -= 1;
                         token.type = BITAND;
                         return token;
              }
    case '*':
              char c1 = getc(self->fp);
              offset += 1;
              token.value = strcat(token.value, c1);
              if(c1 == '=') {
                token.type = MULASSIGN;
                return token;
              } else {
                token.value = token.value[0];
                fseek(self->fp, -1, SEEK_CUR);
                offset -= 1;
                token.type = STAR;
                return token;
              }
    case '~': token.type = UNARY; return token;
    case '!':
              char c1 = getc(self->fp);
              offset += 1;
              token.value = strcat(token.value, c1);
              if(c1 == '=') {
                token.type = NEQ;
                return token;
              } else {
                token.value = token.value[0];
                fseek(self->fp, -1, SEEK_CUR);
                offset -= 1;
                token.type = NOT;
                return token;
              }
    case '/':
              char c1 = getc(self->fp);
              offset += 1;
              token.value = strcat(token.value, c1);
              if(c1 == '=') {
                token.type = DIVASSIGN;
                return token;
              } else {
                token.value = token.value[0];
                fseek(self->fp, -1, SEEK_CUR);
                offset -= 1;
                token.type = DIV;
                return token;
              }
    case '%':
              char c1 = getc(self->fp);
              offset += 1;
              token.value = strcat(token.value, c1);
              switch(c1) {
                case '=': token.type = MODASSIGN; return token;
                case '>': token.type = RBRACEALT; return token;
                case ':': 
                          char c2 = getc(self->fp);
                          char c3 = getc(self->fp);
                          if (c2 == '%' && c3 == ':') {
                            token.type = DOUBLEHASHALT;
                            offset += 2;
                            token.value = strcat(token.value, c2);
                            token.value = strcat(token.value, c3);
                          } else {
                            fseek(self->fp, -2, SEEK_CUR);
                            token.type = HASHALT;
                          }
                          return token;
                default: token.value = token.value[0];
                         fseek(self->fp, -1, SEEK_CUR);
                         offset -= 1;
                         token.type = MODULUS;
                         return token;
              }
    case '<':
              char c1 = getc(self->fp);
              offset += 1;
              token.value = strcat(token.value, c1);
              switch(c1) {
                case '=': token.type = LESSTHANEQUAL; return token;
                case ':': token.type = LBRACKETALT; return token;
                case '%': token.type = LBRACEALT; return token;
                case '<':
                default: token.value = token.value[0];
                         fseek(self->fp, -1, SEEK_CUR);
                         offset -= 1;
                         token.type = LESSTHAN;
                         return token;
              }
    case '>':

    case '=':
    case '^':
    case '|':
    case '?':
    case ':':
    case ';':
    case ',':
    case '#':
  }
}
