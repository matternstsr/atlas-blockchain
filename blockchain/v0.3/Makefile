CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -g
CPPFLAGS = -I. -Itransaction/ -I../../crypto
LDFLAGS = -L../../crypto
LDLIBS = -lhblk_crypto -lllist -lssl -lcrypto

OBJ_DIR = obj
SRC_DIR = .
SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard transaction/*.c)

OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) $(SRC_FILES:transaction/%.c=$(OBJ_DIR)/%.o)

libhblk_blockchain.a: $(OBJ_FILES)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(OBJ_DIR)/%.o: transaction/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o libhblk_blockchain.a
