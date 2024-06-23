#include <stdlib.h>
#include <string.h>
#include "lexer_utils.h"

Token create_token_char(const Loc location, const TokenType type, const char value) {
	Token token;
	token.type = type;
	token.value = malloc(sizeof(char));
	*(char *)token.value = value;
	token.loc = location;

	return token;
}

Token create_token_int(const Loc location, const TokenType type, const int value) {
	Token token;
	token.type = type;
	token.value = malloc(sizeof(int));
	*(int *)token.value = value;
	token.loc = location;

	return token;
}

Token create_token_double(const Loc location, const TokenType type, const double value) {
	Token token;
	token.type = type;
	token.value = malloc(sizeof(double));
	*(double *)token.value = value;
	token.loc = location;

	return token;
}

Token create_token_string(const Loc location, const TokenType type, const char *value) {
	int value_len = strlen(value);

	Token token;
	token.type = type;
	token.value = malloc(sizeof(char) * value_len + 1);
	strcpy(token.value, value);
	token.loc = location;

	return token;
}
