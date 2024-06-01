#include <stdint.h>

#define GPIO_BASE 0x3F200000
#define GPFSEL1 ((volatile unsigned int *)(GPIO_BASE + 0x04))
#define GPSET0 ((volatile unsigned int *)(GPIO_BASE + 0x1C))
#define GPCLR0 ((volatile unsigned int *)(GPIO_BASE + 0x28))
#define GPPUD ((volatile unsigned int *)(GPIO_BASE + 0x94))
#define GPPUDCLK0 ((volatile unsigned int *)(GPIO_BASE + 0x98))

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

void gpio_init(void) {
    // Set GPIO pin 16 as output
    unsigned int ra = *GPFSEL1;
    ra &= ~(7 << 18); // Clear bits 18-20
    ra |= (1 << 18);  // Set bit 18 (select function 1 for GPIO16)
    *GPFSEL1 = ra;

    // Disable pull-up/down for all GPIO pins & delay for 150 cycles
    *GPPUD = 0;
    for (ra = 0; ra < 150; ra++) { __asm__ volatile ("nop"); }

    // Disable pull-up/down for pin 16 & delay for 150 cycles
    *GPPUDCLK0 = (1 << 16);
    for (ra = 0; ra < 150; ra++) { __asm__ volatile ("nop"); }
    *GPPUDCLK0 = 0;
}

void gpio_set(int pin) {
    *GPSET0 = (1 << pin);
}

void gpio_clear(int pin) {
    *GPCLR0 = (1 << pin);
}

void delay(unsigned int ticks) {
    uint64_t start, current;
    // Read the current counter value
    __asm__ volatile ("mrs %0, cntpct_el0" : "=r" (start));
    do {
        __asm__ volatile ("mrs %0, cntpct_el0" : "=r" (current));
    } while ((current - start) < ticks);
}

void KMain(void) {
    uart_init();
    uart_puts("Hello, kernel World!\n");

    char buffer[100];
    uart_puts("Type something and press Enter: ");
    uart_gets(buffer, sizeof(buffer));
    uart_puts("\nYou typed: ");
    uart_puts(buffer);
    uart_puts("\n");

    gpio_init();
    while (1) {
        gpio_set(16);
        delay(9000000); // Delay for a period
        uart_puts("Set! \n");
        gpio_clear(16);
        delay(9000000); // Delay for a period
        uart_puts("Clear! \n");
    }
}
