#ifndef INCLUDE_IDT_H
#define INCLUDE_IDT_H

/** load_idt:
 *  Loads the IDT into the processor. Defined in idt.s
 *
 *  @param idt  The address of the IDT structure
 */
void load_idt(unsigned int *idt);

/** idt_install:
 *  Sets up the IDT
 */
void idt_install(void);

/* Declare interrupt handler functions */
void interrupt_handler_0(void);
void interrupt_handler_1(void);
void interrupt_handler_2(void);
void interrupt_handler_3(void);
void interrupt_handler_4(void);
void interrupt_handler_5(void);
void interrupt_handler_6(void);
void interrupt_handler_7(void);
void interrupt_handler_8(void);
void interrupt_handler_9(void);
void interrupt_handler_10(void);
void interrupt_handler_11(void);
void interrupt_handler_12(void);
void interrupt_handler_13(void);
void interrupt_handler_14(void);
void interrupt_handler_15(void);
void interrupt_handler_16(void);
void interrupt_handler_17(void);
void interrupt_handler_18(void);
void interrupt_handler_19(void);
void interrupt_handler_20(void);
void interrupt_handler_21(void);
void interrupt_handler_22(void);
void interrupt_handler_23(void);
void interrupt_handler_24(void);
void interrupt_handler_25(void);
void interrupt_handler_26(void);
void interrupt_handler_27(void);
void interrupt_handler_28(void);
void interrupt_handler_29(void);
void interrupt_handler_30(void);
void interrupt_handler_31(void);
void interrupt_handler_32(void);
void interrupt_handler_33(void);
void interrupt_handler_34(void);
void interrupt_handler_35(void);
void interrupt_handler_36(void);
void interrupt_handler_37(void);
void interrupt_handler_38(void);
void interrupt_handler_39(void);
void interrupt_handler_40(void);
void interrupt_handler_41(void);
void interrupt_handler_42(void);
void interrupt_handler_43(void);
void interrupt_handler_44(void);
void interrupt_handler_45(void);
void interrupt_handler_46(void);
void interrupt_handler_47(void);
void interrupt_handler_main(unsigned int *esp);


#endif /* INCLUDE_IDT_H */