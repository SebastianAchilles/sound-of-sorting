CC = $(CXX)
CXXFLAGS += -O3 -std=c++11
CXXFLAGS += -Itclap/include
CXXFLAGS += $(shell sdl2-config --cflags)
LDFLAGS += $(shell sdl2-config --libs)

.PHONY: all clean

all: sound-of-sorting

sound-of-sorting: main.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

main.o: main.cpp sorts.hpp sorts/* tclap/include/tclap

clean:
	$(RM) main.o sound-of-sorting
	$(RM) -r tclap

tclap/include/tclap:
	git clone https://github.com/mirror/tclap.git
