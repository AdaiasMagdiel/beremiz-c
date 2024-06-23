#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

const char *tokenTypeStrings[] = {
    #define X(enum_val, string_val) string_val,
    TOKEN_TYPES
    #undef X
};

void tokens_array_cleanup(Array *tokens) {
    for (int i = 0; i < tokens->length; i++) {
        Token *token = &((Token *)tokens->array)[i];
        free(token->value);
    }

    array_free(tokens);
}

void print_tokens(Array tokens) {
    for (int i = 0; i < tokens.length; i++) {
        Token *token = &((Token *)tokens.array)[i];

        printf("Type: %s", token_type_to_str(token->type));

        switch (token->type) {
            case NUMBER:
                printf(" | Value: %d\n", *(int *)token->value);
                break;

            case PLUS:
            case EQUAL:
            case GREATER:
            case LESS:
                printf(" | Value: %c\n", *(char *)token->value);
                break;

            case SHOW:
            case STRING:
            case BOOL_:
            case IDENTIFIER:
            case DUP:
            case OVER:
            case NEQUAL:
                printf(" | Value: %s\n", (char *)token->value);
                break;

            default:
                printf(" | Not Implemented `%s` in `print_tokens`.\n", token_type_to_str(token->type));
                break;
        }
    }
}

const char *token_type_to_str(TokenType type) {
    if (type >= 0 && type < TOKEN_TYPE_COUNT) {
        return tokenTypeStrings[type];
    }
    return "UNKNOWN";
}
