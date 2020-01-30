
all:
	gcc -g3 -O0 *.c -I.  -o test.out

clean:
	rm -rf *.o *.so *.a test.out
