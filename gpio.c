#include "gpio.h"

#define GPIO_BASE 0x3F200000
#define GPFSEL1 ((volatile unsigned int *)(GPIO_BASE + 0x04))
#define GPSET0 ((volatile unsigned int *)(GPIO_BASE + 0x1C))
#define GPCLR0 ((volatile unsigned int *)(GPIO_BASE + 0x28))
#define GPPUD ((volatile unsigned int *)(GPIO_BASE + 0x94))
#define GPPUDCLK0 ((volatile unsigned int *)(GPIO_BASE + 0x98))

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
