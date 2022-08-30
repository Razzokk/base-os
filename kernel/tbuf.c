#include "tbuf.h"

#include "kdefs.h"

tbuf_char* const text_buffer = (tbuf_char* const) TEXT_BUF_ADDR;

void tbuf_clear()
{
    memset(text_buffer, 0, TEXT_BUF_ROWS * TEXT_BUF_COLS * sizeof(tbuf_char));
}
