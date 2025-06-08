.global _start

.section .text
_start:
    mov x1, x0            // preserve FDT pointer from QEMU
    ldr x0, =stack_top
    mov sp, x0
    mov x0, x1            // pass FDT pointer to kernel_main
    bl kernel_main
1:
    wfe
    b 1b

.section .bss
    .align 12
stack:
    .skip 4096
stack_top:
