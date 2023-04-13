CC = gcc
FLAGS = -Wall
CCF = $(CC) $(FLAGS)


all: compile


compile: main.c
	$(CCF) main.c -o main


test: compile
	-@rm report.txt
	@echo "\nSimple tests\n"
	./main 0.001 1
	./main 0.001 10
	@echo "\nComparing tests\n"
	./main 0.000000001 5
	./main 0.000000001 10
	./main 0.000000001 50
	./main 0.000000001 75
	./main 0.000000001 100

clean:
	rm main report.txt