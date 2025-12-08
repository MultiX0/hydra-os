# Hydra OS 🐍

A minimal x86 operating system built from scratch, featuring a custom bootloader pipeline, protected mode initialization, keyboard and framebuffer drivers, interrupt handling, and a built‑in Snake game.

Hydra OS began as a three‑day low‑level programming challenge. The project follows the learning structure from ([The Little Book About OS Development](https://bit.ly/48JIdWp)) and expands it into a functional monolithic kernel. The long‑term goal is to evolve Hydra OS into a fully usable experimental system. Developers interested in contributing are welcome to fork the repository and open pull requests.

<p align="center">
  <img src="images/banner.jpeg" alt="Hydra OS Banner" title="Hydra OS" width="100%" />
</p>

---

## Overview

Hydra OS is a monolithic kernel written in x86 Assembly and C. It implements the core components required for a minimal operating system:

* Multiboot‑compatible kernel image (ELF)
* GRUB bootloader integration
* Protected mode setup and stack initialization
* Global Descriptor Table (GDT)
* Interrupt Descriptor Table (IDT)
* Keyboard driver (IRQ1)
* Framebuffer text mode driver using VGA memory (0xB8000)
* PIC remapping and interrupt handling
* A minimal interactive shell
* Embedded Snake game rendered directly through the framebuffer

---

## Features

### Bootloader & Initialization

* GRUB loads a compliant multiboot kernel.
* Early Assembly code configures protected mode and stack setup.
* The kernel transitions cleanly into C execution.

### Kernel Core

* Monolithic design for simplicity and full hardware control.
* Custom linker script defining memory layout.
* Global Descriptor Table implementation.
* IDT with interrupt stubs and common interrupt handler.

### Drivers

* **Framebuffer driver:** basic character output, cursor positioning, screen clearing, direct memory writes.
* **Keyboard driver:** scan code reading, character translation, and input dispatching.
* **PIC controller:** remapping and acknowledgment handling.
* **Port I/O utilities:** `inb`, `outb` for working with hardware ports.

### Shell

A simple terminal that supports:

* `help` – list commands
* `clear` – reset framebuffer
* `echo` – print text
* `about` – kernel/build information
* `play` – launch Snake game

### Snake Game

A framebuffer‑rendered implementation of the classic Snake game:

* Movement with **W**, **A**, **S**, **D**
* Exit with **Q**
* Collision detection and game reset

---

## Project Structure

```
loader.s                 # Bootloader entry (Multiboot), protected mode setup
link.ld                  # Linker script controlling kernel memory layout
kmain.c                  # Kernel main entry point

fb.c / fb.h              # Framebuffer driver (text mode, VGA memory)
idt.c / idt.h            # Interrupt Descriptor Table setup
idt_asm.s                # Low-level interrupt stubs

gdt.c / gdt.h            # Global Descriptor Table
gdt_asm.s                # Assembly GDT loader

io.s / io.h              # I/O port utilities (inb, outb)
keyboard.c / keyboard.h  # PS/2 keyboard driver
serial.c / serial.h      # Serial COM debugging output

shell.c / shell.h        # Minimal interactive shell
snake.c / snake.h        # Snake game implementation

images/                  # Repository images (banner, assets)
iso/                     # Generated bootable ISO structure
iso/boot/grub/           # GRUB menu configuration (menu.lst)

Makefile                 # Build rules for compiling kernel & ISO
bochsrc.txt              # Bochs emulator configuration
bochslog.txt             # Bochs emulator logs
com1.out                 # Serial console output logs
```
<br>

---

## Building and Running

### Requirements

* GCC with i386 target (or cross‑compiler)
* NASM
* GRUB tools
* xorriso
* Bochs (recommended for testing)

### Build

```
make
```

### Run in Bochs

```
make run
```

A bootable ISO file (`hydra.iso`) will be generated automatically.

---

## Roadmap

Hydra OS was created in three days as an educational exercise, but the project is intended to grow. Planned additions:

* Complete memory manager
* Basic scheduler and multitasking
* File system support
* Improved terminal and input
* Enhanced framebuffer rendering
* Additional system utilities
* More complete C runtime environment

Contributions are encouraged. The repository is structured to allow incremental improvements.

---

## Contributing

Contributions of any kind are welcome. If you would like to participate:

1. Fork the repository
2. Create a feature branch
3. Submit a pull request

The aim is to collaboratively build a small but functional operating system grounded in low‑level, educational design.

---

## Acknowledgements

Hydra OS was developed using concepts and guidance from:

*The Little Book About OS Development* – [https://bit.ly/48JIdWp](https://bit.ly/48JIdWp)

This resource provides an excellent foundation for anyone exploring operating system internals.

---

Hydra OS is an ongoing experimental project. Further development will continue as time permits, and community contributions are welcomed.
