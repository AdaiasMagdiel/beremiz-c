#ifndef LEXER_UTILS_H
#define LEXER_UTILS_H

#include "tokens.h"

Token create_token_char(const Loc location, const TokenType type, const char value);
Token create_token_int(const Loc location, const TokenType type, const int value);
Token create_token_double(const Loc location, const TokenType type, const double value);
Token create_token_string(const Loc location, const TokenType type, const char *value);

#endif  // LEXER_UTILS_H
