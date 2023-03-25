CC = gcc
FLAGS = -Wall

LIB_NAME = lib
LIB_VERSION = 1
LIB_RELEASE = 0.1
STATIC_LIB = lib$(LIB_NAME).a
SHARED_LIB = lib$(LIB_NAME).so
SHARED_LIB_VERSION = $(SHARED_LIB).$(LIB_VERSION)
SHARED_LIB_VERSION_RELEASE = $(SHARED_LIB_VERSION).$(LIB_RELEASE)


all: static_lib shared_lib

static_lib: lib.c lib.h
	$(CC) $(FLAGS) -c $(LIB_NAME).c -o $(LIB_NAME).o
	ar rcs $(STATIC_LIB) $(LIB_NAME).o

shared_lib: lib.c lib.h
	$(CC) $(FLAGS) -c -fpic lib.c -o $(LIB_NAME).o
	$(CC) $(FLAGS) -shared -Wl,-soname,$(SHARED_LIB_VERSION) -o $(SHARED_LIB_VERSION_RELEASE) $(LIB_NAME).o -lc
	ln -s $(SHARED_LIB_VERSION_RELEASE) $(SHARED_LIB_VERSION)
	ln -s $(SHARED_LIB_VERSION) $(SHARED_LIB)


clean:
	rm $(SHARED_LIB) $(SHARED_LIB_VERSION) $(SHARED_LIB_VERSION_RELEASE) $(STATIC_LIB) $(LIB_NAME).o