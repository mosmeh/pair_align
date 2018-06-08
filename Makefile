.PHONY: all

all: pair_align

clean:
	rm -f pair_align

pair_align:
	g++ -std=c++11 pair_align.cpp -o $@
