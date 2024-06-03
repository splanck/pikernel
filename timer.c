#include "timer.h"
#include <stdint.h>

void delay(unsigned int ticks) 
{
    uint64_t start, current;

    // Read the current counter value
    __asm__ volatile ("mrs %0, cntpct_el0" : "=r" (start));
    do {
        __asm__ volatile ("mrs %0, cntpct_el0" : "=r" (current));
    } while ((current - start) < ticks);
}
