CC = gcc
FLAGS = -Wall

all: test

compile:
	$(CC) $(FLAGS) main.c -o main

test: compile
	@echo "Test running:"
	./main .. '#include'
	@echo "Test running:"
	./main ../.. '#include'

test_all: compile
	@echo "Test running:"
	./main .. '#include'
	@echo "Test running:"
	./main ../.. '#include'
	@echo "Test running:"
	./main ~ '#include'
	@echo "Test running:"
	./main /root '#include'

test_wait_fc:
	$(CC) $(FLAGS) main.c -o main -D WAIT_FOR_CHILDREN
	./main .. '#include'
	./main ../.. '#include'
	./main ~ '#include'
	./main /root '#include'


clean:
	rm main