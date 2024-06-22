#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_utils.h"

char *readFile(const char *filename) {
	FILE *file = fopen(filename, "r");

	if (file == NULL) {
		fprintf(stderr, "Error: Unable to open file.");
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = (char *)malloc(fileSize + 1);
	if (buffer == NULL) {
		fprintf(stderr, "Error: Unable to allocate memory to read file.");
		fclose(file);
		exit(EXIT_FAILURE);
	}

	fread(buffer, 1, fileSize, file);
	buffer[fileSize] = '\0';

	fclose(file);

	return buffer;
}



char **readLines(const char *filename, int *lineCount) {
	FILE *file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, "Error: Unable to open file.");
		exit(EXIT_FAILURE);
	}

	int buffer_size = INITIAL_BUFFER_SIZE;
	char *buffer = (char *)malloc(buffer_size * sizeof(char));
	if (!buffer) {
		fprintf(stderr, "Error: Unable to allocate memory to read file.");
		fclose(file);
		exit(EXIT_FAILURE);
	}

	int lines_capacity = INITIAL_LINES_CAPACITY;
	char **lines = (char **)malloc(lines_capacity * sizeof(char *));
	if (!lines) {
		fprintf(stderr, "Error: Unable to allocate memory for lines.");
		free(buffer);
		fclose(file);
		exit(EXIT_FAILURE);
	}

	*lineCount = 0;

	while (fgets(buffer, buffer_size, file)) {
		int len = strlen(buffer);

		while (buffer[len - 1] != '\n' && !feof(file)) {
			buffer_size *= 2;
			char *new_buffer = realloc(buffer, buffer_size * sizeof(char));

			if (!new_buffer) {
				fprintf(stderr, "Error: Unable to allocate memory for buffer.");
				free(buffer);

				for (int i = 0; i < *lineCount; i++) {
					free(lines[i]);
				}

				free(lines);
				fclose(file);

				exit(EXIT_FAILURE);
			}

			buffer = new_buffer;

			if (fgets(buffer + len, buffer_size - len, file)) {
				len += strlen(buffer + len);
			}
		}

		if (buffer[len - 1] == '\n') {
			buffer[len - 1] = '\0';
		}

		if (*lineCount >= lines_capacity) {
			lines_capacity *= 2;
			char **new_lines = realloc(lines, lines_capacity * sizeof(char *));

			if (!new_lines) {
				fprintf(stderr, "Error: Unable to allocate memory for lines.");
				free(buffer);

				for (int i = 0; i < *lineCount; i++) {
					free(lines[i]);
				}

				free(lines);
				fclose(file);

				exit(EXIT_FAILURE);
			}

			lines = new_lines;
		}

		lines[*lineCount] = strdup(buffer);
		if (!lines[*lineCount]) {
			fprintf(stderr, "Error: Unable to duplicate lines.");
			free(buffer);

			for (int i = 0; i < *lineCount; i++) {
				free(lines[i]);
			}

			free(lines);
			fclose(file);

			exit(EXIT_FAILURE);
		}

		(*lineCount)++;
	}

	free(buffer);
	fclose(file);

	return lines;
}

void lines_cleanup(char **lines, int lineCount) {
	if (lines) {
        for (int i = 0; i < lineCount; i++) {
            free(lines[i]);
        }

        free(lines);
    }
}
