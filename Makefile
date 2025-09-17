LIB_NAME = csvparse
TEST_RUNNER_NAME = test

CC = clang
AR = ar

.PHONY: all header-only test clean

ifeq ($(OS),Windows_NT)
	PLATFORM_OS = WINDOWS
	ifndef PLATFORM_SHELL
		CC = x86_64-w64-mingw32-gcc
	endif
endif

all:
	$(CC) -c src/$(LIB_NAME).c -o $(LIB_NAME).o
	$(AR) rcs lib$(LIB_NAME).a $(LIB_NAME).o

header-only:
	-rm csvparse.h
	touch csvparse.h
	touch .buffer
	cat src/csvparse.h >> csvparse.h
	echo "" >> csvparse.h
	echo "#ifdef CSVPARSE_IMPLEMENTATION" >> csvparse.h
	cat src/csvparse.c >> .buffer
	sed -i '1d' .buffer
	cat .buffer >> csvparse.h
	rm .buffer
	echo "#endif" >> csvparse.h

test:
	$(CC) tests/$(TEST_RUNNER_NAME).c -o $(TEST_RUNNER_NAME) -I./src -L./ -lcsvparse -fsanitize=address

clean:
	rm lib$(LIB_NAME).a $(LIB_NAME).o $(TEST_RUNNER_NAME) csvparse.h
