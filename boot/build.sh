#BUILD SCRIPT FOR UEFI BOOTLOADER FOR FIRETRON OPERATING SYSTEM

#!/bin/bash

mkdir -p ../build/

set -e

date >> BuildLog.log

echo "[*] Compiling to Object file with GCC"
gcc -Wall -Wextra -fshort-wchar -I ~/gnu-efi/inc -I ./src -ffreestanding -fpic -fno-stack-protector -fno-stack-check -mno-red-zone -fshort-wchar -maccumulate-outgoing-args -c src/boot.c -o ../build/boot.o >> ./BuildLog.log

gcc -Wall -Wextra -fshort-wchar -I ~/gnu-efi/inc -I ./src -ffreestanding -fpic -fno-stack-protector -fno-stack-check -mno-red-zone -fshort-wchar -maccumulate-outgoing-args -c src/error.c -o ../build/error.o >> ./BuildLog.log

gcc -Wall -Wextra -fshort-wchar -I ~/gnu-efi/inc -I ./src -ffreestanding -fpic -fno-stack-protector -fno-stack-check -mno-red-zone -fshort-wchar -maccumulate-outgoing-args -c src/files.c -o ../build/files.o >> ./BuildLog.log

gcc -Wall -Wextra -fshort-wchar -I ~/gnu-efi/inc -I ./src -ffreestanding -fpic -fno-stack-protector -fno-stack-check -mno-red-zone -fshort-wchar -maccumulate-outgoing-args -c src/elf.c -o ../build/elf.o >> ./BuildLog.log

gcc -Wall -Wextra -fshort-wchar -I ~/gnu-efi/inc -I ./src -ffreestanding -fpic -fno-stack-protector -fno-stack-check -mno-red-zone -fshort-wchar -maccumulate-outgoing-args -c src/memory.c -o ../build/memory.o >> ./BuildLog.log

gcc -Wall -Wextra -fshort-wchar -I ~/gnu-efi/inc -I ./src -ffreestanding -fpic -fno-stack-protector -fno-stack-check -mno-red-zone -fshort-wchar -maccumulate-outgoing-args -c src/graphics.c -o ../build/graphics.o >> ./BuildLog.log

echo "[*] Compiling boot stub"

nasm -fbin src/bios_error.s -o ../build/bios_stub.bin

gcc -ffreestanding -fpic -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -fno-stack-protector -fno-stack-check -I ~/gnu-efi/inc -I ./src -c src/firestarter.c -o ../build/firestarter.o >> ../BuildLog.log

ld -pie -nostdlib -e kmain ../build/firestarter.o -o ../build/firestarter.elf >> ./BuildLog.log



echo "[*] Linking to shared object with LD"
ld -shared -Bsymbolic -L ~/gnu-efi/x86_64/lib -L ~/gnu-efi/x86_64/gnuefi -T src/elf_x86_64_efi.lds ~/gnu-efi/x86_64/gnuefi/crt0-efi-x86_64.o ../build/error.o ../build/files.o ../build/elf.o ../build/memory.o ../build/graphics.o ../build/boot.o -o ../build/boot.so -lgnuefi -lefi >> ../BuildLog.log

echo "[*] Copying vital section from sections from shared object to EFI file with objcopy"
objcopy -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym -j .rel -j .rel.* -j .rela.* -j .reloc --output-target efi-app-x86_64 --subsystem=10 ../build/boot.so ../build/fireboot.efi >> ../BuildLog.log

echo "[*] Creating an empty disk img with dd"
dd if=/dev/zero of=../build/firetron.img bs=1M count=64 >> ../BuildLog.log

echo "[*] Formatting the empty disk and creating a filesystem using mformat"

mformat -i ../build/firetron.img :: >> ../BuildLog.log

echo "[*] Creating EFI/BOOT directory and placeing EFI bootloader file into it using mmd and mcopy"

mmd -i ../build/firetron.img ::/EFI >> ../BuildLog.log
mmd -i ../build/firetron.img ::/EFI/BOOT >> ../BuildLog.log
mcopy -i ../build/firetron.img  ../build/bios_stub.bin ::bios_stub.bin >> ../BuildLog.log
mcopy -i ../build/firetron.img  src/startup.nsh  ::startup.nsh >> ../BuildLog.log
mcopy -i ../build/firetron.img ../build/fireboot.efi ::/EFI/BOOT/fireboot.efi >> ../BuildLog.log
mcopy -i ../build/firetron.img ../build/firestarter.elf ::firestarter.elf >> ../BuildLog.log
echo "[*] Launching qemu"
sudo qemu-system-x86_64 -cpu qemu64 -drive if=pflash,format=raw,unit=0,file=/usr/share/OVMF/OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=/usr/share/OVMF/OVMF_VARS.fd -drive format=raw,file=../build/firetron.img
