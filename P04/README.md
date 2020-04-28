# Practice 4 | Secret-key cryptography
## Introduction
In this session we will start using a cryptographic library of your choice. Please do the following
programming exercises on your own. Use only one programming language (C/C++, Java, Python).
It is recommended that you try to solve the exercises on your own. You can discuss the solution to
the exercises with your colleagues but you should not copy source code. If copying is detected, that
may immediately lead to a grade less than 6.
## Programming exercises
1. Choose a cryptographic library for one of the programming languages mentioned above.
2. Find how to do the following and test it.
    - [x] Use a cryptographically secure pseudorandom generator.
    - [x] Key generation for secret-key cryptography.
    - [x] Encryption and decryption using a stream cipher. Find out which are the stream ciphers available in the cryptographic library of your choice.
    - [x] Encryption and decryption for at least three block ciphers (3DES, AES and another one).
    - [x] Use of modes of operation. Prove all the traditional modes of operation we studied in class: ECB, CBC, CTR, OFB, CFB.
    - [x] Encryption and decryption combining a block cipher and each mode of operation. Use files of different sizes (start at 100kb) to prove this point.
## Crypto++
[Crypto++](https://www.cryptopp.com/) (also known as CryptoPP, libcrypto++, and libcryptopp) is a free and open-source C++ class library of cryptographic algorithms and schemes written by Wei Dai.
## Installation
### Linux :penguin:
Please follow the instructions in [GUIDE.md](GUIDE.md#install-crypto-on-linux) or the [official documentation](https://www.cryptopp.com/wiki/Linux).
### Windows :bug:
I am investigating :c
## Usage
### Client programs on linux
Please follow the instructions in [GUIDE.md](GUIDE.md#programming-with-crypto-on-linux).
### Client programs on Windows
Nothing to see here...

Happy coding! :blush: