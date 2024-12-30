// *INDENT-OFF*
#include "app_config.h"

set ELF_NAME=%1%
cd /d %~dp0

set OBJDUMP=C:\JL\pi32\bin\llvm-objdump.exe
set OBJCOPY=C:\JL\pi32\bin\llvm-objcopy.exe

%OBJDUMP% -d -print-imm-hex -print-dbg %ELF_NAME%.elf > %ELF_NAME%.lst
%OBJCOPY% -O binary -j .app_code %ELF_NAME%.elf  %ELF_NAME%.bin
%OBJCOPY% -O binary -j .data %ELF_NAME%.elf  data.bin

#if ICACHE_RAM_TO_RAM_ENABLE
%OBJCOPY% -O binary -j .cache_ram %ELF_NAME%.elf  cache_ram.bin
#endif

%OBJDUMP% -section-headers  %ELF_NAME%.elf

@echo *******************************************************************************************************
@echo 			                UC04 flash
@echo *******************************************************************************************************
@echo % date %
cd / d % ~dp0

#if ICACHE_RAM_TO_RAM_ENABLE
copy /b %ELF_NAME%.bin+data.bin+cache_ram.bin app.bin
#else
copy /b %ELF_NAME%.bin+data.bin app.bin
#endif

isd_download.exe -tonorflash -dev uc04 -boot 0x101400 -div8 -wait 300 -uboot uboot.boot -app app.bin -flash-params flash_params.bin -output-fw output_flash.fw -output-ufw update.ufw  -key 622_AD12N.lkey

::-format all
@REM 常用命令说明
@rem - format vm        // 擦除VM 区域
@rem - format all       // 擦除所有
@rem - reboot 500       // reset chip, valid in JTAG debug

ping / n 2 127.1 > null
IF EXIST null del null

