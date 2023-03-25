CC = gcc
FLAGS = -Wall

all: test

compile:
	$(CC) $(FLAGS) main.c -o main

test: compile
	@echo "Test few files"
	./main .
	@echo "Test a lot of files"
	./main ../..

clean:
	rm main