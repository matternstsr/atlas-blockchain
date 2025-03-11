CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -g -I.
CPPFLAGS := -I. -Itransaction/ -I../../crypto
LDFLAGS := -L../../crypto
LDLIBS := -lhblk_crypto -lllist -lssl -lcrypto

libhblk_blockchain.a:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) *.c transaction/*.c
	ar -rcs libhblk_blockchain.a *.o

clean:
	rm -rf *.o

