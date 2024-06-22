#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

		} else if (token->type == STRING) {
			Token tk;
		    tk.type = STRING;
		    tk.value = malloc(strlen((char *)token->value) + 1);
		    strcpy(tk.value, (char *)token->value);
		    tk.loc = token->loc;

		    array_push(&stack, &tk);

		} else if (token->type == PLUS) {
			if (stack.length < 2) {
				error_token(
					"Error: Missing operand for PLUS operation. Please ensure "
					"you have at least two values available before performing "
					"the addition.",
					*token
				);

				tokens_array_cleanup(&tokens);
				tokens_array_cleanup(&stack);
			}

			Token b;
			array_pop(&stack, &b);

			Token a;
			array_pop(&stack, &a);

			Token tk;
			tk.loc = token->loc;

			if (a.type == NUMBER && b.type == NUMBER) {
				tk.type = NUMBER;
				tk.value = malloc(sizeof(int));
				*(int *)tk.value = *(int *)a.value + *(int *)b.value;

			} else if (a.type == STRING && b.type == STRING) {
				int a_length = strlen((char *)a.value);
				int b_length = strlen((char *)b.value);

				tk.type = STRING;
				tk.value = malloc(a_length + b_length + 1);
				strcpy(tk.value, (char *)a.value);
				strcat(tk.value, (char *)b.value);

			} else {
				char buffer[128];
				snprintf(
					buffer,
					sizeof(buffer),
					"Error: The PLUS operator expects two numbers to sum or "
					"two strings to concatenate. However, you provided '%s' "
					"and '%s' instead.",
					token_type_to_str(a.type), token_type_to_str(b.type)
				);

				error_token(buffer, *token);

				tokens_array_cleanup(&tokens);
				tokens_array_cleanup(&stack);
			}

			array_push(&stack, &tk);

			free(a.value);
			free(b.value);

		} else if (token->type == SHOW) {
			if (stack.length < 1) {
				error_token(
					"Error: The SHOW operation expects a value "
					"on the stack. Please ensure you have pushed a "
					"value onto the stack before executing SHOW.",
					*token
				);

				tokens_array_cleanup(&tokens);
				tokens_array_cleanup(&stack);
			}

			Token value;
			array_pop(&stack, &value);

			if (value.type == NUMBER) {
				printf("%d\n", *(int *)value.value);

			} else if (value.type == STRING) {
				printf("%s\n", (char *)value.value);

			} else {
				char buffer[72];
				snprintf(
					buffer, sizeof(buffer),
					"Not implemented type `%s` for `SHOW` operation.\n",
					token_type_to_str(value.type)
				);

				error_token(buffer, *token);
			}

		} else {
			char buffer[72];
			snprintf(
				buffer, sizeof(buffer),
				"Not Implemented `%s` in `parser`.\n",
				token_type_to_str(token->type)
			);

			error_token(buffer, *token);
		}
	}

	print_tokens(stack);

	tokens_array_cleanup(&tokens);
	tokens_array_cleanup(&stack);
}
