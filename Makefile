CC = gcc
CFLAGS = -Wall -Wextra -Werror
FLAGS = -I./include

SRC_DIR = src
LEXER_DIR = $(SRC_DIR)/lexer
DATA_TYPES_DIR = $(SRC_DIR)/data_types

SRC = $(wildcard $(SRC_DIR)/*.c) \
      $(wildcard $(LEXER_DIR)/*.c) \
      $(wildcard $(DATA_TYPES_DIR)/*.c)

OBJ = $(SRC:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET = beremiz.exe
else
    TARGET = beremiz
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(FLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(FLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
