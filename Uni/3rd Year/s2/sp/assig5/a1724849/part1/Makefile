run: main.o bn.so
	gcc -o bignum main.o ./libbn.so
	LD_PRELOAD=./libbn.so ./bignum

main.o: main.c bn.h
	gcc -c main.c

bn.so: bn.c bn.h
	gcc -fPIC -shared -o libbn.so bn.c

clean:
	rm *.o *.so bignum