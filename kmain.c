/* kmain.c - Kernel main function */

#include "fb.h"
#include "serial.h"

/** kmain:
 *  The main kernel entry point.
 */
void kmain(void)
{
    /* Configure serial port */
    serial_configure(SERIAL_COM1_BASE, 1); /* divisor 1 = 115200 baud */
    
    /* Test framebuffer */
    fb_write("Hello from the framebuffer!\n", 29);
    fb_write("Hydra OS is running.\n", 22);
    
    /* Test serial port */
    serial_write("Hello from the serial port!\n", 29);
    serial_write("OS kernel initialized successfully.\n", 37);
}