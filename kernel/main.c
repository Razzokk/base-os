#include "kdefs.h"
#include "tbuf.h"

void setup()
{
    tbuf_clear();

    tbuf_put(10, 5, 'A', BLACK, GREEN);
    tbuf_char chr = tbuf_chr('B', CYAN, GRAY);
    tbuf_putchar(11, 5, chr);
}

int i = 0;

void loop()
{
    tbuf_put(8, i++ % TEXT_BUF_COLS, ' ', BLACK, BLACK);
    tbuf_put(8, i % TEXT_BUF_COLS, 'O', LIGHT_RED, RED);
}
