#include "uart.h"

#define UART0_BASE 0x3F201000
volatile unsigned int * const UART0DR = (unsigned int *)(UART0_BASE + 0x00);
volatile unsigned int * const UART0FR = (unsigned int *)(UART0_BASE + 0x18);

void uart_init(void) {
    // UART initialization logic (stub, actual implementation may vary)
}

void uart_putc(unsigned char c) {
    // Wait for UART to become ready to transmit
    while (*UART0FR & (1 << 5)) { }
    *UART0DR = c;
}

void uart_puts(const char *str) {
    while (*str) {
        uart_putc((unsigned char)(*str));
        str++;
    }
}

unsigned char uart_getc(void) {
    // Wait for UART to have received something
    while (*UART0FR & (1 << 4)) { }
    return (unsigned char)(*UART0DR);
}

void uart_gets(char *buf, int max_len) {
    int i = 0;
    unsigned char c;
    while (i < max_len - 1) {
        c = uart_getc();
        uart_putc(c); // Echo the character back
        if (c == '\r') {
            break;
        }
        buf[i++] = c;
    }
    buf[i] = '\0';
}

void uart_put_hex(unsigned int value) {
    char hex_chars[] = "0123456789ABCDEF";
    for (int i = 28; i >= 0; i -= 4) {
        uart_putc(hex_chars[(value >> i) & 0xF]);
    }
}
