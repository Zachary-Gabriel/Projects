run: spalloc.o driver.c spalloc.h
	cc -o driver driver.c spalloc.o 
	./driver

spalloc.o: spalloc.c
	cc -c spalloc.c -o spalloc.o

proper: driver.c spalloc.h spalloc_debug.o
	cc -o driver driver.c spalloc_debug.o
	./driver

spalloc_debug.o: spalloc.c
	cc -DSP_FAKE -c spalloc.c -o spalloc_debug.o

clean:
	rm *.o driver