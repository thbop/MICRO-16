CC = g++
CFLAGS = -g -Wall -fdiagnostics-color=always -Isrc -Iinclude
LDFLAGS = -Llib -lSDL3
SOURCES = Micro16.cpp $(wildcard src/**/*.cpp)

.PHONY: micro16 btp6kasm

all: micro16 btp6kasm

micro16:
	mkdir -p build
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o build/Micro16.exe

btp6kasm:
	mkdir -p build
	$(CC) $(CFLAGS) btp6kasm/btp6kasm.cpp -o build/btp6kasm.exe