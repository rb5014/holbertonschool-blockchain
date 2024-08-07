CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -I. -Itransaction/ 

SRC_FILES = $(wildcard *.c transaction/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)
LIB_NAME = libhblk_blockchain.a

$(LIB_NAME): $(OBJ_FILES)
	ar rcs $@ $^
	make clean_obj

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_FILES) $(LIBNAME) *-test 

clean_obj:
	rm -f $(OBJ_FILES)

tx_out_create: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/tx_out_create-test transaction/tx_out_create.c provided/_print_hex_buffer.c transaction/test/tx_out_create-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

unspent_tx_out_create: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/unspent_tx_out_create-test transaction/tx_out_create.c transaction/unspent_tx_out_create.c provided/_print_hex_buffer.c transaction/test/unspent_tx_out_create-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

tx_in_create: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/tx_in_create-test transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c provided/_print_hex_buffer.c transaction/test/tx_in_create-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

transaction_hash: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/transaction_hash-test transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c provided/_print_hex_buffer.c transaction/test/transaction_hash-main.c provided/_transaction_print.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

tx_in_sign: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/tx_in_sign-test transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c provided/_print_hex_buffer.c transaction/test/tx_in_sign-main.c provided/_transaction_print.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

transaction_create: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/transaction_create-test transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c provided/_print_hex_buffer.c provided/_transaction_print.c transaction/test/transaction_create-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

transaction_is_valid: clean
	gcc -g -std=c90 -Wall -Wextra  -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/transaction_is_valid-test transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c transaction/transaction_is_valid.c provided/_print_hex_buffer.c transaction/test/transaction_is_valid-main.c provided/_transaction_print.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

coinbase_create: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/coinbase_create-test transaction/tx_out_create.c transaction/transaction_hash.c transaction/coinbase_create.c provided/_print_hex_buffer.c transaction/test/coinbase_create-main.c provided/_transaction_print.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

coinbase_is_valid: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/coinbase_is_valid-test transaction/tx_out_create.c transaction/transaction_hash.c transaction/coinbase_create.c transaction/coinbase_is_valid.c provided/_print_hex_buffer.c transaction/test/coinbase_is_valid-main.c provided/_transaction_print.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

transaction_destroy: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/transaction_destroy-test transaction/tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c transaction/coinbase_create.c transaction/transaction_destroy.c transaction/test/transaction_destroy-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

block_create_destroy: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o block_create_destroy-test *.c test/*.c provided/*.c transaction/transaction_destroy.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

block_hash: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o block_hash-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c transaction/tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/coinbase_create.c transaction/transaction_destroy.c provided/_genesis.c provided/_print_hex_buffer.c provided/_blockchain_print.c provided/_transaction_print.c provided/_transaction_print_brief.c test/block_hash-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

block_is_valid: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o block_is_valid-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c block_is_valid.c hash_matches_difficulty.c blockchain_difficulty.c block_mine.c transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c transaction/transaction_is_valid.c transaction/coinbase_create.c transaction/coinbase_is_valid.c transaction/transaction_destroy.c provided/*.c test/block_is_valid-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

update_unspent: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/update_unspent-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c block_is_valid.c hash_matches_difficulty.c blockchain_difficulty.c block_mine.c transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c transaction/transaction_is_valid.c transaction/coinbase_create.c transaction/coinbase_is_valid.c transaction/transaction_destroy.c transaction/update_unspent.c provided/_genesis.c provided/_print_hex_buffer.c provided/_blockchain_print.c provided/_transaction_print.c provided/_transaction_print_brief.c transaction/test/update_unspent-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

blockchain_ser_deser: clean
	gcc -g -std=c90 -Wall -Wextra -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o blockchain_ser_deser-test test/blockchain_ser_deser.c *.c transaction/*.c provided/*.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
