CC = gcc
FLAGS = -Wall

all: test

compile:
	$(CC) $(FLAGS) main.c -o main

test: compile
	./main 4

clean:
	rm main