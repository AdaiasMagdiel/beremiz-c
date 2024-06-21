CC = gcc
CFLAGS = -Wall -Wextra -Werror
FLAGS = -I./include
INPUT = src/main.c src/lexer/*.c
TARGET = beremiz.exe

$(TARGET): $(INPUT)
	$(CC) $(CFLAGS) $(FLAGS) $(INPUT) -o $(TARGET)

clean:
	rm -f $(TARGET)

all:
	$(TARGET)
