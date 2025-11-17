#include "fb.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "shell.h"

int kmain(void)
{
    /* Configure serial port */
    serial_configure_baud_rate(SERIAL_COM1_BASE, 3);
    serial_configure_line(SERIAL_COM1_BASE);
    serial_configure_fifo_buffer(SERIAL_COM1_BASE);
    serial_configure_modem(SERIAL_COM1_BASE);
    
    /* Write to serial */
    serial_write("Kernel starting...\n", 19);
    
    /* Set up GDT */
    gdt_install();
    serial_write("GDT installed\n", 14);
    
    /* Set up IDT */
    idt_install();
    serial_write("IDT installed\n", 14);
    
    /* Initialize keyboard */
    keyboard_init();
    serial_write("Keyboard initialized\n", 21);
    
    /* Enable interrupts */
    __asm__ ("sti");
    serial_write("Interrupts enabled\n", 19);
    
    /* Initialize and run shell */
    shell_init();
    serial_write("Shell started\n", 14);
    
    /* Main loop */
    while (1) {
        shell_update();
    }
    
    return 0;
}