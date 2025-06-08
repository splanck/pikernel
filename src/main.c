#include <stdint.h>
#include "font.h"

static volatile uint32_t *fb_base;
static uint32_t fb_width;
static uint32_t fb_height;

static int streq(const char *a, const char *b)
{
    while (*a && *b && *a == *b) {
        a++; b++;
    }
    return *a == *b;
}

static unsigned int cstrlen(const char *s)
{
    unsigned int n = 0;
    while (s[n])
        n++;
    return n;
}

static uint32_t fdt32_to_cpu(uint32_t x)
{
    return __builtin_bswap32(x);
}

struct fdt_header {
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
};

#define FDT_MAGIC 0xd00dfeed
#define FDT_BEGIN_NODE  1
#define FDT_END_NODE    2
#define FDT_PROP        3
#define FDT_NOP         4
#define FDT_END         9

static void parse_fdt(void *fdt)
{
    const struct fdt_header *hdr = (const struct fdt_header *)fdt;
    if (fdt32_to_cpu(hdr->magic) != FDT_MAGIC)
        return;

    const uint32_t *p = (const uint32_t *)((char *)fdt + fdt32_to_cpu(hdr->off_dt_struct));
    const char *strings = (const char *)fdt + fdt32_to_cpu(hdr->off_dt_strings);

    int depth = 0;
    int sf_depth = -1;
    fb_base = 0;
    fb_width = fb_height = 0;

    while (1) {
        uint32_t token = fdt32_to_cpu(*p++);
        if (token == FDT_END)
            break;
        if (token == FDT_NOP)
            continue;
        if (token == FDT_BEGIN_NODE) {
            const char *name = (const char *)p;
            unsigned int l = cstrlen(name);
            p = (const uint32_t *)(((uintptr_t)p + l + 4) & ~3);
            depth++;
        } else if (token == FDT_END_NODE) {
            if (sf_depth >= 0 && depth == sf_depth)
                break;
            depth--;
        } else if (token == FDT_PROP) {
            uint32_t len = fdt32_to_cpu(*p++);
            uint32_t nameoff = fdt32_to_cpu(*p++);
            const char *propname = strings + nameoff;
            const uint8_t *data = (const uint8_t *)p;
            p = (const uint32_t *)(((uintptr_t)p + len + 3) & ~3);

            if (sf_depth < 0 && streq(propname, "compatible")) {
                const char *s = (const char *)data;
                while (s < (const char *)data + len) {
                    if (streq(s, "simple-framebuffer")) {
                        sf_depth = depth;
                        break;
                    }
                    unsigned int sl = cstrlen(s);
                    s += sl + 1;
                }
            } else if (sf_depth == depth) {
                if (streq(propname, "reg")) {
                    const uint32_t *cells = (const uint32_t *)data;
                    uint64_t addr = ((uint64_t)fdt32_to_cpu(cells[0]) << 32) |
                                     fdt32_to_cpu(cells[1]);
                    fb_base = (volatile uint32_t *)(uintptr_t)addr;
                } else if (streq(propname, "width")) {
                    fb_width = fdt32_to_cpu(*(const uint32_t *)data);
                } else if (streq(propname, "height")) {
                    fb_height = fdt32_to_cpu(*(const uint32_t *)data);
                }
            }
        }
    }
}

static void draw_char(int x, int y, char c, uint32_t color)
{
    const uint8_t *bitmap = font[(int)c];
    for (int row = 0; row < 8; row++) {
        uint8_t bits = bitmap[row];
        for (int col = 0; col < 8; col++) {
            if (bits & (1 << (7 - col))) {
                fb_base[(y + row) * fb_width + x + col] = color;
            }
        }
    }
}

void kernel_main(void *fdt)
{
    const char *msg = "Hello";

    parse_fdt(fdt);
    if (!fb_base || !fb_width || !fb_height)
        return;

    for (uint32_t i = 0; i < fb_width * fb_height; i++)
        fb_base[i] = 0; // clear screen

    for (int i = 0; msg[i]; i++)
        draw_char(i * 8 + 10, 10, msg[i], 0x00FFFFFF);

    while (1)
        __asm__("wfe");
}
