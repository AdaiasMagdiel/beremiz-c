#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "tokens.h"
#include "data_extractor.h"
#include "dynamic_array.h"
#include "error.h"

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

		// NUMBERS
		if (isNumber(ch)) {
			int value = extractNumber(lexer);

			Loc location = {lexer->file, lexer->line, lexer->col};

			Token token;
			token.type = NUMBER;
			token.value = malloc(sizeof(int));
			*(int *)token.value = value;
			token.loc = location;

			array_push(&tokens, &token);

		// IDENTIFIERS
		} else if (isAlphaNumeric(ch)) {
			char *value = extractIdentifier(lexer);

			Loc location = {lexer->file, lexer->line, lexer->col - strlen(value)};
			Token token;
			token.value = (char *)value;
			token.loc = location;

			if (strcmp(value, "show") == 0) {
				token.type = SHOW;

			} else if (strcmp(value, "dup") == 0) {
				token.type = DUP;

			} else if (strcmp(value, "over") == 0) {
				token.type = OVER;

			} else if (strcmp(value, "true") == 0) {
				token.type = BOOL_;

			} else if (strcmp(value, "false") == 0) {
				token.type = BOOL_;

			} else {
				// ERROR (but change later to IDENTIFIER type)
				char buffer[32];
				snprintf(buffer, sizeof(buffer), "Unexpected '%s'", value);

				error_token(buffer, token);

	            if (strcmp(lexer->file, "REPL") != 0) {
	            	tokens_array_cleanup(&tokens);
	            	cleanup(lexer);
					exit(EXIT_FAILURE);
				}
			}

			array_push(&tokens, &token);

		// STRINGS
		} else if (ch == '"') {
			char *value = extractString(lexer);
			
			Loc location = {lexer->file, lexer->line, lexer->col - strlen(value)-2};
			Token token;
			token.type = STRING;
			token.value = (char *)value;
			token.loc = location;

			array_push(&tokens, &token);

		// PLUS
		} else if (ch == '+') {
			Loc location = {lexer->file, lexer->line, lexer->col};

			Token token;
			token.type = PLUS;
			token.value = malloc(sizeof(char));
			*(char *)token.value = '+';
			token.loc = location;

			array_push(&tokens, &token);
			consume(lexer);

		// EQUAL
		} else if (ch == '=') {
			Loc location = {lexer->file, lexer->line, lexer->col};

			Token token;
			token.type = EQUAL;
			token.value = malloc(sizeof(char));
			*(char *)token.value = '=';
			token.loc = location;

			array_push(&tokens, &token);
			consume(lexer);

		// NEQUAL
		} else if (ch == '!' && peek(lexer, 1) == '=') {
			Loc location = {lexer->file, lexer->line, lexer->col};

			Token token;
			token.type = NEQUAL;
			token.value = malloc(sizeof(char) * 2 + 1);
			strcpy(token.value, "!=");
			token.loc = location;

			array_push(&tokens, &token);
			consume(lexer);
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
        lexer->content = NULL;
    }
}
