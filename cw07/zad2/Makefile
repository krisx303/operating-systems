
all: main

main: main.c barber.c
	gcc barber.c -lrt -lpthread -o barber
	gcc client.c -lrt -lpthread -o client
	gcc main.c -lrt -lpthread -o main
	./main

clean:
	rm main barber client