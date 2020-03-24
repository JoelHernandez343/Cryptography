# Permutation encryption
Permutation encryption implementation.
## Build and compile
### With Makefile
Just execute make in order to compile the source code. The output files will be inside `/build`
```bash
make
```
### Manually
```bash
g++ ./src/permutation.cpp -o ./build/permutation -std=c++17
```
## Usage
Change to the `build` folder to a easier usage:
```bash
cd build/
```
This project has its own `Usage` Linux like, to view this:
```bash
./permutation --help
```
### Encrypting / Decrypting
> **NOTE**: Sample JSON file already exist

To encrypt, just use `--encrypt` option, with a [JSON configuration file](#json-file) as argument.
```bash
./permutation --encrypt [JSON file]
```
To decrypt, just use `--decrypt` option, with a [JSON configuration file](#json-file) as argument.
```bash
./permutation --decrypt [JSON file]
```
### View JSON file help on console
To view a JSON file example:
```bash
./permutation --json example
```
To view the JSON file description:
```bash
./permutation --json desc
```
To view all above information:
```bash
./permutation --json all
```
### JSON file
JSON file example:
```JSON
{
    "permutation": [3, 2, 1, 0],
    "encrypt": {
        "inputFile": "in.txt",
        "outputFile": "out.txt"
    },
    "decrypt": {
        "inputFile": "out.txt",
        "outputFile": "in.txt"
    }
}
```
**NOTE**: All the permutations must begin in 0.

`permutation`: An array with a valid permutation with inverse.

`encrypt` and `decrypt`: Define the files to work with.
- `inputFile`: Set the input file, path relative to execution environment.
- `outputFile`: Set the output file, path relative to execution environment.
