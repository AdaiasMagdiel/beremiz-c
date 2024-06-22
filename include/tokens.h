#ifndef TOKENS_H
#define TOKENS_H

#include "dynamic_array.h"

#define TOKEN_TYPES     \
    X(NUMBER, "NUMBER") \
    X(STRING, "STRING") \
    X(PLUS,   "PLUS")   \
    X(SHOW,   "SHOW")   \

typedef enum {
    #define X(enum_val, string_val) enum_val,
    TOKEN_TYPES
    #undef X
    TOKEN_TYPE_COUNT
} TokenType;

typedef struct {
	char *file;
	int line;
	int col;
} Loc;

typedef struct {
	TokenType type;
	void *value;
	Loc loc;
} Token;

void tokens_array_cleanup(Array *tokens);
void print_tokens(Array tokens);
const char *token_type_to_str(TokenType type);

#endif  // TOKENS_H
