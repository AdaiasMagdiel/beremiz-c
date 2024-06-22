#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "tokens.h"
#include "error.h"

void parser(Array tokens) {
	Array stack;
	array_init(&stack, sizeof(Token));

	for (int i=0; i<tokens.length; i++) {
		Token *token = &((Token *)tokens.array)[i];

		if (token->type == NUMBER) {
			Token tk;
            tk.type = NUMBER;
            tk.value = malloc(sizeof(int));
            *(int *)tk.value = *(int *)token->value;
            tk.loc = token->loc;

            array_push(&stack, &tk);

		} else if (token->type == PLUS) {
			// ERROR
        	if (stack.length < 2) {
        		error("Not enough operands in the stack for PLUS operation.", token->loc);

                tokens_array_cleanup(&tokens);
				tokens_array_cleanup(&stack);
            }

            Token b;
            array_pop(&stack, &b);

            Token a;
            array_pop(&stack, &a);

            Token tk;
            tk.type = NUMBER;
            tk.value = malloc(sizeof(int));
            *(int *)tk.value = *(int *)a.value + *(int *)b.value;
            tk.loc = token->loc;

            array_push(&stack, &tk);

            free(a.value);
            free(b.value);

        } else if (token->type == SHOW) {
        	// ERROR
        	if (stack.length < 1) {
                error("Not enough operands in the stack for SHOW operation.", token->loc);

                tokens_array_cleanup(&tokens);
				tokens_array_cleanup(&stack);
            }

            Token value;
            array_pop(&stack, &value);

            if (value.type == NUMBER) {
            	printf("%d\n", *(int *)value.value);

            } else {
            	printf("Not implemented type `%s` for `SHOW` operation.\n", token_type_to_str(token->type));
            }

		} else {
			printf("Not Implemented in `parser`.\n");
		}
	}

	print_tokens(stack);

	tokens_array_cleanup(&tokens);
	tokens_array_cleanup(&stack);
}
