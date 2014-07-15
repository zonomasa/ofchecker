all:
	gcc -fPIC -c ofchecker.c -o ofchecker.o
	gcc -shared -Wl,-soname,libofchecker.so -o libofchecker.so ofchecker.o -ldl
