#ifndef INCLUDE_KEYBOARD_H
#define INCLUDE_KEYBOARD_H

/** keyboard_init:
 *  Initializes the keyboard driver
 */
void keyboard_init(void);

/** keyboard_get_char:
 *  Gets the last character typed (non-blocking)
 *  
 *  @return The character, or 0 if no character available
 */
char keyboard_get_char(void);

#endif /* INCLUDE_KEYBOARD_H */