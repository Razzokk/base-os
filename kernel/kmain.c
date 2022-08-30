#include <stdbool.h>
#include "kdefs.h"

extern void setup();
extern void loop();

void kmain()
{
    setup();

    // Really rough approx.
    // io_delay has ~ 1-4 microsecond delay
    size_t microseconds_delay = 500000;

    while (true)
    {
        loop();
        for (size_t i = 0; i < microseconds_delay; i++) io_delay();
    }
}
