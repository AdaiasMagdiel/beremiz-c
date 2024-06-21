#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

void tokens_cleanup(Array *tokens) {
    for (int i = 0; i < tokens->length; i++) {
        Token *token = &((Token *)tokens->array)[i];
        free(token->value);
    }

    array_free(tokens);
    printf("Limpando tokens");
}

void print_tokens(Array tokens) {
	for (int i=0; i<tokens.length; i++) {
		Token *token = &((Token *)tokens.array)[i];

		printf("Type: %d | ", token->type);

		if (token->type == NUMBER) {
            printf("Value: %d\n", *(int *)token->value);
        }
	}
}
