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

blockchain_create: clean
	gcc -g -std=c90 -Wall -Wextra -Werror -pedantic -I. -o blockchain_create-test blockchain_create.c test/blockchain_create-main.c provided/_genesis.c provided/_blockchain_print.c provided/_blockchain_destroy.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

block_create: clean
	gcc -g -std=c90 -Wall -Wextra -Werror -pedantic -I. -o block_create-test blockchain_create.c block_create.c test/block_create-main.c provided/_blockchain_print.c provided/_blockchain_destroy.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

block_destroy: clean
	gcc -g -std=c90 -Wall -Wextra -Werror -pedantic -I. -o block_destroy-test blockchain_create.c block_create.c block_destroy.c test/block_destroy-main.c provided/_blockchain_destroy.c  -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

blockchain_destroy: clean
	gcc -g -std=c90 -Wall -Wextra -Werror -pedantic -I. -o blockchain_destroy-test blockchain_destroy.c blockchain_create.c block_create.c block_destroy.c test/blockchain_destroy-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

block_hash: clean
	gcc -g -std=c90 -Wall -Wextra -Werror -pedantic -I. -o block_hash-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c test/block_hash-main.c  provided/_blockchain_print.c provided/_blockchain_destroy.c  -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

blockchain_serialize: clean
	gcc -g -std=c90 -Wall -Wextra -Werror -pedantic -I. -o blockchain_serialize-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c blockchain_serialize.c provided/_endianness.c test/blockchain_serialize-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

blockchain_deserialize: clean
	gcc -g -std=c90 -Wall -Wextra -Werror -pedantic -I. -o blockchain_deserialize-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c blockchain_deserialize.c provided/_blockchain_print.c provided/_endianness.c test/blockchain_deserialize-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

block_is_valid: clean
	gcc -g -std=c90 -Wall -Wextra -Werror -pedantic -I. -o block_is_valid-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c block_is_valid.c hash_matches_difficulty.c provided/_genesis.c test/block_is_valid-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

hash_matches_difficulty: clean
	gcc -g -std=c90 -Wall -Wextra -Werror -pedantic -I. -I../../crypto -o hash_matches_difficulty-test hash_matches_difficulty.c test/hash_matches_difficulty-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

block_mine: clean
	gcc -g -std=c90 -Wall -Wextra -Werror -pedantic -I. -I../../crypto -o block_mine-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c block_is_valid.c hash_matches_difficulty.c block_mine.c provided/_genesis.c provided/_blockchain_print.c test/block_mine-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

blockchain_difficulty: clean
	gcc -g -std=c90 -Wall -Wextra -Werror -pedantic -I. -I../../crypto -o blockchain_difficulty-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c blockchain_difficulty.c block_mine.c hash_matches_difficulty.c provided/_genesis.c provided/_blockchain_print.c test/blockchain_difficulty-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread


clean:
	rm -f *-test ./transaction/*-test