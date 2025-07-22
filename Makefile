CC = g++
CFLAGS = -g -O2 -Wall -fdiagnostics-color=always -Isrc -Iinclude
LDFLAGS = -Llib -lSDL3 -lSDL3_image


SOURCES = Micro16.cpp $(wildcard src/**/*.cpp)


all: micro16

micro16:
	mkdir -p build
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o build/Micro16.exe
