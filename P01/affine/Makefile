C       = g++
CFLAGS  = -std=c++17
headers = ../include/cxxopts.hpp ../include/json.hpp ../include/utf8.hpp

affine: $(headers) src/affine.hpp src/affine.cpp
	$(C) src/affine.cpp -o build/affine $(CFLAGS)