CROSS_COMPILE ?= aarch64-linux-gnu-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
CFLAGS = -ffreestanding -nostdlib -nostartfiles -Wall -Wextra
LDFLAGS = -T src/kernel.ld

OBJ = src/start.o src/main.o

all: kernel8.img

run: kernel8.img
	qemu-system-aarch64 -M virt -cpu cortex-a72 -m 512M \
	  -device ramfb -display sdl -kernel kernel8.img

kernel8.img: kernel8.elf
	$(OBJCOPY) -O binary $< $@

kernel8.elf: $(OBJ)
	$(LD) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

src/%.o: src/%.S
	$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJ) kernel8.elf kernel8.img

.PHONY: all clean
