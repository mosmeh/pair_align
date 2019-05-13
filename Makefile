.PHONY: all

all: pair_align

clean:
	rm -f pair_align

pair_align: pair_align.cpp
	g++ -std=c++11 $^ -o $@
