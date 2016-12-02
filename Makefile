CC = $(CXX)
CXXFLAGS += -g

.PHONY: all clean

all: sound-of-sorting

main.o: sorts.hpp sorts/*

sound-of-sorting: main.o
	$(CXX) -o $@ $<

clean:
	$(RM) main.o sound-of-sorting
