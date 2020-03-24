# Practice number two
Implementing simplified des and permutation encryption.
## Author's Note
This version was made on March 24, 2020 for the Classroom presentation, if you want the updated version (possibly, more operation modes supported), please update the repo with:
```bash
git pull --rebase
```
### If you has made changes in the files, first reset the repository before updating
```bash
git reset --hard
```
## Requirements
- Linux operative system.
- GCC 8.1 compiler (or higher), or other compiler with C++17 features and `std::fylesystem` support. See this [instructions](#installing-gcc-92-on-ubuntu-and-variants-or-debian-base-linux) if you want to install GCC 9.2.

## Usage
Each folder program has its own `README.md` file with instructions and usage :blush:.
- [Permutation README.md](permutation/README.md)
- [SDES README.md](sdes/README.md)

## Installing GCC 9.2 on Ubuntu and variants or Debian base Linux
- Install GCC compile packages
```bash
sudo apt install build-essential
```
- Add the repository with GCC 9.2
```bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
```
- Install GCC 9.2 and G++ 9.2
```bash
sudo apt install gcc-9 g++-9
```
- And make it default ones:
```bash
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-9
```
### Just to check:
```bash
g++ --version
```
Possible output:
```bash
g++ (Ubuntu 9.2.1-17ubuntu1~18.04.1) 9.2.1 20191102
Copyright (C) 2019 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```