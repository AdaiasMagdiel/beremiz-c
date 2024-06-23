#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_extractor.h"
#include "error.h"
#include "lexer.h"

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

double extractNumber(Lexer *lexer, int *isFloat) {
    *isFloat = 0;

    int startCol = lexer->col;
    char buffer[64];
    int buffer_idx = 0;
    char ch;

    int isNegative = 0;
    if (
        (lexer->current > 0 && peek(lexer, -1) == '-') || 
        (lexer->current > 1 && (peek(lexer, -1) == '.' && peek(lexer, -2) == '-')) 
    ) {
        isNegative = 1;
    }

    int decimalPointCount = 0;
    if (lexer->current > 0 && peek(lexer, -1) == '.') {
        *isFloat = 1;
        decimalPointCount++;

        buffer[buffer_idx++] = '0';
        buffer[buffer_idx++] = '.';
    }

    while (1) {
        ch = peek(lexer, 0);

        if (ch == '_') {
            consume(lexer);
            continue;
        }

        if (!isNumber(ch)) {
            if (ch == '-' && isNegative) {
                Loc location = {lexer->file, lexer->line, startCol};

                error(
                    "Error: Invalid Number Format.\n\nThe input contains an "
                    "invalid number. Specifically, two minus signs (-) were "
                    "found in a row, which is not allowed in number "
                    "formatting.",
                    location
                );

                if (strcmp(lexer->file, "REPL") != 0) {
                    cleanup(lexer);
                    exit(EXIT_FAILURE);
                } else {
                    break;
                }
            } else if (ch == '.') {
                if (decimalPointCount > 0) {
                    Loc location = {lexer->file, lexer->line, startCol};

                    error(
                        "Error: Invalid Number Format.\n\nThe input contains an "
                        "invalid number. Specifically, multiple decimal points (.) "
                        "were found in a row, which is not allowed in number "
                        "formatting.",
                        location
                    );

                    if (strcmp(lexer->file, "REPL") != 0) {
                        cleanup(lexer);
                        exit(EXIT_FAILURE);
                    } else {
                        break;
                    }
                }

                *isFloat = 1;
                decimalPointCount++;

                buffer[buffer_idx++] = '.';
                consume(lexer);
                continue;

            } else {
                break;
            }
        }

        buffer[buffer_idx++] = ch;
        consume(lexer);
    }

    buffer[buffer_idx] = '\0';

    double value = atof(buffer);

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

char *extractString(Lexer *lexer) {
    int startCol = lexer->col;
    int startLine = lexer->line;

    int buffer_size = 32;
    char *buffer = (char *)malloc(buffer_size * sizeof(char));
    int buffer_idx = 0;

    char ch;

    // Jump the quote "
    consume(lexer);

    while (1) {
        ch = peek(lexer, 0);

        if (ch == '"' && peek(lexer, -1) != '\\') {
            consume(lexer);
            break;
        }

        if (isAtEnd(lexer)) {
            Loc location = {lexer->file, startLine, startCol};
            error("Syntax Error: Unclosed string.", location);

            if (strcmp(lexer->file, "REPL") != 0) {
                exit(EXIT_FAILURE);
            }
        }

        if (buffer_idx >= buffer_size - 1) {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size * sizeof(char));
            buffer = new_buffer;
        }

        if (ch == '\\' && !isAtEnd(lexer)) {
            consume(lexer); // consume the backslash
            ch = peek(lexer, 0);
            switch (ch) {
                case 'n':
                    buffer[buffer_idx++] = '\n';
                    break;
                case 't':
                    buffer[buffer_idx++] = '\t';
                    break;
                case '\\':
                    buffer[buffer_idx++] = '\\';
                    break;
                case '"':
                    buffer[buffer_idx++] = '"';
                    break;
                default:
                    buffer[buffer_idx++] = '\\';
                    buffer[buffer_idx++] = ch;
                    break;
            }
        } else {
            buffer[buffer_idx++] = ch;
        }

        consume(lexer);
    }

    buffer[buffer_idx] = '\0';

    return buffer;
}

