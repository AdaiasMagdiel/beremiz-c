#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "tokens.h"
#include "error.h"
#include "lexer_utils.h"

void parser(Array tokens) {
	Array stack;
	array_init(&stack, sizeof(Token));

	int ip = 0;

	while (ip < tokens.length) {
		Token *token = &((Token *)tokens.array)[ip];

		switch(token->type) {
			case INT_: {
				Token tk = create_token_int(
					token->loc,
					INT_,
					*(int *)token->value
				);

				array_push(&stack, &tk);
				ip++;

			} break;

			case FLOAT_: {
				Token tk = create_token_double(
					token->loc,
					FLOAT_,
					*(double *)token->value
				);

				array_push(&stack, &tk);
				ip++;

			} break;

			case STRING: {
				Token tk = create_token_string(
					token->loc,
					STRING,
					(char *)token->value
				);

			    array_push(&stack, &tk);
			    ip++;

			} break;

			case BOOL_: {
				Token tk = create_token_string(
					token->loc,
					BOOL_,
					(char *)token->value
				);

			    array_push(&stack, &tk);
			    ip++;

			} break;

			case PLUS: {
				if (stack.length < 2) {
					error_token(
						"Error: Missing operands for PLUS operation. Please ensure "
						"you have at least two values available before performing "
						"the addition.",
						*token
					);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				Token b;
				array_pop(&stack, &b);

				Token a;
				array_pop(&stack, &a);

				Token tk;

				if (a.type == INT_ && b.type == INT_) {
					tk = create_token_int(
						token->loc,
						INT_,
						*(int *)a.value + *(int *)b.value
					);

				} else if (a.type == STRING && b.type == STRING) {
					int a_length = strlen((char *)a.value);
					int b_length = strlen((char *)b.value);

					char *value = malloc(a_length + b_length + 1);
					strcpy(value, (char *)a.value);
					strcat(value, (char *)b.value);

					tk = create_token_string(
						token->loc,
						STRING,
						value
					);

					free(value);

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

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				array_push(&stack, &tk);

				free(a.value);
				free(b.value);
				ip++;

			} break;

			case EQUAL: {
				if (stack.length < 2) {
					error_token(
						"Error: Missing operands for EQUAL operation.\n\n"
						"Please ensure you have at least two values available "
						"before performing the comparasion.",
						*token
					);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				Token b;
				array_pop(&stack, &b);

				Token a;
				array_pop(&stack, &a);

				Token tk;

				if (a.type == INT_ && b.type == INT_) {
					if (*(int *)a.value == *(int *)b.value) {
						tk = create_token_string(
							token->loc,
							BOOL_,
							"true"
						);

					} else {
						tk = create_token_string(
							token->loc,
							BOOL_,
							"false"
						);
					}

				} else if (
					(a.type == STRING && b.type == STRING) ||
					(a.type == BOOL_ && b.type == BOOL_)
				) {
					if (strcmp((char *)a.value, (char *)b.value) == 0) {
						tk = create_token_string(
							token->loc,
							BOOL_,
							"true"
						);

					} else {
						tk = create_token_string(
							token->loc,
							BOOL_,
							"false"
						);
					}

				} else {
					char buffer[256];
					snprintf(
						buffer,
						sizeof(buffer),
						"Error: Invalid Comparison\n\n"
						"The EQUAL operation can only compare values of the "
						"same type: numbers, strings, or booleans. You cannot "
						"compare '%s' with '%s' without an explicit conversion.",
						token_type_to_str(a.type), token_type_to_str(b.type)
					);

					error_token(buffer, *token);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				array_push(&stack, &tk);

				free(a.value);
				free(b.value);
				ip++;

			} break;

			case NEQUAL: {
				if (stack.length < 2) {
					error_token(
						"Error: Missing operands for NOT EQUAL operation.\n\n"
						"Please ensure you have at least two values available "
						"before performing the comparasion.",
						*token
					);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				Token b;
				array_pop(&stack, &b);

				Token a;
				array_pop(&stack, &a);

				Token tk;

				if (a.type == INT_ && b.type == INT_) {
					if (*(int *)a.value != *(int *)b.value) {
						tk = create_token_string(
							token->loc,
							BOOL_,
							"true"
						);

					} else {
						tk = create_token_string(
							token->loc,
							BOOL_,
							"false"
						);
					}

				} else if (
					(a.type == STRING && b.type == STRING) ||
					(a.type == BOOL_ && b.type == BOOL_)
				) {
					if (strcmp((char *)a.value, (char *)b.value) != 0) {
						tk = create_token_string(
							token->loc,
							BOOL_,
							"true"
						);

					} else {
						tk = create_token_string(
							token->loc,
							BOOL_,
							"false"
						);
					}

				} else {
					char buffer[256];
					snprintf(
						buffer,
						sizeof(buffer),
						"Error: Invalid Comparison\n\n"
						"The NOT EQUAL operation can only compare values of the "
						"same type: numbers, strings, or booleans. You cannot "
						"compare '%s' with '%s' without an explicit conversion.",
						token_type_to_str(a.type), token_type_to_str(b.type)
					);

					error_token(buffer, *token);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				array_push(&stack, &tk);

				free(a.value);
				free(b.value);
				ip++;

			} break;

			case GREATER: {
				if (stack.length < 2) {
					error_token(
						"Error: Missing operands for GREATER operation.\n\n"
						"Please ensure you have at least two values available "
						"before performing the comparasion.",
						*token
					);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				Token b;
				array_pop(&stack, &b);

				Token a;
				array_pop(&stack, &a);

				Token tk;

				if (a.type == INT_ && b.type == INT_) {
					if (*(int *)a.value > *(int *)b.value) {
						tk = create_token_string(
							token->loc,
							BOOL_,
							"true"
						);

					} else {
						tk = create_token_string(
							token->loc,
							BOOL_,
							"false"
						);
					}

				} else {
					char buffer[256];
					snprintf(
						buffer,
						sizeof(buffer),
						"Error: Invalid Comparison\n\n"
						"The GREATER operation can only compare number values. "
						"You cannot "
						"compare '%s' with '%s' without an explicit conversion.",
						token_type_to_str(a.type), token_type_to_str(b.type)
					);

					error_token(buffer, *token);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				array_push(&stack, &tk);

				free(a.value);
				free(b.value);
				ip++;

			} break;

			case LESS: {
				if (stack.length < 2) {
					error_token(
						"Error: Missing operands for LESS operation.\n\n"
						"Please ensure you have at least two values available "
						"before performing the comparasion.",
						*token
					);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				Token b;
				array_pop(&stack, &b);

				Token a;
				array_pop(&stack, &a);

				Token tk;

				if (a.type == INT_ && b.type == INT_) {
					if (*(int *)a.value < *(int *)b.value) {
						tk = create_token_string(
							token->loc,
							BOOL_,
							"true"
						);

					} else {
						tk = create_token_string(
							token->loc,
							BOOL_,
							"false"
						);
					}

				} else {
					char buffer[256];
					snprintf(
						buffer,
						sizeof(buffer),
						"Error: Invalid Comparison\n\n"
						"The LESS operation can only compare number values. "
						"You cannot "
						"compare '%s' with '%s' without an explicit conversion.",
						token_type_to_str(a.type), token_type_to_str(b.type)
					);

					error_token(buffer, *token);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				array_push(&stack, &tk);

				free(a.value);
				free(b.value);
				ip++;

			} break;

			case SHOW: {
				if (stack.length < 1) {
					error_token(
						"Error: The SHOW operation expects a value "
						"on the stack. Please ensure you have pushed a "
						"value onto the stack before executing SHOW.",
						*token
					);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				Token value;
				array_pop(&stack, &value);

				if (value.type == INT_) {
					printf("%d\n", *(int *)value.value);


				} else if (value.type == FLOAT_) {
					printf("%f\n", *(double *)value.value);

				} else if (value.type == STRING) {
					printf("%s\n", (char *)value.value);

				} else if (value.type == BOOL_) {
					printf("%s\n", (char *)value.value);

				} else {
					char buffer[72];
					snprintf(
						buffer, sizeof(buffer),
						"Not implemented type `%s` for `SHOW` operation.\n",
						token_type_to_str(value.type)
					);

					error_token(buffer, *token);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				ip++;

			} break;

			case DUP: {
				if (stack.length < 1) {
					error_token(
						"Error: The DUP operation expects a value "
						"on the stack. Please ensure you have pushed a "
						"value onto the stack before executing DUP.",
						*token
					);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				Token token_to_dup = ((Token *)stack.array)[stack.length - 1];

				Token tk;

				if (token_to_dup.type == INT_) {
					tk = create_token_int(
						token->loc,
						INT_,
						*(int *)token_to_dup.value
					);

				} else if (token_to_dup.type == STRING) {
					tk = create_token_string(
						token->loc,
						STRING,
						(char *)token_to_dup.value
					);

				} else if (token_to_dup.type == BOOL_) {
					tk = create_token_string(
						token->loc,
						BOOL_,
						(char *)token_to_dup.value
					);

				} else {
					char buffer[128];
					snprintf(
						buffer,
						sizeof(buffer),
						"Error: The DUP operator expects a number, string or "
						"boolean on the stack. However, you provided a value of "
						"type '%s'.",
						token_type_to_str(token_to_dup.type)
					);

					error_token(buffer, *token);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				array_push(&stack, &tk);
				ip++;

			} break;

			case OVER: {
				if (stack.length < 2) {
					error_token(
						"Error: The OVER operation expects two values "
						"on the stack. Please ensure you have these "
						"values onto the stack before executing OVER.",
						*token
					);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				Token token_to_over = ((Token *)stack.array)[stack.length - 2];

				Token tk;

				if (token_to_over.type == INT_) {
					tk = create_token_int(
						token->loc,
						INT_,
						*(int *)token_to_over.value
					);

				} else if (token_to_over.type == STRING) {
					tk = create_token_string(
						token->loc,
						STRING,
						(char *)token_to_over.value
					);

				} else if (token_to_over.type == BOOL_) {
					tk = create_token_string(
						token->loc,
						BOOL_,
						(char *)token_to_over.value
					);

				} else {
					char buffer[128];
					snprintf(
						buffer,
						sizeof(buffer),
						"Error: The OVER operator expects a number, string or "
						"a boolean on the stack. However, you provided a value of "
						"type '%s'.",
						token_type_to_str(token_to_over.type)
					);

					error_token(buffer, *token);

					if (strcmp(token->loc.file, "REPL") != 0) {
						tokens_array_cleanup(&tokens);
						tokens_array_cleanup(&stack);
						exit(EXIT_FAILURE);
					}

					ip++;
					continue;
				}

				array_push(&stack, &tk);
				ip++;

			} break;

			default: {
				char buffer[72];
				snprintf(
					buffer, sizeof(buffer),
					"Not Implemented `%s` in `parser`.\n",
					token_type_to_str(token->type)
				);

				error_token(buffer, *token);

				if (strcmp(token->loc.file, "REPL") != 0) {
					tokens_array_cleanup(&tokens);
					tokens_array_cleanup(&stack);
					exit(EXIT_FAILURE);
				}

				ip++;

			} break;
		}
	}

	print_tokens(stack);

	tokens_array_cleanup(&tokens);
	tokens_array_cleanup(&stack);
}
