#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "data_extractor.h"

int isAtEnd(Lexer *lexer) {
	const int length = strlen(lexer->content);

	return lexer->current > length;
}

char consume(Lexer *lexer) {
	char ch = lexer->content[lexer->current];

	lexer->current++;
	lexer->col++;

	if (ch == '\n') {
		lexer->line++;
		lexer->col = 1;
	}

	return ch;
}

char peek(Lexer *lexer, int idx) {
	if (isAtEnd(lexer)) return '\0';

	return lexer->content[lexer->current + idx];
}

int isNumber(char ch) {
	return (ch >= '0' && ch <= '9');
}

void scan(Lexer *lexer) {
	while (!isAtEnd(lexer)) {
		char ch = peek(lexer, 0);

		if (isNumber(ch)) {
			int value = extractNumber(lexer);
			printf("isNumber: %d\n", value);
		} else {
			consume(lexer);
		}
	}
}

void cleanup(Lexer *lexer) {
	if (strcmp(lexer->file, "REPL")) return;

	if (lexer->content) {
        free(lexer->content);
        lexer->content = NULL; // Set to NULL after freeing
    }
}
