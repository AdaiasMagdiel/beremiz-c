#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

char *readFile(char *filepath);

void runProgram(Lexer *lexer) {
	lexer->file = "REPL";
	printf("The REPL was not implemented yet.\n");
	printf("File: %s\n", lexer->file);
}

void runFile(Lexer *lexer, char *file) {
	lexer->file = file;
	lexer->content = readFile(file);
	lexer->current = 0;
	lexer->line = 1;
	lexer->col = 1;

	scan(lexer);
}

int main(int argc, char **argv) {
	Lexer lexer = {0};

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

	cleanup(&lexer);
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
