CC = gcc
FLAGS = -Wall
CCF = $(CC) $(FLAGS)

all: compile

compile: main.c
	$(CCF) main.c -o main

test: compile
	./main nadawca
	./main data
	./main "Hello" kris@example.org "Message"

info:
	@echo "W przypadku podania argumentu 'data' program nie wykonuje sortowania ponieważ mail domyślnie wyświetla meile posortowane po dacie otrzymania."

clean:
	rm main