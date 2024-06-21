#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_extractor.h"

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
				fprintf(stderr, "Malformed number.\n");
				if (!strcmp(lexer->file, "REPL")) {
					cleanup(lexer);
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
