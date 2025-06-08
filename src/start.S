.global _start

.section .text
_start:
    mov x1, x0            // preserve FDT pointer from QEMU
    ldr x0, =stack_top
    mov sp, x0

    ldr x2, =__bss_start
    ldr x3, =__bss_end
1:
    cmp x2, x3
    b.hs 2f
    str xzr, [x2], #8
    b   1b
2:

    mov x0, x1            // pass FDT pointer to kernel_main
    bl kernel_main
3:
    wfe
    b 3b

.section .bss
    .align 12
stack:
    .skip 4096
stack_top:
