# Guide to develop with Crypto++
## Install Crypto++ on Linux
According to the [official documentation](https://www.cryptopp.com/wiki/Linux), we can use the official respositories through *apt* (or other distribution package manager), but it didn't work for me (KDE neon), and also the apt version is 6 and the latest version of crypto++ is 8.2 (25-04-2020).

So, I install crypto++ through manual installation ([official guide here](https://www.cryptopp.com/wiki/Linux#Build_and_Install_the_Library))
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

## Programing with Crypto++
> *Warning*: The executables are too big (the following example 24mb!), don't commit them 

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
g++ -I/usr/local/include test.cpp /usr/local/lib/libcryptopp.a -o test
```
And run it:
```bash
./test
# Possible output:
x = ac469320f16f659ah
y = 76661cb77ff066fh
x*y = 4fad3f43030d1c99248c2401614a9c6h
```
Happy coding! :blush:

More information in [wiki article](https://www.cryptopp.com/wiki/Linux).