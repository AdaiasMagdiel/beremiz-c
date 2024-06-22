#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_extractor.h"

int isNumber(char ch) {
	return (ch >= '0' && ch <= '9');
}

int isAlpha(char ch) {
	return (ch >= 'a' && ch <= 'z') ||
		   (ch >= 'A' && ch <= 'Z') ||
		   (ch == '_');
}

int isAlphaNumeric(char ch) {
	return isNumber(ch) || isAlpha(ch);
}

int extractNumber(Lexer *lexer) {
	char buffer[17];
	int buffer_idx = 0;
	char ch;

	int isNegative = lexer->current > 0 && peek(lexer, -1) == '-';

	while (1) {
		ch = peek(lexer, 0);

		if (!isNumber(ch)) {
			if (ch == '-' && isNegative) {
				// ERROR
				fprintf(stderr, "Error: Malformed number.\n");
				cleanup(lexer);
				if (strcmp(lexer->file, "REPL") != 0) {
					exit(1);
				}
			}

			break;
		}

		buffer[buffer_idx] = ch;
		buffer_idx++;

		consume(lexer);
	}

	buffer[buffer_idx] = '\0';

	int value = atoi(buffer);

	if (isNegative) {
		value *= -1;
	}

	return value;
}

char *extractIdentifier(Lexer *lexer) {
    int buffer_size = 32;
    char *buffer = (char *)malloc(buffer_size * sizeof(char));
    int buffer_idx = 0;
    char ch;

    while (1) {
        ch = peek(lexer, 0);

        if (!isAlphaNumeric(ch)) {
            break;
        }

        if (buffer_idx >= buffer_size - 1) {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size * sizeof(char));
            buffer = new_buffer;
        }

        buffer[buffer_idx] = ch;
        buffer_idx++;
        consume(lexer);
    }

    buffer[buffer_idx] = '\0';

    return buffer;
}
