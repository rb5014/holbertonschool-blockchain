all:
	gcc -g -std=c90 -Wall -Wextra -pedantic -o cli -I. -I../blockchain/v0.3 -I../blockchain/v0.3/transaction -I../crypto *.c -L../blockchain/v0.3 -L../crypto -lhblk_blockchain -lhblk_crypto -lllist -lssl -lcrypto -lreadline

clean:
	rm -f cli