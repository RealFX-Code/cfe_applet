OUTPUT_ARCH(arm)
ENTRY(__start)
SECTIONS
{
  . = 0x20000000;

  .text :
  {
    _ftext = .;
    *(.init)
    eprol = .;
    *(.text)
    *(.text.*)
    *(.fini)
    *(.rodata)
    *(.rodata.*)
    _etext = .;
  }

  .ARM.exidx :
  {
    *(.ARM.exidx*)
    *(.ARM.extab*)
  }

  .data :
  {
    _fdata = .;
    *(.data)
    *(.data.*)
    CONSTRUCTORS
  }
  _edata = .;

  _fbss = .;
  .bss :
  {
    *(.bss)
    *(.bss.*)
    *(COMMON)
  }
  _end = .;
}