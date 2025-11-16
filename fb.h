/* fb.h - Framebuffer driver header */

#ifndef INCLUDE_FB_H
#define INCLUDE_FB_H

/* Framebuffer colors */
#define FB_BLACK         0
#define FB_BLUE          1
#define FB_GREEN         2
#define FB_CYAN          3
#define FB_RED           4
#define FB_MAGENTA       5
#define FB_BROWN         6
#define FB_LIGHT_GREY    7
#define FB_DARK_GREY     8
#define FB_LIGHT_BLUE    9
#define FB_LIGHT_GREEN   10
#define FB_LIGHT_CYAN    11
#define FB_LIGHT_RED     12
#define FB_LIGHT_MAGENTA 13
#define FB_LIGHT_BROWN   14
#define FB_WHITE         15

/** fb_write:
 *  Writes the contents of the buffer buf of length len to the screen.
 *
 *  @param buf The buffer to write
 *  @param len The length of the buffer
 *  @return    The number of bytes written
 */
int fb_write(char *buf, unsigned int len);

#endif /* INCLUDE_FB_H */