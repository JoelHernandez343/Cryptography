C       = g++
CFLAGS  = -std=c++17
headers = ../include/cxxopts.hpp ../include/json.hpp ../include/utf8.hpp

vigenere: $(headers) src/vigenere.hpp src/vigenere.cpp
	$(C) src/vigenere.cpp -o build/vigenere $(CFLAGS)