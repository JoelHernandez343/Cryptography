C       = g++
CFLAGS  = -std=c++17
headers = ../include/cxxopts.hpp ../include/json.hpp ../include/utf8.hpp

sdes: src/permutation.hpp src/sdes.hpp src/main.cpp
	$(C) src/main.cpp -o build/sdes $(CFLAGS)