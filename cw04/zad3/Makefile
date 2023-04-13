CC = gcc
FLAGS = -Wall
CCF = $(CC) $(FLAGS)


# main version, without waiting for run command but signals are being lost often
catcher: catcher.c
	$(CCF) catcher.c -o catcher
	./catcher

# optional command (this version of catcher, waits for run command and then send signal back to sender)
catcher_wait: catcher_wait.c
	$(CCF) catcher_wait.c -o catcher
	./catcher

sender: sender.c
	$(CCF) sender.c -o sender

clean:
	rm sender catcher