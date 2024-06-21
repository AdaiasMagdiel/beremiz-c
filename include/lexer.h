#ifndef LEXER_H
#define LEXER_H

typedef struct {
	char *content;
	char *file;
	int current;
	int line;
	int col;
} Lexer;

int isAtEnd(Lexer *lexer);
char peek(Lexer *lexer, int idx);
char consume(Lexer *lexer);
int isNumber(char ch);
void scan(Lexer *lexer);
void cleanup(Lexer *lexer);

#endif  // LEXER_H
