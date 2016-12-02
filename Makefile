CC = $(CXX)
CXXFLAGS += -O3 -std=c++11
CXXFLAGS += -Itclap/include
LDFLAGS += -std=c++11

.PHONY: all clean

all: sound-of-sorting

sound-of-sorting: main.o
	$(CXX) $(LDFLAGS) -o $@ $<

main.o: main.cpp sorts.hpp sorts/* tclap/include/tclap

clean:
	$(RM) main.o sound-of-sorting
	$(RM) -r tclap

tclap/include/tclap:
	git clone https://github.com/mirror/tclap.git
