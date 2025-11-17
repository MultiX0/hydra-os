#include "io.h"
#include "fb.h"

/* The framebuffer address */
char *fb = (char *) 0x000B8000;

/* Screen dimensions */
#define FB_WIDTH 80
#define FB_HEIGHT 25

/* Cursor position */
unsigned short cursor_pos = 0;

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;
    fb[i + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);  // ← CORRECT
}
/** fb_clear:
 *  Clears the screen.
 */
void fb_clear(void)
{
    unsigned int i;
    for (i = 0; i < FB_WIDTH * FB_HEIGHT; i++) {
        fb_write_cell(i * 2, ' ', FB_WHITE, FB_BLACK);
    }
    cursor_pos = 0;
    fb_move_cursor(cursor_pos);
}

/** fb_scroll:
 *  Scrolls the screen up by one line.
 */
void fb_scroll(void)
{
    unsigned int i;
    
    /* Move all lines up by one */
    for (i = 0; i < (FB_HEIGHT - 1) * FB_WIDTH; i++) {
        fb[i * 2] = fb[(i + FB_WIDTH) * 2];
        fb[i * 2 + 1] = fb[(i + FB_WIDTH) * 2 + 1];
    }
    
    /* Clear the last line */
    for (i = (FB_HEIGHT - 1) * FB_WIDTH; i < FB_HEIGHT * FB_WIDTH; i++) {
        fb_write_cell(i * 2, ' ', FB_WHITE, FB_BLACK);
    }
    
    cursor_pos = (FB_HEIGHT - 1) * FB_WIDTH;
}

/** fb_putc:
 *  Writes a character to the screen with newline handling.
 *
 *  @param c  The character to write
 */
void fb_putc(char c)
{
    if (c == '\n') {
        /* Move to next line */
        cursor_pos = (cursor_pos / FB_WIDTH + 1) * FB_WIDTH;
    } else if (c == '\b') {
        /* Backspace */
        if (cursor_pos > 0) {
            cursor_pos--;
            fb_write_cell(cursor_pos * 2, ' ', FB_WHITE, FB_BLACK);
        }
    } else if (c == '\t') {
        /* Tab - move to next multiple of 8 */
        cursor_pos = (cursor_pos + 8) & ~7;
    } else {
        /* Regular character */
        fb_write_cell(cursor_pos * 2, c, FB_WHITE, FB_BLACK);
        cursor_pos++;
    }
    
    /* Scroll if needed */
    if (cursor_pos >= FB_WIDTH * FB_HEIGHT) {
        fb_scroll();
    }
    
    fb_move_cursor(cursor_pos);
}

/** fb_puts:
 *  Writes a null-terminated string to the screen.
 *
 *  @param str  The string to write
 */
void fb_puts(char *str)
{
    unsigned int i = 0;
    while (str[i] != '\0') {
        fb_putc(str[i]);
        i++;
    }
}

/** fb_write_char:
 *  Writes a single character to the screen.
 *
 *  @param c  The character to write
 */
void fb_write_char(char c)
{
    fb_putc(c);
}

/** fb_write:
 *  Writes the contents of the buffer buf of length len to the screen.
 *
 *  @param buf  The buffer to write
 *  @param len  The length of the buffer
 *  @return     The number of characters written
 */
int fb_write(char *buf, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++) {
        fb_putc(buf[i]);
    }
    return len;
}