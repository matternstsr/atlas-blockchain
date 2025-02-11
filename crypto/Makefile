CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -Werror -pedantic -std=gnu89
SRC = sha256.c ec_create.c ec_to_pub.c ec_from_pub.c ec_save.c ec_load.c ec_sign.c ec_verify.c
OBJ = $(SRC:.c=.o)
LIB = libhblk_crypto.a

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) rcs $(LIB) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(LIB)

re: fclean all
