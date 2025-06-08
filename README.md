# pikernel

A small ARM64 kernel example.

## Building

Install an ARM64 cross compiler (e.g. `aarch64-linux-gnu-gcc`) and run:

```
make
```

This produces `kernel8.img`.

## Running with QEMU

The kernel expects the pointer to QEMU's device tree blob in register `x0` on
startup. It parses the tree to locate the `simple-framebuffer` node provided by
QEMU's `-device ramfb` option and uses that framebuffer for output.

Launch QEMU with:

```
make run
```

The `run` target already enables the RAM framebuffer device so the kernel can
display text immediately.

## Framebuffer setup

At boot QEMU passes a pointer to its device tree in `x0`. The kernel parses the tree in `src/main.c` to find the `simple-framebuffer` node. The `reg` property of that node provides the framebuffer address along with the display size.

This node is created only when QEMU is started with `-device ramfb`. The `make run` target includes this option so the kernel can locate the framebuffer and draw text.
