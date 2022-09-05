main:
	gcc command.c -o c -Wall -lpthread
	gcc barberShop.c -o b -Wall -lpthread

make clean:
	rm -rf b c
