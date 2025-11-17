#ifndef INCLUDE_GDT_H
#define INCLUDE_GDT_H

/** load_gdt:
 *  Loads the GDT into the processor. Defined in gdt.s
 *
 *  @param gdt  The address of the GDT structure
 */
void load_gdt(unsigned int *gdt);

/** gdt_install:
 *  Sets up the GDT
 */
void gdt_install(void);

#endif /* INCLUDE_GDT_H */