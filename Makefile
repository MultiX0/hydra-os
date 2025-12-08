OBJECTS = loader.o kmain.o io.o fb.o serial.o gdt.o gdt_s.o idt.o idt_s.o keyboard.o shell.o snake.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

hydra.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R \
	            -b boot/grub/stage2_eltorito \
	            -no-emul-boot \
	            -boot-load-size 4 \
	            -A os \
	            -input-charset utf8 \
	            -quiet \
	            -boot-info-table \
	            -o hydra.iso \
	            iso

run: hydra.iso
	bochs -f bochsrc.txt -q

gdt_s.o: gdt_asm.s
	$(AS) $(ASFLAGS) $< -o $@

idt_s.o: idt_asm.s
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf hydra.iso