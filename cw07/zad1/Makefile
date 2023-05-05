



all: main

main: main.c barber.c
	gcc barber.c -o barber
	gcc client.c -o client
	gcc main.c -o main
	./main

clean:
	rm main barber client