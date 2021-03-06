# Guide to develop with Crypto++
## Install Crypto++ on Linux
According to the [official documentation](https://www.cryptopp.com/wiki/Linux), we can use the official repositories through *apt* (or other distribution package manager), but it didn't work for me (KDE neon), and also the apt version is 6 and the latest version of crypto++ is 8.2 (25-04-2020).

So, I install crypto++ through the manual installation ([official guide here](https://www.cryptopp.com/wiki/Linux#Build_and_Install_the_Library))
### Manual installation
1. Download latest or the desired version from [here](https://www.cryptopp.com/#download) and / or
2. Create a subfolder where unzip the zip folder (optional / recommended). e.g `cryptopp`:
```bash
mkdir cryptopp
cd criptopp/

# e.g. download from terminal version 8.2
wget https://www.cryptopp.com/cryptopp820.zip
unzip -a cryptopp820.zip
```
3. Issue `make` to build the static library and cryptest program
```bash
make static dynamic cryptest.exe
``` 
4. After the library successfully builds, run `cryptest.exe v` and `cryptest.exe tv` all to validate the library
```bash
./cryptest.exe v
./cryptest.exe tv
```
5. Tell to `make` to build the static and dynamic libraries
```bash
make libcryptopp.a libcryptopp.so cryptest.exe
```
6. Finally, install the library. By default, Crypto++ installs to `/usr/local`, so you will root to install components
```bash
sudo make install PREFIX=/usr/local
```
## Install Crypto++ on Windows
I don't know how to for now :disappointed:

## Programming with Crypto++ on Linux

If you installed successfully Crypto++ on your Linux SO, you must to be enable to compile and run the following program:

```c++
// test.cpp
// Generate two 64 bits random integers and multiplies them.
#include <cryptopp/cryptlib.h>
#include <cryptopp/osrng.h>
#include <cryptopp/integer.h>
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[])
{

    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::Integer x(prng, 64), y(prng, 64);

    std::cout << "x = " << std::hex << x << std::endl;
    std::cout << "y = " << std::hex << y << std::endl;
    std::cout << "x*y = " << std::hex << x*y << std::endl;

    return 0;
}
```
And compile it with `Static Linking`:
```bash
g++ test.cpp -o test -s -l:libcryptopp.a   
```
`-o`: Indicate the output file.

`-s`: Quit all debug information from the linker.

`-l`: Static link with the library.

> *Warning*: The executables are too big with debug information! -s is important if you don't want to debug / you are in production.

And run it:
```bash
./test
# Possible output:
x = 37c1b5e2853743e3h
y = 88b2cabbb1bc87a0h
x*y = 1dc5d9788900949fe736e12a350b22e0h
```
Happy coding! :blush:

More information in [wiki article](https://www.cryptopp.com/wiki/Linux).