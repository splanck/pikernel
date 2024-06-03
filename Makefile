# Variables
CROSS_COMPILER_PREFIX = aarch64-linux-gnu
ASSEMBLY_FILE = boot.s
C_FILES = main.c uart.c gpio.c timer.c framebuffer.c
OBJECTS = boot.o $(C_FILES:.c=.o)
LINKER_SCRIPT = link.lds
OUTPUT_ELF = kernel
OUTPUT_BINARY = kernel8.img
QEMU_MACHINE = raspi3b

# Default target
all: $(OUTPUT_BINARY)

# Compile the assembly file
boot.o: $(ASSEMBLY_FILE)
	$(CROSS_COMPILER_PREFIX)-gcc -c $< -o $@

# Compile the C files
%.o: %.c
	$(CROSS_COMPILER_PREFIX)-gcc -std=c99 -ffreestanding -mgeneral-regs-only -c $< -o $@

# Link the object files
$(OUTPUT_ELF): $(OBJECTS)
	$(CROSS_COMPILER_PREFIX)-ld -nostdlib -T $(LINKER_SCRIPT) -o $@ $^

# Create the binary image
$(OUTPUT_BINARY): $(OUTPUT_ELF)
	$(CROSS_COMPILER_PREFIX)-objcopy -O binary $< $@

# Run the kernel in QEMU
run: $(OUTPUT_BINARY)
	qemu-system-aarch64 -M $(QEMU_MACHINE) -kernel $< -serial stdio

# Clean build files
clean:
	rm -f *.o $(OUTPUT_ELF) $(OUTPUT_BINARY)

# Phony targets
.PHONY: all clean run
