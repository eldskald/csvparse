LIB_NAME = csvparse

CC = clang
AR = ar

.PHONY: all test clean

ifeq ($(OS),Windows_NT)
	PLATFORM_OS = WINDOWS
	ifndef PLATFORM_SHELL
		CC = x86_64-w64-mingw32-gcc
	endif
endif

all:
	$(CC) -c src/$(LIB_NAME).c -o $(LIB_NAME).o
	$(AR) rcs lib$(LIB_NAME).a $(LIB_NAME).o

test:
	$(CC) tests/test.c -o test -I./src -L./ -lcsvparse

clean:
	rm lib$(LIB_NAME).a $(LIB_NAME).o test
