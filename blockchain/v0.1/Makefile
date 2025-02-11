# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -I.

# Directories
SRC_DIR = blockchain/v0.1
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(SRC_DIR)/blockchain_serialize.c \
       $(SRC_DIR)/block_create.c \
       $(SRC_DIR)/block_destroy.c \
       $(SRC_DIR)/block_hash.c \
       $(SRC_DIR)/block_is_valid.c \
       $(SRC_DIR)/blockchain_create.c \
       $(SRC_DIR)/blockchain_deserialize.c \
       $(SRC_DIR)/blockchain_destroy.c

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Executable files
EXEC = $(BIN_DIR)/blockchain

# Create necessary directories if not exists
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Default target
all: $(EXEC)

# Rule to create the executable
$(EXEC): $(OBJS)
       $(CC) $(OBJS) -o $(EXEC) -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

# Rule to compile .c files into .o object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
       $(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
       rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean
