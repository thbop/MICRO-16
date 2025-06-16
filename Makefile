CC = g++
CFLAGS = -g -Wall -fdiagnostics-color=always -Isrc -Iinclude
LDFLAGS = -Llib -lSDL3

all: micro16

micro16:
	mkdir -p build
	$(CC) $(CFLAGS) Micro16.cpp $(LDFLAGS) -o build/Micro16.exe