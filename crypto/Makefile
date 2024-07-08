CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -I.

SRC_FILES = sha256.c ec_create.c ec_to_pub.c ec_from_pub.c ec_save.c ec_load.c ec_sign.c ec_verify.c
OBJ_FILES = $(SRC_FILES:.c=.o)
LIB_NAME = libhblk_crypto.a

$(LIB_NAME): $(OBJ_FILES)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_FILES) $(LIBNAME) *-test

clean_obj:
	rm -f $(OBJ_FILES)

sha256:
	$(CC) $(CFLAGS) -o sha256-test test/sha256-main.c provided/_print_hex_buffer.c sha256.c -lssl -lcrypto

ec_create:
	$(CC) $(CFLAGS) -o ec_create-test test/ec_create-main.c ec_create.c -lssl -lcrypto

ec_to_pub:
	$(CC) $(CFLAGS) -o $@-test test/$@-main.c provided/_print_hex_buffer.c ec_to_pub.c ec_create.c -lssl -lcrypto

ec_from_pub:
	$(CC) $(CFLAGS) -o $@-test test/$@-main.c provided/_print_hex_buffer.c ec_from_pub.c ec_to_pub.c ec_create.c -lssl -lcrypto

ec_save:
	$(CC) $(CFLAGS) -o $@-test test/$@-main.c ec_save.c ec_create.c ec_to_pub.c provided/_print_hex_buffer.c -lssl -lcrypto

ec_load:
	$(CC) $(CFLAGS) -o $@-test test/$@-main.c ec_load.c ec_save.c ec_create.c ec_to_pub.c provided/_print_hex_buffer.c -lssl -lcrypto

ec_sign:
	$(CC) $(CFLAGS) -o $@-test test/$@-main.c provided/_print_hex_buffer.c ec_sign.c ec_create.c -lssl -lcrypto

ec_verify:
	$(CC) $(CFLAGS) -o $@-test test/$@-main.c provided/_print_hex_buffer.c ec_verify.c ec_sign.c ec_create.c -lssl -lcrypto

