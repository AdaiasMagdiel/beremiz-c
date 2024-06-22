#ifndef ERROR_H
#define ERROR_H

#include "tokens.h"

void error(char *message, Loc location);
void error_token(char *message, Token token);

#endif  // ERROR_H
