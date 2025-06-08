# pikernel

A small ARM64 kernel example.

## Building

Install an ARM64 cross compiler (e.g. `aarch64-none-elf-gcc`) and run:

```
make
```

This produces `kernel8.img`.

## Running with QEMU

Launch the image with:

```
qemu-system-aarch64 -M virt -cpu cortex-a72 -m 512M -device ramfb -display sdl -kernel kernel8.img
```
