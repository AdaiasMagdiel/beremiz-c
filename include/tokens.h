#ifndef TOKENS_H
#define TOKENS_H

#include "dynamic_array.h"

enum TokenType {
	NUMBER,
	PLUS,
	SHOW
};

typedef struct {
	char *file;
	int line;
	int col;
} Loc;

typedef struct {
	enum TokenType type;
	void *value;
	Loc loc;
} Token;

void tokens_array_cleanup(Array *tokens);
void print_tokens(Array tokens);
const char *token_type_to_str(enum TokenType type);

#endif  // TOKENS_H
