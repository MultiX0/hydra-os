#include "keyboard.h"
#include "serial.h"

/* US QWERTY keyboard layout scan code to ASCII table */
static char scan_code_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

static char last_char = 0;

/** keyboard_init:
 *  Initializes the keyboard driver
 */
void keyboard_init(void)
{
    last_char = 0;
}

/** keyboard_handle_interrupt:
 *  Handles a keyboard interrupt and converts scan code to ASCII
 *  
 *  @param scan_code The scan code from the keyboard
 */
void keyboard_handle_interrupt(unsigned char scan_code)
{
    serial_write("SCAN: ", 6);
    serial_write((char*)&scan_code, 1);
    serial_write("\n", 1);
    
    /* Only handle key presses (scan codes < 0x80) */
    if (scan_code < 0x80) {
        if (scan_code < sizeof(scan_code_to_ascii)) {
            last_char = scan_code_to_ascii[scan_code];
            serial_write("ASCII: ", 7);
            serial_write(&last_char, 1);
            serial_write("\n", 1);
        }
    }
}
/** keyboard_get_char:
 *  Gets the last character typed (non-blocking)
 *  
 *  @return The character, or 0 if no character available
 */
char keyboard_get_char(void)
{
    char c = last_char;
    last_char = 0;
    return c;
}