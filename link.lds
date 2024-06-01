ENTRY(start)
SECTIONS
{
    . = 0x80000;

    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    . = ALIGN(16);
    .data : {
        *(.data)
    }

    .bss : {
        __bss_start__ = .;
        *(.bss)
        __bss_end__ = .;
    }

    .vectors : {
        *(.vectors)
    }
}
