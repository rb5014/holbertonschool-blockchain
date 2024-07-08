sha256:
	gcc -Wall -Wextra -Werror -pedantic  -Wno-deprecated-declarations -I. -o sha256-test test/sha256-main.c provided/_print_hex_buffer.c sha256.c -lssl -lcrypto

ec_create:
	gcc -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -I. -o ec_create-test test/ec_create-main.c ec_create.c -lssl -lcrypto

ec_to_pub:
	gcc -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -I. -o ec_to_pub-test test/ec_to_pub-main.c provided/_print_hex_buffer.c ec_to_pub.c ec_create.c -lssl -lcrypto

ec_from_pub:
	gcc -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -I. -o ec_from_pub-test test/ec_from_pub-main.c provided/_print_hex_buffer.c ec_from_pub.c ec_to_pub.c ec_create.c -lssl -lcrypto

ec_save:
	gcc -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -I. -o ec_save-test test/ec_save-main.c ec_save.c ec_create.c ec_to_pub.c provided/_print_hex_buffer.c -lssl -lcrypto

ec_load:
	gcc -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -I. -o ec_load-test test/ec_load-main.c ec_load.c ec_save.c ec_create.c ec_to_pub.c provided/_print_hex_buffer.c -lssl -lcrypto

ec_sign:
	gcc -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -I. -o ec_sign-test test/ec_sign-main.c provided/_print_hex_buffer.c ec_sign.c ec_create.c -lssl -lcrypto

ec_verify:
	gcc -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations -I. -o ec_verify-test test/ec_verify-main.c provided/_print_hex_buffer.c ec_verify.c ec_sign.c ec_create.c -lssl -lcrypto
