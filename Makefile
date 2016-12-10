CC = $(CXX)
CXXFLAGS += -O3 -std=c++11
CXXFLAGS += -Itclap/include
CXXFLAGS += $(shell sdl2-config --cflags)
LDFLAGS += $(shell sdl2-config --libs)

.PHONY: all clean

sound-of-sorting: main.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

all: sound-of-sorting test

main.o: main.cpp sorts.hpp sorts/* tclap/include/tclap

test: test.o

test.o: test.cpp sorts.hpp sorts/*

clean:
	$(RM) main.o sound-of-sorting test.o test
	$(RM) -r tclap

tclap/include/tclap:
	git clone https://github.com/mirror/tclap.git
