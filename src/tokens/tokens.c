#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

void tokens_array_cleanup(Array *tokens) {
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

		printf("Type: %d", token->type);

        switch (token->type) {
            case NUMBER:
                printf(" | Value: %d\n", *(int *)token->value);
                break;
            case PLUS:
                printf(" | Value: %c\n", *(char *)token->value);
                break;
            default:
                printf(" | Not Implemented in `print_tokens`.\n");
                break;
        }
	}
}
