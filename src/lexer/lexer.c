#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "tokens.h"
#include "data_extractor.h"
#include "dynamic_array.h"

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

Array scan(Lexer *lexer) {
	Array tokens;
	array_init(&tokens, sizeof(Token));

	while (!isAtEnd(lexer)) {
		char ch = peek(lexer, 0);

		if (isNumber(ch)) {
			int value = extractNumber(lexer);
			// printf("isNumber: %d\n", value);

			Loc location = {lexer->file, lexer->line, lexer->col};

			Token token;
			token.type = NUMBER;
			token.value = malloc(sizeof(int));
			*(int *)token.value = value;
			token.loc = location;

			array_push(&tokens, &token);

		} else if (isAlphaNumeric(ch)) {
			char *value = extractIdentifier(lexer);

			Loc location = {lexer->file, lexer->line, lexer->col - strlen(value)};

			Token token;
			token.type = SHOW;
			token.value = (char *)value;
			token.loc = location;

			array_push(&tokens, &token);

		} else if (ch == '+') {
			Loc location = {lexer->file, lexer->line, lexer->col};

			Token token;
			token.type = PLUS;
			token.value = malloc(sizeof(char));
			*(char *)token.value = '+';
			token.loc = location;

			array_push(&tokens, &token);
			consume(lexer);

		} else {
			consume(lexer);
		}
	}

	cleanup(lexer);
	return tokens;
}

void cleanup(Lexer *lexer) {
	if (strcmp(lexer->file, "REPL") == 0) return;

	if (lexer->content) {
        free(lexer->content);
        lexer->content = NULL; // Set to NULL after freeing
    }
}
