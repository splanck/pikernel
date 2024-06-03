#include "framebuffer.h"
#include "uart.h"

#define MAILBOX_BASE 0x3F00B880
#define MAILBOX_READ (MAILBOX_BASE + 0x00)
#define MAILBOX_STATUS (MAILBOX_BASE + 0x18)
#define MAILBOX_WRITE (MAILBOX_BASE + 0x20)
#define MAILBOX_FULL 0x80000000
#define MAILBOX_EMPTY 0x40000000

volatile unsigned int * const MAILBOX = (unsigned int *)MAILBOX_BASE;

unsigned int width = 1024, height = 768, pitch = 0, isrgb = 0;
unsigned char *fb = 0;

unsigned int mailbox[36] __attribute__((aligned(16)));

int mailbox_call(unsigned char ch) {
    unsigned int r;
    unsigned int message = ((unsigned int)((unsigned long)mailbox & ~0xF) | (ch & 0xF));

    uart_puts("mailbox_call: Waiting for mailbox to be ready...\n");
    while (MAILBOX[6] & MAILBOX_FULL); // Wait for mailbox to be not full

    uart_puts("mailbox_call: Writing message to mailbox...\n");
    MAILBOX[8] = message;

    uart_puts("mailbox_call: Waiting for response...\n");
    while (1) {
        while (MAILBOX[6] & MAILBOX_EMPTY); // Wait for mailbox to have data

        r = MAILBOX[0];
        if ((r & 0xF) == ch) {
            uart_puts("mailbox_call: Response received.\n");
            return (mailbox[1] == 0x80000000); // Return success/failure based on response
        } else {
            uart_puts("mailbox_call: Response not for our channel.\n");
        }
    }
    return 0; // Should not reach here
}

