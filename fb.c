/* fb.c - Framebuffer driver implementation */

#include "fb.h"
#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Framebuffer memory-mapped I/O starting address */
#define FB_ADDRESS              0x000B8000

/* Framebuffer dimensions */
#define FB_NUM_COLS             80
#define FB_NUM_ROWS             25

/* Current cursor position */
static unsigned short cursor_pos = 0;

/* Framebuffer pointer */
static char *fb = (char *) FB_ADDRESS;

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
static void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
static void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

/** fb_write:
 *  Writes the contents of the buffer buf of length len to the screen.
 *
 *  @param buf The buffer to write
 *  @param len The length of the buffer
 *  @return    The number of bytes written
 */
int fb_write(char *buf, unsigned int len)
{
    unsigned int i;
    
    for (i = 0; i < len; i++) {
        char c = buf[i];
        
        /* Handle newline */
        if (c == '\n') {
            /* Move to next line */
            cursor_pos = (cursor_pos / FB_NUM_COLS + 1) * FB_NUM_COLS;
        } else {
            /* Write character at current cursor position */
            fb_write_cell(cursor_pos * 2, c, FB_BLACK, FB_LIGHT_GREY);
            cursor_pos++;
        }
        
        /* Check if we need to scroll */
        if (cursor_pos >= FB_NUM_COLS * FB_NUM_ROWS) {
            /* Simple solution: wrap to beginning (no actual scrolling) */
            cursor_pos = 0;
        }
    }
    
    /* Move the cursor to the new position */
    fb_move_cursor(cursor_pos);
    
    return len;
}