#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#define INITIAL_BUFFER_SIZE 128
#define INITIAL_LINES_CAPACITY 10

char *readFile(const char *filename);
char **readLines(const char *filename, int *lineCount);
void lines_cleanup(char **lines, int lineCount);

#endif  // FILE_UTILS_H