void framebuffer_init(void) {
    uart_puts("framebuffer_init: Start\n");

    mailbox[0] = 35 * 4;
    mailbox[1] = 0;

    mailbox[2] = 0x00048003;
    mailbox[3] = 8;
    mailbox[4] = 8;
    mailbox[5] = width;
    mailbox[6] = height;

    uart_puts("framebuffer_init: Set physical display\n");

    mailbox[7] = 0x00048004;
    mailbox[8] = 8;
    mailbox[9] = 8;
    mailbox[10] = width;
    mailbox[11] = height;

    uart_puts("framebuffer_init: Set virtual display\n");

    mailbox[12] = 0x00048009;
    mailbox[13] = 8;
    mailbox[14] = 8;
    mailbox[15] = 0;
    mailbox[16] = 0;

    uart_puts("framebuffer_init: Set virtual offset\n");

    mailbox[17] = 0x00048005;
    mailbox[18] = 4;
    mailbox[19] = 4;
    mailbox[20] = 32;

    uart_puts("framebuffer_init: Set depth\n");

    mailbox[21] = 0x00048006;
    mailbox[22] = 4;
    mailbox[23] = 4;
    mailbox[24] = 1;

    uart_puts("framebuffer_init: Set pixel order\n");

    mailbox[25] = 0x00040001;
    mailbox[26] = 8;
    mailbox[27] = 8;
    mailbox[28] = 4096;

    uart_puts("framebuffer_init: Allocate framebuffer\n");

    mailbox[29] = 0x00040008;
    mailbox[30] = 4;
    mailbox[31] = 4;
    mailbox[32] = 0;

    uart_puts("framebuffer_init: Get pitch\n");

    mailbox[33] = 0x00040008;
    mailbox[34] = 4;
    mailbox[35] = 4;
    mailbox[36] = 0;

    uart_puts("framebuffer_init: Call mailbox\n");
    if (mailbox_call(8)) {
        width = mailbox[5];
        height = mailbox[6];
        pitch = mailbox[33];
        fb = (unsigned char *)((unsigned long)mailbox[28] & 0x3FFFFFFF);
        isrgb = mailbox[24];
        uart_puts("framebuffer_init: Success\n");
        uart_puts("Framebuffer Details:\n");
        uart_puts("width: "); uart_put_hex(width); uart_puts("\n");
        uart_puts("height: "); uart_put_hex(height); uart_puts("\n");
        uart_puts("pitch: "); uart_put_hex(pitch); uart_puts("\n");
        uart_puts("fb: "); uart_put_hex((unsigned long)fb); uart_puts("\n");
        uart_puts("isrgb: "); uart_put_hex(isrgb); uart_puts("\n");
    } else {
        uart_puts("framebuffer_init: Failed\n");
        uart_puts("Response data:\n");
        uart_puts("mailbox[0]: "); uart_put_hex(mailbox[0]); uart_puts("\n");
        uart_puts("mailbox[1]: "); uart_put_hex(mailbox[1]); uart_puts("\n");
        uart_puts("mailbox[2]: "); uart_put_hex(mailbox[2]); uart_puts("\n");
        uart_puts("mailbox[3]: "); uart_put_hex(mailbox[3]); uart_puts("\n");
        uart_puts("mailbox[4]: "); uart_put_hex(mailbox[4]); uart_puts("\n");
        uart_puts("mailbox[5]: "); uart_put_hex(mailbox[5]); uart_puts("\n");
        uart_puts("mailbox[6]: "); uart_put_hex(mailbox[6]); uart_puts("\n");
        uart_puts("mailbox[7]: "); uart_put_hex(mailbox[7]); uart_puts("\n");
        uart_puts("mailbox[8]: "); uart_put_hex(mailbox[8]); uart_puts("\n");
        uart_puts("mailbox[9]: "); uart_put_hex(mailbox[9]); uart_puts("\n");
        uart_puts("mailbox[10]: "); uart_put_hex(mailbox[10]); uart_puts("\n");
        uart_puts("mailbox[11]: "); uart_put_hex(mailbox[11]); uart_puts("\n");
        uart_puts("mailbox[12]: "); uart_put_hex(mailbox[12]); uart_puts("\n");
        uart_puts("mailbox[13]: "); uart_put_hex(mailbox[13]); uart_puts("\n");
        uart_puts("mailbox[14]: "); uart_put_hex(mailbox[14]); uart_puts("\n");
        uart_puts("mailbox[15]: "); uart_put_hex(mailbox[15]); uart_puts("\n");
        uart_puts("mailbox[16]: "); uart_put_hex(mailbox[16]); uart_puts("\n");
        uart_puts("mailbox[17]: "); uart_put_hex(mailbox[17]); uart_puts("\n");
        uart_puts("mailbox[18]: "); uart_put_hex(mailbox[18]); uart_puts("\n");
        uart_puts("mailbox[19]: "); uart_put_hex(mailbox[19]); uart_puts("\n");
        uart_puts("mailbox[20]: "); uart_put_hex(mailbox[20]); uart_puts("\n");
        uart_puts("mailbox[21]: "); uart_put_hex(mailbox[21]); uart_puts("\n");
        uart_puts("mailbox[22]: "); uart_put_hex(mailbox[22]); uart_puts("\n");
        uart_puts("mailbox[23]: "); uart_put_hex(mailbox[23]); uart_puts("\n");
        uart_puts("mailbox[24]: "); uart_put_hex(mailbox[24]); uart_puts("\n");
        uart_puts("mailbox[25]: "); uart_put_hex(mailbox[25]); uart_puts("\n");
        uart_puts("mailbox[26]: "); uart_put_hex(mailbox[26]); uart_puts("\n");
        uart_puts("mailbox[27]: "); uart_put_hex(mailbox[27]); uart_puts("\n");
        uart_puts("mailbox[28]: "); uart_put_hex(mailbox[28]); uart_puts("\n");
        uart_puts("mailbox[29]: "); uart_put_hex(mailbox[29]); uart_puts("\n");
        uart_puts("mailbox[30]: "); uart_put_hex(mailbox[30]); uart_puts("\n");
        uart_puts("mailbox[31]: "); uart_put_hex(mailbox[31]); uart_puts("\n");
        uart_puts("mailbox[32]: "); uart_put_hex(mailbox[32]); uart_puts("\n");
        uart_puts("mailbox[33]: "); uart_put_hex(mailbox[33]); uart_puts("\n");
    }
}

void draw_pixel(unsigned int x, unsigned int y, unsigned int attr) {
    if (x >= width || y >= height) {
        return; // Out of bounds
    }
    unsigned int where = (x * 4) + (y * pitch);
    uart_puts("Drawing pixel at: ");
    uart_put_hex(where);
    uart_puts(" with color: ");
    uart_put_hex(attr);
    uart_puts("\n");

    // Calculate the total framebuffer size
    unsigned int framebuffer_size = pitch * height;
    uart_puts("Framebuffer size: ");
    uart_put_hex(framebuffer_size);
    uart_puts("\n");

    // Ensure we're within bounds
    if (where + 4 <= framebuffer_size) {  // Ensure we're within bounds
        unsigned char* pixel = fb + where;
        pixel[0] = attr & 0xFF;
        pixel[1] = (attr >> 8) & 0xFF;
        pixel[2] = (attr >> 16) & 0xFF;
        pixel[3] = (attr >> 24) & 0xFF;
    } else {
        uart_puts("Error: Drawing out of bounds\n");
    }
}
