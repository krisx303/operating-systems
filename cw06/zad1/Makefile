all: compile

compile: server.c client.c
	gcc server.c -o server
	gcc client.c -o client

server: server.c
	gcc server.c -o server
	./server

client: client.c
	gcc client.c -o client

clean:
	rm server client log.log
