all:lib test run


lib:
	gcc -g -fPIC -c ofchecker.c -o ofchecker.o 
	gcc -shared -Wl,-soname,libofchecker.so -o libofchecker.so ofchecker.o -ldl -rdynamic -g


test:
	gcc -g -Wall test.c


run:
	LD_PRELOAD=./libofchecker.so ./a.out

clean:
	rm *.o *.so a.out
