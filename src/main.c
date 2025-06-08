#include <stdint.h>
#include "font.h"

#define FB_BASE ((volatile uint32_t *)0xFF000000)
#define FB_WIDTH 1024

static void draw_char(int x, int y, char c, uint32_t color)
{
    const uint8_t *bitmap = font[(int)c];
    for (int row = 0; row < 8; row++) {
        uint8_t bits = bitmap[row];
        for (int col = 0; col < 8; col++) {
            if (bits & (1 << (7 - col))) {
                FB_BASE[(y + row) * FB_WIDTH + x + col] = color;
            }
        }
    }
}

void kernel_main(void)
{
    const char *msg = "Hello";
    for (int i = 0; i < FB_WIDTH * 768; i++)
        FB_BASE[i] = 0; // clear screen

    for (int i = 0; msg[i]; i++)
        draw_char(i * 8 + 10, 10, msg[i], 0x00FFFFFF);

    while (1)
        __asm__("wfe");
}
