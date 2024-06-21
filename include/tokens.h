#ifndef TOKENS_H
#define TOKENS_H

#include "dynamic_array.h"

enum TokenType {
	NUMBER
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

void tokens_cleanup(Array *tokens);
void print_tokens(Array tokens);

#endif  // TOKENS_H
