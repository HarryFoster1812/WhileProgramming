CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o) build/main.o

OUT = bin/main

.PHONY: all clean

all: $(OUT)

build:
	mkdir -p build

bin:
	mkdir -p bin

$(OUT): $(OBJ) | bin
	$(CC) $(OBJ) -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/main.o: main.c | build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/*.o $(OUT)
