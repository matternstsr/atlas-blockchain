# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -g
CPPFLAGS = -I. -Itransaction/ -I../../crypto
LDFLAGS = -L../../crypto
LDLIBS = -lhblk_crypto -lllist -lssl -lcrypto

# Directories
OBJ_DIR = obj
SRC_DIR = .
SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard transaction/*.c)

# Object files
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) $(SRC_FILES:transaction/%.c=$(OBJ_DIR)/%.o)

# Library target
libhblk_blockchain.a: $(OBJ_FILES)
	ar rcs $@ $^

# Rule to compile .c files into .o object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(OBJ_DIR)/%.o: transaction/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

# Clean up object files and the library
clean:
	rm -rf $(OBJ_DIR)/*.o libhblk_blockchain.a
