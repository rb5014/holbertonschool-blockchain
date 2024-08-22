all:
	gcc -g -Wall -Wextra -pedantic -o cli-test -Icli -Iblockchain/v0.3 -Iblockchain/v0.3/transaction -Icrypto cli/*.c -Lblockchain/v0.3 -Lcrypto -lhblk_blockchain -lhblk_crypto -lllist -lssl -lcrypto -lreadline

clean:
	rm -f cli-test