C       = g++
CFLAGS  = -std=c++17
headers = ../include/cxxopts.hpp ../include/json.hpp ../include/utf8.hpp

sdes: src/bitperm.hpp src/sdes.hpp src/user.cpp
	$(C) src/user.cpp -o build/sdes $(CFLAGS)