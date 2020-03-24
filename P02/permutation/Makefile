 
C       = g++
CFLAGS  = -std=c++17
headers = ../include/cxxopts.hpp ../include/json.hpp ../include/utf8.hpp

sdes: $(headers) src/permutation.hpp src/permutation.cpp
	$(C) src/permutation.cpp -o build/permutation $(CFLAGS)