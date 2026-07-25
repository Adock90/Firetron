# Firetron
A Open Source Operating System made for a future alternative OS


Firetron will eventually have a working console based system shell as well as having a graphical interface. 
It doesn't follow the POSIX standard however does support ELF files.

## At the moment.

# Bootloader

Firetron do not support BIOS at the minute as intentions are focused on UEFI as it is more universal and is what the majority of Computers and Laptops support. However we may support BIOS in the future.
Bootloader uses <a href="https://github.com/ncroxon/gnu-efi">gnu-efi</a> as the UEFI framework.
Bootloader also includes a ELF loader and ASM stub to run if the user runs in legacy or BIOS mode.

<h4>
  Thanks to many for resources for the bootloader.
</h4>

EmbeddedOS: <a href=https://github.com/EmbeddedOS>https://github.com/EmbeddedOS</a>

Queso Fuego: <a href=https://github.com/queso-fuego>https://github.com/queso-fuego</a>

OSDev: <a href=https://wiki.osdev.org/UEFI>https://wiki.osdev.org/UEFI</a>

UEFI Specs: <a href=https://uefi.org/specs/UEFI/2.10/index.html>https://uefi.org/specs/UEFI/2.10/index.html</a>

# Kernel

Firetron's kernel is in very early development so it doesn't load yet is being developed.



# Building (the Bootloader)


Git Clone:

<code> git clone https://github.com/Adock90/Firetron-boot.git</code>

cd into the boot dir:

<code> cd boot</code>

Run:

<code> chmod +x ./build.sh </code>

<code> ./build.sh </code>

This should run in qemu.
