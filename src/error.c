#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

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
