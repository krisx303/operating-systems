CC = gcc
FLAGS = -Wall
LIB_NAME = command
STATIC_LIB = lib$(LIB_NAME).a

all: test

library: $(LIB_NAME).c $(LIB_NAME).h
	$(CC) $(FLAGS) -c $(LIB_NAME).c -o $(LIB_NAME).o
	ar rcs $(STATIC_LIB) $(LIB_NAME).o

compile: library main.c main_exec.c
	$(CC) $(FLAGS) -c main_exec.c
	$(CC) $(FLAGS) main_exec.o $(STATIC_LIB) -o main_exec
	$(CC) $(FLAGS) -c main.c
	$(CC) $(FLAGS) main.o $(STATIC_LIB) -o main


test: compile
	@echo "\033[35m\nTest 'ignore' running:\033[0m"
	./main ignore
	@echo "\033[35m\nTest 'handler' running:\033[0m"
	./main handler
	@echo "\033[35m\nTest 'mask' running:\033[0m"
	./main mask
	@echo "\033[35m\nTest 'pending' running:\033[0m"
	./main pending

clean:
	rm main.o main_exec.o $(STATIC_LIB) $(LIB_NAME).o main main_exec