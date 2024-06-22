#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

void tokens_array_cleanup(Array *tokens) {
    for (int i = 0; i < tokens->length; i++) {
        Token *token = &((Token *)tokens->array)[i];
        free(token->value);
    }

    array_free(tokens);
    printf("Cleaning tokens...\n");
}

void print_tokens(Array tokens) {
	for (int i=0; i<tokens.length; i++) {
		Token *token = &((Token *)tokens.array)[i];

        printf("Type: %s", token_type_to_str(token->type));

        switch (token->type) {
            case NUMBER:
                printf(" | Value: %d\n", *(int *)token->value);
                break;
            case PLUS:
                printf(" | Value: %c\n", *(char *)token->value);
                break;
            case SHOW:
                printf(" | Value: %s\n", (char *)token->value);
                break;
            default:
                printf(" | Not Implemented `%s` in `print_tokens`.\n", token_type_to_str(token->type));
                break;
        }
	}
}

const char *token_type_to_str(enum TokenType type) {
    const char *types[] = {
        "NUMBER",
        "PLUS",
        "SHOW"
    };

    return types[type];
}
