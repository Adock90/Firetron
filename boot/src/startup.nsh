@echo -off

echo "[Booting firetron]"

if exist .\EFI\BOOT\fireboot.efi then
	.\EFI\BOOT\fireboot.efi
	goto END
endif

if exist fs0:EFI\BOOT\fireboot.efi then
        fs0:
	EFI\BOOT\fireboot.efi
        goto END
endif

if exist fs1:EFI\BOOT\fireboot.efi then
        fs1:
        EFI\BOOT\fireboot.efi
        goto END
endif

if exist fs2:EFI\BOOT\fireboot.efi then
        fs2:
        EFI\BOOT\fireboot.efi
        goto END
endif

if exist fs3:EFI\BOOT\fireboot.efi then
        fs3:
        EFI\BOOT\fireboot.efi
        goto END
endif

echo "[firetron startup.nsh] Unable to find bootloader: fireboot.efi"

:END
