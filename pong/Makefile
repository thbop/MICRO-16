ASM = ../build/b6as
LD = ../build/b6ld
BUILD_DIR = ../build/pong

all: pong

pong:
	mkdir -p $(BUILD_DIR)
	$(ASM) pong.asm -o $(BUILD_DIR)/pong.o
	$(LD) $(BUILD_DIR)/pong.o -o $(BUILD_DIR)/pong.cart