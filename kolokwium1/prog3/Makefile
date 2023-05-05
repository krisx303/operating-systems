CFLAGS = -Wall
CC = gcc

main: main.o

clean:
	rm -f main.o main

test:  main
	@echo -e "\n\nTESTING\n\n"
	./main A 12
	./main B 11
	./main C 10
	./main D 9
all: main test
