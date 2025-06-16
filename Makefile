CC = g++
CFLAGS = -g -Wall -fdiagnostics-color=always -Isrc

all: micro16

micro16:
	$(CC) $(CFLAGS) Micro16.cpp -o Micro16.exe