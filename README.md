# OpenSSL Cryptographic Functions

This project contains several fundamental cryptographic functions and structures implemented using the OpenSSL library. The main focus is on hashing, elliptic curve cryptography, and key management, essential components in the field of blockchain technology.

## Project Overview

The purpose of this project is to provide implementations of key cryptographic operations, leveraging the OpenSSL library in C. These operations include SHA256 hashing, elliptic curve key pair generation, public key extraction, and key file management.

## Files and Their Purposes

### 1. `sha256.c`
- **Purpose**: Implements a function to compute the SHA256 hash of a given sequence of bytes.
- **Function**: `uint8_t *sha256(const int8_t *s, size_t len, uint8_t digest[SHA256_DIGEST_LENGTH])`
  - **Parameters**:
    - `s`: Input string to hash.
    - `len`: Length of the input string.
    - `digest`: Buffer to store the resulting hash.

### 2. `ec_create.c`
- **Purpose**: Creates a new EC key pair using the secp256k1 elliptic curve.
- **Function**: `EC_KEY *ec_create(void)`
  - **Returns**: A pointer to the created EC_KEY structure.

### 3. `ec_to_pub.c`
- **Purpose**: Extracts the public key from an EC_KEY structure.
- **Function**: `uint8_t *ec_to_pub(EC_KEY *key, uint8_t pub[EC_PUB_LEN])`
  - **Parameters**:
    - `key`: Pointer to the EC_KEY structure.
    - `pub`: Buffer to store the extracted public key.

### 4. `ec_from_pub.c`
- **Purpose**: Creates an EC_KEY structure from a given public key.
- **Function**: `EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])`
  - **Parameters**:
    - `pub`: Public key used to create the EC_KEY structure.

### 5. `ec_save.c`
- **Purpose**: Saves an EC key pair to a file in the PEM format.
- **Function**: `int ec_save(EC_KEY *key, char const *folder, char const *file)`
  - **Parameters**:
    - `key`: Pointer to the EC_KEY structure to save.
    - `folder`: Directory to save the key files.
    - `file`: Filename to save the key files.

### 6. `ec_load.c`
- **Purpose**: Loads an EC key pair from files in the PEM format.
- **Function**: `EC_KEY *ec_load(char const *folder, char const *file)`
  - **Parameters**:
    - `folder`: Directory containing the key files.
    - `file`: Filename of the key files to load.

### 7. `ec_sign.c`
- **Purpose**: Signs a given set of bytes using a private EC key.
- **Function**: `uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen, sig_t *sig)`
  - **Parameters**:
    - `key`: Pointer to the EC_KEY structure.
    - `msg`: Message to sign.
    - `msglen`: Length of the message.
    - `sig`: Signature structure to store the result.

### 8. `ec_verify.c`
- **Purpose**: Verifies a signature against a given set of bytes using a public EC key.
- **Function**: `int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen, sig_t const *sig)`
  - **Parameters**:
    - `key`: Pointer to the EC_KEY structure.
    - `msg`: Message to verify.
    - `msglen`: Length of the message.
    - `sig`: Signature to verify.

### 9. `hblk_crypto.h`
- **Purpose**: Header file defining shared constants and function prototypes for the project.

## Getting Started

### Prerequisites

- OpenSSL Library
- C Compiler (e.g., GCC)

### Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/rb5014/holbertonschool-blockchain.git
    cd crypto
    ```

2. Install OpenSSL (if not already installed):
    ```bash
    sudo apt-get install libssl-dev
    ```

### Building the Project

1. Compile the code:
    ```bash
make sha256
make ec_create
make ec_to_pub
make ec_from_pub
make ec_save
make ec_load
make ec_sign
make ec_verify
    ```

### Usage

1. **SHA256 Hashing**
    ```bash
    ./sha256-test <input_string>
    ```

2. **Elliptic Curve Key Creation**
    ```bash
    ./ec_create-test
    ```

3. **Extract Public Key from EC_KEY**
    ```bash
    ./ec_to_pub-test <private_key_file>
    ```

4. **Create EC_KEY from Public Key**
    ```bash
    ./ec_from_pub-test <public_key_file>
    ```

5. **Save EC_KEY to File**
    ```bash
    ./ec_save-test <private_key_file> <public_key_file>
    ```

6. **Load EC_KEY from File**
    ```bash
    ./ec_load-test <private_key_file> <public_key_file>
    ```

7. **Sign a Message**
    ```bash
    ./ec_sign-test <private_key_file> <message>
    ```

8. **Verify a Signature**
    ```bash
    ./ec_verify-test <public_key_file> <message> <signature>
    ```

