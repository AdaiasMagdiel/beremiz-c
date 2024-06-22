#ifndef LEXER_H
#define LEXER_H

#include "dynamic_array.h"

typedef struct {
	char *content;
	char *file;
	int current;
	int line;
	int col;
} Lexer;

int isAtEnd(Lexer *lexer);
char peek(Lexer *lexer, int idx);
char consume(Lexer *lexer);
Array scan(Lexer *lexer);
void cleanup(Lexer *lexer);

#endif  // LEXER_H
