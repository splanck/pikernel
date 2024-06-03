#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

extern unsigned int width, height, pitch, isrgb;
extern unsigned char *fb;

void framebuffer_init(void);
void draw_pixel(unsigned int x, unsigned int y, unsigned int attr);

#endif // FRAMEBUFFER_H
