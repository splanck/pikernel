#include "uart.h"
#include "gpio.h"
#include "timer.h"
#include "framebuffer.h"

void KMain(void) {
    uart_init();
    uart_puts("Hello, kernel World!\n");

    uart_puts("Initializing framebuffer...\n");
    framebuffer_init();
    uart_puts("Framebuffer initialization complete.\n");

    if (fb) {
        uart_puts("Framebuffer initialized successfully\n");

        // Add a debug message before the drawing loop
        uart_puts("Starting to draw pixels...\n");

        unsigned int max_pixels = (pitch * height) / 4; // Maximum number of pixels that can be drawn
        unsigned int pixel_count = 0;

        for (unsigned int y = 0; y < height && pixel_count < max_pixels; y++) {
            for (unsigned int x = 0; x < width && pixel_count < max_pixels; x++) {
                draw_pixel(x, y, 0x00FF00); // Draw green pixels
                pixel_count++;
            }
        }
        uart_puts("Finished drawing pixels.\n");
    } else {
        uart_puts("Framebuffer initialization failed\n");
    }

    char buffer[100];
    uart_puts("Type something and press Enter: ");
    uart_gets(buffer, sizeof(buffer));
    uart_puts("\nYou typed: ");
    uart_puts(buffer);
    uart_puts("\n");

    gpio_init();
    while (1) {
        uart_puts("LED ON\n");
        gpio_set(16);
        delay(1000000); // Delay for a period

        uart_puts("LED OFF\n");
        gpio_clear(16);
        delay(1000000); // Delay for a period
    }
}
