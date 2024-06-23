#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "tokens.h"
#include "data_extractor.h"
#include "dynamic_array.h"
#include "lexer_utils.h"

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
			int isFloat;
			double value = extractNumber(lexer, &isFloat);

			Loc location = {lexer->file, lexer->line, lexer->col};
			Token token;

			if (isFloat) {
				token = create_token_double(location, FLOAT_, value);
			} else {
				token = create_token_int(location, INT_, (int) value);
			}

			array_push(&tokens, &token);

		// IDENTIFIERS
		} else if (isAlphaNumeric(ch)) {
			char *value = extractIdentifier(lexer);

			Loc location = {lexer->file, lexer->line, lexer->col - strlen(value)};
			Token token = create_token_string(location, IDENTIFIER, value);

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
			}

			free(value);
			array_push(&tokens, &token);

		// STRINGS
		} else if (ch == '"') {
			char *value = extractString(lexer);
			
			Loc location = {lexer->file, lexer->line, lexer->col - strlen(value)-2};
			Token token = create_token_string(location, STRING, value);

			free(value);
			array_push(&tokens, &token);

		// PLUS
		} else if (ch == '+') {
			Loc location = {lexer->file, lexer->line, lexer->col};
			Token token = create_token_char(location, PLUS, '+');

			array_push(&tokens, &token);
			consume(lexer);

		// EQUAL
		} else if (ch == '=') {
			Loc location = {lexer->file, lexer->line, lexer->col};
			Token token = create_token_char(location, EQUAL, '=');

			array_push(&tokens, &token);
			consume(lexer);

		// GREATER
		} else if (ch == '>') {
			Loc location = {lexer->file, lexer->line, lexer->col};
			Token token = create_token_char(location, GREATER, '>');

			array_push(&tokens, &token);
			consume(lexer);

		// LESS
		} else if (ch == '<') {
			Loc location = {lexer->file, lexer->line, lexer->col};
			Token token = create_token_char(location, LESS, '<');

			array_push(&tokens, &token);
			consume(lexer);

		// NEQUAL
		} else if (ch == '!' && peek(lexer, 1) == '=') {
			Loc location = {lexer->file, lexer->line, lexer->col};
			Token token = create_token_string(location, NEQUAL, "!=");

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
