#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "file_utils.h"

void error(char *message, Loc location) {
	fprintf(
		stderr,
		"\x1b[31m%s:%d:%d:\x1b[0m %s\n",
		location.file, location.line, location.col,
		message
	);

	if (strcmp(location.file, "REPL") != 0) {
		exit(EXIT_FAILURE);
	}
}

void error_token(char *message, Token token) {
	fprintf(
		stderr,
		"\x1b[31m%s:%d:%d:\x1b[0m %s\n\n",
		token.loc.file, token.loc.line, token.loc.col,
		message
	);

	if (strcmp(token.loc.file, "REPL") == 0) return;

	int lineCount;
    char **lines = readLines(token.loc.file, &lineCount);
    char *line = lines[token.loc.line-1];

    fprintf(stderr, "\x1b[31m%d\x1b[0m | %s\n", token.loc.line, line);
    fprintf(stderr, "\x1b[31m%d\x1b[0m | ", token.loc.line);

    for (int i=0; i<token.loc.col-1; i++) {
    	fprintf(stderr, " ");
    }
    fprintf(stderr, "\x1b[31m^");

    int value_len = strlen(token.value);
    for (int i=0; i<value_len-1; i++) {
    	fprintf(stderr, "~");
    }
    fprintf(stderr, "\x1b[0m\n");

    lines_cleanup(lines, lineCount);
    exit(EXIT_FAILURE);
}
