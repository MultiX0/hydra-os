#include "gdt.h"

/* GDT entry structure */
struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

/* GDT pointer structure */
struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
}__attribute__((packed));

/* Our GDT, with 3 entries */
struct gdt_entry gdt[3];
struct gdt_ptr gp;

/** gdt_set_gate:
 *  Sets a GDT gate
 *
 *  @param num          Index of the GDT entry
 *  @param base         Base address
 *  @param limit        Limit
 *  @param access       Access flags
 *  @param granularity  Granularity
 */
void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char granularity)
{
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    gdt[num].granularity |= (granularity & 0xF0);
    gdt[num].access = access;
}

/** gdt_install:
 *  Sets up the GDT
 */
void gdt_install(void)
{
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (unsigned int)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                /* NULL descriptor */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); /* Code segment */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); /* Data segment */

    load_gdt(&gp);  // Changed from: load_gdt((unsigned int *)&gp);
}