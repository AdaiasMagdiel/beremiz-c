#include <stdio.h>
#include <stdlib.h>
#include "file_utils.h"

char *readFile(char *filepath) {
	FILE *fp = fopen(filepath, "r");

	if (fp == NULL) {
		fprintf(stderr, "Error: Unable to open file.");
		exit(EXIT_FAILURE);
	}

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *buffer = (char *)malloc(fileSize + 1);
	if (buffer == NULL) {
		fprintf(stderr, "Error: Unable to allocate memory to read file.");
		fclose(fp);
		exit(EXIT_FAILURE);
	}

	fread(buffer, 1, fileSize, fp);
	buffer[fileSize] = '\0';

	fclose(fp);

	return buffer;
}
