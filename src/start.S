.global _start

.section .text
_start:
    ldr x0, =stack_top
    mov sp, x0
    bl kernel_main
1:
    wfe
    b 1b

.section .bss
    .align 12
stack:
    .skip 4096
stack_top:
