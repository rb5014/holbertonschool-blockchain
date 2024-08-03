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

clean:
	rm -f *-test ./transaction/*-test