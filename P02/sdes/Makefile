C       = g++
CFLAGS  = -std=c++17
headers = ../include/cxxopts.hpp ../include/json.hpp

sdes: $(headers) src/bitperm.hpp src/sdes.hpp src/sdes.cpp
	$(C) src/sdes.cpp -o build/sdes $(CFLAGS)