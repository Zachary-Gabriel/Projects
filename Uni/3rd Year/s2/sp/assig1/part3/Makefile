run: main.o dltest.o libmalloc.so
	gcc -o dltest dltest.o -ldl
	./dltest
	gcc -o heap main.o ./libmalloc.so
	LD_PRELOAD=./libmalloc.so ./heap

dltest.o: dltest.c
	gcc -c dltest.c

main.o: main.c heapstats.h
	gcc -c main.c

libmalloc.so: libmalloc.c heapstats.h
	gcc -fPIC -shared -o libmalloc.so libmalloc.c

clean:
	rm *.o *.so heap dltest