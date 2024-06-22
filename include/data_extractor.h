#ifndef DATA_EXTRACTOR_H
#define DATA_EXTRACTOR_H value

#include "lexer.h"

int isNumber(char ch);
int isAlpha(char ch);
int isAlphaNumeric(char ch);

int extractNumber(Lexer *lexer);
char *extractIdentifier(Lexer *lexer);

#endif  // DATA_EXTRACTOR_H
