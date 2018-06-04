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

token_t next_idkey(lexer_t *self, c) {
  token_t token;
  char c;
  do {
    c = getc(self->fp);
    strcat(token.value, c);
  } while (isalpha(c) || isdigit(c) || c == '_');
  fseek(self->fp, -1, SEEK_CUR);
  token.value[strlen(token.value)-1] = 0;

  if (strcmp(token.value, "auto") == 0) {
  }
  else if (strcmp(token.value, "break") == 0) {
  }
  else if (strcmp(token.value, "case") == 0) {
  }
  else if (strcmp(token.value, "char") == 0) {
  }
  else if (strcmp(token.value, "const") == 0) {
  }

}

token_t next_float(lexer_t *self, c) {
}

token_t next_num(lexer_t *self, c) {
}

token_t next_string(lexer_t *self, c) {
}

token_t next_token(lexer_t *self) {
  token_t token;

  char c = getc(self->fp);
  token.value = strcat(token.value, c);
  offset += 1;
  switch (c) {
    case EOF: token.type = EOFTOKEN; return token;
    case ' ':
    case '\t':
    case '\n':
    case '\r':
              return next_token(self);
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
              } else if (isdigit(c)) {
                fseek(self->fp, -2, SEEK_CUR);
                next_float(c);
              } else {
                token.type = DOT;
                fseek(self->fp, -2, SEEK_CUR);
              }
              return token;
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
              if(c1 == '=') {
                token.value = strcat(token.value, c1);
                token.type = MULASSIGN;
              } else {
                fseek(self->fp, -1, SEEK_CUR);
                offset -= 1;
                token.type = STAR;
              }
              return token;
    case '~': token.type = UNARY; return token;
    case '!':
              char c1 = getc(self->fp);
              offset += 1;
              if(c1 == '=') {
                token.type = NEQ;
                token.value = strcat(token.value, c1);
              } else {
                fseek(self->fp, -1, SEEK_CUR);
                offset -= 1;
                token.type = NOT;
              }
              return token;
    case '/':
              char c1 = getc(self->fp);
              offset += 1;
              if(c1 == '=') {
                token.type = DIVASSIGN;
                token.value = strcat(token.value, c1);
              } else {
                fseek(self->fp, -1, SEEK_CUR);
                offset -= 1;
                token.type = DIV;
              }
              return token;
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
                          char c2 = getc(self->fp);
                          offset += 1;
                          if (c2 == '=') {
                            token.type = SHLASSIGN;
                            offset += 1;
                            token.value = strcat(token.value, c2);
                          } else {
                            fseek(self->fp, -1, SEEK_CUR);
                            token.type = SHL;
                          }

                          return token;

                default: token.value = token.value[0];
                         fseek(self->fp, -1, SEEK_CUR);
                         offset -= 1;
                         token.type = LESSTHAN;
                         return token;
              }
    case '>':
              char c1 = getc(self->fp);
              offset += 1;
              token.value = strcat(token.value, c1);
              switch(c1) {
                case '=': token.type = GREATERTHANEQUAL; return token;
                case '>':
                          char c2 = getc(self->fp);
                          offset += 1;
                          if (c2 == '=') {
                            token.type = SHRASSIGN;
                            offset += 1;
                            token.value = strcat(token.value, c2);
                          } else {
                            fseek(self->fp, -1, SEEK_CUR);
                            token.type = SHR;
                          }
                          return token;

                default: token.value = token.value[0];
                         fseek(self->fp, -1, SEEK_CUR);
                         offset -= 1;
                         token.type = GREATERTHAN;
                         return token;
              }
    case '=':
              char c1 = getc(self->fp);
              offset += 1;
              if(c1 == '=') {
                token.type = EQ;
                token.value = strcat(token.value, c1);
              } else {
                fseek(self->fp, -1, SEEK_CUR);
                offset -= 1;
                token.type = ASSIGN;
              }
              return token;
    case '^':
              char c1 = getc(self->fp);
              offset += 1;
              if(c1 == '=') {
                token.type = XORASSIGN;
                token.value = strcat(token.value, c1);
              } else {
                fseek(self->fp, -1, SEEK_CUR);
                offset -= 1;
                token.type = BITXOR;
              }
              return token;
    case '|':
              char c1 = getc(self->fp);
              offset += 1;
              token.value = strcat(token.value, c1);
              switch(c1) {
                case '|': token.type = LOGOR; return token;
                case '=': token.type = ORASSIGN; return token;
                default: token.value = token.value[0];
                         fseek(self->fp, -1, SEEK_CUR);
                         offset -= 1;
                         token.type = BITOR;
                         return token;
              }
    case '?': token.type = QMARK; return token;
    case ':':
              char c1 = getc(self->fp);
              offset += 1;
              if(c1 == '>') {
                token.type = RBRACKETALT;
                token.value = strcat(token.value, c1);
              } else {
                fseek(self->fp, -1, SEEK_CUR);
                offset -= 1;
                token.type = COLON;
              }
              return token;
    case ';': token.type = SEMICOLON; return token;
    case ',': token.type = COMMA; return token;
    case '#':
              char c1 = getc(self->fp);
              offset += 1;
              if(c1 == '#') {
                token.type = DOUBLEHASH;
                token.value = strcat(token.value, c1);
              } else {
                fseek(self->fp, -1, SEEK_CUR);
                offset -= 1;
                token.type = HASH;
              }
    case '"':
    case '\'':
              return next_string(c);
    default:
              if (isalpha(c)) return next_idkey(c);
              if (isdigit(c)) return next_num(c);
  }
}
