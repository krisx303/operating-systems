CC = gcc
FLAGS = -Wall

MAIN_STATIC = main_static
MAIN_SHARED = main_shared
MAIN_DYNAMIC = main_dynamic

LIB_NAME = lib
LIB_VERSION = 1
LIB_RELEASE = 0.1
STATIC_LIB = lib$(LIB_NAME).a
SHARED_LIB = lib$(LIB_NAME).so
SHARED_LIB_VERSION = $(SHARED_LIB).$(LIB_VERSION)
SHARED_LIB_VERSION_RELEASE = $(SHARED_LIB_VERSION).$(LIB_RELEASE)


all: 
	make test OPTIM_FLAG="-O"  OPTIM_SUFFIX="_O"
	make clean_after_test
	make test OPTIM_FLAG="-O0" OPTIM_SUFFIX="_O0"
	make clean_after_test
	make test OPTIM_FLAG="-O1" OPTIM_SUFFIX="_O1"
	make clean_after_test
	make test OPTIM_FLAG="-O2" OPTIM_SUFFIX="_O2"
	make clean_after_test
	make test OPTIM_FLAG="-O3" OPTIM_SUFFIX="_O3"
	make clean_after_test
	make test OPTIM_FLAG="-Os" OPTIM_SUFFIX="_Os"


static_lib: lib.c lib.h
	$(CC) $(FLAGS) $(OPTIM_FLAG) -c $(LIB_NAME).c -o $(LIB_NAME).o
	ar rcs $(STATIC_LIB) $(LIB_NAME).o

shared_lib: lib.c lib.h
	$(CC) $(FLAGS) $(OPTIM_FLAG) -c -fpic lib.c -o $(LIB_NAME).o
	$(CC) $(FLAGS) $(OPTIM_FLAG) -shared -Wl,-soname,$(SHARED_LIB_VERSION) -o $(SHARED_LIB_VERSION_RELEASE) $(LIB_NAME).o -lc
	ln -s $(SHARED_LIB_VERSION_RELEASE) $(SHARED_LIB_VERSION)
	ln -s $(SHARED_LIB_VERSION) $(SHARED_LIB)


main_static: main.c
	$(CC) $(FLAGS) $(OPTIM_FLAG) -c main.c
	$(CC) $(FLAGS) $(OPTIM_FLAG) main.o liblib.a -o $(MAIN_STATIC)

main_shared: main.c
	$(CC) $(FLAGS) $(OPTIM_FLAG) main.c -llib -L. -Wl,-rpath=. -o $(MAIN_SHARED)


main_dynamic: main_dynamic.c
	$(CC) $(FLAGS) $(OPTIM_FLAG) main_dynamic.c -o $(MAIN_DYNAMIC)

test: static_lib shared_lib main_static main_shared main_dynamic
	./$(MAIN_STATIC) < test1.test > results_static$(OPTIM_SUFFIX).txt
	./$(MAIN_SHARED) < test1.test > results_shared$(OPTIM_SUFFIX).txt
	./$(MAIN_DYNAMIC) < test1.test > results_dynamic$(OPTIM_SUFFIX).txt


clean_after_test:
	rm main.o $(SHARED_LIB) $(SHARED_LIB_VERSION) $(SHARED_LIB_VERSION_RELEASE) $(STATIC_LIB) $(LIB_NAME).o $(MAIN_STATIC) $(MAIN_SHARED) $(MAIN_DYNAMIC)

clean:
	rm results_* main.o $(SHARED_LIB) $(SHARED_LIB_VERSION) $(SHARED_LIB_VERSION_RELEASE) $(STATIC_LIB) $(LIB_NAME).o $(MAIN_STATIC) $(MAIN_SHARED) $(MAIN_DYNAMIC)