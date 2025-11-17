#include "idt.h"
#include "io.h"
#include "serial.h"

/* Forward declaration */
void keyboard_handle_interrupt(unsigned char scan_code);

/* IDT entry structure */
struct idt_entry
{
    unsigned short offset_low;
    unsigned short selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short offset_high;
} __attribute__((packed));

/* IDT pointer structure */
struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Our IDT, with 256 entries */
struct idt_entry idt[256];
struct idt_ptr ip;

/* PIC constants */
#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0

#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT PIC2_START_INTERRUPT + 7

#define PIC_ACK 0x20

/* Keyboard */
#define KBD_DATA_PORT 0x60

/** pic_acknowledge:
 *  Acknowledges an interrupt from either PIC 1 or PIC 2.
 */
void pic_acknowledge(unsigned int interrupt)
{
    if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT)
    {
        return;
    }

    if (interrupt < PIC2_START_INTERRUPT)
    {
        outb(PIC1_PORT_A, PIC_ACK);
    }
    else
    {
        outb(PIC2_PORT_A, PIC_ACK);
    }
}

/** read_scan_code:
 *  Reads a scan code from the keyboard
 */
unsigned char read_scan_code(void)
{
    return inb(KBD_DATA_PORT);
}

/** idt_set_gate:
 *  Sets an IDT gate
 */
void idt_set_gate(int num, void (*handler)(void), unsigned short selector, unsigned char type_attr)
{
    unsigned int handler_address = (unsigned int)handler;

    idt[num].offset_low = handler_address & 0xFFFF;
    idt[num].offset_high = (handler_address >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].type_attr = type_attr;
}

/** pic_remap:
 *  Remaps the PIC interrupts
 */
void pic_remap(int offset1, int offset2)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, offset1);
    outb(0xA1, offset2);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

/** idt_install:
 *  Sets up the IDT
 */
void idt_install(void)
{
    ip.limit = (sizeof(struct idt_entry) * 256) - 1;
    ip.base = (unsigned int)&idt;

    /* Set up interrupt handlers */
    idt_set_gate(0, interrupt_handler_0, 0x08, 0x8E);
    idt_set_gate(1, interrupt_handler_1, 0x08, 0x8E);
    idt_set_gate(2, interrupt_handler_2, 0x08, 0x8E);
    idt_set_gate(3, interrupt_handler_3, 0x08, 0x8E);
    idt_set_gate(4, interrupt_handler_4, 0x08, 0x8E);
    idt_set_gate(5, interrupt_handler_5, 0x08, 0x8E);
    idt_set_gate(6, interrupt_handler_6, 0x08, 0x8E);
    idt_set_gate(7, interrupt_handler_7, 0x08, 0x8E);
    idt_set_gate(8, interrupt_handler_8, 0x08, 0x8E);
    idt_set_gate(9, interrupt_handler_9, 0x08, 0x8E);
    idt_set_gate(10, interrupt_handler_10, 0x08, 0x8E);
    idt_set_gate(11, interrupt_handler_11, 0x08, 0x8E);
    idt_set_gate(12, interrupt_handler_12, 0x08, 0x8E);
    idt_set_gate(13, interrupt_handler_13, 0x08, 0x8E);
    idt_set_gate(14, interrupt_handler_14, 0x08, 0x8E);
    idt_set_gate(15, interrupt_handler_15, 0x08, 0x8E);
    idt_set_gate(16, interrupt_handler_16, 0x08, 0x8E);
    idt_set_gate(17, interrupt_handler_17, 0x08, 0x8E);
    idt_set_gate(18, interrupt_handler_18, 0x08, 0x8E);
    idt_set_gate(19, interrupt_handler_19, 0x08, 0x8E);
    idt_set_gate(20, interrupt_handler_20, 0x08, 0x8E);
    idt_set_gate(21, interrupt_handler_21, 0x08, 0x8E);
    idt_set_gate(22, interrupt_handler_22, 0x08, 0x8E);
    idt_set_gate(23, interrupt_handler_23, 0x08, 0x8E);
    idt_set_gate(24, interrupt_handler_24, 0x08, 0x8E);
    idt_set_gate(25, interrupt_handler_25, 0x08, 0x8E);
    idt_set_gate(26, interrupt_handler_26, 0x08, 0x8E);
    idt_set_gate(27, interrupt_handler_27, 0x08, 0x8E);
    idt_set_gate(28, interrupt_handler_28, 0x08, 0x8E);
    idt_set_gate(29, interrupt_handler_29, 0x08, 0x8E);
    idt_set_gate(30, interrupt_handler_30, 0x08, 0x8E);
    idt_set_gate(31, interrupt_handler_31, 0x08, 0x8E);
    idt_set_gate(32, interrupt_handler_32, 0x08, 0x8E);
    idt_set_gate(33, interrupt_handler_33, 0x08, 0x8E);
    idt_set_gate(34, interrupt_handler_34, 0x08, 0x8E);
    idt_set_gate(35, interrupt_handler_35, 0x08, 0x8E);
    idt_set_gate(36, interrupt_handler_36, 0x08, 0x8E);
    idt_set_gate(37, interrupt_handler_37, 0x08, 0x8E);
    idt_set_gate(38, interrupt_handler_38, 0x08, 0x8E);
    idt_set_gate(39, interrupt_handler_39, 0x08, 0x8E);
    idt_set_gate(40, interrupt_handler_40, 0x08, 0x8E);
    idt_set_gate(41, interrupt_handler_41, 0x08, 0x8E);
    idt_set_gate(42, interrupt_handler_42, 0x08, 0x8E);
    idt_set_gate(43, interrupt_handler_43, 0x08, 0x8E);
    idt_set_gate(44, interrupt_handler_44, 0x08, 0x8E);
    idt_set_gate(45, interrupt_handler_45, 0x08, 0x8E);
    idt_set_gate(46, interrupt_handler_46, 0x08, 0x8E);
    idt_set_gate(47, interrupt_handler_47, 0x08, 0x8E);

    /* Remap the PIC */
    pic_remap(0x20, 0x28);

    /* Load the IDT */
    load_idt((unsigned int *)&ip);
}

/** interrupt_handler_main:
 *  C function called by the common interrupt handler
 */
void interrupt_handler_main(unsigned int *regs)
{
    serial_write("INT!\n", 5);
    
    // Stack layout when we get here:
    // regs points to the top of the stack after we pushed esp
    // Working backwards from there:
    // regs[0] points to: [gs][fs][es][ds][edi][esi][ebp][esp][ebx][edx][ecx][eax][int_no][err_code]
    
    // Since regs IS the stack pointer we saved, we can access directly:
    // After pusha (8 registers), then ds, es, fs, gs (4 more) = 12 total
    // Then int_no and error_code are pushed BEFORE all that
    
    // Let's access from the saved stack pointer
    unsigned int *stack_ptr = regs;
    
    // Skip: gs(0), fs(1), es(2), ds(3), and 8 pusha registers = 12 total
    unsigned int interrupt = stack_ptr[12];  // interrupt number
    
    serial_write("INT NUM: ", 9);
    char buf[10];
    buf[0] = '0' + (interrupt / 10);
    buf[1] = '0' + (interrupt % 10);
    serial_write(buf, 2);
    serial_write("\n", 1);
    
    /* Handle keyboard interrupt */
    if (interrupt == 33) {  // 33 = 0x21 in decimal
        serial_write("KBD!\n", 5);
        unsigned char scan_code = read_scan_code();
        keyboard_handle_interrupt(scan_code);
        pic_acknowledge(interrupt);
    } else {
        pic_acknowledge(interrupt);
    }
}