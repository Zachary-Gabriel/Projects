.PHONY: all

all: loop-O0 loop-O1 loop-O3

loop-O0: main.cpp loop.cpp
	@echo "Compiling with no optimisation"
	g++ --std=c++11 -O0 -o loop-O0 $^
	g++ --std=c++11 -O0 -S loop.cpp
	mv loop.s loop-O0.s

loop-O1: main.cpp loop.cpp
	@echo "Compiling with level-1 optimisations"
	g++ --std=c++11 -O1 -o loop-O1 $^
	g++ --std=c++11 -O1 -S loop.cpp
	mv loop.s loop-O1.s

loop-O3: main.cpp loop.cpp
	@echo "Compiling with level-3 optimisations"
	g++ --std=c++11 -O3 -o loop-O3 $^
	g++ --std=c++11 -O3 -S loop.cpp
	mv loop.s loop-O3.s

clean:
	rm -f loop-O? loop-O?.s
