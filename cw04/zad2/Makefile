CC = gcc
FLAGS = -Wall
CCF = $(CC) $(FLAGS)
USE_FLAG = 0

all: test

sa_siginfo:
	@echo "\n\033[35mTesting SA_SIGINFO flag\033[0m\n"
	$(CCF) flag_info.c -o main
	./main

sa_restart:
	@echo "\n\033[35mTesting SA_RESTART flag\033[0m\n"
	$(CCF) flag_restart.c -o main -D USE_FLAG=$(USE_FLAG)
	./main

sa_resethand:
	@echo "\n\033[35mTesting SA_RESETHAND flag\033[0m\n"
	$(CCF) flag_handreset.c -o main
	./main

test:
	@make sa_siginfo --silent
	@make sa_restart --silent
	@make sa_restart USE_FLAG=1 --silent
	-@make sa_resethand --silent

clean:
	rm main