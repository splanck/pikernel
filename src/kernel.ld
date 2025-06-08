ENTRY(_start)
OUTPUT_FORMAT("elf64-littleaarch64")

SECTIONS
{
    . = 0x40080000;

    .text : {
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }
}
