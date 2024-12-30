





CONFIG_CODE_SIZE = 0x40000;
ICACHE_RAM_SIZE = 0x4000;
MEMORY
{

 cache_ram : ORIGIN = 0x300000 + ICACHE_RAM_SIZE - 8192, LENGTH = 8192

 app_code(rx) : ORIGIN = 0x4000100, LENGTH = CONFIG_CODE_SIZE
    ram0(rw) : ORIGIN = 0x100000 + 64*4, LENGTH = (0x10A000 - 0x24) - (0x100000 + 64*4)
    boot_ram(rw) : ORIGIN = 0x10A000 - 0x24, LENGTH = 0x24
}

ENTRY(_start)

SECTIONS
{


    . = ORIGIN(cache_ram);
    .cache_ram ALIGN(4):
 {
  *(*.usr_data)
        . = ALIGN(4);
 } > cache_ram



    . = ORIGIN(ram0);
    .data ALIGN(4):
    {
        PROVIDE(data_buf_start = .);
        *(.data_magic)
        *(.data)
        *(.*.data)
        *(.common)

        cache_Lx_code_text_begin = .;
        *(.audio_isr_text)
        *(.*.text.cache.L1)
        *(.*.text.cache.L2)
        *(.*.text.cache.L3)
        *(.ram_code)
        *(.insram)
        . = (. + 3) / 4 * 4 ;
        cache_Lx_code_text_end = .;
 } > ram0

    .bss (NOLOAD) : SUBALIGN(4)
    {
        PROVIDE(bss_buf_start = .);
        . = ALIGN(32);
        _cpu0_sstack_begin = .;
        PROVIDE(cpu0_sstack_begin = .);
        . = ALIGN(32);
        *(.intr_stack)
        . = ALIGN(32);
  *(.stack_magic);
        . = ALIGN(32);
        *(.stack)
        . = ALIGN(32);
  *(.stack_magic0);
        . = ALIGN(32);
        _cpu0_sstack_end = .;
        PROVIDE(cpu0_sstack_end = .);
        . = ALIGN(32);

        _system_data_begin = .;
        *(.uac_var);
        *(.uac_rx);
        *(.uac_spk);
        *(.mass_storage);
        *(.usb_msd_dma);
        *(.usb_ep0_dma);
        *(.usb_iso_dma);
        *(.usb_hid_dma);
        *(.usb_config_var);
        *(.hid_config_var);
        *(.bss)
        *(.*.data.bss*)
  *(.non_volatile_ram)
        _system_data_end = .;
    } > ram0

 OVERLAY : AT(0x200000) SUBALIGN(4)
    {
  .overlay_g729
  {
            *(.tone_play_mem);
            *(.g729_mem);
        }
        .overlay_algorithm
        {
            *(.algorithm_mem);
        }
    } > ram0

    .heap_buf ALIGN(4):
    {
        PROVIDE(_free_start = .);
        . = LENGTH(ram0) + ORIGIN(ram0) - 1;
        PROVIDE(_free_end = .);
    } > ram0

    _ram_end = .;

 . = ORIGIN(boot_ram);
 .boot_data ALIGN(4):
 {
  *(.boot_info)
 } > boot_ram

    . = ORIGIN(app_code);
    .app_code ALIGN(32):
    {
        app_code_text_begin = .;
        KEEP(*(.start))
        *(.start.text)
        *(*exception_isr)
        *(.*.text.const)
        *(.*.text)
        *(.log_ut_text)
        *(.version)
        *(.debug)
        *(.debug.text.const)
        *(.debug.text)
        *(.debug.string)
        *(.text)
        *(.text.*)
        *(.opcore_maskrom)
        *(.flash_data)
     *(.wtg_dec_sparse_code)
        *(.wtg_dec_const)
        *(.wtg_dec_code)
        _CLK_CODE_START = .;


        _CLK_CODE_END = .;
        _SPI_CODE_START = .;
        *(.spi_code)
        . = ALIGN(4);
        _SPI_CODE_END = .;
        *(.rodata*)
        *(.ins)
        app_code_text_end = . ;

        . = ALIGN(4);
        vfs_ops_begin = .;
        KEEP(*(.vfs_operations))
        vfs_ops_end = .;

        . = ALIGN(4);
        PROVIDE(device_node_begin = .);
        KEEP(*(.device))
        PROVIDE(device_node_end = .);

        . = ALIGN(4);
        PROVIDE(iap_auth_begin = .);
        KEEP(*(.iap_chip))
        PROVIDE(iap_auth_end = .);

        . = ALIGN(4);
        tool_interface_begin = .;
        PROVIDE(tool_interface_begin = .);
        KEEP(*(.tool_interface))
        PROVIDE(tool_interface_end = .);
        tool_interface_end = .;

  . = ALIGN(4);
        cmd_interface_begin = .;
        PROVIDE(cmd_interface_begin = .);
        KEEP(*(.eff_cmd))
        cmd_interface_end = .;
        PROVIDE(cmd_interface_end = .);

  . = ALIGN(4);
     lp_target_begin = .;
     PROVIDE(lp_target_begin = .);
     KEEP(*(.lp_target))
     lp_target_end = .;
     PROVIDE(lp_target_end = .);

  . = ALIGN(4);
     decoder_plug_begin = .;
     PROVIDE(decoder_plug_begin = .);
     KEEP(*(.decoder_plug))
     decoder_plug_end = .;
     PROVIDE(decoder_plug_end = .);

        . = ALIGN(32);
        text_end = .;
 } >app_code
}

bss_begin = ADDR(.bss);
bss_size = SIZEOF(.bss);
ASSERT((bss_size % 4) == 0,"!!! bss_size Not Align 4 Bytes !!!");


bss_size1 = _system_data_end - _system_data_begin;
bss_begin1 = _system_data_begin;

data_addr = ADDR(.data);
data_begin = text_end;
data_size = SIZEOF(.data);
ASSERT((data_size % 4) == 0,"!!! data_size Not Align 4 Bytes !!!");


cache_ram_addr = ADDR(.cache_ram);
cache_ram_begin = data_begin + data_size;
cache_ram_size = SIZEOF(.cache_ram);
ASSERT((cache_ram_size % 4) == 0,"!!! cache_ram_size Not Align 4 Bytes !!!");


text_size = SIZEOF(.app_code);
ASSERT((text_size % 4) == 0,"!!! text_size Not Align 4 Bytes !!!");

ASSERT((text_size + data_size) <= CONFIG_CODE_SIZE,"!!! code space too small !!!");
