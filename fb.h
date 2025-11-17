#ifndef INCLUDE_FB_H
#define INCLUDE_FB_H

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Colors */
#define FB_BLACK        0
#define FB_BLUE         1
#define FB_GREEN        2
#define FB_CYAN         3
#define FB_RED          4
#define FB_MAGENTA      5
#define FB_BROWN        6
#define FB_LIGHT_GREY   7
#define FB_DARK_GREY    8
#define FB_LIGHT_BLUE   9
#define FB_LIGHT_GREEN  10
#define FB_LIGHT_CYAN   11
#define FB_LIGHT_RED    12
#define FB_LIGHT_MAGENTA 13
#define FB_LIGHT_BROWN  14
#define FB_WHITE        15

/** fb_write:
 *  Writes the contents of the buffer buf of length len to the screen.
 *
 *  @param buf  The buffer to write
 *  @param len  The length of the buffer
 *  @return     The number of characters written
 */
int fb_write(char *buf, unsigned int len);

/** fb_write_char:
 *  Writes a single character to the screen.
 *
 *  @param c  The character to write
 */
void fb_write_char(char c);

/** fb_clear:
 *  Clears the screen.
 */
void fb_clear(void);

/** fb_putc:
 *  Writes a character to the screen with newline handling.
 *
 *  @param c  The character to write
 */
void fb_putc(char c);

/** fb_puts:
 *  Writes a null-terminated string to the screen.
 *
 *  @param str  The string to write
 */
void fb_puts(char *str);

#endif /* INCLUDE_FB_H */