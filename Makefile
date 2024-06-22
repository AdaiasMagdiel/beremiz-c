CC = gcc
CFLAGS = -Wall -Wextra -Werror
FLAGS = -I./include

SRC_DIR = src
LEXER_DIR = $(SRC_DIR)/lexer
PARSER_DIR = $(SRC_DIR)/parser
DATA_TYPES_DIR = $(SRC_DIR)/data_types

SRC = $(wildcard $(SRC_DIR)/*.c) \
      $(wildcard $(LEXER_DIR)/*.c) \
      $(wildcard $(PARSER_DIR)/*.c) \
      $(wildcard $(DATA_TYPES_DIR)/*.c)

OBJ = $(SRC:.c=.o)

TARGET_DIR = beremiz

ifeq ($(OS),Windows_NT)
    TARGET = $(TARGET_DIR)/beremiz.exe
    REMOVE_DIR = if exist $(TARGET_DIR) (rd /s /q $(TARGET_DIR))
else
    TARGET = $(TARGET_DIR)/beremiz
    REMOVE_DIR = rm -rf $(TARGET_DIR)
endif

.PHONY: all build clean

all: build

build: $(TARGET)

$(TARGET): $(OBJ)
	@echo "Cleaning up the target directory..."
	-@$(REMOVE_DIR)
	@echo "Creating the target directory..."
	@mkdir -p $(TARGET_DIR)
	@echo "Building the target..."
	$(CC) $(CFLAGS) $(FLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(FLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
