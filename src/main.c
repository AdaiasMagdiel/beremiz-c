#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

char *readFile(char *filepath);

int main() {
	char *file = "files/numbers.brz";

	Lexer lexer = {0};

	lexer.file = file;
	lexer.content = readFile(file);
	lexer.current = 0;
	lexer.line = 1;
	lexer.col = 1;

	scan(&lexer);

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
