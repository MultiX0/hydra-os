/* serial.h - Serial port driver header */

#ifndef INCLUDE_SERIAL_H
#define INCLUDE_SERIAL_H

/* The COM1 serial port */
#define SERIAL_COM1_BASE 0x3F8

/** serial_configure:
 *  Configures the serial port for use.
 *
 *  @param com The COM port to configure (e.g., SERIAL_COM1_BASE)
 *  @param divisor The baud rate divisor
 */
void serial_configure(unsigned short com, unsigned short divisor);

/** serial_write:
 *  Writes the contents of the buffer buf of length len to the serial port.
 *
 *  @param buf The buffer to write
 *  @param len The length of the buffer
 *  @return    The number of bytes written
 */
int serial_write(char *buf, unsigned int len);

#endif /* INCLUDE_SERIAL_H */