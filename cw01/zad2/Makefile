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


all: static_lib shared_lib main_static main_shared main_dynamic

static_lib: lib.c lib.h
	$(CC) $(FLAGS) -c $(LIB_NAME).c -o $(LIB_NAME).o
	ar rcs $(STATIC_LIB) $(LIB_NAME).o

shared_lib: lib.c lib.h
	$(CC) $(FLAGS) -c -fpic lib.c -o $(LIB_NAME).o
	$(CC) $(FLAGS) -shared -Wl,-soname,$(SHARED_LIB_VERSION) -o $(SHARED_LIB_VERSION_RELEASE) $(LIB_NAME).o -lc
	ln -s $(SHARED_LIB_VERSION_RELEASE) $(SHARED_LIB_VERSION)
	ln -s $(SHARED_LIB_VERSION) $(SHARED_LIB)


main_static: main.c
	$(CC) $(FLAGS) -c main.c
	$(CC) $(FLAGS) main.o liblib.a -o $(MAIN_STATIC)

main_shared: main.c
	$(CC) $(FLAGS) main.c -llib -L. -Wl,-rpath=. -o $(MAIN_SHARED)


main_dynamic: main_dynamic.c
	$(CC) $(FLAGS) main_dynamic.c -o $(MAIN_DYNAMIC)


clean:
	rm main.o $(SHARED_LIB) $(SHARED_LIB_VERSION) $(SHARED_LIB_VERSION_RELEASE) $(STATIC_LIB) $(LIB_NAME).o $(MAIN_STATIC) $(MAIN_SHARED) $(MAIN_DYNAMIC)