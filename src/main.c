#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "dynamic_array.h"
#include "tokens.h"

#define MAX_LINE_LENGTH 128

char *readFile(char *filepath);

void runProgram(Lexer *lexer) {
	lexer->file = "REPL";

	printf("Beremiz C Version  Copyright (C) 2024 Adaías Magdiel\n"
		   "Use Ctrl+C ou Ctrl+Z to exit.\n\n");

	while (1) {
		char buffer[MAX_LINE_LENGTH];

		printf("> ");
		char *input = fgets(buffer, sizeof(buffer), stdin);

		if (input == NULL) {
            if (feof(stdin)) {
                break;
            } else {
                perror("Unable to run the REPL.\n");
                exit(1);
            }
        }

		lexer->content = buffer;
		lexer->current = 0;
		lexer->line = 1;
		lexer->col = 1;

		scan(lexer);
	}
}

void parser(Array tokens) {
	Array stack;
	array_init(&stack, sizeof(Token));

	// print_tokens(tokens);

	for (int i=0; i<tokens.length; i++) {
		Token *token = &((Token *)tokens.array)[i];
		Token tk;

        switch (token->type) {
            case NUMBER:
	            tk.type = NUMBER;
	            tk.value = malloc(sizeof(int));
	            *(int *)tk.value = *(int *)token->value;
	            tk.loc = token->loc;

	            array_push(&stack, &tk);

                break;

            case PLUS:
            	// ERROR
            	if (stack.length < 2) {
	                fprintf(stderr, "Not enough operands in the stack for PLUS operation\n");

	                tokens_array_cleanup(&tokens);
					tokens_array_cleanup(&stack);

	                exit(EXIT_FAILURE);
	            }

                Token b;
                array_pop(&stack, &b);

                Token a;
                array_pop(&stack, &a);

                tk.type = NUMBER;
	            tk.value = malloc(sizeof(int));
	            *(int *)tk.value = *(int *)a.value + *(int *)b.value;
	            tk.loc = token->loc;

	            array_push(&stack, &tk);

	            free(a.value);
	            free(b.value);

                break;
            default:
                printf("Not Implemented.\n");
                break;
        }
	}

	print_tokens(stack);

	tokens_array_cleanup(&tokens);
	tokens_array_cleanup(&stack);
}

void runFile(Lexer *lexer, char *file) {
	lexer->file = file;
	lexer->content = readFile(file);

	Array tokens = scan(lexer);
	parser(tokens);
}

int main(int argc, char **argv) {
	Lexer lexer = {0};

	lexer.current = 0;
	lexer.line = 1;
	lexer.col = 1;

	if (argc == 1) {
		runProgram(&lexer);
	} else if (argc == 2) {
		runFile(&lexer, argv[1]);
	} else {
		printf("Usage:\n"
			   "    beremiz [file]\n\n"
			   "Arguments:\n"
			   "    file (optional):    Path to the Beremiz script file to execute.\n"
			   "                        If no file is provided, the program will start the REPL.\n");
	}

	return 0;
}

char *readFile(char *filepath) {
	FILE *fp = fopen(filepath, "r");

	if (fp == NULL) {
		perror("Error: Unable to open file.");
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *buffer = (char *)malloc(fileSize + 1);
	if (buffer == NULL) {
		perror("Error: Unable to allocate memory to read file.");
		fclose(fp);
		exit(1);
	}

	fread(buffer, 1, fileSize, fp);
	buffer[fileSize] = '\0';

	fclose(fp);

	return buffer;
}
