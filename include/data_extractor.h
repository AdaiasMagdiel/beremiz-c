#ifndef DATA_EXTRACTOR_H
#define DATA_EXTRACTOR_H value

#include "lexer.h"

int isNumber(char ch);
int isAlpha(char ch);
int isAlphaNumeric(char ch);

double extractNumber(Lexer *lexer, int *isFloat);
char *extractIdentifier(Lexer *lexer);
char *extractString(Lexer *lexer);

#endif  // DATA_EXTRACTOR_H
